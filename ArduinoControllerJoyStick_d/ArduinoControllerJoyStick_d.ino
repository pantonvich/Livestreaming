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

#define DPIN_XBEE_RX 2
#define DPIN_XBEE_TX 3

#define DPIN_TM_STROBE0 5 
#define DPIN_TM_DATA 6 
#define DPIN_TM_CLOCK 7

#define DPIN_LAP_10 9
#define DPIN_LAP_UP 10
#define DPIN_LAP_DN 11
#define DPIN_PJON 12
#define DPIN_LED 13

#define APIN_PTZ_PAN00 A0
#define APIN_PTZ_TLT00 A1
#define APIN_PTZ_ZOM00 A2
 #define DPIN_00_01 52;
 #define DPIN_00_01 50; 
 #define DPIN_00_01 48;
 
#define APIN_PTZ_PAN01 A3
#define APIN_PTZ_TLT01 A4
#define APIN_PTZ_ZOM01 A5
 #define DPIN_01_01 53;
 #define DPIN_01_01 51; 
 #define DPIN_01_01 49;

 #define APIN_PTZ_PAN02 A8
#define APIN_PTZ_TLT02 A9
#define APIN_PTZ_ZOM02 A10
 #define DPIN_02_01 46;
 #define DPIN_02_01 44; 
 #define DPIN_02_01 42;

//#define APIN_PTZ_POT_PAN00 A2
//#define APIN_PTZ_POT_TLT00 A3

  #define PTZ_LT_DN '1'
  #define PTZ_CN_DN '2'
  #define PTZ_RT_DN '3'
  #define PTZ_LT_CN '4'
  #define PTZ_CN_CN '5'
  #define PTZ_RT_CN '6'
  #define PTZ_LT_UP '7'
  #define PTZ_CN_UP '8'
  #define PTZ_RT_UP '9'  
  #define ZOM_OT 'o'
  #define ZOM_IN 'i'
  #define ZOM_CN 'c'

#define PAN_TLT_JOG 25
//0,1,2,3,5,6,7,12
/*
#define dPinLapMode A0
#define dPinLapUp A1
#define dPinLapDown A2
#define dPinCam4In A3 //btnPressed+=2;
#define dPinCam3In A4 //btnPressed+=4;
#define dPinCam2In A5 //btnPressed+=8;
#define dPinCam4Ou 9 //btnPressed+=16;
#define dPinCam2Ou 10 //btnPressed+=32; 
#define dPinCam3Ou 11 //btnPressed+=64;
*/

//#define btnArrCt 7
//unsigned long btnLastPjonMillis = 0;


#define MAX_ATTEMPTS 2
#define PACKET_MAX_LENGTH 10

//#define PJON_NETWORK_PIN 12
#define PJON_ME 1

#define PJON_1 1  //don't skip as we need to set - need to rethink this
#define PJON_2 2
#define PJON_3 3
#define PJON_4 4

// network(Arduino pin used, selected device id)
PJON<SoftwareBitBang> pjonNetwork(PJON_ME);

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(DPIN_XBEE_RX,DPIN_XBEE_TX); // RX, TX



#define TM_LED_OFF 0
#define TM_LED_GREEN 1
#define TM_LED_RED 2
 

// Blinking led variables 
//const byte DPIN_LED  = 13;  // Pin of internal Led
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
  cmdMessenger.sendCmd(kAcknowledge,F("** ArduinoControllerJoyStick_d"));

  // set pin for blink LED
  PJON_IO_MODE(DPIN_LED, OUTPUT);
  
  pjonNetwork.strategy.set_pin(DPIN_PJON);
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
  PJON_IO_MODE(DPIN_TM_CLOCK, OUTPUT);
  PJON_IO_MODE(DPIN_TM_DATA, OUTPUT);
  PJON_IO_MODE(DPIN_TM_STROBE0, OUTPUT);
  tmSendCommand(0x8f);  // activate
  tmReset();

  PJON_IO_MODE(DPIN_LAP_10, INPUT);
  PJON_IO_MODE(DPIN_LAP_UP, INPUT);
  PJON_IO_MODE(DPIN_LAP_DN, INPUT);

  PJON_IO_WRITE(DPIN_LAP_10, HIGH);
  PJON_IO_WRITE(DPIN_LAP_UP, HIGH);
  PJON_IO_WRITE(DPIN_LAP_DN, HIGH);

 #define DPIN_00_01 52;
 #define DPIN_00_01 50; 
 #define DPIN_00_01 48;
  
