#define CHAVE (1 << PD3) //INPUT
#define LIGA (1 << PC0) //INPUT
#define DESLIGA (1 << PC1) //INPUT
#define BAIXO (1 << PD4) //INPUT
#define ALTO (1 << PD5) //INPUT
#define BOMBA (1 << PB0) //OUTPUT

int main(){
    DDRB |= BOMBA;
    short int chave;

    for (;;)
    {
        chave = PIND & CHAVE;
        //SE ESTIVER MODO MANUAL
        if(chave == CHAVE){
            short int liga;
            liga = PIND |= 
        }
    }
    
}