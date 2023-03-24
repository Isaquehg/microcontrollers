/*
chave seletora: PD3
botao liga: PC0
botao desliga: PC1
sensor_baixo: PD4
sensor_alto: PD5
bomba: PB0
*/
//------------------------------------------------------------
//inclusão de bibliotecas

//declaração de constantes

//declaraao de var global

//declaração de funções

int main(){
    //direção pinos (utiliza apenas os portais de saida)
    DDRB = 0b00000001; //PB0 como saída
    //DDRC
    //DDRD

    //configurar os resistores de pull-up para botões q ativam GND
    //PORTB = 
    PORTC = 0b00000010; //PC com resistor de pull-up habilitado
    //PORTD

    //desligar saídas que precisam comecar desligadas
    PORTB = 0b00000000;

    for(;;){
        //leitura de entrada da chave PD3
        short int chave;
        chave = PIND & 0b00001000; //zerar posições, exceto posicao da chave e realiza leitura

        //verificar se esta no modo manual
        if(chave == 0b00001000){
            //leitura do botao desliga PC1
            short int desliga;
            desliga = PINC & 0b0000010;

            if(desliga == 0b00000000){
                PORTB = 0b00000000;
            }

            //leitura botao liga PC0
            short int liga;
            liga = PINC & 0b00000001;

            if(liga == 0b00000001){
                PORTB = 0b00000001;//ligar bomba
            }
        }
        //chave modo automatico
        else{
            //leitura dos sensores
            short int baixo, alto; //PD4 e PD5, respectivamente
            baixo = PIND & 0b00010000;
            alto = PIND & 0b00100000;

            if((baixo == 0b000100000) && (alto == 0b00100000)) //ambos ativados, deve desligar
                PORTB = 0b00000000;
            else if((baixo == 0) && (alto == 0))//ambos desativados, ligar bomba
                PORTB = 0b00000001;
        }
    }
}