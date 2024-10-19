void stopAll() {
  digitalWrite(COIL1, LOW);
  digitalWrite(COIL2, LOW);
  digitalWrite(COIL3, LOW);
  digitalWrite(COIL4, LOW);
}

void stopFB() {
  digitalWrite(COIL1, LOW);
  digitalWrite(COIL2, LOW);
}

void forward() {
  digitalWrite(COIL2, LOW);
  digitalWrite(COIL1, HIGH);
}

void backward() {
  digitalWrite(COIL1, LOW);
  digitalWrite(COIL2, HIGH);
}

void stopUD() {
  digitalWrite(COIL3, LOW);
  digitalWrite(COIL4, LOW);
}

void up() {
  digitalWrite(COIL4, LOW);
  digitalWrite(COIL3, HIGH);
}

void down() {
  digitalWrite(COIL3, LOW);
  digitalWrite(COIL4, HIGH);
}
