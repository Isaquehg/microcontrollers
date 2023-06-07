# Comunicação Serial
## Transmissão
### Tipos
- Comum - GND = 0, 1 = -5V
- Balanceada: Rx = 2P

## ATMEGA
- Txd -> PD1
- Rxd -> PD0
- Xck -> PD4

### Modo Assíncrono (UART)
- 1 start-bit
- 7/8 bits de dados
- 0/1 bit de paridade
- 1/2 stop-bits
- Canal Parado -> nível lógico 1
- Baud Rate (300, 115200, 9600, ..., divisíveis por 300) -> taxa de transmissão

### Registradores
- UDR0: Registrador de escrita e leitura (buffer)
- UCSR0A: Registrador de monitoramento
- UCSR0B: Registrador de 
- UCSR0C: Configuração principal.
7. Sincronismo do UART
6. Sincronismo do UART
5. Tratamento de Paridade
4. Tratamento de Paridade
3. Tamanho do campo de dados
2. Tamanho do campo de dados
1. Tamanho do campo de dados
0. Config. sincronismo

### Etapas de Configuração
1. Taxa de transmissão (Baud Rate)

        UBRR (INT de 16 bits) = Fosc/(16 * (BAUD - 1))

2. Seleção do modo de transmissão
    * paridade
    * 
3. 
4. 
5. 
6. Definição do fluxo