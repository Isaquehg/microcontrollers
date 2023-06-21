#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define F_CPU 16000000UL

// Definições de pinos
#define SENSOR_DE_GOTAS PD1 // Botão no PD1
#define MOTOR PB7 // Motor no PB7
#define BUZZER PD0 // Buzzer no PD0

// Variáveis globais
volatile uint16_t gotas = 0; // Variável auxiliar para contar as contas
volatile uint16_t contador = 0; // Variável auxiliar para contar tempo

// Função para limpar o buffer de recepção
void UART_LimparBuffer() {
  while (UCSR0A & (1 << RXC0))
    volatile char lixo = UDR0; // Lê e descarta o caractere do buffer de recepção
}

// Função para transmitir um caractere via UART
void UART_TransmiteChar(unsigned char dados) {
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = dados;
}

// Função para transmitir uma string via UART
void UART_TransmiteString(const char* dados) {
  for (size_t i = 0; i < strlen(dados); i++) {
    UART_TransmiteChar(dados[i]);
  }
}

// Função para receber um caractere via UART
unsigned char UART_RecebeChar() {
  while (!(UCSR0A & (1 << RXC0)));
  return UDR0;
}

// Interrupção para o sensor de gotas
ISR(INT1_vect) {
  gotas++; // Incrementa a variável gotas
}

// Interrupção de tempo
ISR(TIMER1_COMPA_vect) {
  contador++; // Incrementa a variável contador
}

// Interrupção do sensor de bolhas
ISR(ADC_vect){
  while(ADCSRA & (1<<ADSC)); // Aguarda o fim da conversão
  int conv = map(ADC,0,1023,0,255); //Modifica a escala (0~1023 -> 0~255)
  ADCSRA |= (1<<ADSC);        //Inicia uma nova conversão
  if (conv > 150){
    PORTD |= (1 << BUZZER); // Liga o buzzer
    OCR0A = 0; // Desliga o motor
  }
  else
    PORTD &= ~(1 << BUZZER);
}

