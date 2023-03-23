//EX PAG 9
//a
unsigned int contador;
//b
signed int entrada;
//c
unsigned int tempo;
//d
int temperatura;
//e
#include <avr.h>
//f
const int IN = 365

//EX PAG 25
unsigned char a = 85, b = 240
bool x;
//a)
x = a & b
x = 1
//b)
x = a | b;
x = 1
//c)
x = a && b;
x = 1
//d)
x = a || b;
x = 1
//e)
x = b >> 4;
x = 1
//f)
x = a << 1;
x = 1

//EX PAG 33
unsigned char cont = 0
void main (void){
    for(;;){
        P1 = ~cont;
        cont ++;
        delayms(1000);
    }
}
/*
1 -Analise o programa a seguir e responda:
a)Indique quais partes são:
    •Programa principal: Linhas 39 a 45
    •Declaração de variável global: Linha 38
    •Chamada de função: Linha 43
    •Loop Infinito: Linha 40

b) Qual o limite de armazenamento da variável cont?
É de 255 bits

c) Qual o valor carregado em cont a cada vez que o loop é executado?
É somado 1 a cada iteração

d) Qual a relação que existe entre os valores de conte P1?
P1 corresponderá sempre a cont - 1
*/

//EX PAG 34
/*
a) Para quais valores de “a” a variável j será igualada a zero?
para valores de a menores ou iguais a 0

b) Para quais valores de “a” a variável j será incrementada?
Para valores maiores que 0

c) Seria viável substituir essas estruturas “if” por “switch”?
Não seria, pois as condições estão relacionadas a uma faixa de valor, não a valores exatos
*/

//EX PAG 35
/*
a) Qual o valor final de “j” quando as estruturas 1,2 e 3 chegarem ao fim?
10, 5 e infinito, respectivamente
b) Qual das estruturas poderia ser substituída por uma estrutura do tipo:
while(1){
    j++;
}
A última estrutura, pois ambas representam loops infinitos
*/
    