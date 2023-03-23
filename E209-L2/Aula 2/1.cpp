#define CHEIO 5
#define VAZIO 0
unsigned char estado_atual = VAZIO, sensor_min = 0, sensor_max = 0, valvula = 1;

int main(void)
{
    for (;;){
        switch (estado_atual){
            case CHEIO:
                valvula = 0;
                if (sensor_min == 0)
                    estado_atual = VAZIO;
                break;
            case VAZIO:
                valvula = 1;
                if (sensor_max == 1)
                    estado_atual = CHEIO;
                break;
            default:
                break;
        }
    }
}