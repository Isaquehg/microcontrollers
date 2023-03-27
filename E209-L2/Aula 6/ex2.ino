#define LED1 (1 << PD5)
#define LED2 (1 << PD4) //interrupcao INT0
#define BOTAO1 (1 << PD2) //interrupcao INT1
#define BOTAO2 (1 << PD3) //interrupcao INT1

int main(){
    DDRD = (LED1 | LED2);

    PORTD &= ~(LED1 | LED2);

    EICRA = (1<<ISC11) + (0<<ISC10) + (1<<ISC01) + (1<<ISC00);
    EIMSK = (1<<INT1) | (0 << INT0);

    sei();

    for(;;){
        PORTD |= LED1;
        _delay_ms(500);
        PORTD &= ~LED1;
    }
}

ISR(INT1_vect){
    EIMSK &= ~INT0;
}

ISR(INT0_vect){
    PORTD |= LED2;
    _delay_ms(1000);
    PORTD &= ~LED2;
}