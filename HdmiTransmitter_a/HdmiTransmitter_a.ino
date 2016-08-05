/* 
Notes and inputs
https://blog.3d-logic.com/2015/01/10/using-a-tm1638-based-board-with-arduino/
https://github.com/moozzyk/TM1638/blob/master/TM1638_buttons/TM1638_buttons.ino
http://hamsterworks.co.nz/mediawiki/index.php/Dx_display
https://github.com/gioblu/PJON

https://batchloaf.wordpress.com/2013/02/12/simple-trick-for-sending-characters-to-a-serial-port-in-windows/
  mode COM16 BAUD=115200 PARITY=n DATA=8 DTR=OFF RTS=OFF
echo 22 >\\.\COM16
*/

/*
const char* myIpAddress(void);

// allow us to use itoa() in this scope
extern char* itoa(int a, char* buffer, unsigned char radix);

const char* myIpAddress(void)
{
  IPAddress myIp = WiFi.localIP();
  static char buf[24] = "";
  //sprintf(buf, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  // The following code does the same thing as the above sprintf()
  // but uses 20KB less flash!
  char num[4];
  itoa(myIp[0],num,10);
  strcat(buf,num); strcat(buf,".");
  itoa(myIp[1],num,10);
  strcat(buf,num); strcat(buf,".");
  itoa(myIp[2],num,10);
  strcat(buf,num); strcat(buf,".");
  itoa(myIp[3],num,10);
  strcat(buf,num);
  return buf;
}
*/

#include <PJON.h>

#define PJON_NETWORK_PIN  12
#define PJON_ME 1

#define PJON_1 1  //don't skip as we need to set - need to rethink this
#define PJON_2 2
#define PJON_3 3
#define PJON_4 4

const byte idCts = 5;

const byte tmClock = 7; //9
const byte tmData = 6;  //8
int tmStrobe;

const byte tmStrobe0 = 5;  //7
const byte tmStrobe1 = 4;  //6
//const byte tmStrobe2 = 3;

//const byte tmPins[] = {tmStrobe0,tmStrobe1};
//const byte tmPinsCt = 2;

const byte tmPins[] = {tmStrobe0,tmStrobe1};
//char tmMsg[] = "012345678901234567";
const byte tmPinsCt = 2;
byte tmIdxCurrent = 0;
#define TM_LED_OFF 0
#define TM_LED_GREEN 1
#define TM_LED_RED 2

byte tmLastButtons[] = {0,0,0};
byte tmNewButtons[] = {0,0,0};

long unsigned ms = 0;
const byte pjonToTm[] = {0,1}; //index of tmPins
//const byte pjonToTmPosition[] = {4,8,12};


const byte pjonClientIds[] = {PJON_2,PJON_3};
const byte pjonIdCts = 2;
long unsigned errorMs[] = {0,0};
byte hasMsg[] = {false,false};

// network(Arduino pin used, selected device id)
PJON<SoftwareBitBang> pjonNetwork(1);//PJON_ME

#define INPUT_STRING_SIZE 10
char inputString[INPUT_STRING_SIZE+1];         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
byte inputStringPos = 0;

void setup() {
  pjonNetwork.set_pin(PJON_NETWORK_PIN);
  pjonNetwork.begin();
  pjonNetwork.set_receiver(pjon_receiver_function);
  pjonNetwork.set_error(pjon_error_handler);
  //byte currentPjonClientId = 44;
  //pjonNetwork.send(currentPjonClientId, "S",1);
  
  Serial.begin(115200);

  Serial.println("HDMI Transmitter va");
  Serial.print(" PJON_NETWORK_PIN: ");
  Serial.println(PJON_NETWORK_PIN);
  Serial.print(" Device Id (PJON_ME): ");
  Serial.println(PJON_ME);
  
  pinModeFastAlt(tmClock, OUTPUT);
  pinModeFastAlt(tmData, OUTPUT);
  
  for(byte x = 0; x < tmPinsCt; x++){
    tmStrobe = tmPins[x];
    pinModeFastAlt(tmStrobe, OUTPUT);
    tmSendCommand(0x8f);  // activate
    tmReset();
  }
}

