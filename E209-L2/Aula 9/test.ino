#define LED1 (1 << PC2)
#define LED2 (1 << PC3)
#define LED3 (1 << PC4)
#define LED4 (1 << PC5)
#define BTN1 (1 << PD2)
#define BTN2 (1 << PD3)
#define BTN3 (1 << PD4)
#define BTN4 (1 << PD5)

//LED VERMELHO
ISR(INT0_vect){
	PORTC |= LED1;
  	_delay_ms(2000);
  	PORTC &= ~LED1;
}

//LED AMARELO
ISR(INT1_vect){
    PORTC |= LED2;
  	_delay_ms(1000);
  	PORTC &= ~LED2;
}

int main(void){
  //SAÍDAS
  DDRC |= (LED1 + LED2 + LED3 + LED4);
  PORTC &= ~(LED1 + LED2 + LED3 + LED4);
  
  //Declarar o modo de funcionamento da INT0 e INT1
  EICRA = (1 << ISC11) + (0 << ISC10) + (1 << ISC01) + (0 << ISC00); 
  //Habilitar a interrupção INT0 e INT1
  EIMSK = (1 << INT1) + (1 << INT0);
  
  sei(); 

  while(1){
    //LED VERDE (VERDE BAIXO)
    if(!(PIND & BTN3)){
        PORTC |= LED3;
        _delay_ms(500);
        PORTC &= ~LED3;
    }
    //LED AZUL(VERDE ALTO)
    else if(!(PIND & BTN4)){
        PORTC |= LED4;
        _delay_ms(250);
        PORTC &= ~LED4;
    }
  }
}