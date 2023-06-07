#define LIGA (1 << PC0)
#define DESLIGA (1 << PC1)
#define S1 (1 << PB0)
#define S2 (1 << PB1)
#define MOTOR (1 << PD7)

unsigned int cont = 0;//estouro timer
unsigned int segS1 = 0;//tempo parado ao acionar Sensor 1 (5,5s)
unsigned int segS2 = 0;//tempo parado ao acionar Sensor 2 (7,5s)
bool sentido = 0;//direita(0) -> S1, esquerda(1) -> S2
bool ativarTimer = 0;//caso ativado, comeca a contar

ISR(TIMER0_COMPA_vect){

    if(ativarTimer){
        cont ++;
        //Ao acionar Sensor 1
        if(PINB & S1){
            //Ao passar 0.5s
            if(cont == 5000){
                cont = 0;
                segS1 += 500;
                //Reativa o motor em sentido oposto
                if(segS1 == 5500){
                    PORTD |= MOTOR;
                }
            }
        }

        //Ao acionar Sensor 2
        if(PINB & S2){
            //Ao passar 0.5s
            if(cont == 5000){
                cont = 0;
                segS1 += 500;
                //Reativa o motor em sentido oposto
                if(segS1 == 5500){
                    PORTD |= MOTOR;
                }
            }
        }
    }
}

int main(){
    //Saídas
    DDRD |= MOTOR;
    PORTD &= ~MOTOR;

    //Configuração do Temporizador - Modo Comparador
    TCCR0A = (1 << WGM01); //Configuração do modo de funcionamento para Comparador
    TCCR0B = (1 << CS01); //Pre-scaler de 8 (Frequência de 2MHz - Período de 500 ns em cada contagem)

    OCR0A = 199; //200 contagens de 500 ns, o que gera uma interrupção a cada 100 us

    TIMSK0 = (1 << OCIE0A); //Gerar uma interrupção no estouro do comparador A

    sei();

    for(;;){
        if(PINB & LIGA)
            PORTD |= MOTOR;

        if(PINB & DESLIGA)
            PORTD &= ~MOTOR;

        //Se o S1 estiver ativado
        if(PINB & S1){
            ativarTimer = true;
            PORTD &= ~MOTOR;
        }
        //Se o S2 estiver ativado
        if(PINB & S2){
            ativarTimer = true;
            PORTD &= ~MOTOR;
        }
    }
}