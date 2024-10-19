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
