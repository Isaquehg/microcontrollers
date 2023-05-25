#define PINO 2
#define LED PD6

unsigned int Leitura_AD;
float tensao;
//float DC = 0;

int main(){
	Serial.begin(115200);
  
    DDRD |= LED;
    PORTD &= ~LED;
  
    //Fast-PWM
    TCCR0A = (1 << COM0A1) + (1 << WGM01) + (1 << WGM00);
    TCCR0B = (1 << CS02) + (1 << CS00);
    //OCR0A = 0;
	
    //ADC
	ADMUX = (1 << REFS0); //Utiliza 5V como referência (1023)
	
	ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0); //Habilita ADC e PS 128 (10 bits)
	
	ADCSRB = 0; //Conversão Única
	
	DIDR0 = (1 << PINO); //Desabilita o PC4 como pino digital - Não obrigatório
  
  	sei();
  
	for(;;){
	
		//Determinar o pino de leitura
		ADMUX = (ADMUX & 0xF8) | PINO;
		
		ADCSRA |= (1 << ADSC); //Inicia a conversão
		
		while((ADCSRA & (1<<ADSC)) == (1<<ADSC)); //Esperar a conversão
		
		Leitura_AD = ADC; //Armazenamento da leitura
		
		tensao = (Leitura_AD * 5) / 1023.0; //Cálculo da Tensão

        OCR0A = int(tensao);
		
		Serial.println(tensao);
        Serial.println(OCR0A);
    }
}