void loop() {
  
  if ( millis() > ms )
  {
    ms = millis() + 200;
    for(byte tm=0; tm < tmPinsCt; tm++) {
      tmSetStrobe(tm);
      tmButtons(tm);    
      if (errorMs[tm] > 0 && (errorMs[tm] + 10000 < ms)) {
        tmSetMsgClear(tm);
        
      }
      //buttons pressed so update before reading more inputs
      if (newButtonsPressed(tm)) {
        pjonNetwork.update();
      }      
    }
    tmLastButtons[tmIdxCurrent] = 0;    
  }
  
 
  processSerialEvent();
  pjonNetwork.receive(1000); //lets setup for data
  pjonNetwork.update();


}

void processSerialEvent(){
  if (stringComplete) {
    inputString[inputStringPos] = '\0';
    Serial.println(inputString);
    processSerialReceive();
    // clear the string:
    inputStringPos=0;
    //inputString[0] = '\0';
    stringComplete = false;
  }
}

void processSerialReceive(){
  if (inputStringPos!=2) {Serial.print("inputStringPos: "); Serial.println(inputStringPos); return;}
  byte pjonIdIdx = getPjonClientIdIdx(inputString[0]);
  byte payload = inputString[1] - '0';
  Serial.print("pjonIdIdx: "); Serial.print(pjonIdIdx);
  Serial.print(" payload: "); Serial.println(payload);
  
  tmNewButtons[pjonIdIdx] = payload;
  
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
Serial.print(inChar);
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n' || inChar == 'z') { 
      stringComplete = true;
      Serial.print("end");
    } else {
      // add it to the inputString:
      if(inputStringPos < INPUT_STRING_SIZE) inputString[inputStringPos++] = inChar;
    }
  }
}

//void pjon_receiver_function(uint8_t length, uint8_t *payload) {
void pjon_receiver_function(uint8_t id, uint8_t *payload, uint8_t length) {
  byte ct = min(length,4); 
  if (ct == 0) return;
  char msg[5];
  
  for(byte x = 0; x < ct; x++) msg[x] =  payload[x];
  msg[ct]='\0';
  
  byte idx = getPjonClientIdIdx((char)payload[0]); 
  tmSetStrobe(pjonToTm[idx] );
  
  Serial.print("prf: ");
  Serial.print(msg);
  Serial.print(" idx: ");
  Serial.print(idx);
  Serial.print(" toTm: ");
  Serial.print(pjonToTm[idx]);
  Serial.print(" tmIdx: ");
  Serial.print(tmIdxCurrent);
  Serial.print(" strobe ");
  Serial.print(tmStrobe);
    
  tmSetMsg(msg,{},8,0);
  
  if(length == 3) {
    tmSetLed(TM_LED_RED,msg[2]-'1');
    //tmLastButtons[tmIdxCurrent] = 0; //clear so we can push again if needed
  } else if (length == 2) {
    tmSetLed(TM_LED_GREEN,msg[1]-'1');
  }
  

}
void pjon_error_handler(uint8_t code, uint8_t tmData) {
 
  char msg[8];
  byte msgCt = 0;
  uint8_t dig[3];
  byte digCt = 0;

  int r = tmData;
  int d = r / 100;
  r-=d*100;
  dig[digCt++] = d;
  d = r / 10 ;
  dig[digCt++] = d;
  dig[digCt++] = r - d*10;
    
  tmIdxCurrent = tmData-2;  //set the index of the error  //don't skip as we need to set - need to rethink this
  errorMs[tmIdxCurrent] = millis();  //so we know when to reset msg
  tmLastButtons[tmIdxCurrent] = 0; // set this so we can push it again during an error 
  if(code == CONNECTION_LOST) {
    
    Serial.print("Connection lost with device id ");
    Serial.println(tmData);    
    msg[msgCt++] = 'L';
    msg[msgCt++] = 'O';
    msg[msgCt++] = 'S';
    msg[msgCt++] = 'T';
    msg[msgCt++] = ' ';
  } else if (code == CONTENT_TOO_LONG) {
     Serial.print("CONTENT_TOO_LONG - Length: ");
     Serial.println(tmData);
     msg[msgCt++] = '2';
     msg[msgCt++] = 'L';
     msg[msgCt++] = '0';
     msg[msgCt++] = 'N';
     msg[msgCt++] = 'G';    
  } else if (code == MEMORY_FULL) {
     Serial.print("MEMORY_FULL - Fail: ");
     Serial.println(tmData);  
     msg[msgCt++] = 'M';
     msg[msgCt++] = 'E';
     msg[msgCt++] = 'M';
     msg[msgCt++] = ' ';
     msg[msgCt++] = ' ';
  } else if (code == PACKETS_BUFFER_FULL) {
      Serial.print("PACKETS_BUFFER_FULL - MAX_PACKETS: ");
      Serial.println(tmData);  
      msg[msgCt++] = 'B';
      msg[msgCt++] = 'U';
      msg[msgCt++] = 'F';
      msg[msgCt++] = ' ';
      msg[msgCt++] = ' ';
  } else {
    Serial.print("unknow code: ");
    Serial.print(code);
    Serial.print(" with data: ");
    Serial.println(tmData);  
    msg[msgCt++] = 'U';
    msg[msgCt++] = 'N';
    msg[msgCt++] = 'K';
    msg[msgCt++] = ' ';
    msg[msgCt++] = ' ';
  }
  
  Serial.print("err ");
  tmSetMsg(msg,dig,msgCt,digCt);
  
}

