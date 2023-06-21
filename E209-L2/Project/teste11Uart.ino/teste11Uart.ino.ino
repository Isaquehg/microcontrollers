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
char msg_rx[5];
int pos_msg_rx = 0;
int tamanho_msg_rx = TAMANHO;

volatile unsigned int n_gotas = 0; // Numero de gotas detectadas
volatile unsigned int segundos = 0; // Intervalo entre gotas
volatile unsigned int cont = 0;
volatile bool contando = true; // Verificar recebimento
unsigned int Leitura_AD; // ADC
float tensao; // Tensao Ultrassonico
unsigned int volume; // Volume desejado
unsigned int tempo; // Tempo de injeção
bool change = true; // Alterar parametros = true
// Inibir exibição mais q uma vez
bool volumePrompted = false;
bool timePrompted = false;
bool modifyPrompted = false;

float fluxo_definido = 0;
float fluxo_real = 0;
float DC; // Duty-Cycle motor
int aux_rx = 0; // Auxiliar no recebimento
int erro; // Erro entre fluxo real e fluxo definido
float dist; // Distancia mangueira do sensor (padrao = 5cm)

enum State {
  STATE_WAIT_VOLUME,
  STATE_WAIT_TIME,
  STATE_CALCULATE,
  STATE_ERROR,
  STATE_COUNT,
  STATE_MODIFY,
  STATE_ALERT
};

// Conta-gotas
ISR(INT0_vect) {
    // Contar a partir de 2 gotas
    if (n_gotas == 0) {
        TCCR0B = (1 << CS01); // Ativar timer com pre-scaler de 8(f = 2MHz, t = 500ns)
    }

    n_gotas ++;

    // Desativar contagem após 3 gotas
    if(n_gotas >= 2){
        contando = false;
    }

    UART_Transmit("Num Gotas:");
    itoa(n_gotas, msg_tx, 10);
    UART_Transmit(msg_tx);
    UART_Transmit("\n");
}

// Timer 0 - Entra a cada 100us
ISR(TIMER0_COMPA_vect) {
    cont++;

    if (cont == 10000) {
        cont = 0;
        segundos ++;

        int X = segundos;
        itoa(X, msg_tx, 10);
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

void initialize(){
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
    TCCR2A = (1 << COM2A1) | (0 << COM2A0) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS22) | (1 << CS20); // Pre-scaler de 1024
    OCR2A = 0;

    // ADC
    ADMUX = (0 << REFS1) + (1 << REFS0); //Utiliza 5V como referência (1023)
    ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0); //Habilita ADC e PS 128 (10 bits)
    ADCSRB = 0; //Conversão Única

    sei();
}

