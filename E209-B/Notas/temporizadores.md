# Temporizadores
## Modos
- Normal
- CTC: Interrupção por comparação (utilizado nas aulas) -> WSG00 = 0, WSG01 = 1
- PWM: Gera forma de onda para controlar
- PWM Fast

## Timers ATMEGA32
- TIMER0
- TIMER1
- TIMER2

## Contagem
- contagem = (Tempo desejado/período de clock) - 1
- Delay max, para clock 16 MHz:
    - 8 bits: 16 us
    - 16 bits: 4 ms
- Prescaler: Resolução X Duração (inversamente proporcionais)

## Prescaler

    Prescaler    Freq. Clock    Período   Delay Max (8 bits)
    1             16 MHz       62,5ns     16 us
    8             2  MHz       500 ns     128 us
    64            250 kHz      4 micro    1024 us
    256           62,5 kHz     16 micro   4096 us
    1024          15,625 kHz   64 micro   16384 us

Obs.: Para escolher o prescaler, o delay desejado deve ser divisível pelo periodo do prescaler!!

## Registradores
Obs.: X pode ser 0, 1 ou 2
- TCCRXA: Controle A - > Modo de funcionamento
    - TCCR0A

                  7     6       5       4       3    2      1      0
               COM0A1 COM0A0  COM0B1  COM0B0    X    X    WGM01  WGM00
                  0     0       ->   Operação normal do pino, OC0A desconectado
                  0     1       ->   Mudança de estado de OC0B na igualdade
                  1     0       ->   É limpo na igualdade da comparação
                  1     1       ->   É ativo na igualdade da comparação


    - Obs1.: Mesmo vale para COM0B
    - Obs2.: Usamos apenas WGM00 = 0

- TCCRXB: Controle B -> Clock
    - TCCR0B    
                  
          CS02    CS01    CS00
            0      0        0
            0      0        1
            0      1        0  -> Prescaler = 8
            0      1        1
            1      0        0
            1      0        1
            1      1        0
            1      1        1

- TCNT0: Contagem no TC0
- OCR0A: Compara valor com o TCNT0 p/ gerar interrupção no pino OC0A
- OCR0B: Compara valor com o TCNT0 p/ gerar interrupção no pino OC0B
- TIMSK0: 

        7    6    5   4   3    2       1      0
        X    X    X   X   X  OCIE0B  OCIE0A  TOIE0

## Passos
1. Configurar o modo de operação do timer e o divisor de clock: TCCR0A e TCCR0B. 
2. Configurar o valor máximo de contagem: OCR0A 
3. Habilitar a interrupção do comparador desejado: TIMSK0 
4. Habilitar a interrupção global do microcontrolador: sei(); 