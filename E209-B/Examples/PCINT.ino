#define BUZZER (1 << PB4)
#define BTN1 (1 << PB2)

int main(void){
    DDRB |= BUZZER;
  	PORTB = BTN1;//pull-up

    PORTB &= ~BUZZER;

    PCICR = (1 << PCIE0);//habilita interrupção no portal B
    PCMSK0 = BTN1;

    sei();

    for(;;){

    }

}

ISR(PCINT0_vect){
    PORTB |= BUZZER;
}