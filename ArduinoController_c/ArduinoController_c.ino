// *** ArduinoController ***

// This example expands the previous Receive example. The Arduino will now send back a status.
// It adds a demonstration of how to:
// - Handle received commands that do not have a function attached
// - Send a command with a parameter to the PC
// - Shows how to invoke on the UI thread

#include <CmdMessenger.h>  // CmdMessenger
#include <PJON.h>
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>

#define dPinXbeeRx 2
#define dPinXbeeTx 3
//0,1,2,3,5,6,7,12
#define dPinLapMode A0
#define dPinLapUp A1
#define dPinLapDown A2
#define dPinCam4In A3 //btnPressed+=2;
#define dPinCam3In A4 //btnPressed+=4;
#define dPinCam2In A5 //btnPressed+=8;
#define dPinCam4Ou 9 //btnPressed+=16;
#define dPinCam2Ou 10 //btnPressed+=32; 
#define dPinCam3Ou 11 //btnPressed+=64;

//#define btnArrCt 7
//unsigned long btnLastPjonMillis = 0;


#define MAX_ATTEMPTS 2
#define PACKET_MAX_LENGTH 5

#define PJON_NETWORK_PIN 12
#define PJON_ME 1

#define PJON_1 1  //don't skip as we need to set - need to rethink this
#define PJON_2 2
#define PJON_3 3
#define PJON_4 4

// network(Arduino pin used, selected device id)
PJON<SoftwareBitBang> pjonNetwork(PJON_ME);

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(dPinXbeeRx,dPinXbeeTx); // RX, TX


#define tmClock 7
#define tmData 6 
#define tmStrobe0 5 
#define TM_LED_OFF 0
#define TM_LED_GREEN 1
#define TM_LED_RED 2
 

// Blinking led variables 
const byte kBlinkLed  = 13;  // Pin of internal Led
bool ledState  = 1;   // Current state of Led
float ledFrequency = 1.0; // Current blink frequency of Led

unsigned long intervalOn;
unsigned long intervalOff;
unsigned long prevBlinkTime = 0;

//unsigned long prevSendStatusTime = 0;
//unsigned long ledCt=0;
//unsigned long intervalSendStatus = 10000;
int laps = 0;

#define btnCamDebounce 100
#define btnLapDebounce 300
 
// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

// This is the list of recognized commands. These can be commands that can either be sent or received. 
// In order to receive, attach a callback function to these events
enum
{
  kAcknowledge,
  kError,
  kSetLed, // Command to request led to be set in specific state
  kSetLedFrequency,
  kLapStatus, // Command to report Lap status
  kSetCamMsg, // Command to send message to cam
  kBtnStatus,
  kPjonRemoveAll, // reset the pjon buffer
  kPjonBufferCt, // number of unsent messages in buffer - if more that 1000 than includes Id
};

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kSetLed, OnSetLed);
  cmdMessenger.attach(kSetLedFrequency, OnSetLedFrequency);
  cmdMessenger.attach(kSetCamMsg, OnSetSetCamMsg);
  cmdMessenger.attach(kPjonRemoveAll, OnPjonRemoveAll);
  cmdMessenger.attach(kLapStatus, OnSetLaps);
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError,F("Command without attached callback"));
}

void OnSetSetCamMsg()
{
  // Read led state argument, interpret string as boolean
  //char buf[10];
  char * value = cmdMessenger.readStringArg();

  byte len = 0;
  while( (len < 10) && (value[ len ] != '\0') ) len++;

  if(len>1) {
      //first char should be the pjon id
      byte sent2PjonId = value[0] - '0';
      if (sent2PjonId < 10) pjonNetwork.send(sent2PjonId,value,len+1);
  }
  cmdMessenger.sendCmd(kAcknowledge, value);
}

// Callback function that sets led on or off
void OnSetLed()
{
  // Read led state argument, interpret string as boolean
  ledState = cmdMessenger.readBoolArg();
  cmdMessenger.sendCmd(kAcknowledge, ledState);
}

