// |= -> liga
// &= ~ -> desliga

#define PD7 = 0b10000000;

int main(void){
    DDRD |= 0b10000000; //Pino PD7 como saída
    PORTD |= 0b00010000; //Habilitar resistor de Pull-up PD4
    PORTD &~ (0b10000000); //Desligar saida PD7
    
    for(;;){
        //Se o botão estiver pressionado
        if((PIND & 0B00010000) == 0){
            //Aciona a saída
            PORTD |= 0b10000000;
            _delay_ms(1000);
            //Desliga a saída
            PORTD &= ~(0b10000000);
        }
    }
}