void pjon_network_analysis(byte clientId){
  char content[] = "01234567890123456789";
  unsigned long test;
  unsigned long mistakes;
  unsigned long busy;
  unsigned long fail; 
  
  Serial.println(" --------------------- ");
  Serial.println("PJON - Network analysis");
  Serial.print("Client Id: ");
  Serial.println(clientId);
  
  Serial.println("Setting Client to Test Mode...");
  
  long time = millis() + 2000;
  while(millis() < time) {
    int response = pjonNetwork.send_string(clientId, "TST", 3);
    if(response == ACK) {
      char msg[] = "ACK     ";
      uint8_t dig[] = {};
      tmSetMsg(msg,dig,8,0);
      Serial.println("Client ACKed");
      break;
    }
    
  }  
  
  Serial.println("Starting a 10 seconds communication test..");
  Serial.println();

  time = millis()+10000;
  while(millis() < time) {

    /* Here send_string low level function is used to
    be able to catch every single sending result. */

    int response = pjonNetwork.send_string(clientId, content, 20);
    if(response == ACK)
      test++;
    if(response == NAK)
      mistakes++;
    if(response == BUSY)
      busy++;
    if(response == FAIL)
      fail++;
  }
  
  int m = 0;
  Serial.print("Packets sent: ");
  Serial.println(test);
  Serial.print("Mistakes (error found with CRC) ");
  Serial.println(mistakes);
  Serial.print("Fail (no answer from receiver) ");
  Serial.println(fail);
  Serial.print("Busy (Channel is busy or affected by interference) ");
  Serial.println(busy);
  Serial.print("Accuracy: ");
  if (mistakes > 0) {
    Serial.print(100.0f - (100.0f / ((float)test / (float)mistakes))); 
     m = (100.0f - (100.0f / ((float)test / (float)mistakes)))*10;
  } else {
    Serial.print("0");
  }
  Serial.println(" %");
  
  Serial.println("Sending Results to Client: ");
  
  //delay(1000);

  int remain = m;
  char c[5] = {'R'};
  char msg[0];
  uint8_t dig[4];
  
  byte i = 0; 
  byte d = remain / 1000; 
  remain -= d * 1000; 
  dig[i++]=d;
  c[i] = '0' + d;
  
  d = remain / 100;
  remain -= d * 100;
  dig[i++]=d;
  c[i] = '0' + d; 
  
  d = remain / 10; 
  remain -= d * 10;
  dig[i++]=d + 10; //add . place
  c[i] = '0' + d;
  
  dig[i++]=d;
  c[i++] = '0' + remain;
  
  c[i] = '\0';
  
  pjonNetwork.send(clientId, c, 5);
  Serial.println(c);
  tmSetMsg("ACC ",dig,4,4);
  Serial.println(" --------------------- ");
  
 
  test = 0;
  mistakes = 0;
  busy = 0;
  fail = 0;
}

