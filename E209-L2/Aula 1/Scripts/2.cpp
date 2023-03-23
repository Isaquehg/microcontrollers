#include <iostream>
#include <iomanip>

using namespace std;

int main(){
    int num;
    int dividendo;

    cin >> num;
    dividendo = num - 1;

    if(num <= 50 && num >= 0){
        while(dividendo > 0){
            cout << "Resto da divisao de " << num << " por " << dividendo << ":" << num % dividendo << endl;
            dividendo --;
        }

    }

    return 0;
}