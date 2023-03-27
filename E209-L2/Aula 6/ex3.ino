#define LIGA (1 << PB0)
#define DESLIGA (1 <<PB1)
#define S1 (1 << PB2)
#define S2 (1 << PB3)
#define S3 (1 << PB4)
#define M1 (1 << PD1)
#define LED_ON (1 << PD2)
#define LED_OFF (1 << PD3)
#define LED_ALARME (1 << PD4)

int main(){
    DDRD = LED_ALARME + LED_ON + LED_OFF + M1;


    for(;;){

    }
}