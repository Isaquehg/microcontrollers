#define LED1 (1 << PD5)//LED 1 - VERMELHO
#define LED2 (1 << PD6)//LED 2 - AMARELO
#define LED3 (1 << PD7)//LED 3 - VERDE
#define LED4 (1 << PB1)//LED 4 - VERMELHO
#define BTN1 (1 << PB0)//BOTAO 1 PCINT0
#define BTN2 (1 << PB2)//BOTAO 2 PCINT2
#define BTN3 (1 << PD4)//BOTAO 3 PCINT20

ISR(PCINT0_vect){
    //Acender PD7
    PORTD |= LED3;
    _delay_ms(1000);
}
ISR(PCINT2_vect){
    //Acender PD6
    PORTD |= LED2;
    _delay_ms(500);
}
ISR(PCINT20_vect){
    //Acender PB1
    PORTD |= LED4;
    _delay_ms(2000);
}

int main(void){
    //Habilitar saida LEDs
    DDRB |= LED1 + LED2 + LED3 + LED4;
    //Desativar LEDs
    PORTD &= ~(LED1 + LED2 + LED3);

    //Habilita interrupção no grupo PCINT2
    //PCIE0 -> PCMSK0
    //PCIE2 -> PCMSK2
    PCICR |= (1 << PCIE0) + (1 << PCIE2);
    // Habilita PCINT no pino PD4 -> PCINT20
    PCMSK0 |= BTN1 + BTN2;
    PCMSK2 |= BTN3;

    // Habilita Interrupções globais
    sei();

    while (1)
    {
        PORTB ^= LED1;
        _delay_ms(250);
    }
}