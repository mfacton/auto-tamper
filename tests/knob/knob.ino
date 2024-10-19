bool lastDT = false;
bool lastCK = false;
bool lastSW = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Serial.print("-1,4,");
  // Serial.print(rdt());
  // Serial.print(",");
  // Serial.println(rck()+2);

  const bool sw = rsw();
  if (sw && !lastSW) {
    Serial.println("pressed");
  }
  lastSW = sw;

  const bool dt = rdt();
  const bool ck = rck();
  if (!lastDT && !lastCK) {
    if (dt) {
      Serial.println("right");
    }else if (ck) {
      Serial.println("left");
    }
  }
  lastDT = dt;
  lastCK = ck;
}

bool rsw() {
  return !digitalRead(2);
}

bool rdt() {
  return !digitalRead(3);
}

bool rck() {
  return !digitalRead(4);
}