/*
Crie um programapara controlar dois leds, um vermelho e um verde. toda vez que o computador enviar o comando “9”o vermelho liga e o verde desliga e “8”o verde liga e vermelho desliga.
*/

#define LED1 (1 << PD4)
#define LED2 (1 << PD5)
#define FOSC 16000000U //ClockSpeed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

char msg_tx[20];
char msg_rx[32];
int pos_msg_rx = 0;
int tamanho_msg_rx = 6;
unsigned int x = 0, valor = 0;

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
    DDRD |= LED1 + LED2;
    PORTD &= ~(LED1 + LED2);

    UART_Init(MYUBRR);
    sei();

    x = 0;
    UART_Transmit("Aperte o BOTAO:\n");
    //Super-loop
    while(1){
        if((msg_rx[0]=='9')){
            PORTD |= LED1;
            PORTD &= ~LED2;
        }
        else if((msg_rx[0]=='8')){
            PORTD |= LED2;
            PORTD &= ~LED1;
        }
        // Aguarda um tempo para evitar o bounce
        _delay_ms(500);
    }
}
