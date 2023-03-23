#include <iostream>
#include <iomanip>

using namespace std;

void teste(int aux[]){
    int par, impar, neg, pos = 0;

    for(int i = 0; i < 5; i ++){
        if(aux [i] % 2 == 0)
            par ++;
        else
            impar ++;
        if(aux[i] >= 0)
            pos ++;
        else
            neg ++;
    }
    cout << "Quantidade de numeros pares: " << par << endl;
    cout << "Quantidade de numeros impares: " << impar << endl;
    cout << "Quantidade de numeros positivos: " << pos << endl;
    cout << "Quantidade de numeros negativos: " << neg << endl;
}

int main(){
    int ntestes;
    int aux;
    int numeros[6];

    cin >> ntestes;


    while(ntestes > 0){
        aux = 0;
        for(int i = 0; i < aux; i ++){
            cin >> numeros[aux];
        }
        teste(numeros);
    }

    return 0;
}