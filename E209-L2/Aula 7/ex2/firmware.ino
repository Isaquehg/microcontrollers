#define SMIN (1 << PD2)
#define SMAX (1 << PD3)
#define VALVULA (1 << PB0)
#define ENCHENDO (1<< PB1)
#define CHEIO (1 << PB2)

int main(){
    DDRB = (VALVULA + ENCHENDO + CHEIO);
    PORTB &= ~(VALVULA + ENCHENDO);
    PORTB |= CHEIO;

    while (1)
    {
        short int smin, smax;
        smin = PIND & SMIN;
        smax = PIND & SMAX;

        //SMAX INATIVO -> VALVULA E LED ENCHENDO ATIVOS
        if(((smin != SMIN) && (smax != SMAX)) || ((smin == SMIN) && (smax != SMAX))){
            PORTB |= (VALVULA + ENCHENDO);
            PORTB &= ~CHEIO;
        }
        //SMIN E SMAX ATIVOS -> VALVULA E LED ENCHENDO DESLIGADOS
        if((smin == SMIN) && (smax == SMAX)){
            PORTB &= ~(VALVULA + ENCHENDO);
            PORTB |= CHEIO;
        }
    }
    

}