byte getPjonClientIdIdx(char c){
  for (byte x = 0; x < pjonIdCts; x++) {
    if (c == '0' + pjonClientIds[x]) return x;
    //if (c == '0' + pjonToTm[x]) return x;
  }
  
  return 0;
}
void tmSendCommand(uint8_t value)
{
  digitalWriteFastAlt(tmStrobe, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, value);
  digitalWriteFastAlt(tmStrobe, HIGH);
}

void tmReset()
{
  tmSendCommand(0x40); // set auto increment mode
  digitalWriteFastAlt(tmStrobe, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(tmData, tmClock, LSBFIRST, 0x00);
  }
  digitalWriteFastAlt(tmStrobe, HIGH);
}

void tmButtons(byte tm)
{
    //for(byte tm = 0; tm < tmPinsCt; tm++){
      
     // tmStrobe = tmPins[tm];
      
      uint8_t buttons = tmReadButtons();
      
      if (buttons != 0 && buttons != tmLastButtons[tm]) {
          tmNewButtons[tm] = buttons;
          //tmLastButtons[tm] = ;
          Serial.print(tmNewButtons[tm]);
          Serial.print(" pressed");
          Serial.println(tm);
      }
    //}
}

uint8_t tmReadButtons()
{
  uint8_t buttons = 0;
  digitalWriteFastAlt(tmStrobe, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, 0x42);

  pinModeFastAlt(tmData, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(tmData, tmClock, LSBFIRST) << i;
    buttons |= v;
  }

  pinModeFastAlt(tmData, OUTPUT);
  digitalWriteFastAlt(tmStrobe, HIGH);
  return buttons;
}

byte  newButtonsPressed(byte tm) {  
    
  if (tmNewButtons[tm] == 0 ) return false;
  //clear any msg when we press buttons
  //if (hasMsg[tm]) tmSetMsgClear(tm);
  
  Serial.print("strobe: ");
  Serial.print(tmPins[tm]);
  Serial.print(" tm: ");
  Serial.print(tm);
  Serial.print(" button value: ");
  Serial.print(tmNewButtons[tm]);
  Serial.print(" ClientId: ");
  Serial.print(pjonClientIds[tm]);
  Serial.println(" ");
      
  if (tmNewButtons[tm] == 128) { //button 7 test mode
    for(byte i=0; i<7; i++) {
      tmSetLed(TM_LED_OFF, i);
    }           
    tmSetLed(TM_LED_RED, 7);
    pjon_network_analysis(pjonClientIds[tm]); 
    tmSetLed(TM_LED_GREEN, 7);    
  } else {
    for(uint8_t position = 0; position < 8; position++)
    {
      //Serial.println(position);
      uint8_t mask = 0x1 << position; 
      if(tmNewButtons[tm] & mask) {
        tmSetLed(TM_LED_GREEN, position); //turn on
        
        char c[] = {'V','0' + position,'\0'};
        pjonNetwork.send(pjonClientIds[tm], c, 2);
        Serial.println(c);
      } else {
        tmSetLed(TM_LED_OFF, position); //turn off
      }          
    }
    
  }
  tmLastButtons[tm] = tmNewButtons[tm];  
  tmNewButtons[tm] = 0; 
  return true;
}

