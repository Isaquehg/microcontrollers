#define desliga (1 << PC1)
#define liga (1 << PC0)
#define s1 (1 << PB0)
#define s2 (1 << PB1)
#define s3 (1 << PB2)
#define s4 (1 << PB3)
#define lamp (1 << PD3)
#define m1 (1 << PD0)
#define m2 (1 << PD1)
#define ele (1 << PD2) 

#define ESPERA 10
#define ESTEIRA1 20
#define ELEVADOR 30
#define ESTEIRA2 40
#define VERIF 50

int main() {
  //Declarando as saídas:
    DDRD = lamp + m1 + m2 + ele;
  PORTD &= ~(lamp + m1 + m2 + ele);

  short int estado = ESPERA;
  for (;;) {

    short int Desliga;
    Desliga = PINC & desliga;

    if (Desliga == desliga)
      estado = ESPERA;

    switch (estado) {

      case ESPERA:

        PORTD &= ~(lamp + m1 + m2 + ele);
        short int Liga;
        Liga = PINC & liga;
        if (Liga == liga)
          estado = ESTEIRA1;

        break;

      case ESTEIRA1:
        PORTD |= m1;

        short int S1;
        S1 = PINB & s1;
        if (S1 == s1)
          estado = ELEVADOR;

        break;

      case ELEVADOR:
        PORTD &= ~(m1);
        _delay_ms(3000);
        PORTD |= ele;

        short int S2;
        S2 = PINB & s2;

        if (S2 == s2)
          estado = ESTEIRA2;

        break;

      case ESTEIRA2:

        PORTD &= ~ele;
        PORTD |= m2;

        short int S3, S4;

        S3 = PINB & s3;
        S4 = PINB & s4;

        if (S3 == s3)
          estado = VERIF;

        else if (S4 == s4)
          estado = ESPERA;

        break;

      case VERIF:

        PORTD &= ~m2;
        for (int i = 0; i <= 6; i++) {
          PORTD |= lamp;
          _delay_ms(250);
          PORTD &= ~lamp;
          _delay_ms(250);
        }
        estado = ESTEIRA2;

        break;
    }
      
  }
}