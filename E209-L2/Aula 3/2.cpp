int main(void){
    DDRB = 0b00000011;

    for(;;){
        PORTB = 0b00000000;
        _delay_ms(500);
        PORTD = 0b00000001;
        _delay_ms(500);
        PORTD = 0b00000010;
        _delay_ms(500);
        PORTD = 0b00000011;
        _delay_ms(500);
        PORTD = 0b00000000;
        _delay_ms(500);
    }
}