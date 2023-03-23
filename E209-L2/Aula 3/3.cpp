int main(void){
    DDRD = 0b00000011;

    for(;;){
        PORTD = 0b00000001;
        _delay_ms(1000);
        PORTD = 0b00000010;
        _delay_ms(1000);
        PORTD = 0b00000000;
        _delay_ms(1000);
        PORTD = 0b00000011;
        _delay_ms(1000);
    }
}