// Callback function that sets leds blinking frequency
void OnSetLedFrequency()
{
  // Read led state argument, interpret string as boolean
  ledFrequency = cmdMessenger.readFloatArg();
  // Make sure the frequency is not zero (to prevent divide by zero)
  if (ledFrequency < 0.001) { ledFrequency = 0.001; }
  // translate frequency in on and off times in miliseconds
  intervalOn  = (500.0/ledFrequency);
  intervalOff = (1000.0/ledFrequency);
  cmdMessenger.sendCmd(kAcknowledge, ledFrequency);
}

void OnPjonRemoveAll() {
  //char * value = cmdMessenger.readStringArg();
  byte device_id = cmdMessenger.readInt16Arg();
  pjonNetwork.remove_all_packets(device_id);
}

void OnSetLaps(){
 //char * value = cmdMessenger.readStringArg();
  laps = cmdMessenger.readInt16Arg();
  sendLaps();
 /* 
  byte len = 0;
  int neg = 1;
  byte j=0;
  laps=0;
  while( (len < 10) && (value[ len ] != '\0') )  len++;
  if(len>0 && value[0]=='-') { j=1; neg=-1; }
  for(byte i=j;i<len;i++) {
    laps*=10;
    laps+= (value[i]-'0');
  }
  laps*=neg;

  sendLaps();
    */
}
// Setup function
void setup() 
{
  XBee.begin(9600);
  // Listen on serial connection for messages from the PC
  Serial.begin(115200); 

  // Adds newline to every command
  //cmdMessenger.printLfCr();   

  // Attach my application's user-defined callback methods
  attachCommandCallbacks();

  // Send the status to the PC that says the Arduino has booted
  // Note that this is a good debug function: it will let you also know 
  // if your program had a bug and the arduino restarted  
  cmdMessenger.sendCmd(kAcknowledge,F("** Start"));

  // set pin for blink LED
  pinModeFast(kBlinkLed, OUTPUT);
  
  pjonNetwork.strategy.set_pin(PJON_NETWORK_PIN);
  //pjonNetwork.set_packet_auto_deletion(true); //auto delete packets
  
  pjonNetwork.set_error(pjon_error_handler);
  pjonNetwork.set_receiver(pjon_receiver_function);
  pjonNetwork.begin();
/*
  char buf[65]; //="Controller_a PJON Pin/Id ";
  char num[4];
  strcat(buf,"Controller_a PJON Pin: ");
  itoa(PJON_NETWORK_PIN,num,10);
  strcat(buf,num);
  strcat(buf," Id: ");
  itoa(PJON_ME,num,10);
  strcat(buf,num);
  
  cmdMessenger.sendCmd(kAcknowledge,buf);
*/
  pinModeFast(tmClock, OUTPUT);
  pinModeFast(tmData, OUTPUT);
  pinModeFast(tmStrobe0, OUTPUT);
  tmSendCommand(0x8f);  // activate
  tmReset();

  pinModeFast(dPinLapMode, INPUT);
  pinModeFast(dPinLapUp, INPUT);
  pinModeFast(dPinLapDown, INPUT);

  digitalWriteFast(dPinLapMode, HIGH);
  digitalWriteFast(dPinLapUp, HIGH);
  digitalWriteFast(dPinLapDown, HIGH);
  
  //for( byte x=0;x<btnArrCt;x++){ pinMode(btnArr[x],INPUT_PULLUP); }
  //{dPinLapMode,dPinCam4In,dPinCam3In,dPinCam2In,dPinCam4Ou,dPinCam2Ou,dPinCam3Ou}
  //pinModeFast(dPinLapMode,INPUT);
  pinModeFast(dPinCam4In, INPUT);  
  pinModeFast(dPinCam3In, INPUT);
  pinModeFast(dPinCam2In, INPUT);
  pinModeFast(dPinCam4Ou, INPUT);
  pinModeFast(dPinCam2Ou, INPUT);
  pinModeFast(dPinCam3Ou, INPUT);

  digitalWriteFast(dPinCam4In, HIGH);
  digitalWriteFast(dPinCam3In, HIGH);
  digitalWriteFast(dPinCam2In, HIGH);
  digitalWriteFast(dPinCam4Ou, HIGH);
  digitalWriteFast(dPinCam2Ou, HIGH);
  digitalWriteFast(dPinCam3Ou, HIGH);

}

