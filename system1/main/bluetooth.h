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
          //Serial.print("sending back value ");
          sendSetting(id);
        }else{
          //Serial.print("parsing setting ");
          parseSetting(id, data);
        }
        //Serial.println(id);
      }
    }
  }
}