int main() {
    initialize();

    // Setting states
    State state = STATE_WAIT_VOLUME;

    for(;;){
        switch (state)
        {
            case STATE_WAIT_VOLUME:
                if(!volumePrompted){
                    UART_Transmit("Entre com o Volume: \n");
                    volumePrompted = true;
                }
                _delay_ms(100);
                // Conversao p/ int
                aux_rx = (msg_rx[0] - '0') * 100 + (msg_rx[1] - '0') * 10 + (msg_rx[2] - '0');
                if ((aux_rx <= 999) && (aux_rx >= 100)) {
                    // Atribuir volume
                    volume = aux_rx;
                    itoa(volume, msg_tx, 10);
                    UART_Transmit(msg_tx);
                    UART_Transmit("\n");

                    // Reset
                    state = STATE_WAIT_TIME;
                    timePrompted = false;
                    UART_LimparBuffer();
                    msg_rx[0] = '\0';
                    msg_rx[0] = '\0';
                    msg_rx[0] = '\0';
                }
                break;
            
            case STATE_WAIT_TIME:
                if (!timePrompted){
                    UART_Transmit("Entre com o Tempo Infusao em minutos \n");
                    timePrompted = true;
                }
                _delay_ms(100);
                // Conversao p/ int
                aux_rx = (msg_rx[0] - '0') * 100 + (msg_rx[1] - '0') * 10 + (msg_rx[2] - '0');
                if ((aux_rx <= 999) && (aux_rx >= 100)) {
                    // Atribuir tempo
                    tempo = aux_rx;
                    itoa(volume, msg_tx, 10);
                    UART_Transmit(msg_tx);
                    UART_Transmit("\n");

                    // Mudar estado e reset
                    state = STATE_CALCULATE;
                    UART_LimparBuffer();
                }
                break;

            case STATE_CALCULATE:
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

                // Mudar estado
                state = STATE_COUNT;
                contando = true;

                break;

            case STATE_COUNT:
                // Calcular fluxo real [ml/h]
                UART_Transmit("Detectando gotas...");
                while(contando){
                    _delay_ms(1000);
                    fluxo_real = ((n_gotas * 60) / segundos) * 0.05;
                    itoa(fluxo_real, msg_tx, 10);
                    UART_Transmit("\nFluxo Real int: ");
                    UART_Transmit(msg_tx);
                    UART_Transmit("\nNum Gotas: ");
                    itoa(n_gotas, msg_tx, 10);
                    UART_Transmit(msg_tx);
                    UART_Transmit("\nSegundos: ");
                    itoa(segundos, msg_tx, 10);
                    UART_Transmit(msg_tx);
                    UART_Transmit("\n no loop... \n");
                }
                state = STATE_ERROR;
                // Reset
                TCCR0B = 0;// Desativar timer
                n_gotas = 0;
                segundos = 0;
                UART_Transmit("\nParou gotas");
                UART_Transmit("\n");

                break;

            case STATE_ERROR:
                // Calculo de Erro
                erro = ((fluxo_real - fluxo_definido) / fluxo_definido) * 1.0;
                itoa(erro, msg_tx, 10);

                // Output
                UART_Transmit("Erro: ");
                UART_Transmit(msg_tx);
                UART_Transmit("% \n");

                // Alterar estado
                state = STATE_MODIFY;
                modifyPrompted = false;

                break;
                
            case STATE_ALERT:
                PORTD |= BUZZER;
                OCR0A = 0;
                UART_Transmit("Bolhas detectadas! \n");
                _delay_ms(2000);
                PORTD &= ~BUZZER;
                state = STATE_WAIT_VOLUME;
                volumePrompted = false;

                break;

            case STATE_MODIFY:
                // Verificar se usuário deseja modificar dados inseridos
                if (pos_msg_rx > 0) {
                    if (!modifyPrompted){
                        UART_Transmit("Deseja alterar os dados inseridos [s/n]? \n");
                        modifyPrompted = true;
                    }
                    if (msg_rx[0] == 's') {
                        state = STATE_WAIT_VOLUME;
                        volumePrompted = false;
                    } 
                    else if (msg_rx[0] == 'n') {
                        state = STATE_COUNT;
                    }
                    UART_LimparBuffer();
                }

                break;

            default:
                // Resetar
                msg_rx[0] = '\0';
                msg_rx[1] = '\0';
                msg_rx[2] = '\0';
                msg_rx[3] = '\0';
                aux_rx = 0;
                pos_msg_rx = 0;
                contando = true;
                volumePrompted = false;
                timePrompted = false;
                modifyPrompted = false;
                UART_LimparBuffer();
                state = STATE_WAIT_VOLUME;

                break;
        }

        // Detecção de bolhas
        ADMUX = (ADMUX & 0xF8) | ULTRASONIC; // Determinar o pino de leitura
        ADCSRA |= (1 << ADSC); //Inicia a conversão
        while ((ADCSRA & (1 << ADSC)) == (1 << ADSC)); //Esperar a conversão
        Leitura_AD = ADC; //Armazenamento da leitura

        tensao = (Leitura_AD * 5) / 1023.0; //Cálculo da Tensão
        dist = (tensao * 20) / 5.0;// Calculo da distancia
        itoa(dist, msg_tx, 10);
        //UART_Transmit("\nDist int:");
        //UART_Transmit(msg_tx);
        //UART_Transmit("\n");

        // Se detectado algo a menos de 5cm
        if (dist < 5) {
            state = STATE_ALERT;
        }
    }
}
