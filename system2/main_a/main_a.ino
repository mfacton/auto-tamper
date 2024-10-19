#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

bool editMode = false;
bool autoMode = false;
//independent module
//functions that read sensor inputs
namespace sensors {
  inline void init() {
    DDRD &= 0b11100011;
    DDRC &= 0b11111110;
  }
  inline bool pressure() { //D3
    return !((PIND >> 3)&1);
  }
  bool feeler(uint8_t number) {
    if (number == 0) {//feeler 1
      return !(PINC&1); //C0
    }else{//feeler 2
      return !((PIND>>2)&1); //D2
    }
  }
  inline bool oil() { //D4
    return !((PIND>>4)&1);
  }
}
//independent module
//functions that control movement
namespace movement {
  //coil1 B0 UP
  //coil2 D7 DOWN
  //coil3 D6 FWD
  //coil4 D5 BACK
  inline void init() {
    DDRB |= 0b00000001;
    DDRD |= 0b11100000;
  }
  // bool moving() {
  //   return (PORTD>>5) > 0 || (PORTB&1);
  // }
  inline void up() {
    PORTD &= 0b01111111;
    PORTB |= 0b00000001;
  }
  inline void down() {
    PORTD |= 0b10000000;
    PORTB &= 0b11111110;
  }
  inline void stopUD() {
    PORTB &= 0b11111110;
    PORTD &= 0b01111111;
  }
  inline void forward() {
    PORTD &= 0b11011111;
    PORTD |= 0b01000000;
  }
  inline void backward() {
    PORTD |= 0b00100000;
    PORTD &= 0b10111111;
  }
  void moveFB(uint8_t direction) {
    if (direction) {
      forward();
    }else{
      backward();
    }
  }
  inline void stopFB() {
    PORTD &= 0b10011111;
  }
  inline void stopAll() {
    PORTD &= 0b00011111;
    PORTB &= 0b11111110;
  }
}
//independent module
//functions that control program LEDs
namespace LEDs {
  inline void init() {
    DDRC |= 0b00000010;
    DDRB |= 0b00000110;
  }

  inline void clear() {
    PORTC &= 0b11111101;
    PORTB &= 0b11111001;
  }

  inline void set(uint8_t led) {
    switch (led) {
      case 1: //C1
        PORTC |= 0b00000010;
        break;
      case 2: //B2
        PORTB |= 0b00000100;
        break;
      case 3: //B1
        PORTB |= 0b00000010;
        break;
    }
  }
}
//independent module
//declares some memory locations
//functions that abstract memory managment
namespace mem {
  //0-15 non-used
  //16-63 programs
  //64-127 individual

  constexpr uint8_t height = 0;
  constexpr uint8_t distance = 1;
  constexpr uint8_t delay = 2;
  constexpr uint8_t passes = 3;
  constexpr uint8_t safety = 4;

  uint16_t getVariable (uint8_t address) {
    const uint8_t mappedAddress = address<<1;
    return (EEPROM.read(mappedAddress) << 8) | EEPROM.read(mappedAddress|1);
  }

  void setVariable (uint8_t address, uint16_t value) {
    const uint8_t mappedAddress = address<<1;
    EEPROM.update(mappedAddress, value>>8);
    EEPROM.update(mappedAddress|1, value);
  }

  void reset() {
    for (uint16_t address = 0; address < 256; address++) {
      EEPROM.update(address, EEPROM.read(address+256));
    }
  }

  void setDefaults() {
    for (uint16_t address = 0; address < 256; address++) {
      EEPROM.update(address+256, EEPROM.read(address));
    }
  }
}
//independent module
//intitialize lcd and function to draw text
namespace lcd {
  namespace {
    constexpr char* blankLiteral = "                    ";//20 spaces
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
  }

  inline void init() {
    lcd.init();
    lcd.backlight();
    lcd.noAutoscroll();
    lcd.cursor_off();
    lcd.blink_off();
    lcd.clear();
  }

  inline void print(char* text) {
    lcd.print(text);
  }

  void printText(uint8_t row, uint8_t col, char* text) {
    lcd.setCursor(col, row);
    lcd.print(text);
  }

  void printValue(uint16_t value) {
    char cstr[6];
    ltoa(value, cstr, 10);
    lcd.print(cstr);
    const uint8_t len = strlen(cstr);
    lcd.print(blankLiteral+len+15);
  }

