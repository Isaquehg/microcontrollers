# Conversores
## Estrutura
- Var. -> Transdutor -> ADC -> Microcontrolador -> DAC -> Atuador
- ADC
    - Filtro low-pass
    - Amostragem/Armazenamento
        - Freq. amostragem >= 2x freq. input
        - Níveis de Quantização -> Quanto maior freq. amostragem, maior precisão
        - ATMEGA 328: 10 bits
        - Resolução = 5V / 2^10bits - 1
        - Leitura de 1 pino por vez
        - Número de amostras da média deve ser < que o valor máx da variável int soma / 1023

## Registradores
1. ADMUX - Tensão de Referência e Pino que será lido
    - ADMUX = REFS1 REFS0 ADLAR --- MUX3 MUX2 MUX1 MUX0
    - AVcc
        - REFS1 = 0 & REFS0 = 1 para utilizar AVcc, 5V
        - ADLAR = 0
        - ADMUX = (0 << REFS1) + (1 << REFS0);

2. ADCSRA - Habilitador, Start & Prescaler
    - ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0);
    - Quanto maior prescaler, maior fator de divisão

3. ADCSRB - Conversão com Timer
    - ADCSRB = 0;

4. DIDR - Desabilitar Leitura Digital
    - DIDR0 = (1 << PINO);

5. ADC - Armazenamento de 16 bits
