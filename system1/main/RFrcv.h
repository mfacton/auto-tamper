void recieverLoop() {
  if (reciever.available()) {
    //shift over
    for (byte i = buffSize-1; i > 0; i--) {
      rcvTime[i]=rcvTime[i-1];
      rcvBuff[i]=rcvBuff[i-1];
    }
    //set first
    rcvBuff[0] = reciever.read();
    rcvTime[0] = currentTime;
    
    byte valid = 1;
    for (byte i = buffSize-1; i >= 1; i--) {
      if (rcvBuff[i] == 0xFF) {
        continue;
      }
      if (currentTime - rcvTime[i] > rcvTimeout) {
        rcvBuff[i] == 0xFF;
        continue;
      }
      valid = i+1;
      break;
    }

    if (valid >= rcvMinValid) {
      parseInstruction(rcvBuff[0]);
    }
  }
}
