#define RX_RECIEVER 4
#define TX_RECIEVER 3

#define RX_BLUETOOTH 8
#define TX_BLUETOOTH 7

//#define EDIT A2
//#define PRESSURE_SWITCH A3
//#define FEELER2 A1 PC1
//#define FEELER1 A0 PC0

#define COIL3 5
#define COIL4 6
#define COIL1 9
#define COIL2 10

SoftwareSerial reciever(RX_RECIEVER, TX_RECIEVER);
SoftwareSerial bluetooth(RX_BLUETOOTH, TX_BLUETOOTH);

long currentTime = 0;

uint16_t switchUpdateTime = 200;
uint16_t rcvTimeout = 100;
uint16_t btTimeout = 500;
byte buffSize = 5;
byte rcvMinValid = 3;

//start at 0
//16 bytes per program
//4 program slots
//single variables should start on byte 64
// struct value {
//   char* name;
//   uint16_t value;
//   uint8_t address;
// };

uint16_t height[3];//ms
uint16_t travelTime[3];//times per row
uint16_t wait[3];//ms
uint16_t passes[3];//number

bool feeler1Latch = false;
bool feeler2Latch = false;

bool editMode = false;
long checkTime = 0;
byte btBuff[4];
long btStartTime = 0;

byte runProg = 0;
bool progStart = false;

byte *rcvBuff = new byte[buffSize];
long *rcvTime = new long[buffSize];
