/*
Configurações padrão Arduino IDE
PB0 = 0
PB1 = 1
PB2 = 2
PB3 = 3
PB4 = 4
PB5 = 5
PB6 = 6
PB7 = 7
*/

//inclusão de bibliotecas

//declaração de constantes
#define BOMBA (1 << PB0)
#define LIGA (1 << PC0)
#define DESLIGA (1 << PC1)
#define CHAVE (1 << PD3)
#define BAIXO (1 << PD4)
#define ALTO (1 << PD5)

//declaraao de var global

//declaração de funções

int main(void){
    //direção pinos (utiliza apenas os portais de saida)
    DDRB = BOMBA;

    //configurar os resistores de pull-up para botões q ativam GND
    PORTC = DESLIGA;

    //desligar saídas que precisam comecar desligadas
    PORTB &= ~BOMBA;

    for(;;){
        //LEITURA DA CHAVE
        short int chave;
        chave = PIND & CHAVE;

        //VERIFICAR SE CHAVE ESTA NO MODO MANUAL
        if(chave == CHAVE){
            //LEITURA DA CHAVE
            short int desliga;
            desliga = PINC & DESLIGA;

            //DESLIGAR BOMBA
            if(!(desliga == DESLIGA))
                PORTB &= BOMBA;

            //LEITURA BOTAO LIGA
            short int liga;
            liga = PINC & LIGA;

            //LIGAR BOMBA
            if(liga == LIGA)
                PORTB |= BOMBA;
        }
        else{
            //Leitura dos Sensores Baixo (PD4) e Alto (PD5)
            short int baixo, alto;
            baixo = PIND & BAIXO;
            alto = PIND & ALTO;

            if((baixo == BAIXO) && (alto == ALTO))
                PORTB &= ~BOMBA;//DESLIGAR BOMBA

            else if((baixo == 0) && (alto == 0))//ambos desativados, ligar bomba
                PORTB |= BOMBA;//LIGAR BOMBA
        }


    }
}