//Gerar um cronômetro H:M:S com print na Serial

unsigned int cont = 0;
unsigned char hora = 0, minuto = 0, segundo = 0;

int main (){

  Serial.begin(115200);
  //Configurar pino de saída?? Não
  //Configurar Pull-up?? Não

  //Configuração do Temporizador - Modo Comparador
  TCCR0A = (1 << WGM01); //Configuração do modo de funcionamento para Comparador
  TCCR0B = (1 << CS01); //Pre-scaler de 8 (Frequência de 2MHz - Período de 500 ns em cada contagem)

  OCR0A = 199; //200 contagens de 500 ns, o que gera uma interrupção a cada 100 us

  TIMSK0 = (1 << OCIE0A); //Gerar uma interrupção no estouro do comparador A

  sei();

  for(;;){}
}

//O que acontece a cada 100 us???
ISR(TIMER0_COMPA_vect){

  cont++;

  if(cont == 10000){
    cont = 0;
    segundo++;
    if(segundo == 60){
      segundo = 0;
      minuto++;
      if(minuto == 60){
        minuto = 0;
        hora++;
        if(hora == 24){
          hora = 0;
        }
      }
    }
    Serial.print(hora);
    Serial.print(":");
    Serial.print(minuto);
    Serial.print(":");
    Serial.println(segundo);
  }
}