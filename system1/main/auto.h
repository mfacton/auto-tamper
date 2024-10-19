byte pass = 0;
bool dir = true;//true away, false toward
byte currentProcess = 0;
long eventTime = 0;

//0 = move dir
//1 = down until
//2 = up

//repeat passes {
//  for (tps-1) -> direction move, down, up
//  turn around
//}

void autoLoop() {
  checkFeelers();
  if (progStart) {
    progStart = false;
    feeler1Latch = false;
    feeler2Latch = false;
    stopAll();

    pass = 0;
    dir = true;
    currentProcess = 1;
    eventTime = -1;
    down();
    Serial.print("running prog: ");
    Serial.println(runProg);
    Serial.println("P3 = DOWN");
  }
  if (currentProcess == 0) {
    bool nextProcess = false;
    bool flip = false;
    if (dir) {
      if (feeler1Latch) {
        flip = true;
        feeler2Latch = false;
      }
    }else{
      if (feeler2Latch) {
        flip = true;
        feeler1Latch = false;
      }
    }
    if (flip) {
      Serial.println("flip");
      nextProcess = true;
      dir = !dir;
      pass++;
      if (pass >= passes[runProg-1]) {
        pass = 0;
        runProg++;
        Serial.print("running prog: ");
        Serial.println(runProg);
        if (runProg == 4) {
          stopAll();
          runProg = 0;
          return;
        }
      }
    }
    if (currentTime - eventTime >= travelTime[runProg-1]) {
      nextProcess = true;
    }
    if (nextProcess) {
      Serial.println("1");
      currentProcess++;
      stopFB();
      down();
      eventTime = -1;
    }
  }else if (currentProcess == 1) {
    if (eventTime == -1) {
      if (pressurePressed()) {
        eventTime = currentTime;
        Serial.println("pressure");
      }
    }else if (currentTime - eventTime >= wait[runProg - 1]){
      currentProcess++;
      Serial.println("2");
      eventTime = currentTime;
      up();
    }
  }else {
    if (currentTime - eventTime >= height[runProg - 1] ) {
      stopUD();
      if (dir) {
        forward();
      }else{
        backward();
      }
      eventTime = currentTime;
      currentProcess = 0;
      Serial.println("0");
    }
  }
}