// Loop function
void loop() 
{
  static unsigned long m = 0;
  static unsigned long sendLapMillis = 0;
  //static int laps = 0;
  byte btnPressed = 0;
  static byte btnPressedLast = 0;
  static unsigned long sendBtnMillis = 0;
  bool sendLap = false;
  
  pjonNetwork.receive(1500);
  
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
  xBeeLapStatus();
  blinkLed();
  
   if (m < millis() ) {
     int lastLap = laps;
     uint8_t button = tmReadButtons();
     if (button != 0  ) {
       if (button == 1) laps++;
       else if (button == 2) laps--;
       else if (button == 4) laps += 10;
       else if (button == 8) laps -= 10;     
     }
    
    int setLap=1;
    
    if(!digitalReadFast(dPinLapMode)) setLap += 9; //set to 1 or 10
    if( !digitalReadFast(dPinLapUp)) {laps += setLap; sendLap = true;}
    else if (!digitalReadFast(dPinLapDown)) {laps -= setLap; sendLap = true;}

  //cmdMessenger.sendCmd(kAcknowledge,digitalReadFast(dPinLapMode));
  // cmdMessenger.sendCmd(kAcknowledge,digitalReadFast(dPinLapUp));
 //   cmdMessenger.sendCmd(kAcknowledge,digitalReadFast(dPinLapDown));
  
    if (laps < -1 ) laps = -1;
  //  0,1,2,3,4,5,6,
  //2,4,8,16,32,64,128

    //btnPressed = 0;
    /*
     byte powTwo=0;
     for( byte x=0;x<btnArrCt;x++){ 
      if (x<2) powTwo+=1;
      else powTwo*=2;
      if (!digitalRead(btnArr[x])) { 
        btnPressed+=powTwo; 
      } 
    }
   */
   //{dPinLapMode,dPinCam4In,dPinCam3In,dPinCam2In,dPinCam4Ou,dPinCam2Ou,dPinCam3Ou}
    if (!digitalReadFast(dPinLapMode)) btnPressed += 1;
    if (!digitalReadFast(dPinCam4In)) btnPressed += 2;
    if (!digitalReadFast(dPinCam3In)) btnPressed += 4;
    if (!digitalReadFast(dPinCam2In)) btnPressed += 8;
    if (!digitalReadFast(dPinCam4Ou)) btnPressed += 16;
    if (!digitalReadFast(dPinCam2Ou)) btnPressed += 32; 
    if (!digitalReadFast(dPinCam3Ou)) btnPressed += 64;
        
    if (lastLap != laps) { 
      m = millis() + btnLapDebounce;
      sendLapMillis = m;
    }
    
    if (btnPressedLast != btnPressed) {
      m = millis() + btnCamDebounce;
      sendBtnMillis = m;
      btnPressedLast = btnPressed;
    }
  }
   
  if (sendLapMillis < millis() || sendLap) {
    sendLaps();
    sendLapMillis = -1;
  }

  if (sendBtnMillis < millis() ) {
    if (btnPressed > 0) {
      //char buf[4];
      //itoa (btnPressed,buf,10);
      cmdMessenger.sendCmd(kBtnStatus, btnPressed);
      sendBtnMillis = -1;
    } 
  }
  static int pjonBufCtLast = 0;
  int pjonBufCt = pjonNetwork.update();
  if (pjonBufCtLast != pjonBufCt) {
    //char buf[4];
    //itoa (pjonBufCt,buf,10);
    cmdMessenger.sendCmd(kPjonBufferCt, pjonBufCt);
    pjonBufCtLast = pjonBufCt;

    cmdMessenger.sendCmd(kPjonBufferCt,PJON_2 * 1000 + pjonNetwork.get_packets_count(PJON_2));
    cmdMessenger.sendCmd(kPjonBufferCt,PJON_3 * 1000 + pjonNetwork.get_packets_count(PJON_3));
    cmdMessenger.sendCmd(kPjonBufferCt,PJON_4 * 1000 + pjonNetwork.get_packets_count(PJON_4));
  }
 
}

