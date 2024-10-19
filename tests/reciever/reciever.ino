/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 8, 1000000); // CE, CSN

const byte address[6] = "00001";
byte data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(1);
  radio.openReadingPipe(1, address);
  radio.startListening();
}


void loop() {
  if (radio.available()) {
    radio.read(&data, 1);
    printByte(data);
    data = 0;
  }
}

void printByte(uint8_t val) {
  for (int i = 0; i < 8; i++)
  {
      bool b = val & 0x80;
      Serial.print(b);
      val = val << 1;
  }
  Serial.println();
}