int main() 
{
  // Inicializando a UART
  unsigned int prescaler = F_CPU / (16UL * (9600 - 1)); // Define UBRR
  UBRR0H = (unsigned char)(prescaler >> 8);
  UBRR0L = (unsigned char)(prescaler);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Ativa o transmissor e o receptor
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Comunicção assíncrona, sem paridade, um bit de parada e 8 bits de dados

  // CONFIGURANDO ENTRADAS E SAÍDAS
  DDRD &= ~(1 << SENSOR_DE_GOTAS); // Define sensor de gotas como entrada
  DDRD |= (1 << BUZZER); // Define alarme como saída
  DDRB |= (1 << MOTOR); // Define motor como saída
  DDRC |= 0; // Define potenciometro como entrada
  PORTD = 0; // Saídas iniciam desligadas
  PORTD |= (1 << SENSOR_DE_GOTAS); // Habilita pull-up no botão do sensor de gotas
  PORTB = 0; // Saídas iniciam desligadas

  // Configurar interrupção externa no pino PD1 para o sensor de gotas
  EICRA |= (1 << ISC01); // Configurar para borda de descida
  EIMSK |= (1 << INT1);  // Habilitar interrupção INT1
  
  // Configurar PWM no TIMER 0 
  TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); // Modo não invertido e PWM rápido
  TCCR0B |= (1 << CS01); // Prescaler 8
  OCR0A = 0; // Inicialmente desligado

  // Configura TIMER 1 para fazer contagens de tempo
  TCCR1B |= (1 << WGM12); // Modo CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
  OCR1A = 15624;  // 16 MHz / (1024 * 1 Hz) - 1
  TIMSK1 |= (1 << OCIE1A); // INT por comparação com o OCR1A
  TCNT1 = 0; // Inicia em 0

  // ADC
  ADMUX |= (1<<REFS0); // Vref de 5V
  ADCSRA |= (1 << ADIE); // Habilitar interrupção por comparação de ADC
  ADCSRA |= (1<<ADEN); // Liga o ADC
  ADCSRA |= (1<<ADSC); // Incializa a conversão
  ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Prescaler de 128 (ADPS = 111)

  // Habilita interrupções globais
  sei();

  while (1) 
  {
    // Espera um segundo (Interrupção que conta um segundo em cima da main)
    contador = 0;  // Zerar o contador
    // Aguardar até que o contador atinja o valor de 1 segundo (1 Hz)
    while (contador < 1);

    // Variáveis para armazenar o volume e o tempo
    char volumestring[4];
    char tempostring[4];

    // Limpar buffer de recepção antes de fazer uma nova pergunta
    UART_LimparBuffer();

    // Pergunta o volume
    UART_TransmiteString("\nEntre com o volume em ml (3 dígitos): ");

    // Recebe o volume
    for (int i = 0; i < 3; i++) {
      volumestring[i] = UART_RecebeChar();
      UART_TransmiteChar(volumestring[i]);
    }
    volumestring[3] = '\0';

    // Armazena o string de volume em outra variável
    char bufferVolume[4];
    strcpy(bufferVolume, volumestring);

    // Convertendo a string de volume para um valor int e armazenando
    int volumeINT = atoi(volumestring);

    // Pula uma linha
    UART_TransmiteString("\r\n");

    // Limpar buffer de recepção antes de fazer uma nova pergunta
    UART_LimparBuffer();

    // Pergunta o tempo
    UART_TransmiteString("Entre com o tempo de infusao em minutos (3 digitos): ");

    // Recebe o tempo
    for (int i = 0; i < 3; i++) {
      tempostring[i] = UART_RecebeChar();
      UART_TransmiteChar(tempostring[i]);
    }
    tempostring[3] = '\0';

    // Pula uma linha
    UART_TransmiteString("\r\n");

    // Convertendo a string de tempo para um valor int e armazenando
    int tempoINT = atoi(tempostring);
    
    // Armazena o string de tempo em outra variável
    char bufferTempo[4];
    strcpy(bufferTempo, tempostring);
    
    // Mostra os dados inseridos através da UART
    UART_TransmiteString("Volume: ");
    UART_TransmiteString(bufferVolume);
    UART_TransmiteString(" ml");
    UART_TransmiteString("\r\n"); // Pula uma linha
    UART_TransmiteString("Tempo: ");
    UART_TransmiteString(bufferTempo);
    UART_TransmiteString(" minutos");
    UART_TransmiteString("\r\n"); // Pula uma linha

    // Calcular fluxo definido e potência do motor
    float fluxo_definido = (volumeINT * 60)/ tempoINT; // Fórmulas dadas no projeto
    int potencia_do_motor = (fluxo_definido * 100) / 450; // Fórmulas dadas no projeto

    // Configurar velocidade do motor
    int pot = map(potencia_do_motor, 0, 100, 0, 255); // Convertendo para valor máximo de 255 para que a potência máxima seja 255

    if (pot > 255) // Se a potência passar do limite ela vai avisar ao usuário e retornar ao inicio do código
    {
      UART_TransmiteString("\nA potência da infusão é maior do que a suportada por este disposistivo, favor inserir os dados novamente");
      OCR0A = OCR0A; // Mantém a potência anterior
      continue; // Volta para o início do while
    }
    else
    {
      char resposta; // Variável para receber resposta do usuário
      int alterar_dados = 1; // Variável auxiliar

      // Pergunta se deseja alterar os dados
      UART_TransmiteString("Deseja alterar os dados inseridos? (s/n): ");

      while (alterar_dados) {
        // Recebe a resposta do usuário
        resposta = UART_RecebeChar();
        UART_TransmiteChar(resposta);

        // Se a resposta for não entra no loop após o while
        if (resposta == 'n') 
          alterar_dados = 0; // Não alterar os dados, sair do loop
        // Se a resposta for sim volta pro inicio do codigo
        else if (resposta == 's')
          break; // Volta para o início do código
      }

      // Se a resposta for não, ativa o motor com o PWM calculado
      if (!alterar_dados) {
        OCR0A = pot; // Ativa o PWM na potência calculada
        UART_TransmiteString("\nMotor peristáltico rotativo ativado na potencia definida"); // Avisa o usuário que o motor peristáltico foi ativado
        UART_TransmiteString("\r\n"); // Pula uma linha

        // Calcular erro
        uint16_t fluxo_real = (gotas * 60) / 20; // Fórmulas dadas no projeto
        int16_t erroint = ((fluxo_real - fluxo_definido) * 100) / fluxo_definido; // Fórmulas dadas no projeto

        // Transforma o erro em string
        char errostring[3];
        sprintf(errostring, "%d", erroint);

        // Mostra o erro pro usuário
        UART_TransmiteString("Erro: ");
        UART_TransmiteString(errostring);
        UART_TransmiteString(" %");
      }
    }
  }
  return 0;
}