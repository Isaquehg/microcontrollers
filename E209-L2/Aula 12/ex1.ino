/*
Crie um programa que quando o botão for pressionado (interrupção externa)envie o número de vezes  que ele foi  pressionado(incluindo  a  atual) para  o  PC  através  da  UART.  E  toda  vez  que o computador enviar o comando “zerar”, devemos zerar a contagem.
*/

#define BOTAO (1 << PD4)
#define FOSC 16000000U //ClockSpeed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

char msg_tx[20];
char msg_rx[32];
int pos_msg_rx = 0;
int tamanho_msg_rx = 6;
unsigned int x = 0, valor = 0;
unsigned int vezes = 0;

// Prototipos das funcoes
//void UART_Init(unsignedintubrr);
//void UART_Transmit(char*dados);

ISR(PCINT2_vect){
    vezes ++;
}

ISR(USART_RX_vect){
  // Escreve o valor recebido pela UART na posição pos_msg_rx do buffer msg_rx 
  msg_rx[pos_msg_rx++]=UDR0;
  if(pos_msg_rx == tamanho_msg_rx)
    pos_msg_rx=0;
}

void UART_Transmit(char* dados){
  // Envia todos os caracteres do buffer dados ate chegar um final de linha
  while(*dados != 0){
    // Aguarda a transmissão acabar
    while(!(UCSR0A&(1<<UDRE0)));
    // Escreve o caractere no registro de tranmissão
    UDR0=*dados;
    // Passa para o próximo caractere do buffer dados
    dados++;
  }
}
void UART_Init(unsigned int ubrr){
  //Configuraabaudrate
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  // Habilita a recepcao, tranmissao e interrupcao na recepcao
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  // Configura o formato da mensagem: 8 bits de dados e 1bits de stop
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

int main(void){
    vezes = 0;
    PCICR = (1 << PCIE2);//habilita interrupção no portal B
    PCMSK2 = BOTAO;
    PORTD |= BOTAO;

    UART_Init(MYUBRR);
    sei();

    x = 0;
    UART_Transmit("Aperte o BOTAO:\n");
    //Super-loop
    while(1){
        if((msg_rx[0]=='z')&&(msg_rx[1]=='e')&&(msg_rx[2]=='r')&&(msg_rx[3]=='a')&&(msg_rx[4]=='r')){
            vezes = 0;
        }
        // O BOTAO foi pressionado?
        // Se sim, envia mensagem
        UART_Transmit("Contagem botao apertado\n");
        UART_Transmit("Num vezes BOTAO press:");
        itoa(vezes, msg_tx, 10);
        UART_Transmit(msg_tx);
        UART_Transmit("\n");

        // Aguarda um tempo para evitar o bounce
        _delay_ms(500);
    }
}