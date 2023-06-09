#define MOTOR (1 << PD6)
#define LIGA (1 << PB1)
#define DESLIGA (1 << PB2)

float DC = 0;//Duty Cycle
unsigned long int cont = 0;//conta a cada 100us
short int contTotal = 0;//conta segundos

ISR(TIMER2_COMPA_vect) {
    cont++;
    //A cada 1s
    if(cont % 10000 == 0){
        contTotal ++;
        DC += 31.87; //Adiciona 12,5% ao PWM
        OCR0A = int(DC);
        Serial.print("Cont: ");
        Serial.println(cont);
        Serial.print("Duty Cycle: ");
        Serial.println(DC);
        Serial.print("Cont. Total: ");
        Serial.println(contTotal);
    }
    //Quando passar 8s, desativa tudo
    if(contTotal == 7){
        cont = 0;
        contTotal = 0;
        TCCR2B = 0;
        DC = 0;
        OCR0A = int(DC);
        Serial.print("Cont: ");
        Serial.println(cont);
        Serial.print("Duty Cycle: ");
        Serial.println(DC);
        Serial.print("Cont. Total: ");
        Serial.println(contTotal);
    }
}

ISR(PCINT0_vect) {
    //Ativar sistema
    if (!(PINB & LIGA)){
        TCCR2B = (1 << CS01); // Ativa o timer
        PORTD |= MOTOR;
    }
    //Se desliga for pressionado, desligar todo sistema
    if (!(PINB & DESLIGA)){
        cont = 0;
        contTotal = 0;
        TCCR2B = 0;
        DC = 0;
        OCR0A = 0;
        Serial.println(OCR0A);
        Serial.println(DC);
    }
}

int main() {
    Serial.begin(115200);
    //Config. output/input
    DDRD = MOTOR;
    PORTB = (LIGA + DESLIGA);

    //Timer 0 - PWM
    TCCR0A = (1 << COM0A1) + (0 << COM0A0) + (1 << WGM01) + (1 << WGM00);
    TCCR0B = (1 << CS02) + (1 << CS00);
    OCR0A = 0;

    //Timer 2 - Temporizador CTC
    TCCR2A = (1 << WGM21); //Configuração do modo de funcionamento para Comparador
    TCCR2B = 0; //Pre-scaler de 8 (Frequência de 2MHz - Período de 500 ns em cada contagem)
    OCR2A = 199; //200 contagens de 500 ns, o que gera uma interrupção a cada 100 us
    TIMSK2 = (1 << OCIE2A); //Gerar uma interrupção no estouro do comparador A

    //PCINT
    PCICR = (1 << PCIE0);//habilita interrupções no portal A
    PCMSK0 = (LIGA + DESLIGA);

    sei();

    while(1)
    {}
}
