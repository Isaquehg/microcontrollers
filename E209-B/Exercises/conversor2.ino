/*
3. (Médio) Elaborar um firmware para ler e apresentar na Serial a temperatura de um PT100 (sensor de 
temperatura) operando em escala cheia transmitido por um transmissor de corrente padronizado de 4 
à 20 mA, cuja corrente de saída alimenta um resistor de 250 Ohms colocado em paralelo ao pino PC1.
*/
#define PINO 1
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
		ADMUX = (ADMUX & 0xF8) | PINO;
		
		//Leitura do ADC (Sem média)
		ADCSRA |= (1 << ADSC); //Inicia a conversão
		
		while((ADCSRA & (1<<ADSC)) == (1<<ADSC)); //Esperar a conversão
		
		Leitura_AD = ADC; //Armazenamento da leitura
		
		tensao = (Leitura_AD * 5) / 1023.0; //Cálculo da Tensão
		
		Serial.println(tensao);
	}
	
}