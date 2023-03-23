//Inclusão de Bibliotecas

//Declaração de Constantes
#define BOMBA (1<<PB0)
#define LIGA (1<<PC0)
#define DESLIGA (1<<PC1)
#define CHAVE (1<<PD3)
#define BAIXO (1<<PD4)
#define ALTO (1<<PD5)

//Declaração de Variável Global

//Declaração de Funções

int main(){
  //Configurar a direção dos pinos
  DDRB = BOMBA; //PB0 declarado como saída
  //DDRC
  //DDRD

  //Configurar os Resistores de Pull-up
  //PORTB
  PORTC = DESLIGA; //PC1 com resistor de pull-up habilitado
  //PORTD

  //Desligar as saídas que precisam começar desligadas
  PORTB &= ~BOMBA;

  for(;;){
    //Leitura da Chave (PD3)
    short int chave;
    chave = PIND & CHAVE; //Realiza a leitura da chave

    //Verificar se está no modo manual
    if(chave == CHAVE){ //Chave no modo manual
      //Leitura do Botão Desliga (PC1)
      short int desliga;
      desliga = PINC & DESLIGA;

      if(!(desliga == DESLIGA))
        PORTB &= ~BOMBA; //Desligar a bomba

      //Leitura do Botão Liga (PC0)
      short int liga;
      liga = PINC & LIGA;

      if(liga == LIGA)
        PORTB |= BOMBA; //Ligar a bomba

    }
    else{ //Chave no modo automático

      //Leitura dos Sensores Baixo (PD4) e Alto (PD5)
      short int baixo, alto;
      baixo = PIND & BAIXO;
      alto = PIND & ALTO; 

      if((baixo == BAIXO) && (alto == ALTO)) //Ambos ativados
        PORTB &= ~BOMBA; //Desligar a bomba

      else if((!(baixo == BAIXO)) && (!(alto == ALTO))) //Ambos desativados
        PORTB |= BOMBA; //Ligar a bomba
    }
  }
}