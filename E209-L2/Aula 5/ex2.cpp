#define BTN1 (1 << PB1)
#define BTN2 (1 << PB2)
#define BTN3 (1 << PB3)
#define MT1 (1 << PD1) //30HP
#define MT2 (1 << PD2) //50HP
#define MT3 (1 << PD3) //70HP


int main(void){
    DDRD = (MT1 | MT2 | MT3);
    DDRB = (BTN1 | BTN2 | BTN3);

    PORTB |= BTN1;
    PORTB |= BTN2;
    PORTB |= BTN3;

    PORTD &= ~MT1;
    PORTD &= ~MT2;
    PORTD &= ~MT3;
    
    for(;;){
        //leitura botoes
        short int btn1, btn2, btn3;
        btn1 = PINB & BTN1;
        btn2 = PINB & BTN2;
        btn3 = PINB & BTN3;

        short int mt1, mt2, mt3;
        mt1 = PINB & btn1;
        mt2 = PINB & btn2;
        mt3 = PINB & btn3;

        //1
        if((PINB & btn1) == 0){
            PORTD |= mt1;
            PORTD &= ~mt3;
        }
        if((PINB & btn2) == 0){
            PORTD |= mt2;
            PORTD &= ~mt3;
        }
        if((PINB & btn3) == 0){
            PORTD |= mt3;
            PORTD &= ~mt1;
            PORTD &= ~mt2;
        }
    }
}