bool editPressed () {
  return !(PINC&(1<<2));
  //return analogRead(EDIT) < 1000;
}

bool pressurePressed () {
  return !(PINC&(1<<3));
  //return analogRead(PRESSURE_SWITCH) < 1000;
}

void checkFeelers () {
  //using latch type stuff
  if ((PINC&1) == 0) {
    //Serial.println("feel1");
    feeler1Latch = true;
  }else if ((PINC&2) == 0) {
    //Serial.println("feel1");
    feeler2Latch = true;
  }
}
