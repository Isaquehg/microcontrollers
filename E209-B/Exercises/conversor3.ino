/*
(Médio) Elaborar um firmware para apresentar na Serial a média das tensões lidas nos pinos PC0 a PC5 
(0 a 5V).
*/

#define PINO0 0
#define PINO1 1
#define PINO2 2
#define PINO3 3
#define PINO4 4
#define PINO5 5
#define AMOSTRAS 50

unsigned int Leitura_AD;
float tensao;

int main(){

	Serial.begin(115200);
	
	ADMUX = (0 << REFS1) + (1 << REFS0); //Utiliza 5V como referência (1023)
	
	ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0); //Habilita ADC e PS 128 (10 bits)
	
	ADCSRB = 0; //Conversão Única
	
	//DIDR0 = (1 << PINO); //Desabilita o PC4 como pino digital - Não obrigatório
	
	for(;;){
	
		//Determinar o pino de leitura
		ADMUX = (ADMUX & 0xF8) | (PINO0 + PINO1 + PINO2 + PINO3 + PINO4 + PINO5);

		//Leitura do ADC (Com média)
		unsigned int SomaLeitura = 0, MediaLeitura;
		for(int i = 0; i < AMOSTRAS; i++){
		
			ADCSRA |= (1 << ADSC); //Inicia a conversão
		
			while((ADCSRA & (1 << ADSC)) == (1 << ADSC)); //Esperar a conversão
		
			Leitura_AD = ADC;
			
			SomaLeitura += Leitura_AD;
		}
		
		MediaLeitura = SomaLeitura / AMOSTRAS;
		
		tensao = (MediaLeitura * 5) / 1023.0; //Cálculo da Tensão
		
		Serial.println(tensao);
	}
	
}