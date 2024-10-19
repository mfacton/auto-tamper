#include <SoftwareSerial.h>
#include <avr/interrupt.h>
#include <EEPROM.h>

#include "variables.h"
#include "storage.h"
#include "setup.h"

#include "sensors.h"
#include "movements.h"

#include "auto.h"

#include "parser.h"
#include "bluetooth.h"
#include "RFrcv.h"

void loop () {
  currentTime = millis();
  if (currentTime - checkTime > switchUpdateTime) {
    if (editPressed() != editMode) {
      editMode = !editMode;
      if (editMode) {
        Serial.println("bluetooth mode");
        bluetooth.listen();
        runProg = 0;
        stopAll();
      }else{
        Serial.println("reciever mode");
        reciever.listen();
      }
    }
    checkTime = currentTime;
  }
  if (editMode) {
    settingsLoop();
  }else{
    recieverLoop();
    if (runProg != 0) {
      autoLoop();
    }
  }
}
