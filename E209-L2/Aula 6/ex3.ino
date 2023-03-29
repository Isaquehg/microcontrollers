#define LIGA (1 << PB0) //botao liga motor
#define DESLIGA (1 << PB1) //botao desliga motor
#define S1 (1 << PB2) //sensor caixa grande
#define S2 (1 << PB3) //sensor caixa media
#define S3 (1 << PB4) //sensor caixa pequena
#define CH1 (1 << PB5) //chave1
#define CH2 (1 << PB6) //chave2
#define M1 (1 << PD1) //motor
#define LED_ON (1 << PD2)
#define LED_OFF (1 << PD3)
#define LED_ALARME (1 << PD4) //se passar caixa errada

int main(){
    DDRD = LED_ALARME + LED_ON + LED_OFF + M1;
    PORTD &= ~(LED_ALARME | LED_ON | M1);
    PORTD |= LED_OFF;

    int short liga;
    liga = PINB & LIGA;

    while (1)
    {
        //SE ESTIVER LIGADO PULL-UP
        if(!(liga == LIGA)){
            //DESATIVA LED_OFF E ATIVA LED_ON
            PORTD &= ~LED_OFF;
            PORTD |= (LED_ON | M1);
            
            short int ch1, ch2;
            ch1 = PINB & CH1;
            ch2 = PINB & CH2;

            short int s1, s2, s3;
            s1 = PIND & S1;
            s2 = PIND & S2;
            s3 = PIND & S3;

            //SE FOR PARA PASSAR CAIXA PEQUENA (0 & 0) -> CONSIDERANDO PULL-UP!!
            if((ch1 == CH1) && (ch2 == CH2)){
                //SE PASSAR CAIXA GRANDE OU MEDIA
                if(!(s1 == S1) || !(s2 == S2)){
                    PORTD |= LED_ALARME;
                    PORTD &= ~M1;
                }
            }

            //SE FOR PARA PASSAR CAIXA MEDIA (0 & 1)
            if((ch1 == CH1) && !(ch2 == CH2)){
                //SE PASSAR CAIXA PEQUENA OU GRANDE
                if(!(s1 == S1) || !(s3 == S3)){
                    PORTD |= LED_ALARME;
                    PORTD &= ~M1;
                }
            }

            //SE FOR PARA PASSAR CAIXA GRANDE (1 & 0)
            if(!(ch1 == CH1) && (ch2 == CH2)){
                //SE PASSAR CAIXA PEQUENA OU MEDIA
                if(!(s3 == S3) || !(s2 == S2)){
                    PORTD |= LED_ALARME;
                    PORTD &= ~M1;
                }
            }
        }
        else{
            short int m1 = PIND & M1;
            short int led_off;
            led_off = PIND & LED_OFF;
            
            //SE O MOTOR ESTIVER LIGADO, DESLIGÁ-LO
            if(m1 == M1)
                PORTD &= ~M1;
            if(!(led_off == LED_OFF)){
                PORTD |= LED_OFF;
                PORTD &= ~LED_ON;
            }
        }
    }
}