#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(A1, A0, 1000000);

#define SERIAL_DEBUG

constexpr uint8_t address[6] = "00001";//0x3030303031 000010

uint8_t data;
//variables

FILE f_out;

int sput(char c, __attribute__((unused)) FILE* f) {return !Serial.write(c);}

inline void initRF() {
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setPayloadSize(1);
  radio.openReadingPipe(1, address);
  radio.startListening();
}

void setup () {
  pinMode(8, OUTPUT);
  Serial.begin(9600);

  delay(1000);

  Serial.println("output");

  fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE); // cf https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaf41f158c022cbb6203ccd87d27301226
  stdout = &f_out;



  initRF();

  delay(1000);

  radio.printDetails();
  // radio.printPrettyDetails();
}

void loop () {
  if (radio.available()) {
    radio.read(&data, 1);
    // Serial.println(data);
  }
}