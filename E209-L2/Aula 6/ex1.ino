#define LED1 (1 << PD5)
#define LED2 (1 << PD4)
#define BOTAO (1 << PD2)

int main(){
    DDRD = (LED1 | LED2);

    PORTD &= ~(LED1 | LED2);

    EICRA = (1<<ISC00) + (0<<ISC00) + (1<<ISC0) + (1<<ISC00);
    EIMSK = (0<<INT1) | (1 << INT0);

    sei();

    for(;;){
        PORTD |= LED1;
        _delay_ms(500);
        PORTD &= ~LED1;
        _delay_ms(500);
    }
}
ISR(INT0_vect){
    PORTD |= LED2;
    _delay_ms(1000);
    PORTD &= ~LED2;
}