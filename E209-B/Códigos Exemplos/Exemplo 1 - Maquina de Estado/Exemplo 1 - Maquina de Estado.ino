/*
  Definir os Pinos de Entrada e Saída
*/

#define Desligado 0
#define Ligado 1
#define Automatico 20
#define Manual 10

int main() {

  unsigned char estado, sensorAlto, sensorBaixo, chave, botaoLiga, botaoDesliga;

  /*
  
  Configurar pinos de entrada e saída

  Chave Seletora, Sensores de Nível (Alto e Baixo) e Botôes (Liga e Desliga) são entradas

  Bomba é saída
  
  */

  for (;;) {

    //Realizar leitura da Chave Seletora

    if (chave == Ligado)
      estado = Manual;
    else
      estado = Automatico;

    switch (estado) {

      case Manual:
        //Realizar leitura dos botões desliga e liga

        if (botaoDesliga == Ligado)
          //Desligar a bomba
          else if (botaoLiga == Ligado)
            //Ligar a bomba
        break;

      case Automatico:

        //Realizar leitura dos sensores baixo e alto

        if (sensorAlto == Ligado && sensorBaixo == Ligado)
          //Desligar a bomba
          else if (sensorAlto == Desligado && sensorBaixo == Desligado)
            //Ligar a bomba
        break;
    }
  }
}