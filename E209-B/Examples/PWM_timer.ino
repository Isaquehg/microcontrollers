#define BOTAO (1<<PC0)
#define MOTOR (1<<PD6)

float DC = 0;

int main(){
  
  Serial.begin(115200);
  DDRD = MOTOR;
  
  PORTC = BOTAO;
  
  PORTD &= ~MOTOR;
  
  TCCR0A = (1<<COM0A1) + (0<<COM0A0) + (1<<WGM01) + (1<<WGM00);
  TCCR0B = (1<<CS02) + (1<<CS00);
  OCR0A = 0;
  
  PCICR = (1<<PCIE1);
  PCMSK1 = BOTAO;
  
  sei();
  
  for(;;){}
  
}

ISR(PCINT1_vect){
  
  if(!(PINC & BOTAO)){
    DC += 25.5;
    if(DC > 255)
      DC = 0;
    OCR0A = int(DC);
    Serial.print("Duty Cycle: ");
    Serial.println(DC);
    Serial.print("OCROA: ");
    Serial.println(OCR0A);
  }
}