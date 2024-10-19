#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial bluetooth(4, 5);
byte btBuff[4];

void setup() {
  setVariable(16, 27);
  setVariable(18, 1125);
  setVariable(20, 1234);
  setVariable(22, 4321);
  bluetooth.begin(9600);
  bluetooth.listen();

  for(int i = 0; i < 4; i++) {
    btBuff[i] = 0xFF;
  }

  Serial.begin(9600);
}

void loop() {
  settingsLoop();
  delay(1);
}

void printByte(byte val) {
  for (int i = 0; i < 8; i++)
    {
        bool b = val & 0x80;
        Serial.print(b);
        val = val << 1;
    }
    Serial.println();
}

bool buffValid() {
  for (byte i = 0; i < 4; i++) {
    if (btBuff[3-i] >> 6 != i) {
      return false;
    }
  }
  return true;
}

void sendSetting(byte id) {
  uint16_t value = readVariable(id);
  bluetooth.write(id>>2);
  bluetooth.write(((id&0x3)<<4)|(value>>12)|0x40);
  bluetooth.write(((value>>6)&0x3F)|0x80);
  bluetooth.write((value&0x3F)|0xC0);
}

void settingsLoop() {
  //test
  //FF BF 4F 04
  //00000100
  //01001111
  //10111111
  //11111111

  //x = 01000000
  //a = 
  
  //00xxxxxx 3
  //01xxaaaa 2
  //10aaaabb 1
  //11bbbbbb 0

  if (bluetooth.available()) {
    for (byte i = 3; i > 0; i--) {
      btBuff[i] = btBuff[i-1];
    }

    btBuff[0] = bluetooth.read();
    if (btBuff[0] >> 6 == 0) {
      //sent sig byte - reset all
      for (byte i = 1; i < 4; i++) {
        btBuff[i] = 0xFF;
      }
    }else{
      if (buffValid()) {
        const byte id = ((btBuff[3]&0x3F)<<2)|((btBuff[2]>>4)&0x3);
        const uint16_t data = ((btBuff[2]&0xF)<<12)|
        ((btBuff[1]&0x3F)<<6)|
        (btBuff[0]&0x3F);
        if (data == 0xFFFF) {
          Serial.print("sending back value ");
          sendSetting(id);
        }else{
          Serial.print("parsing setting ");
          parseSetting(id, data);
        }
        Serial.println(id);
      }
    }
  }
}

void parseSetting(byte id, uint16_t data) {
  Serial.print(id);//66 68
  Serial.print(" - ");
  Serial.println(data);
  setVariable(id, data);
}

uint16_t readVariable(byte address) {
  return (EEPROM.read(address)<<8) | EEPROM.read(address+1);
}

void setVariable(byte address, uint16_t value) {
  EEPROM.update(address, value>>8);
  EEPROM.update(address+1, value&0xFF);
}
