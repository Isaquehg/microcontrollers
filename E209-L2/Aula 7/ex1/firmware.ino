#define SA (1 << PD0)
#define SF (1 << PD1)
#define BT (1 << PD2)
#define MA (1 << PB0)
#define MF (1 << PB1)

int main(void){
    DDRD = (MA | MF);
    PORTD &= ~(MA | MF);

    EICRA = (1 << ISC01) + (0 << ISC00);
    EIMSK = (0 << INT1) | (1 << INT0);

    sei();

    while(1){

    }
}

ISR(INT0_vect){
    short int sa, sf;
    sa = PIND & sa;
  	sf = PIND & sf;

    //se ja estiver aberto
    if((sa == SA) && (sf != SF)){
        PORTD &= ~MA;
        PORTD |= MF;
    }
    //se ja estiver fechado
    if ((sa != SA) && (sf == SF)){
        PORTD &= ~MF;
        PORTD |= MA;
    }
    //para o motor
    else{
        PORTD &= ~(MA + MF);
    }
}