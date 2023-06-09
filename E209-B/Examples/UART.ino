//Declarações para UART
#include <stdio.h>
#include <stdlib.h>

#define FOSC 16000000U
#define BAUD 9600
#define MYUBRR FOSC / 16 / (BAUD - 1)
#define TAMANHO 1

char msg_tx[20];
char msg_rx[32];
int pos_msg_rx = 0;
int tamanho_msg_rx = TAMANHO;

void UART_config(unsigned int ubrr);
void UART_Transmit(char *dados);

unsigned int cont = 0;
unsigned char segundos = 0, minutos = 0, horas = 0;
unsigned char leitura;

int main(){
	//Serial.begin(9600);
	UART_config(MYUBRR);
	
	TCCR0A = (1<<WGM01);
	TCCR0B = 0;
	OCR0A = 249;
	TIMSK0 = (1<<OCIE0A);
	
	sei();
	
	for(;;){
		//leitura = Serial.read();
		int x = 0;
    		while (x == 0) {
			UART_Transmit("Teste \n");
      		if ((msg_rx[0] == 'L') || (msg_rx[0] == 'D')) {
        		x = 1;
      		}
		}

		leitura = msg_rx[0];
		msg_rx[0] = 0;
		
		if(leitura == 'L'){
			//Serial.println("Ligado");
			UART_Transmit("Ligado \n");
			TCCR0B = (1<<CS01) + (1<<CS00);// Ativa timer
			segundos = 0;
			minutos = 0;
			horas = 0;
		}
		if(leitura == 'D'){
			//Serial.println("Desligado");
			UART_Transmit("Desligado \n");
			
			TCCR0B = 0;// Desativa timer
			segundos = 0;
			minutos = 0;
			horas = 0;	
		}	
	}
	
}

ISR(TIMER0_COMPA_vect){
	cont++;
	
	if(cont == 1000){
		cont = 0;
		segundos++;
		if(segundos == 60){
			segundos = 0;
			minutos++;
			if(minutos == 60){
				minutos = 0;
				horas++;
				if(horas == 24){
				horas = 0;
				}
			}
		}
		/*
		Serial.print(horas);
		Serial.print(":");
		Serial.print(minutos);
		Serial.print(":");
		Serial.println(segundos);
		*/
		int X = horas;
		itoa(X, msg_tx, 10);
		UART_Transmit(msg_tx);
		UART_Transmit(":");
		X = minutos;
		itoa(X, msg_tx, 10);
		UART_Transmit(msg_tx);	
		UART_Transmit(":");
		X = segundos;
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