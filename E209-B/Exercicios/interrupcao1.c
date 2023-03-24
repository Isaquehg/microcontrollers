#define LIGA (1 << PD2) //INT0
#define DESLIGA (1 << PD3) //INT1
#define LED (1 << PB0)

int main(){
  DDRB = LED;
  PORTD |= DESLIGA;
  
  //declar modo de funcionamento da INT0 (PD2) rising edge
  EICRA = (1 << ISC01) + (1 << ISC00);
  
  //habilita o uso do INT0
  EIMSK = (1 <<INT0);

  //start external interupt
  sei();
  
  //começar LED desativado
  PORTB &= ~LED;
    
    for(;;){
      //PORTB &= ~LED;
      
      
      /*
      short int botao;
      botao = PIND & BOTAO;
      
      if(botao == BOTAO)
        PORTB |= LED;
      else
        PORTB &= ~LED;
        */
    }
}

//o que acontece se a interrupção INT0 for gerada
ISR(INT0_vect){
  PORTB |= LED;
}