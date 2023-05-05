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
bool imAlive = false;

ISR(PCINT0_vect){
    if(!(PINB & LIGA)){
        bool imAlive = true;
    }
    //Se desliga for pressionado
    if (!(PINB & DESLIGA)){
        PORTD &= ~MOTOR;
        DC = 0;
        OCR0A = int(DC);
        contTotal = 0;
        cont = 0;
    }
}


//O que acontece a cada 100 us???
ISR(TIMER2_COMPA_vect){
    cont++;
    if(cont == 10000){
        contTotal += 1;
    }
    Serial.print("Cont: ");
    Serial.println(cont);
}

int main(){
    Serial.begin(115200);
    DDRD = MOTOR;
    PORTD &= ~MOTOR;

    PORTB = (LIGA + DESLIGA);

    //Timer 0 - PWM
    //16.38ms
    TCCR0A = (1 << COM0A1) + (0 << COM0A0) + (1 << WGM01) + (1 << WGM00);
    TCCR0B = (1 << CS02) + (1 << CS00);
    OCR0A = 0;

    //Timer 2 - TIMER
    //Configuração do Temporizador - Modo Comparador
    TCCR2A = (1 << WGM01); //Configuração do modo de funcionamento para Comparador
    TCCR2B = (1 << CS01); //Pre-scaler de 8 (Frequência de 2MHz - Período de 500 ns em cada contagem)

    OCR2A = 199; //200 contagens de 500 ns, o que gera uma interrupção a cada 100 us

    TIMSK2 = (1 << OCIE1A); //Gerar uma interrupção no estouro do comparador A

    //Configurando interrupções em PB0 e PB1
    PCICR = (1 << PCIE0);//habilita interrupções no portal B
    PCMSK0 = (LIGA + DESLIGA);

    sei();


    while(1)
    {
        if(imAlive){
            //se passar 1 segundo
            if(cont == 10000){
                cont = 0;
                DC += 31.87;
            }
            if(DC > 255){
                DC = 0;
                OCR0A = int(DC);
            }
            if(contTotal == 8){
                PORTD &= ~MOTOR;
                contTotal = 0;
                cont = 0;
                imAlive = false;
            }
        }
        Serial.print("Duty Cycle: ");
        Serial.println(DC);
        Serial.print("OCR0A: ");
        Serial.println(OCR0A);
        Serial.print("Cont. Total: ");
        Serial.println(contTotal);
        Serial.print("I'm Alive: ");
        Serial.println(imAlive);
    }
}