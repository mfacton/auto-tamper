#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define K2 9
#define K3 8
#define K4 7
#define K5 6
#define K6 5
#define K7 4
#define K8 3
#define K9 A3
#define K10 A2

#define POW 2

#define LED 10

#define UP K4
#define DOWN K3
#define FWD K9
#define BACK K10
#define PROG1 K5
#define PROG2 K7
#define PROG3 K8
#define STOP K6
#define EXTRA_BUTTON K2

#define CSN A1
#define CE A0

RF24 radio(CE, CSN, 1000000);

const byte address[6] = "00001";

const byte pins[8] = {DOWN, UP, BACK, FWD, STOP, PROG3, PROG2, PROG1};
byte data = 0;
long lastInput;

//variables
uint16_t delaySend = 20;
uint16_t sendTimeout = 1000;
uint16_t powerTimeout = 30000;

void setup() {
  pinMode(POW, OUTPUT);
  digitalWrite(POW, HIGH);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  pinMode(LED, OUTPUT);
  pinMode(EXTRA_BUTTON, INPUT);

  for (byte i = 0; i < 8; i++) {
    pinMode(pins[i], INPUT);
  }
  
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(1); //bytes
  radio.openWritingPipe(address);
  radio.stopListening();
  
  lastInput = millis();
}

void loop() {
  long currentTime = millis();
  data = 0;
  for (byte i = 0; i < 8; i++) {
    data |= (!digitalRead(pins[i])) << i;
  }
  if (data != 0) {
    lastInput = currentTime;
  }
  const long delta = currentTime - lastInput;
  if (delta < sendTimeout) {
    radio.write(&data, 1);
  }
  if (delta > powerTimeout) {
    digitalWrite(POW, LOW);
  }
  delay(delaySend);
}