// Returns if it has been more than interval (in ms) ago. Used for periodic actions
void blinkLed() {
  if (  millis() - prevBlinkTime > intervalOff ) {
    // Turn led off during halfway interval
    prevBlinkTime = millis();
    digitalWriteFast(kBlinkLed, LOW);
  } else if (  millis() - prevBlinkTime > intervalOn ) {
    // Turn led on at end of interval (if led state is on)
    digitalWriteFast(kBlinkLed, ledState ? HIGH : LOW);
  } 
}

void xBeeLapStatus() {
  #define maxbuf 4
  static char buf[maxbuf + 1];
  static byte bufCt = 0;
  static unsigned long lastXBeeRead = -1;
  byte lastChar=' ';
  if (XBee.available()) {
    // If data comes in from XBee, send it out to serial monitor
    lastChar = XBee.read(); 
    buf[bufCt++]=lastChar;  
    lastXBeeRead = millis() + 100;
    //Serial.print("X");
    //Serial.println(lastChar);
  }
  //|| lastXBeeRead < millis() || buf[bufCt-1]=='\n'
  if (bufCt > 0 && (bufCt == maxbuf || lastChar == 'z' || lastChar == 'Z' || lastXBeeRead < millis() )) {
      buf[bufCt] = '\0';
      cmdMessenger.sendCmd(kLapStatus, buf);
      bufCt = 0;
      lastXBeeRead = -1;
  }
}

/*void sendLapStatus(){
  if (  millis() - prevSendStatusTime > intervalSendStatus ) {
   cmdMessenger.sendCmd(kLapStatus,(int)++ledCt);
   prevSendStatusTime = millis();
  }
}
*/
void pjon_receiver_function(uint8_t *payload, uint16_t length, const PacketInfo &packet_info) {
  const byte maxMsg = 4;
  byte ct = min(length, maxMsg); 
  if (ct == 0) return;
  char msg[maxMsg + 1];
  for(byte x = 0; x < ct; x++) msg[x] =  payload[x];
  msg[ct] = '\0';

  cmdMessenger.sendCmd(kSetCamMsg, msg);
  //Serial.print("pjon r ");
  //Serial.println(msg);
  //cmdMessenger.sendCmd(kAcknowledge,msg);
}
void pjon_error_handler(uint8_t code, uint8_t pjonId) {
  char buf[5];
  //char num[4]; 
  byte idx = 0;
  
  buf[idx++] = pjonId + '0';
   //buf[idx++]='E';
   //itoa(pjonId,num,10);
   //strcat(buf,num);
 
   
  if(code == CONNECTION_LOST) {   //101
    buf[idx++] = 'L';
    buf[idx++] = '_';  
    buf[idx++] = 'C';
  } else if (code == CONTENT_TOO_LONG ) { //104
      buf[idx++] = 'C';
      buf[idx++] = '2';  
      buf[idx++] = 'L';
  } else if (code == PACKETS_BUFFER_FULL) { //102
      //strcat(buf,"BUF");
      buf[idx++] = 'P';
      buf[idx++] = 'B';  
      buf[idx++] = 'F'; 
    } else if (code == DEVICES_BUFFER_FULL) { //254
      //strcat(buf,"BUF");
      buf[idx++] = 'D';
      buf[idx++] = 'B';  
      buf[idx++] = 'F'; 
    } else if (code == ID_ACQUISITION_FAIL) { //254
      //strcat(buf,"BUF");
      buf[idx++] = 'I';
      buf[idx++] = 'A';  
      buf[idx++] = 'F'; 
  } else {
      buf[idx++] = 'U';
      buf[idx++] = 'K';  
      buf[idx++] = 'N';
  }

  buf[idx] = '\0';
  cmdMessenger.sendCmd(kSetCamMsg, buf);
  //cmdMessenger.sendCmd(kAcknowledge,buf); 
  
}

