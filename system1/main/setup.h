void initCoil(byte pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void setup () {
  //edit pressure feelers as inputs
  DDRC &= B11110000;
//  pinMode(EDIT, INPUT);
//  pinMode(PRESSURE_SWITCH, INPUT);
//  pinMode(FEELER1, INPUT);
//  pinMode(FEELER2, INPUT);

  initCoil(COIL1);
  initCoil(COIL2);
  initCoil(COIL3);
  initCoil(COIL4);

  bluetooth.begin(9600);
  reciever.begin(9600);
  reciever.listen();

  for (byte i = 0; i < buffSize; i++) {
    rcvBuff[i] = 0xFF;
    rcvTime[i] = 0;
  }

  for (byte i = 0; i < 4; i++) {
    btBuff[i] = 0xFF;
  }

  loadData();

  Serial.begin(9600);
}
