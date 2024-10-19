#include "LowPower.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define F_CPU 16000000L

constexpr uint16_t DISPLAY_BATTERY_TIME = 500;
constexpr uint16_t OFF_SEQUENCE_DELAY = 200;
constexpr uint8_t DELAY_SEND = 20;
constexpr uint16_t SEND_TIMEOUT = 1000;
constexpr uint16_t POWER_TIMEOUT = 30000;

// #define L1 8
// #define L2 9
// #define L3 10

// #define SIG 5

// #define POW 6
// #define STOP 7

// #define UP 4
// #define DOWN 3
// #define FWD A2
// #define BACK A3
// #define PROG1 2
// #define PROG2 A5
// #define PROG3 A4

RF24 radio(A0, A1, 1000000);

constexpr uint8_t address[6] = "00001";

// const uint8_t pins[8] = {DOWN, UP, BACK, FWD, STOP, PROG3, PROG2, PROG1};

long lastInput;
bool lastPower = true;
bool input = false;

inline void setupRegisters() {
  //sig high
  DDRD |= 0b00100000;
  PORTD |= 0b00100000;
  
  //leds
  DDRB |= 0b00000111;
  PORTB &= 0b11111000;

  //inputs
  DDRD &= 0b00100011;
  DDRC &= 0b11000011;
}

inline void disableInterfaces() {
  power_adc_disable();
  power_usart0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
}

inline void setupRF() {
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(1);
  radio.openWritingPipe(address);
  radio.stopListening();
}

inline void displayBattery() {
  ADMUX = 0b01000111;
  _delay_ms(2);
  ADCSRA |= (1<<ADEN) | (1<<ADSC);
  while (bit_is_set(ADCSRA,ADSC));
  const uint8_t seg16 = ADC>>6;
  //11 12 13

  for (uint8_t i = 0; i < 3; i++) {
    if (seg16 >= i+11) {
      PORTB |= (1<<i);
    }
  }
  
  _delay_ms(DISPLAY_BATTERY_TIME);
  PORTB &= 0b11111000;
}

inline void off() {
  PORTB &= 0b11111000;
  for (uint8_t i = 0; i < 3; i++) {
    const uint8_t mask = 1<<i;
    PORTB |= mask;
    _delay_ms(OFF_SEQUENCE_DELAY);
    PORTB &= ~mask;
  }
  PORTD &= 0b11011111;
}

void setup() {
  setupRegisters();

  if ((PIND>>7)&1) {//STOP
    displayBattery();
  }
  
  disableInterfaces();

  setupRF();

  _delay_ms(10);
  lastInput = millis();
}

void loop() {
  const long currentTime = millis();

  //blinking led
  if (currentTime % 1000 < 200) {
    PORTB |= 0b00000001;
  }else{
    PORTB &= 0b11111110;
  }

  //power
  const bool power = !((PIND >> 6)&1);
  if (!lastPower && power) {
    off();
  }
  lastPower = power;

  //data
  const uint8_t NPIND = ~PIND;
  const uint8_t NPINC = ~PINC;
  const uint8_t data =
  ((NPIND & 0b10011000) >> 3) |
  ((NPINC & 0b00001000) >> 1) |
  ((NPINC & 0b00110100) << 1) |
  ((NPIND & 0b00000100) << 5);
  //D2 C5 C4 !D7 C2 C3 D4 D3 //7-0
  //{DOWN, UP, BACK, FWD, STOP, PROG3, PROG2, PROG1}

  if (data != 0) {
    lastInput = currentTime;
    input = true;
  }

  //send
  const long delta = currentTime - lastInput;
  if (input && delta < SEND_TIMEOUT) {
    radio.write(&data, 1);
    PORTB |= 0b00000010;
  }else {
    PORTB &= 0b11111101;
  }
  if (delta > POWER_TIMEOUT) {
    off();
  }
  _delay_ms(DELAY_SEND);
}
