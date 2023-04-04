#define SMIN (1 << PD2)
#define SMAX (1 << PD3)
#define VALVULA (1 << PB0)
#define ENCHENDO (1<< PB1)
#define CHEIO (1 << PB2)

int main(){
    DDRB = (VALVULA + ENCHENDO + CHEIO);
    PORTB &= ~(VALVULA + ENCHENDO);
    PORTB |= CHEIO;

    EICRA = (1 << ISC11) + (1 << ISC10) + (1 << ISC01) + (0 < ISC00);
    EIMSK = (1 << INT1) | (1 << INT0);

    sei();

    while (1)
    {

    }
}
//INTERRUPCOES
ISR(INT0_vect){
    PORTB |= (VALVULA + ENCHENDO);
    PORTB &= ~CHEIO;
}
ISR(INT1_vect){
    PORTB &= ~(VALVULA + ENCHENDO);
    PORTB |= CHEIO;
}