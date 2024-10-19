void parseSetting(byte id, uint16_t data) {
//  Serial.print(id);//66 68
//  Serial.print(" - ");
//  Serial.println(data);
  setVariable(id, data);
}

void parseInstruction(byte data) {
  //Serial.println(data, BIN);
  //bit and function 76543210
  //7 = program 1
  //6 = program 2
  //5 = program 3
  //4 = stop
  //3 = forward
  //2 = backward
  //1 = up
  //0 = down
  if ((data & 0x1F) > 0) {
    runProg = 0;
    if ((data >> 4)&1) {//stop all coils
      stopAll();
    }else {
      if ((data >> 3)&1) {//forward
        forward();
      }else if ((data >> 2)&1) {//backward
        backward();
      }else{
        stopFB();
      }
      if ((data >> 1)&1) {//up
        up();
      }else if (data&1) {//down
        down();
      }else {
        stopUD();
      }
    }
  }else{
    if (runProg == 0) {
      stopAll();
    }
    if ((data >> 7)&1) {//prog 1
      runProg = 1;
    }else if ((data >> 6)&1) {//prog 2
      runProg = 2;
    }else if ((data >> 5)&1) {//prog 3
      runProg = 3;
    }else {
      return;
    }
    progStart = true;
  }
}
