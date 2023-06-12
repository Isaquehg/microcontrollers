//Declarações para UART
#include <stdio.h>
#include <stdlib.h>

// Input
#define SGOTAS PD2
#define ULTRASONIC 1

// Output
#define BUZZER PD4
#define MOTOR PD6

// UART Setup
#define FOSC 16000000U
#define BAUD 9600
#define MYUBRR FOSC / 16 / (BAUD - 1)
#define TAMANHO 4

// Max. vazão motor a 100% PWM
#define MAX_MOTOR 450 / 60.0

// UART variables
char msg_tx[20];
char msg_rx[32];
int pos_msg_rx = 0;
int tamanho_msg_rx = TAMANHO;

void UART_config(unsigned int ubrr);
void UART_Transmit(char *dados);

unsigned int cont = 0; // Aux
unsigned int n_gotas = 0; // Numero de gotas detectadas
unsigned int segundos = 0; // Intervalo entre gotas
unsigned int Leitura_AD; // ADC
float tensao; // Tensao Ultrassonico
float ml = n_gotas / 20.0; // ml recebido
unsigned char volume[4]; // Desired volume
unsigned char time[4]; // Tempo de injeção
bool change = true; // Alterar parametros = true
bool iniciado = false; // Momento de iniciar a contagem de tempo

int main(){
	//Serial.begin(BAUD);
	UART_config(MYUBRR);

    // Habilitando saidas e pull-up
    DDRD |= (MOTOR + BUZZER);
    PORTD &= ~(MOTOR + BUZZER);
    PORTD |= ~SGOTAS;

    // INT0(PD2) - Falling edge
    EICRA = (1 << ISC01) + (0 << ISC00);
    EIMSK = (1 << INT0);
	
    // Timer 0 - CTC
	TCCR0A = (1 << WGM01);
	TCCR0B = 0;
	OCR0A = 199;
	TIMSK0 = (1 << OCIE0A);

    // Timer 2 - Fast-PWM
    TCCR2A = (1 << COM2A1) | (0 << COM2A0) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // Pre-scaler de 1024
    OCR2A = 0;

    // ADC
    ADMUX = (0 << REFS1) + (1 << REFS0); //Utiliza 5V como referência (1023)
	ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0); //Habilita ADC e PS 128 (10 bits)
	ADCSRB = 0; //Conversão Única
	
	sei();
	
	for(;;){
        int x;// Verificação de recebimento
        if(change){
            // Confirmar recebimento msg volume
            int aux_0, aux_1, aux_2;
            x = 0;
            UART_Transmit("Entre com o Volume: \n");
            while (x == 0) {
                // Conversao
                itoa(aux_0, msg_rx[0], 10);
                itoa(aux_1, msg_rx[0], 10);
                itoa(aux_2, msg_rx[0], 10);

                if ((aux_0 < 10) && (aux_0 >= 0) && (aux_1 < 10) && (aux_1 >= 0) && (aux_2 < 10) && (aux_2 >= 0)) {
                    x = 1;
                }
            }
            UART_Transmit("aqui");
            UART_Transmit("\n");

            // Atribuir volume
            volume[0] = msg_rx[0];
            volume[1] = msg_rx[1];
            volume[2]= msg_rx[2];
            msg_rx[0] = 0;
            msg_rx[1] = 0;
            msg_rx[2] = 0;
            
            // Confirmar recebimento msg tempo desejado
            x = 0;
            UART_Transmit("Entre com o Tempo de Infusao em minutos: ");
            while (x == 0) {
                // Conversao
                itoa(aux_0, msg_rx[0], 10);
                itoa(aux_1, msg_rx[0], 10);
                itoa(aux_2, msg_rx[0], 10);

                if ((aux_0 < 10) && (aux_0 >= 0) && (aux_1 < 10) && (aux_1 >= 0) && (aux_2 < 10) && (aux_2 >= 0)) {
                    x = 1;
                }
            }
            UART_Transmit(msg_rx);
            UART_Transmit("\n");

            // Atribuir tempo
            time[0] = msg_rx[0];
            time[1] = msg_rx[1];
            time[2]= msg_rx[2];
            msg_rx[0] = 0;
            msg_rx[1] = 0;
            msg_rx[2] = 0;
        }

        // Convert time char to int
        int time_min_int;
		itoa(time_min_int, time, 10);
        // Convert volume char to int
        int volume_int;
        itoa(volume_int, volume, 10);

        // Calculating defined flux
        float fluxo_definido = volume_int / time_min_int * 1.0;

        // Calculating DC needed
        float DC = (fluxo_definido * 100) / 450.0;

        // Ligar motor com potencia definida
        OCR0A = int(DC);

        // Calcular fluxo real
        float fluxo_real;
        while(iniciado == false){
            if(n_gotas >= 2){
                fluxo_real = (n_gotas / segundos) * 1.0;
                // Voltar ao estado inicial
                TCCR0B = 0;// Desativar timer
                iniciado = true;
                n_gotas = 0;
            }
        }

        // Calculo de Erro
        int erro = ((fluxo_real - fluxo_definido) / fluxo_definido) * 100;

        // Conversion & Output
        char erro_char[4];
        sprintf(erro_char, "%d", erro);
        UART_Transmit("Erro: ");
        UART_Transmit(erro_char);
        UART_Transmit("\n");

        // Verificar se usuário deseja modificar dados inseridos
        x = 0;
        while (x == 0) {
            UART_Transmit("Deseja alterar os dados inseridos[s/n]? \n");
            if (msg_rx[0] == 's') {
                x = 1;
                change = true;
            }
            else if(msg_rx[0] == 'n'){
                x = 1;
                change = false;
            }
        }

        // Detecção de bolhas
		ADMUX = (ADMUX & 0xF8) | ULTRASONIC; // Determinar o pino de leitura
		ADCSRA |= (1 << ADSC); //Inicia a conversão
		while((ADCSRA & (1 << ADSC)) == (1 << ADSC)); //Esperar a conversão
		Leitura_AD = ADC; //Armazenamento da leitura

		tensao = (Leitura_AD * 5) / 1023.0; //Cálculo da Tensão
        float dist = (tensao * 20) / 5.0;// Calculo da distancia

        // Se detectado algo a menos de 5cm
        if(dist < 5){
            PORTD |= BUZZER;
            OCR0A = 0;
            UART_Transmit("Bolhas detectadas! \n");
        }
	}
}