/*  
  pinModeFast(dPinCam4In, INPUT);  
  pinModeFast(dPinCam3In, INPUT);
  pinModeFast(dPinCam2In, INPUT);
  pinModeFast(dPinCam4Ou, INPUT);
  pinModeFast(dPinCam2Ou, INPUT);
  pinModeFast(dPinCam3Ou, INPUT);

  PJON_IO_WRITE(dPinCam4In, HIGH);
  PJON_IO_WRITE(dPinCam3In, HIGH);
  PJON_IO_WRITE(dPinCam2In, HIGH);
  PJON_IO_WRITE(dPinCam4Ou, HIGH);
  PJON_IO_WRITE(dPinCam2Ou, HIGH);
  PJON_IO_WRITE(dPinCam3Ou, HIGH);
*/
}

unsigned long CurMillis = 0;
// Loop function
void loop() 
{
  static unsigned long tripMillis  = 0;
  static unsigned long sendLapMillis = 0;
  //static int laps = 0;
  byte btnPressed = 0;
  static byte btnPressedLast = 0;
  static unsigned long sendBtnMillis = 0;
  bool sendLap = false;
  
  CurMillis = millis();
  
  pjonNetwork.receive(1500);
  
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
  xBeeLapStatus();
  blinkLed();

  readPTZ();

  //readPtzPot();
  
   if (tripMillis  < CurMillis ) {
     int lastLap = laps;
     uint8_t button = tmReadButtons();
     if (button != 0  ) {
       if (button == 1) laps++;
       else if (button == 2) laps--;
       else if (button == 4) laps += 10;
       else if (button == 8) laps -= 10;     
     }
    
    int setLap=1;
    
    if(!PJON_IO_READ(DPIN_LAP_10)) setLap += 9; //set to 1 or 10
    if( !PJON_IO_READ(DPIN_LAP_UP)) {laps += setLap; sendLap = true;}
    else if (!PJON_IO_READ(DPIN_LAP_DN)) {laps -= setLap; sendLap = true;}

  //cmdMessenger.sendCmd(kAcknowledge,PJON_IO_READ(dPinLapMode));
  //cmdMessenger.sendCmd(kAcknowledge,PJON_IO_READ(dPinLapUp));
  //cmdMessenger.sendCmd(kAcknowledge,PJON_IO_READ(dPinLapDown));
  
    if (laps < -1 ) laps = -1;
  //0,1,2,3,4,5,6,
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
   /*
   //{dPinLapMode,dPinCam4In,dPinCam3In,dPinCam2In,dPinCam4Ou,dPinCam2Ou,dPinCam3Ou}
    if (!PJON_IO_READ(dPinLapMode)) btnPressed += 1;
    if (!PJON_IO_READ(dPinCam4In)) btnPressed += 2;
    if (!PJON_IO_READ(dPinCam3In)) btnPressed += 4;
    if (!PJON_IO_READ(dPinCam2In)) btnPressed += 8;
    if (!PJON_IO_READ(dPinCam4Ou)) btnPressed += 16;
    if (!PJON_IO_READ(dPinCam2Ou)) btnPressed += 32; 
    if (!PJON_IO_READ(dPinCam3Ou)) btnPressed += 64;
       */
        
    if (lastLap != laps) { 
      tripMillis  = CurMillis + btnLapDebounce;
      sendLapMillis = tripMillis ;
    }
    
    if (btnPressedLast != btnPressed) {
      tripMillis  = CurMillis + btnCamDebounce;
      sendBtnMillis = tripMillis;
      btnPressedLast = btnPressed;
    }
  }
   
  if (sendLapMillis < CurMillis || sendLap) {
    sendLaps();
    sendLapMillis = -1;
  }

  if (sendBtnMillis < CurMillis ) {
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
/*
void readPtzPot() {
  static short panF00 = 513;
  static short tltF00 = 513;
  static short lstPanF00 = 513;
  static short lstTltF00 = 513;
  
  readSetFilter(APIN_PTZ_POT_PAN00,panF00,5);
  readSetFilter(APIN_PTZ_POT_TLT00,tltF00,5);

  if (panF00 != lstPanF00) {
    
  }
  if (tltF00 != lstTltF00) {
    lstTltF00 = tltF00;
    char value[] = "2t000";
    short v = 300 + tltF00 / 3;
    byte t = 100;
    for(byte x = 2; x < 5; x++) {
      byte n =  (v / t);
       value[x] = '0' + n;
       v -= n * t;
       t /= 10;
    }
    //pja 3/6/2017 pjonNetwork.send(2,value,5);
    //Serial.print(tltF00);
    //Serial.print(" ");
    //Serial.println(value);
  }
}
*/

void readPTZ() {
  static short panF00 = 513;
  static short tltF00 = 513;
  static short zomF00 = 513;
  static bool ptzSwState00 = false;
  static unsigned long nxtMillis = CurMillis + PAN_TLT_JOG;

  readSetFilter(APIN_PTZ_PAN00,panF00,5);
  readSetFilter(APIN_PTZ_TLT00,tltF00,5);
  readSetFilter(APIN_PTZ_ZOM00,zomF00,5);

  if (nxtMillis < CurMillis ) {
   getPtzChar(panF00, tltF00, zomF00);
   nxtMillis = CurMillis + 50; //PAN_TLT_JOG/2;
  }
}

char getPtzChar(short panF, short tltF, short zomF){
/*  #define PTZ_LT_DN '1'
  #define PTZ_CN_DN '2'
  #define PTZ_RT_DN '3'
  #define PTZ_LT_CN '4'
  #define PTZ_CN_CN '5'
  #define PTZ_RT_CN '6'
  #define PTZ_LT_UP '7'
  #define PTZ_CN_UP '8'
  #define PTZ_RT_UP '9'  
 */  
   if (panF < 350 && tltF < 350) {setPanTilt(PTZ_RT_UP);}
   else if (panF < 350 && tltF > 650) {setPanTilt(PTZ_RT_DN);}
   else if (panF < 350 ){ setPanTilt(PTZ_RT_CN); }
   else if (panF > 650 && tltF < 350) {setPanTilt(PTZ_LT_UP);}
   else if (panF > 650 && tltF > 650) {setPanTilt(PTZ_LT_DN);}
   else if (panF > 650 ){ setPanTilt(PTZ_LT_CN); }
   else if (tltF > 650){ setPanTilt(PTZ_CN_DN); }
   else if (tltF < 350){ setPanTilt(PTZ_CN_UP); }
   else { setPanTilt(PTZ_CN_CN); }

   if (zomF < 350) {setZoom(ZOM_IN);}
   else if (zomF > 650) {setZoom(ZOM_OT);}
   else {setZoom(ZOM_CN);}
}

void setZoom(char zom) {
  static char lst = ZOM_CN;
  static char lst0 = ZOM_CN;
  static char lst1 = ZOM_CN;
    if (zom == ZOM_CN && lst == ZOM_CN && lst0 == ZOM_CN && lst1 == ZOM_CN) return;

  char value[] = "4 0";
  value[1] = zom;
  pjonNetwork.send(4,value,3);
   lst1 = lst0;
   lst0 = lst;
   lst = zom;
}
void setPanTilt(char ptz){
  static char lst = PTZ_CN_CN;
  static char lst0 = PTZ_CN_CN;
  static char lst1 = PTZ_CN_CN;
  static byte i = '0';
  if (ptz == PTZ_CN_CN && lst == PTZ_CN_CN && lst0 == PTZ_CN_CN && lst1 == PTZ_CN_CN) return;
  char value[] = "4j  ";
   value[2] = ptz;
   value[3] = ++i;
   //Serial.print("ptz ");
   //Serial.println(value);
   pjonNetwork.send(4,value,4);
   lst1 = lst0;
   lst0 = lst;
   lst = ptz;
   if (i == '9') i = '0';
}

void readSetFilter(byte apin,short &value,short filter) {
  value += (( analogRead(apin)- value)/filter);
}

// Returns if it has been more than interval (in ms) ago. Used for periodic actions
void blinkLed() {
  if (  CurMillis - prevBlinkTime > intervalOff ) {
    // Turn led off during halfway interval
    prevBlinkTime = CurMillis;
    PJON_IO_WRITE(DPIN_LED, LOW);
  } else if (  CurMillis - prevBlinkTime > intervalOn ) {
    // Turn led on at end of interval (if led state is on)
    PJON_IO_WRITE(DPIN_LED, ledState ? HIGH : LOW);
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
    lastXBeeRead = CurMillis + 100;
    //Serial.print("X");
    //Serial.println(lastChar);
  }
  //|| lastXBeeRead < CurMillis || buf[bufCt-1]=='\n'
  if (bufCt > 0 && (bufCt == maxbuf || lastChar == 'z' || lastChar == 'Z' || lastXBeeRead < CurMillis )) {
      buf[bufCt] = '\0';
      cmdMessenger.sendCmd(kLapStatus, buf);
      bufCt = 0;
      lastXBeeRead = -1;
  }
}

/*void sendLapStatus(){
  if (  CurMillis - prevSendStatusTime > intervalSendStatus ) {
   cmdMessenger.sendCmd(kLapStatus,(int)++ledCt);
   prevSendStatusTime = CurMillis;
  }
}
*/
void pjon_receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  const byte maxMsg = 5;
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
 
   
  if(code == PJON_CONNECTION_LOST) {   //101
    buf[idx++] = 'L';
    buf[idx++] = '_';  
    buf[idx++] = 'C';
  } else if (code == PJON_CONTENT_TOO_LONG ) { //104
      buf[idx++] = 'C';
      buf[idx++] = '2';  
      buf[idx++] = 'L';
  } else if (code == PJON_PACKETS_BUFFER_FULL) { //102
      //strcat(buf,"BUF");
      buf[idx++] = 'P';
      buf[idx++] = 'B';  
      buf[idx++] = 'F'; 
    } else if (code == PJON_DEVICES_BUFFER_FULL) { //254
      //strcat(buf,"BUF");
      buf[idx++] = 'D';
      buf[idx++] = 'B';  
      buf[idx++] = 'F'; 
    } else if (code == PJON_ID_ACQUISITION_FAIL) { //254
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
  PJON_IO_WRITE(DPIN_TM_STROBE0, LOW);
  shiftOut(DPIN_TM_DATA, DPIN_TM_CLOCK, LSBFIRST, value);
  PJON_IO_WRITE(DPIN_TM_STROBE0, HIGH);
}

void tmReset()
{
  tmSendCommand(0x40); // set auto increment mode
  PJON_IO_WRITE(DPIN_TM_STROBE0, LOW);
  shiftOut(DPIN_TM_DATA, DPIN_TM_CLOCK, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(DPIN_TM_DATA, DPIN_TM_CLOCK, LSBFIRST, 0x00);
  }
  PJON_IO_WRITE(DPIN_TM_STROBE0, HIGH);
}

uint8_t tmReadButtons()
{
  uint8_t buttons = 0;
  PJON_IO_WRITE(DPIN_TM_STROBE0, LOW);
  shiftOut(DPIN_TM_DATA, DPIN_TM_CLOCK, LSBFIRST, 0x42);

  //pinModeFast(DPIN_TM_DATA, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(DPIN_TM_DATA, DPIN_TM_CLOCK, LSBFIRST) << i;
    buttons |= v;
  }

  //pinModeFast(DPIN_TM_DATA, OUTPUT);
  PJON_IO_WRITE(DPIN_TM_STROBE0, HIGH);
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
  PJON_IO_WRITE(DPIN_TM_STROBE0, LOW);
  shiftOut(DPIN_TM_DATA, DPIN_TM_CLOCK, LSBFIRST, 0xc0);
  
  for (byte x = 0; x < mCt; x++) {
    //Serial.print(msg[x]);
    shiftOut(DPIN_TM_DATA, DPIN_TM_CLOCK, LSBFIRST, msg[x]); 
    shiftOut(DPIN_TM_DATA, DPIN_TM_CLOCK, LSBFIRST, 0x00);
  }
  
  PJON_IO_WRITE(DPIN_TM_STROBE0, HIGH); 
  
}

bool DebounceSwitch(byte pin)
{
//http://www.eng.utah.edu/~cs5780/debouncing.pdf
static uint8_t State = 0; // Current debounce status
State = (State << 1) | !PJON_IO_READ(pin) | 0xe0;
if (State == 0xf0) return true;
return false;
} 

