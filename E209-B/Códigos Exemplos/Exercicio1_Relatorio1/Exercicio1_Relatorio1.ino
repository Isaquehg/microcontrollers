//Inclusão de Bibliotecas

//Declaração de Constantes

//Declaração de Variável Global

//Declaração de Funções

int main(){

  //Configurar a direção dos pinos
  DDRB = 0b00000001; //PB0 declarado como saída
  //DDRC
  //DDRD

  //Configurar os Resistores de Pull-up
  //PORTB
  PORTC = 0b00000010; //PC1 com resistor de pull-up habilitado
  //PORTD

  //Desligar as saídas que precisam começar desligadas
  PORTB = 0b00000000;

  for(;;){

    //Leitura da Chave (PD3)
    short int chave;
    chave = PIND & 0b00001000; //Realiza a leitura da chave

    //Verificar se está no modo manual
    if(chave == 0b00001000){ //Chave no modo manual
      //Leitura do Botão Desliga (PC1)
      short int desliga;
      desliga = PINC & 0b00000010;

      if(desliga == 0b00000000)
        PORTB = 0b00000000; //Desligar a bomba

      //Leitura do Botão Liga (PC0)
      short int liga;
      liga = PINC & 0b00000001;

      if(liga == 0b00000001)
        PORTB = 0b00000001; //Ligar a bomba

    }
    else{ //Chave no modo automático

      //Leitura dos Sensores Baixo (PD4) e Alto (PD5)
      short int baixo, alto;
      baixo = PIND & 0b00010000;
      alto = PIND & 0b00100000; 

      if((baixo == 0b00010000) && (alto == 0b00100000)) //Ambos ativados
        PORTB = 0b00000000; //Desligar a bomba

      else if((baixo == 0) && (alto == 0)) //Ambos desativados
        PORTB = 0b00000001; //Ligar a bomba
    }



  }

}