void tmSendCommand(uint8_t value)
{
  digitalWriteFast(tmStrobe0, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, value);
  digitalWriteFast(tmStrobe0, HIGH);
}

void tmReset()
{
  tmSendCommand(0x40); // set auto increment mode
  digitalWriteFast(tmStrobe0, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(tmData, tmClock, LSBFIRST, 0x00);
  }
  digitalWriteFast(tmStrobe0, HIGH);
}

uint8_t tmReadButtons()
{
  uint8_t buttons = 0;
  digitalWriteFast(tmStrobe0, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, 0x42);

  pinModeFast(tmData, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(tmData, tmClock, LSBFIRST) << i;
    buttons |= v;
  }

  pinModeFast(tmData, OUTPUT);
  digitalWriteFast(tmStrobe0, HIGH);
  return buttons;
}

void sendLaps(){
  const byte maxBuf = 4;
  byte bufCt = 0;
  char buf[maxBuf];
  char msg[] = "   ";
  int b = 0; 
  
  b = laps / 100;
 
  if (b > 0) { 
    buf[bufCt++]= '0' + b;
     msg[0]= '0' + b;
    laps -= b *100;
  }

  b = laps/10;
  
  if (b>0 || bufCt > 0) {
    buf[bufCt++]= '0' + b;
    msg[1]= '0' + b;
  }
  b = laps - (b * 10);
  
  if (b >= 0 || bufCt > 0 ) {
    buf[bufCt++]= '0' + b;
    msg[2]= '0' + b;
  }

  if (bufCt == 0) buf[bufCt++] = ' ';
  
  buf[bufCt] = '\0';
  //Serial.print("lapSend ");
  //Serial.println(buf);
  cmdMessenger.sendCmd(kLapStatus, buf);
  tmSetMsg(msg, 3);
}

void tmSetMsg(char msgString[], byte mCt)
{
char letters[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";
uint8_t seg7HexCode[] = 
{0x00,0x30,0x22,0x14,0x2D,0x1B,0x70,0x20,0x39,0x0F,0x63,0x46,0x10,0x40,0x80,0x52,
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x09,0x36,0x58,0x48,0x4C,0x53,
0x5F,0x77,0x7C,0x58,0x5E,0x79,0x71,0x3D,0x74,0x10,0x1E,0x76,0x38,0x37,0x54,0x5C,
0x73,0x67,0x50,0x6D,0x78,0x1C,0x3E,0x2A,0x49,0x6E,0x5B,0x32,0x64,0x0E,0x23,0x08};


  uint8_t msg[mCt];
  for (int x = 0; x < mCt; x++) {
   for (int l = 0; l < 64; l++) {
     if(msgString[x] == letters[l]) { msg[x] = seg7HexCode[l]; break;}
   }  
  }

  tmSendCommand(0x40);
  digitalWriteFast(tmStrobe0, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, 0xc0);
  
  for (byte x = 0; x < mCt; x++) {
    //Serial.print(msg[x]);
    shiftOut(tmData, tmClock, LSBFIRST, msg[x]); 
    shiftOut(tmData, tmClock, LSBFIRST, 0x00);
  }
  
  digitalWriteFast(tmStrobe0, HIGH); 
  
}

bool DebounceSwitch(byte pin)
{
//http://www.eng.utah.edu/~cs5780/debouncing.pdf
static uint16_t State = 0; // Current debounce status
State = (State << 1) | !digitalReadFast(pin) | 0xe000;
if (State == 0xf000) return true;
return false;
} 

