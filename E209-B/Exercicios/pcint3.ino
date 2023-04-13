#define LIGA (1 << PB0)
#define DESLIGA (1 << PD3)
#define S1 (1 << PB1)
#define S2 (1 << PB2)
#define S3 (1 << PD2)
#define CHAVE (1 << PB3);
#define M1 (1 << PD1) //OUTPUT
#define ALARME (1 << PD4) //OUTPUT
#define BUZZER (1 << PD5) //OUTPUT

int main(void){
    DDRD |= M1 + ALARME + BUZZER;

    //desativando outputs e config. pull-up
    PORTD &= ~(M1 + ALARME + BUZZER + DESLIGA);
    PORTB &= ~LIGA;

    PCICR = ((1 << PCIE0) + (1 << PCIE2));//habilita interrupção no portal B
    PCMSK0 = S1 + S2;
    PCMSK2 = S3;

    for(;;){

    }
}

ISR(PCINT0_vect){
    if(PINB & S1){
        
    }
}
ISR(PCINT2_vect){
    
}
