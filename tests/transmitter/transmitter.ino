/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8, 1000000); // CE, CSN

const byte address[6] = "00001";
const byte data1 = 0b11101011;
const byte data2 = 0b10001011;

bool sig;

void setup() {
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(1); //bytes
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  radio.write(sig?(&data1):(&data2), 1);
  sig = !sig;
  delay(50);
}
