//Declarações para UART
#include <stdio.h>
#include <stdlib.h>

// Input
#define SGOTAS (1 << PD2)
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
char msg_tx[38];
char msg_rx[4];
int pos_msg_rx = 0;
int tamanho_msg_rx = TAMANHO;

void UART_config(unsigned int ubrr);
void UART_Transmit(char *dados);

unsigned int cont = 0; // Aux
unsigned int n_gotas = 0; // Numero de gotas detectadas
unsigned int segundos = 0; // Intervalo entre gotas
unsigned int Leitura_AD; // ADC
float tensao; // Tensao Ultrassonico
unsigned int volume; // Volume desejado
unsigned int tempo; // Tempo de injeção
bool change = true; // Alterar parametros = true
bool iniciado = false; // Momento de iniciar a contagem de tempo

float fluxo_definido = 0;
float fluxo_real = 0;
float DC;
int aux_rx = 0;
int fase = 0;// Verificação de recebimento
int erro;
float dist;

int main() {
    UART_config(MYUBRR);
    msg_rx[0] = '\0';
    msg_rx[1] = '\0';
    msg_rx[2] = '\0';

    // Habilitando saidas e pull-up
    DDRD |= (MOTOR + BUZZER);
    PORTD &= ~(MOTOR + BUZZER);
    PORTD |= SGOTAS;

    // INT0(PD2) - Falling edge
    EICRA = (1 << ISC01);
    EIMSK = (1 << INT0);

    // Timer 0 - CTC
    TCCR0A = (1 << WGM01);
    TCCR0B = 0;
    OCR0A = 199;
    TIMSK0 = (1 << OCIE0A);

    // Timer 2 - Fast-PWM
    // VER QUESTAO COMPARADOR A!!
    TCCR2A = (1 << COM2A1) | (0 << COM2A0) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS22) | (1 << CS20); // Pre-scaler de 1024
    OCR2A = 0;

    // ADC
    ADMUX = (0 << REFS1) + (1 << REFS0); //Utiliza 5V como referência (1023)
    ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0); //Habilita ADC e PS 128 (10 bits)
    ADCSRB = 0; //Conversão Única

    sei();
    for(;;){
        if(change){
            UART_LimparBuffer();

            // Confirmar recebimento msg Volume
            UART_Transmit("Entre com o Volume: \n");
            while (fase == 0) {
                _delay_ms(100);
                // Conversao p/ int
                aux_rx = (msg_rx[0] - 48) * 1 + (msg_rx[1] - 48) * 10 + (msg_rx[2] - 48) * 100;
                if ((aux_rx <= 999) && (aux_rx >= 100)) {
                    fase ++;
                }
            }

            // Atribuir volume
            volume = aux_rx;
            itoa(volume, msg_tx, 10);
            UART_Transmit(msg_tx);
            UART_Transmit("\n");

            // Resetar
            msg_rx[0] = '\0';
            msg_rx[1] = '\0';
            msg_rx[2] = '\0';
            msg_rx[3] = '\0';
            aux_rx = 0;
            pos_msg_rx = 0;
            UART_LimparBuffer();

            // Confirmar recebimento msg Tempo Infusao
            UART_Transmit("Entre com o Tempo Infusao em minutos \n");
            while (fase == 1) {
                _delay_ms(100);
                // Conversao p/ int
                aux_rx = (msg_rx[0] - 48) * 1 + (msg_rx[1] - 48) * 10 + (msg_rx[2] - 48) * 100;
                if ((aux_rx <= 999) && (aux_rx >= 100)) {
                    fase ++;
                }
            }
            // Sair do loop
            change = false;
        }
        UART_Transmit(msg_rx);
        UART_Transmit("\n");
        // Atribuir tempo
        tempo = aux_rx;

        // Resetar msg_rx
        msg_rx[0] = '\0';
        msg_rx[1] = '\0';
        msg_rx[2] = '\0';
        msg_rx[3] = '\0';
        aux_rx = 0;

        // Calculating defined flux
        fluxo_definido = (volume * 60) / tempo * 1.0;
        itoa(fluxo_definido, msg_tx, 10);
        UART_Transmit("Fluxo Definido: ");
        UART_Transmit(msg_tx);
        UART_Transmit("\n");

        // Calculating DC needed
        DC = (fluxo_definido * 100) / 450.0;

        // Ligar motor com potencia definida
        OCR2A = int(DC);
        
        // Calcular fluxo real [ml/h]
        UART_Transmit("Detectar gotas...");
        _delay_ms(100);
        segundos = 5;
        n_gotas = 3;
        fluxo_real = ((n_gotas * 60) / segundos) * 0.05;
        itoa(fluxo_real, msg_tx, 10);
        UART_Transmit("\nFluxo Real int: ");
        UART_Transmit(msg_tx);
        _delay_ms(100);
        UART_Transmit("\nNum Gotas: ");
        itoa(n_gotas, msg_tx, 10);
        UART_Transmit(msg_tx);
        _delay_ms(100);
        UART_Transmit("\nSegundos: ");
        itoa(segundos, msg_tx, 10);
        UART_Transmit(msg_tx);
        _delay_ms(100);

        UART_Transmit("\nParou gotas");
        UART_Transmit("\n");

        // Calculo de Erro
        erro = ((fluxo_real - fluxo_definido) / fluxo_definido) * 1.0;
        itoa(erro, msg_tx, 10);

        // Output
        UART_Transmit("Erro: ");
        UART_Transmit(msg_tx);
        UART_Transmit("% \n");

        UART_Transmit("\n");
        // Resetar msg_rx
        msg_rx[0] = '\0';
        msg_rx[1] = '\0';
        msg_rx[2] = '\0';

        // Detecção de bolhas
        ADMUX = (ADMUX & 0xF8) | ULTRASONIC; // Determinar o pino de leitura
        ADCSRA |= (1 << ADSC); //Inicia a conversão
        while ((ADCSRA & (1 << ADSC)) == (1 << ADSC)); //Esperar a conversão
        Leitura_AD = ADC; //Armazenamento da leitura

        tensao = (Leitura_AD * 5) / 1023.0; //Cálculo da Tensão
        dist = (tensao * 20) / 5.0;// Calculo da distancia
        itoa(dist, msg_tx, 10);
        UART_Transmit("\nDist int:");
        UART_Transmit(msg_tx);
        UART_Transmit("\n");

        // Se detectado algo a menos de 5cm
        if (dist < 5) {
            PORTD |= BUZZER;
            OCR0A = 0;
            UART_Transmit("Bolhas detectadas! \n");
        }
    }
}

// Interrupção de Recebimento da Serial
ISR(USART_RX_vect) {
    // Escreve o valor recebido pela UART na posição pos_msg_rx do buffer msg_rx
    msg_rx[pos_msg_rx++] = UDR0;
    //UART_Transmit(msg_rx);

    if (pos_msg_rx == tamanho_msg_rx - 1)
        pos_msg_rx = 0;
}

// Transmissão de Dados Serial
void UART_Transmit(char *dados) {
    // Envia todos os caracteres do buffer dados ate chegar um final de linha
    while (*dados != 0) {
    while ((UCSR0A & (1 << UDRE0)) == 0); // Aguarda a transmissão acabar
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
    UCSR0B = ((1 << RXCIE0) + (1 << RXEN0) + (1 << TXEN0));

    // Configura o formato da mensagem: 8 bits de dados e 1 bits de stop */
    UCSR0C = ((1 << UCSZ01) + (1 << UCSZ00));
}

// Limpar o buffer de recepção
void UART_LimparBuffer() {
    while (UCSR0A & (1 << RXC0))
        volatile char lixo = UDR0; // Lê e descarta o caractere do buffer de recepção
}