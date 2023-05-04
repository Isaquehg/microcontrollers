/* 4. Elaborar um firmware, para manter um motor (PD6) ligado por 8 segundos sempre que o 
botão LIGA/NA (PB1 – Interrupção) for pressionado. A velocidade desse motor será incrementada em 
12,5% (31.875 de 255) a cada segundo. É preciso prever o desligamento através do botão DESLIGA/NF (PB2 – 
Interrupção) em qualquer momento da operação.
*/
#define MOTOR (1 << PD6)
#define LIGA (1 << PB1)
#define DESLIGA (1 << PB2)

ISR()

int main(){



    sei();

    while(1){
        continue;
    }
}