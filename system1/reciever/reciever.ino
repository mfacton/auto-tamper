#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSN A0
#define CE A1

RF24 radio(CE, CSN, 1000000);

const byte address[6] = "00001";
byte dataIn = 0;
byte dataOut = 0;
long lastTime[8];

//variables
bool delayCheck = 10;
long timeOut = 100; //millis
byte buffSize = 5;

void setup () {
  Serial.begin(9600);
  
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(1);
  radio.openReadingPipe(1, address);
  radio.startListening();
  
  for (byte i = 0; i < 8; i++) {
    lastTime[i] = 0;
  }
}

void loop () {
  long currentTime = millis();
  if (radio.available()) {
    radio.read(&dataIn, 1);
    for (byte i = 0; i < 8; i++) {
      if ((dataIn >> (7-i)) & 1) {
        lastTime[i] = currentTime;
      }else{
        lastTime[i] = 0;
      }
    }
  }
  byte calcData = 0;
  for (byte i = 0; i < 8; i++) {
    calcData = calcData << 1;
    if (currentTime - lastTime[i] <= timeOut) {
      calcData = calcData | ((dataIn >> (7-i)) & 1);
    }
  }
  if (calcData != dataOut) {
    dataOut = calcData;
    for (byte i = 0; i < buffSize; i++) {
      Serial.write(dataOut);
    }
  }
    delay(delayCheck);
}
