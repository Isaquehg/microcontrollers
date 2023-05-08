#define BOTAO (1 << PC0)
#define LED (1 << PD6)

float DC = 0;

ISR(PCINT1_vect){
  while (!(PINC & BOTAO)){
    DC = 127.5;
    if(DC > 255)
      DC = 0;
    OCR0A = int(DC);
    Serial.print("Duty Cycle: ");
    Serial.println(DC);
    Serial.print("OCROA: ");
    Serial.println(OCR0A);
  }
}

int main(){
    Serial.begin(115200);
    DDRD = LED;
    PORTD &= ~LED;
    
    PORTC = BOTAO;
    
    TCCR0A = (1 << COM0A1) + (1 << WGM01) + (1 << WGM00);
    TCCR0B = (1 << CS02) + (1 << CS00);
    OCR0A = 0;
    
    PCICR = (1 << PCIE1);
    PCMSK1 = BOTAO;
    
    sei();
    
    for(;;){}
  
}