void tmSetLed(uint8_t value, uint8_t position)
{
    /*Serial.print("tmSetLed ");
    Serial.print(tmIdxCurrent);
    Serial.print(" ");
    Serial.println(tmStrobe);
    */
  pinMode(tmData, OUTPUT);

  tmSendCommand(0x44);
  digitalWriteFastAlt(tmStrobe, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, 0xC1 + (position << 1));
  shiftOut(tmData, tmClock, LSBFIRST, value);
  digitalWriteFastAlt(tmStrobe, HIGH);
}

void tmSetMsgClear(byte tm){
        errorMs[tm] = 0;
        char msg[] = "        ";
        Serial.print("clr ");
        tmSetMsg(msg,{},8,0);
        hasMsg[tm] = false;
        Serial.print("clearing ");
        Serial.println(tm);
}

void tmSetStrobe(byte tm) {
 if (tm < 0 || tm >= tmPinsCt) return;
 tmIdxCurrent = tm;
 tmStrobe = tmPins[tm];

}
void tmSetMsg( char msgString[],uint8_t dig[],byte mCt,byte dCt)
{
    Serial.print("  tmSetMsg ");
    Serial.print(tmIdxCurrent);
    Serial.print(" ");
    Serial.print(tmStrobe);
    
                      /*0*/ /*1*/ /*2*/ /*3*/ /*4*/ /*5*/ /*6*/ /*7*/ /*8*/ /*9*/
 static uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                      0x3f + 0x80, 0x06 + 0x80, 0x5b + 0x80, 0x4f + 0x80, 0x66 + 0x80, 0x6d + 0x80, 0x7d + 0x80, 0x07 + 0x80, 0x7f + 0x80, 0x6f + 0x80 };

 static char letters[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";
 static uint8_t seg7HexCode[] = 
{0x00,0x30,0x22,0x14,0x2D,0x1B,0x70,0x20,0x39,0x0F,0x63,0x46,0x10,0x40,0x80,0x52,
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x09,0x36,0x58,0x48,0x4C,0x53,
0x5F,0x77,0x7C,0x58,0x5E,0x79,0x71,0x3D,0x74,0x10,0x1E,0x76,0x38,0x37,0x54,0x5C,
0x73,0x67,0x50,0x6D,0x78,0x1C,0x3E,0x2A,0x49,0x6E,0x5B,0x32,0x64,0x0E,0x23,0x08};
  
  uint8_t msg[mCt];
  for (int x = 0; x < mCt; x++) {
   for ( int l = 0; l < 64;l ++) {
     if(msgString[x] == letters[l]) { msg[x]=seg7HexCode[l]; break;}
   }  
  }
  
  //set flag that we have msg
  hasMsg[tmIdxCurrent]=true;
  
  tmSendCommand(0x40);
  digitalWriteFastAlt(tmStrobe, LOW);
  shiftOut(tmData, tmClock, LSBFIRST, 0xc0);
  
  for (byte x = 0; x<mCt;x++) {
    //Serial.print(msg[x]);
    shiftOut(tmData, tmClock, LSBFIRST, msg[x]); 
    shiftOut(tmData, tmClock, LSBFIRST, 0x00);
  }
  
  for (byte x = 0; x<dCt;x++) { 
    //Serial.print(dig[x]);
    shiftOut(tmData, tmClock, LSBFIRST, digits[dig[x]]);
    shiftOut(tmData, tmClock, LSBFIRST, 0x00);
  }
  Serial.println();
  digitalWriteFastAlt(tmStrobe, HIGH);  
}

void digitalWriteFastAlt(uint8_t pin,uint8_t state)
{
  if (pin == tmStrobe0)
    digitalWriteFast(tmStrobe0,state);
  else if (pin == tmStrobe1)
     digitalWriteFast(tmStrobe1,state);
  else  digitalWriteFast(pin,state);
}

void pinModeFastAlt(uint8_t pin,uint8_t state)
{
  if (pin == tmStrobe0)
    pinModeFast(tmStrobe0,state);
  else if (pin == tmStrobe1)
     pinModeFast(tmStrobe1,state);
  else  pinModeFast(pin,state);
}

