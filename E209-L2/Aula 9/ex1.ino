#define LED PB5

int contador = 0;

ISR (TIMER0_OVF_vect) {
  if (contador > 10000) {
    PORTB ^= (1 << LED); // inverte o estado do LED
    contador = 0;
  }
  contador ++;
}

int main()
{
  DDRB |= (1 << LED);
  TCCR0A = (1 << WGM02); //modo de operação CTC
  TCCR0B = (1 << CS02) | (1 << CS00); //divisor de frequencia
  TIMSK0|= (1 << TOIE0); //habilita comparador A
  
  sei(); 
  PORTB &= ~(1 << LED);
  
  while (1) {
    
  }
}