  void printValue(uint8_t row, uint8_t col, uint16_t value) {
    lcd.setCursor(col, row);
    printValue(value);
  }

  inline void printTitle(char* text) {
    uint8_t len = strlen(text);
    printText(0, 10-len/2, text);
  }

  void printLine(uint8_t row, uint8_t col, char* text) {
    const uint8_t len = strlen(text);
    printText(row, col, text);
    const uint8_t start = col+len;
    lcd.print(blankLiteral+start);
  }

  inline void clear() {
    lcd.clear();
  }
}
//display control for edit mode
namespace edit {//7480 989
  namespace {
    constexpr uint16_t DOUBLE_PRESS_TIME = 200;

    constexpr char* scrollIncrements[3] = {"x1", "x50", "x1000"};
    constexpr char* menuLiterals[4] = {"Program 1", "Program 2", "Program 3", "General"};
    constexpr char* programLiterals[8] = {"Height", "Distance Time", "Bottom Delay", "Total Passes", "Safety Timeout", "BACK"};
    constexpr char* generalLiterals[3] = {"Reset", "Set As Default", "BACK"};
    constexpr char* confirmLiterals[2] = {"BACK", "Confirm"};

    constexpr uint8_t confirmRedirects[2] = {0, 0};

    struct page {
      const uint8_t optionLength;
      const uint8_t* redirects;
      const char* title;
      const char** options;
    };

    constexpr page menu = {4, (const uint8_t [4]){1,2,3,4}, "Menu", menuLiterals};
    constexpr page prog1 = {6, (const uint8_t[6]){16,17,18,19,20, 0}, menuLiterals[0], programLiterals};
    constexpr page prog2 = {6, (const uint8_t[6]){32,33,34,35,36, 0}, menuLiterals[1], programLiterals};
    constexpr page prog3 = {6, (const uint8_t[6]){48,49,50,51,52, 0}, menuLiterals[2], programLiterals};
    constexpr page general = {3, (const uint8_t[3]){5, 6, 0}, menuLiterals[3], generalLiterals};
    constexpr page reset = {2, confirmRedirects, generalLiterals[0], confirmLiterals};
    constexpr page setDefaults = {2, confirmRedirects, generalLiterals[1], confirmLiterals};

    constexpr page pages[7] = {
      menu,
      prog1,
      prog2,
      prog3,
      general,
      reset,
      setDefaults
    };

    //menu variables
    uint8_t pageNum;
    uint8_t select;//index of ">"
    uint8_t maxSelect;
    uint8_t view;//index of top selection option
    bool editValue;

    //temp value variables
    uint8_t address;
    uint16_t value;
    uint8_t scroll;//0,1,2 amount to change variable by x1 x50 x1000
    uint16_t increment;//1,50,1000 gets loaded based on "scroll" to save processing

    //-1 no press yet
    //double press to enter
    //single press to change scroll
    long lastPress;

    void initValue(uint8_t valueAddress) {
      editValue = true;
      address = valueAddress;
      value = mem::getVariable(address);
      scroll = 0;
      increment = 1;
      lastPress = 0;

      lcd::clear();
      lcd::printTitle(*(pages[pageNum].options+scroll));
      lcd::printText(1, 0, "Original:");
      lcd::printValue(1, 10, value);
      lcd::printText(2, 0, "Edited:");
      lcd::printValue(2, 8, value);
      lcd::printText(3, 0, "Increment:");
      lcd::printText(3, 11, scrollIncrements[0]);
    }

    void updateIncrement() {
      lcd::printLine(3, 11, scrollIncrements[scroll]);
      switch (scroll) {
        case 0:
          increment = 1;
          return;
        case 1:
          increment = 50;
          return;
        case 2:
          increment = 1000;
          return;
      }
    }

    inline void drawValue() {
      lcd::printValue(2, 8, value);
    }

    void drawOptions() {
      const uint8_t maxOptions = min(3, maxSelect);
      for (uint8_t i = 0; i < maxOptions; i++) {
        lcd::printLine(i+1, 1, *(pages[pageNum].options+i+view));
      }
    }

    void drawSelect() {
      const uint8_t row = select - view;
      for (uint8_t i = 0; i < 3; i++) {
        lcd::printText(i+1, 0, i==row?">":" ");
      }
    }
  }

  void initPage(uint8_t address) {
    pageNum = address;
    select = 0;
    maxSelect = pages[pageNum].optionLength;
    view = 0;
    editValue = false;

    lcd::clear();
    lcd::printTitle(pages[pageNum].title);
    drawSelect();
    drawOptions();
  }

