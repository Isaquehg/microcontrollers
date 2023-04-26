# Interrupção - INT0 & INT1
É um recurso que permite interromper o fluxo de execução para atender uma demanda urgente
## Exemplos
- Conversão concluída no ADC
- Temporizador transbordou
- Se um byte chegou na porta Serial

## Resposta ao pressionar um botão
- Varredura: Utiliza processamento desnecessário
- ISR: Interrompe a execução quando detectar movimento no pino de entrada

## Tipos
- Mascarável: 
- Fixas

## Interrupções INT

    int main(){}
    ISR(INT0_vect){}
    ISR(INT1_vect){}

1. INT0 - INT0_vect
Interrupção Externa do Pino PD2

2. INT1 - INT1_vect
INterrupção Externa do Pino PD3

## Registrador
### EICRA - Controla o modo de funcionamento da INT0 e INT1
- bits ISC00 e ISC01: INT0
- bits ISC11 e ISC10: INT1

        0b0000 ISC11 ISC10 ISC01 ISC00
                0     0     0     0    -> Gera interrupção se o pino estiver em LOW
                0     1     0     1    -> Gera interrupção se o pino ALTERAR seu estado
                1     0     1     0    -> Gera interrupção se o pino estiver em FALLING EDGE
                1     1     1     1    -> Gera interrupção se o pino estiver em RISING EDGE

### EIMSK - Habilita a possibilidade das interrupções INT0 e INT1 acontecerem
- 0: desabilitado
- 1: habilitado

        0b000000 INTF1 INTF0
                   0     0     -> Ambos desabilitados
                   1     0
                   0     1
                   1     1     -> Ambos habilitados

# Interrupção PCINT
Acontece em TODA mudança de estado!
- PCICR: Utilizado para habilitar a interrupção no portal
- PCMSKn: Especifica qual pino do portal terá interrupção
        
        PCICR = 0b  0  0  0  0  0  PCIE2  PCIE1 PCIE0
        Portal:                      B      C     D

        PCMSKn = 0b  Px7  Px6  Px5  Px4  Px3  Px2  Px1  Px0

        n = 0 -> Portal B
        n = 1 -> Portal C
        n = 2 -> Portal D
