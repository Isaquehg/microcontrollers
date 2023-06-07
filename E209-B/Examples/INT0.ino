#define LIGA (1<<PD2) //INT0
#define DESLIGA (1<<PD3) //INT1
#define LED (1<<PB0)

int main(){
  DDRB = LED;
  PORTD |= DESLIGA;
  
  //Declarar o modo de funcionamento da INT0
  EICRA = (1<<ISC11)+(0<<ISC10)+(1<<ISC01)+(1<<ISC00); 
  //Habilitar a interrupção INT0
  EIMSK = (1<<INT1)+(1<<INT0);
  
  PORTB &= ~LED;
  
  //Utilizei interrupção? Se sim, COLOQUE "sei();"
  sei();
  
  for(;;){
  
  }
}

//O que acontece se a interrupção INT0 for gerada?
ISR(INT0_vect){

      PORTB |= LED;


}
//O que acontece se a interrupção INT1 for gerada?
ISR(INT1_vect){

      PORTB &= ~LED;


}