  // 10000lrb
  // l = left
  // r = right
  // b = button
  // is one on event (only once)
  void parseData(uint8_t data) {
    if (data&1) {//pressed
      if (editValue) {
        const long currentTime = millis();
        if (lastPress != 0 && currentTime - lastPress < DOUBLE_PRESS_TIME) {
          mem::setVariable(address, value);
          initPage(pageNum);
        }else{
          scroll=(scroll+1)%3;
          updateIncrement();
        }
        lastPress = currentTime;
      }else{
        //special case
        if (select == 1) {
          if (pageNum == 5) {
            mem::reset();
          }else if (pageNum == 6) {
            mem::setDefaults();
          }
        }
        const uint8_t redirect = *(pages[pageNum].redirects+select);
        if (redirect < 16) {
          initPage(redirect);
        }else{
          initValue(redirect);
        }
      }
    }else{//dial
      int8_t direction = (data & 0b00000010) - 1;
      if (editValue) {
        value += direction*increment;
        drawValue();
      }else{
        select += maxSelect + direction;
        select %= maxSelect;
        if (maxSelect > 3) {
          const uint8_t newView = max(min(select, view), select-2);
          if (newView != view) {
            view = newView;
            drawOptions();
          }
        }
        drawSelect();
      }
    }
  }
}

//display control for manual mode
namespace manual {
  namespace {
    constexpr char* horizontalDirections[3] = {"Idle", "Backward", "Forward"};
    constexpr char* verticalDirections[3] = {"Idle", "Down", "Up"};

    uint8_t lastHorizontal = 0;
    uint8_t lastVertical = 0;

    inline void drawVertical() {
      lcd::printLine(2, 12, verticalDirections[lastVertical]);
    }

    inline void drawHorizontal() {
      lcd::printLine(3, 12, horizontalDirections[lastHorizontal]);
    }    
  }

  void initPage() {
    lcd::clear();
    lcd::printTitle("Manual Mode");
    lcd::printText(2, 0, "Vertical:");
    drawVertical();
    lcd::printText(3, 0, "Horizontal:");
    drawHorizontal();
  }

  void updateVertical(uint8_t direction) {
    if (direction != lastVertical) {
      lastVertical = direction;
      drawVertical();
    }
  }

  void updateHorizontal(uint8_t direction) {
    if (direction != lastHorizontal) {
      lastHorizontal = direction;
      drawHorizontal();
    }
  }
}
namespace error {
  namespace {
    constexpr char* instructionLiteral = "Check and Restart";
  }

  void drawError(const char* title) {
    movement::stopAll();
    lcd::clear();
    lcd::printTitle(title);
    lcd::printText(1, 1, instructionLiteral);
    while (true) {
      delay(1000);
    }
  }
}
namespace oil {
  namespace {
    constexpr char* oilLiteral = "Detected Low Oil";
  }

  inline void loop() {
    if (sensors::oil()) {
      error::drawError(oilLiteral);
    }
  }
}
//control automate mode
//also display
namespace automate {
  uint8_t runningProgram = 0;

  void stopProgram();
  void startProgram(uint8_t program);

  namespace {
    constexpr char* directionLiterals[2] = {"back", "fwd "};

    bool latches[2];

    uint8_t pass;
    uint8_t process;
    uint8_t direction; //using first bit only
    long currentTime;
    long passTime; //added time of previous movements
    long eventTime;

    uint16_t programVariables[16];

    // Running Program 1
    //pass: xxxxx/65535
    //dst: xxxxx dir: back (fwd)
    //ht: xxxxx del: xxxxx

    void drawPasses() {
      char cstr[6];
      ltoa(pass, cstr, 10);
      const uint8_t len = strlen(cstr);
      lcd::printText(1, 11-len, cstr);
    }

    void drawDirection() {
      lcd::printText(2, 16, directionLiterals[direction&1]);
    }

    void initPage() {
      lcd::clear();
      lcd::printText(0, 1, "Running Program ");
      const char str[2] = {'0'+runningProgram, '\0'};
      lcd::print(str);
      lcd::printText(1, 0, "pass:     0/");
      lcd::printValue(programVariables[mem::passes]);
      lcd::printText(2, 0, "dst: ");
      lcd::printValue(programVariables[mem::distance]);
      lcd::printText(2, 11, "dir: fwd");
      lcd::printText(3, 0, "ht: ");
      lcd::printValue(programVariables[mem::height]);
      lcd::printText(3, 10, "del: ");
      lcd::printValue(programVariables[mem::delay]);
    }

