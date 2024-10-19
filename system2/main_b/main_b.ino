#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(A1, A0, 1000000);

constexpr uint8_t address[6] = "00001";
constexpr uint8_t DELAY_EDIT = 10;
constexpr uint8_t DELAY_RECIEVE = 10;
constexpr uint8_t TIMEOUT = 100;

long lastTime = 0;

uint8_t data = 0;
uint8_t lastData = 0;

bool editMode = false;
bool lastEdit = false;

bool lastSwitch = false;
bool lastDT = false;
bool lastCK = false;

//variables

inline void initRF() {
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(1);
  radio.openReadingPipe(1, address);
  radio.startListening();
}

inline void checkEdit() {
  const bool edit = !((PIND >> 6) & 1);
  if (edit && !lastEdit) {
    editMode = !editMode;
    Serial.write(editMode?0b10000000:0b00000000);
    if (editMode) {
      PORTB |= 0b00000100;
    }else{
      PORTB &= 0b11111011;
    }
  }
  lastEdit = edit;
}

inline void loopEdit() {
  const bool sw = !((PINC>>3)&1);
  const bool dt = !((PIND>>3)&1);
  const bool ck = !((PIND>>4)&1);

  //send data
  //10000lrb
  uint8_t fdata = 0b10000000;
  
  if (sw && !lastSwitch) {
    fdata |= 0b00000001;
  }
  if (!lastDT && !lastCK) {
    if (dt) {
      fdata |= 0b00000010;
    }else if (ck) {
      fdata |= 0b00000100;
    }
  }

  lastSwitch = sw;
  lastDT = dt;
  lastCK = ck;

  if (fdata != 0b10000000) {
    Serial.write(fdata);
  }
  delay(DELAY_EDIT);
}

inline void loopRecieve() {
  const long currentTime = millis();
  if (radio.available()) {
    radio.read(&data, 1);
    lastTime = currentTime;
  }
  if (currentTime - lastTime > TIMEOUT) {
    data = 0;
  }

  if (!((PIND>>7)&1)) {
    data |= 0b10000000;
  }else if (!(PINB&1)) {
    data |= 0b01000000;
  }else if (!((PINB>>1)&1)) {
    data |= 0b00100000;
  }

  if (!((PIND>>5)&1)) {
    data |= 0b00010000;
  }

  if (data != lastData) {
    lastData = data;

    //{DOWN, UP, BACK, FWD, STOP, PROG3, PROG2, PROG1}
    uint8_t pressed = 0;
    if (((data >> 7) & 1)) {
      pressed = 1;
    }else if ((data >> 6) & 1) {
      pressed = 2;
    }else if ((data >> 5) & 1) {
      pressed = 3;
    }
    Serial.write((data&0x1F)|(pressed<<5));
  }
  delay(DELAY_RECIEVE);
}

void setup () {
  Serial.begin(9600);

  DDRB |= 0b00000100;

  DDRB &= 0b11111100;
  DDRC &= 0b11110111;
  DDRD &= 0b00000111;

  initRF();
}

void loop () {
  checkEdit();
  
  if (editMode) {
    loopEdit();
  }else{
    loopRecieve();
  }
}