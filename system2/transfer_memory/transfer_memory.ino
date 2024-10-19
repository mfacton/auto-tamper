#include <EEPROM.h>

namespace newMemory {
  //0-15 non-used
  //16-63 programs
  //64-127 individual

  constexpr uint8_t height = 0;
  constexpr uint8_t distance = 1;
  constexpr uint8_t delay = 2;
  constexpr uint8_t passes = 3;
  constexpr uint8_t safety = 4;

  uint16_t getVariable (uint8_t address) {
    const uint8_t mappedAddress = address<<1;
    return (EEPROM.read(mappedAddress) << 8) | EEPROM.read(mappedAddress|1);
  }

  void setVariable (uint8_t address, uint16_t value) {
    const uint8_t mappedAddress = address<<1;
    EEPROM.update(mappedAddress, value>>8);
    EEPROM.update(mappedAddress|1, value);
  }

  void reset() {
    for (uint16_t address = 0; address < 256; address++) {
      EEPROM.update(address, EEPROM.read(address+256));
    }
  }

  void setDefaults() {
    for (uint16_t address = 0; address < 256; address++) {
      EEPROM.update(address+256, EEPROM.read(address));
    }
  }
}

namespace oldMemory {
  uint16_t height[3];//ms
  uint16_t travelTime[3];//times per row
  uint16_t wait[3];//ms
  uint16_t passes[3];//number

  uint16_t readVariable(byte address) {
    return (EEPROM.read(address)<<8) | EEPROM.read(address+1);
  }

  void loadData () {
    for (byte p = 0; p < 3; p++) {
      const byte base = (p+1)*16;
      height[p] = readVariable(base);
      travelTime[p] = readVariable(base+2);
      wait[p] = readVariable(base+4);
      passes[p] = readVariable(base+6);
    }
    const byte base = 64;
  }

  void setVariable(byte address, uint16_t value) {
    EEPROM.update(address, value>>8);
    EEPROM.update(address+1, value&0xFF);
    loadData();
  }

  void printData() {
    constexpr char* labels[4] = {"heights", "distances", "delays", "passes"};
    uint16_t* vars[4] = {height, travelTime, wait, passes};
    for (uint8_t i = 0; i < 3; i++) {
      Serial.print("program ");
      Serial.println(i);
      for (uint8_t j = 0; j < 4; j++) {
        Serial.print(labels[j]);
        Serial.print(" ");
        uint16_t* baseptr = vars[j];
        Serial.println(*(baseptr+i));
      }
    }
  }
}

void setup() {
  // Serial.begin(9600);
  for (uint16_t i = 0; i < 512; i++) {
    EEPROM.update(i, 0);
  }

  //prog1
  newMemory::setVariable(16, 2000);
  newMemory::setVariable(17, 2000);
  newMemory::setVariable(18, 400);
  newMemory::setVariable(19, 2);
  newMemory::setVariable(20, 15000);

  //prog2
  newMemory::setVariable(32, 1600);
  newMemory::setVariable(33, 1200);
  newMemory::setVariable(34, 400);
  newMemory::setVariable(35, 4);
  newMemory::setVariable(36, 15000);

  //prog3
  newMemory::setVariable(48, 1300);
  newMemory::setVariable(49, 500);
  newMemory::setVariable(50, 500);
  newMemory::setVariable(51, 6);
  newMemory::setVariable(52, 15000);

  // put your setup code here, to run once:
  // oldMemory::loadData();
  // oldMemory::printData();
}

void loop() {
  // put your main code here, to run repeatedly:
}

/*
program 1--------------------
heights 2000
distances 2000
delays 400
passes 2
program 2-------------------
heights 1600
distances 1200
delays 500
passes 4
program 3--------------------
heights 1300
distances 500
delays 500
passes 6
*/

/*
heights
2000
1600
1300
distances
2000
1200
500
delays
400
500
500
passes
2
4
6
*/