    void downProcess() {
      if (eventTime == -1) {
        if (sensors::pressure()) {
          eventTime = currentTime;
        }
      }else if (currentTime - eventTime >= programVariables[mem::delay]){
        process++;
        eventTime = currentTime;
        movement::up();
      }
    }

    void upProcess() {
      if (currentTime - eventTime >= programVariables[mem::height]) {
        movement::stopUD();
        movement::moveFB(direction&1);
        eventTime = currentTime;
        process++;
      }
    }

    void moveProcess() {
      bool nextProcess = false;

      const long deltaTime = currentTime - eventTime;

      if (latches[direction&1]) {
        latches[(direction&1)^1] = false;
        direction = ~direction;
        passTime = 0;
        nextProcess = true;
        pass++;
        drawDirection();
        drawPasses();
        if (pass >= programVariables[mem::passes]) {
          if (runningProgram == 3) {
            stopProgram();
            manual::initPage();
          }else{
            startProgram(runningProgram+1);
          }
          return;
        }
      }else if (passTime + deltaTime >= programVariables[mem::safety]) {
        error::drawError("Safety Timeout");
      } else if (deltaTime >= programVariables[mem::distance]) {
        passTime += deltaTime;
        nextProcess = true;
      }

      if (nextProcess) {
        process = 0;
        movement::stopFB();
        movement::down();
        eventTime = -1;
      }
    }
  }

  void stopProgram() {
    runningProgram = 0;
    autoMode = false;
    LEDs::clear();
    movement::stopAll();
  }

  void startProgram(uint8_t program) {
    autoMode = true;
    runningProgram = program;

    latches[0] = false;
    latches[1] = false;

    pass = 0;
    direction = 1;
    process = 0;
    passTime = 0;
    eventTime = -1;

    const uint8_t base = runningProgram<<4;
    for (uint8_t i = 0; i < 16; i++) {
      programVariables[i] = mem::getVariable(base|i);
    }

    initPage();

    LEDs::clear();
    LEDs::set(program);
    movement::stopFB();
    movement::down();
  }

  void loop() {
    currentTime = millis();

    for (uint8_t i = 0; i < 2; i++) {
      latches[i] = latches[i] || sensors::feeler(i);
    }
  
    if (process == 0) {
      downProcess();
    }else if (process == 1) {
      upProcess();
    }else {
      moveProcess();
    }
  }
}
namespace run {
  void parseData(uint8_t data) {
    //7 = 0(ignore)
    //5-6 = programs
    //4 = stop
    //3 = forward
    //2 = backward
    //1 = up
    //0 = down
    const uint8_t dataProg = data>>5;
    if (data&0x1F) {
      if (autoMode) {
        automate::stopProgram();
        manual::initPage();
      }
      if ((data>>4)&1) {
        movement::stopAll();
        manual::updateHorizontal(0);
        manual::updateVertical(0);
      }else{
        if ((data >> 3)&1) {
          movement::forward();
        }else if ((data >> 2)&1) {
          movement::backward();
        }else{
          movement::stopFB();
        }
        manual::updateHorizontal((data>>2)&0b00000011);
        if ((data >> 1)&1) {
          movement::up();
        }else if (data&1) {
          movement::down();
        }else {
          movement::stopUD();
        }
        manual::updateVertical(data&0b00000011);
      }
    }else if (dataProg > 0) {
      if (automate::runningProgram != dataProg) {
        automate::startProgram(dataProg);
      }
    }else if (!autoMode) {
      movement::stopAll();
      manual::updateHorizontal(0);
      manual::updateVertical(0);
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  sensors::init();
  movement::init();
  LEDs::init();
  lcd::init();
  
  manual::initPage();
}

void loop() {
  oil::loop();

  if (Serial.available()) {
    const uint8_t data = Serial.read();
    if ((const bool)(data>>7) != editMode) {
      editMode = !editMode;
      if (editMode) {
        automate::stopProgram();
        edit::initPage(0);
        return;
      }else{
        manual::initPage();
      }
    }

    if (editMode) {
      edit::parseData(data);
    }else{
      run::parseData(data);
    }
  }

  if (autoMode) {
    automate::loop();
  }
}