// Conta-gotas
ISR(INT0_vect){
    if(n_gotas == 0){
        TCCR0B = (1 << CS01); // Ativar timer com pre-scaler de 8(f = 2MHz, t = 500ns)
    }
    // Contar a partir de 2 gotas
    else{
        iniciado = true;
    }
    n_gotas ++;
}

// Timer 0 - Entra a cada 100us
ISR(TIMER0_COMPA_vect){
	cont++;
	
	if(cont == 10000){
		cont = 0;
		segundos ++;

		int X = segundos;
		itoa(X, msg_tx, 10);
		UART_Transmit(msg_tx);
		UART_Transmit("\n");
	}
}

//Interrupção de Recebimento da Serial
ISR(USART_RX_vect) {
	// Escreve o valor recebido pela UART na posição pos_msg_rx do buffer msg_rx
	msg_rx[pos_msg_rx++] = UDR0;

	if (pos_msg_rx == tamanho_msg_rx)
	pos_msg_rx = 0;
}

//Transmissão de Dados Serial
void UART_Transmit(char *dados) {
  	// Envia todos os caracteres do buffer dados ate chegar um final de linha
	while (*dados != 0) {
		while ((UCSR0A & (1<<UDRE0)) == 0);  // Aguarda a transmissão acabar

		// Escreve o caractere no registro de tranmissão
		UDR0 = *dados;
		// Passa para o próximo caractere do buffer dados
		dados++;
	}
}

//Configuração da Serial
void UART_config(unsigned int ubrr) {
	// Configura o baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	// Habilita a recepcao, tranmissao e interrupcao na recepcao */
	UCSR0B = ((1<<RXCIE0) + (1<<RXEN0) + (1<<TXEN0));

	// Configura o formato da mensagem: 8 bits de dados e 1 bits de stop */
	UCSR0C = ((1<<UCSZ01) + (1<<UCSZ00));
}