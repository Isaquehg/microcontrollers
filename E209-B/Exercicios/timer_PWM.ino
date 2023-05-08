/* 4. Elaborar um firmware, para manter um motor (PD6) ligado por 8 segundos sempre que o 
botão LIGA/NA (PB1 – Interrupção) for pressionado. A velocidade desse motor será incrementada em 
12,5% (31.875 de 255) a cada segundo. É preciso prever o desligamento através do botão DESLIGA/NF (PB2 – 
Interrupção) em qualquer momento da operação.
*/
#define MOTOR (1 << PD6)
#define LIGA (1 << PB1)
#define DESLIGA (1 << PB2)

float DC = 0;//Duty Cycle
unsigned int cont = 0;//conta a cada 100us
short int contTotal = 0;//conta segundos

ISR(PCINT0_vect){
    Serial.println("PCINT0");
    if(!(PINB & LIGA))
        TCCR2B = (1 << CS01);
    //Se desliga for pressionado
    if (!(PINB & DESLIGA)){
        PORTD &= ~MOTOR;
        DC = 0;
        OCR0A = int(DC);
        contTotal = 0;
        cont = 0;
    }
}

//O que acontece a cada 100 us
ISR(TIMER2_COMPA_vect){
    cont++;
    //Se passar 1s e não chegar em 8s
    if(cont >= 100){
        cont = 0;
        contTotal ++;
        DC += 31.87;
        OCR0A = int(DC);
        Serial.print("Cont: ");
        Serial.println(cont);
    }
    if(contTotal > 8) {
        PORTD &= ~MOTOR;
        contTotal = 0;
        cont = 0;
        TCCR2B = 0;
        Serial.print("Cont else: ");
        Serial.println(cont);
    }
    Serial.print("Cont: ");
    Serial.println(cont);
    Serial.print("Duty Cycle: ");
    Serial.println(DC);
    Serial.print("Cont. Total: ");
    Serial.println(contTotal);
}

int main(){
    Serial.begin(115200);
    DDRD = MOTOR;
    PORTD &= ~MOTOR;

    PORTB = (LIGA + DESLIGA);

    //Timer 0 - PWM
    TCCR0A = (1 << COM0A1) + (0 << COM0A0) + (1 << WGM01) + (1 << WGM00);
    TCCR0B = (1 << CS02) + (1 << CS00);
    OCR0A = 0;

    //Timer 2 - Temporizador CTC
    TCCR2A = (1 << WGM01); //Configuração do modo de funcionamento para Comparador
    TCCR2B = 0; //Pre-scaler de 8 (Frequência de 2MHz - Período de 500 ns em cada contagem)

    OCR2A = 199; //200 contagens de 500 ns, o que gera uma interrupção a cada 100 us

    TIMSK2 = (1 << OCIE2A); //Gerar uma interrupção no estouro do comparador A

    //PCINT
    PCICR = (1 << PCIE0);//habilita interrupções no portal B
    PCMSK0 = (LIGA + DESLIGA);

    sei();

    while(1)
    {}
}