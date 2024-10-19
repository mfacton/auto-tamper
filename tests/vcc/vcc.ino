unsigned int readVcc(void) {
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  ADMUX = (1<<REFS0) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= (1<<ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
  unsigned int result = ADC;
  result = 1125300UL / (unsigned long)result; // Calculate Vcc (in mV); 1125300 = 1.1*1024*1000
  return result; // Vcc in millivolts
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println("voltage");
  Serial.println(readVcc());
  delay(100);
}
