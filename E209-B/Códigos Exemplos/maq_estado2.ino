/*
  Definir os Pinos de Entrada e Saída
*/

#define Desligado 0
#define Ligado 1
#define Espera 10
#define Enchendo 20
#define Medicao 30
#define Frio 40
#define Normal 50
#define Alarme 60

int main() {

  unsigned char estado = Espera, botaoLiga, botaoACK, sensorS1;
  unsigned int sensorTemperatura;
  float temperatura;

  /*
  
  Configurar pinos de entrada e saída

  Sensores (S1 e Temperatura) e Botôes (Liga e ACK) são entradas

  Bomba, Motor, Sinalizador, Buzzer e Válvulas (V1 e V3) são saídas
  
  */

  for (;;) {

    switch (estado) {

      case Espera:
        //Realizar leitura do botão liga

        if (botaoLiga == Ligado)
          estado = Enchendo;
        break;

      case Enchendo:
        //Ligar a bomba

        //Realizar leitura do sensor S1

        if (sensorS1 == Ligado){
          //Desligar a bomba
          estado = Medicao;
        }
        break;

      case Medicao:

        //Espera 5 segundos

        //Realiza a leitura do sensor de temperatura e as conversões de escalas

        if (temperatura < 28.0)
          estado = Frio;
        else if (temperatura < 35.0)
          estado = Normal;
        else
          estado = Alarme;
        break;

      case Frio;

        //Liga V3 por 5 segundos
        //Liga M1 por 4 segundos
        //Liga V1 por 2 segundos
        //Liga M1 por 2 segundos

        estado = Espera;
        break;

        case Normal;

        //Liga V3 por 5 segundos
        //Liga M1 por 3 segundos
        //Liga V1 por 4 segundos
        //Liga M1 por 2 segundos

        estado = Espera;
        break;

        case Alarme:

        //Pisca o Sinalizador com frequência de 2 Hz

        //Aciona o Buzzer

        //Realiza a leitura do botão de reconhecimento

        if (botaoACK == Ligado)
          estado = Espera;
        break;
    }
  }
}