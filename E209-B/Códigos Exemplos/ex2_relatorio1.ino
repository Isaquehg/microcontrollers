// Definição das constantes
#define MOTOR (1 << PD2)
#define LIGA (1 << PB0)
#define DESLIGA (1 << PB1)
#define ALARME (1 << PD6)
#define SENSOR (1 << PB3)

int main(void)
{
  // Configurar a direção dos pinos
  DDRD = MOTOR | ALARME; // PD0 declarado como saída

  // Desliga as saídas que precisam iniciar desligadas
  PORTD &= ~(MOTOR | ALARME);

  // Super Loop
  while (1)
  {
	// Verifica se o botão LIGA foi pressionado e se o sensor não está acionado
    if (!(PINB & LIGA) && (PINB & SENSOR))
    {
      PORTD &= ~ALARME; // desativando o alarme
      PORTD |= MOTOR; // ativando o motor
     _delay_ms(100); // delay para evitar bouncing
    }
    // Verifica se o botão DESLIGA foi pressionado
    if (!(PINB & DESLIGA))
    {
      PORTD &= ~MOTOR; // desativando o motor
     _delay_ms(100); // delay para evitar bouncing
    }
    // Verifica se o SENSOR foi acionado
    if(!(PINB & SENSOR))
    {
      PORTD &= ~MOTOR; // desativando o motor
      PORTD |= ALARME; // ativando o alarme
      _delay_ms(100); // delay para evitar bouncing
    }
  }
}