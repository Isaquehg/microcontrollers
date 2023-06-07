#define LIGA (1 << PC0)
#define DESLIGA (1 << PC1)
#define S1 (1 << PB0)
#define S2 (1 << PB1)
#define MOTOR (1 << PD1)
#define AVANCO (1 << PD2)
#define RECUO (1 << PD3)

int main(void){
    DDRD |= MOTOR + AVANCO + RECUP;

    PORTC = DESLIGA;
    PORTB &= ~(MOTOR + AVANCO + RECUO);

    //configurando interrupções em PB0 e PB1
    PCICR = (1 << PCIE0);//habilita interrupções no portal B
    PCMSK0 = (S1 + S2);

    sei();

    for(;;){
        short int liga, desliga;
        liga = PINC & LIGA;
        desliga = PINC & DESLIGA;

        if(desliga == 0)
            PORTD &= ~(MOTOR + AVANCO + RECUO);
        else if ()
            PORTD |= MOTOR;
        
    }
}

//Rotina de interrupção no Portal B
ISR(PCINT0_vect){
    if(((PINB & S1) == S1)) && ((PIND & RECUO)){
        PORTD |= AVANCO;
    }
    if ((PINB & S2) == S2)
    {
        PORTD &= ~(MOTOR + AVANCO);
        PORTD |= RECUO;
    }
    if ((PINB & S1) == 0)
    {
        PORTD &= ~RECUO;
    }
    
    

}