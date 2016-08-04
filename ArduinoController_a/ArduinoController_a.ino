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



#define PJON_NETWORK_PIN  12
#define PJON_ME 1

#define PJON_1 1  //don't skip as we need to set - need to rethink this
#define PJON_2 2
#define PJON_3 3
#define PJON_4 4

// network(Arduino pin used, selected device id)
PJON<SoftwareBitBang> pjonNetwork(PJON_ME);

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2,3); // RX, TX

// Blinking led variables 
const int kBlinkLed            = 13;  // Pin of internal Led
bool ledState                  = 1;   // Current state of Led
float ledFrequency             = 1.0; // Current blink frequency of Led

unsigned long intervalOn;
unsigned long intervalOff;
unsigned long prevBlinkTime = 0;

unsigned long prevSendStatusTime = 0;
unsigned long ledCt=0;
unsigned long intervalSendStatus = 10000;

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
  kSetCamMsg,
};

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kSetLed, OnSetLed);
  cmdMessenger.attach(kSetLedFrequency, OnSetLedFrequency);
  cmdMessenger.attach(kSetCamMsg, OnSetSetCamMsg);
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError,"Command without attached callback");
}

void OnSetSetCamMsg()
{
  // Read led state argument, interpret string as boolean
  //char buf[10];
  char * value = cmdMessenger.readStringArg();

byte len = 0;
while( (len < 10) && (value[ len ] != '\0') ) {
  len++;
}
  if(len>1){
    //first char should be the pjon id
    byte sent2PjonId = value[0] - '0';
    if (sent2PjonId < 10)
      pjonNetwork.send(sent2PjonId,value,len);
  }
  cmdMessenger.sendCmd(kAcknowledge,value);
}

// Callback function that sets led on or off
void OnSetLed()
{
  // Read led state argument, interpret string as boolean
  ledState = cmdMessenger.readBoolArg();
  cmdMessenger.sendCmd(kAcknowledge,ledState);
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
  cmdMessenger.sendCmd(kAcknowledge,ledFrequency);
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
  cmdMessenger.sendCmd(kAcknowledge,"** Started!");

  // set pin for blink LED
  pinModeFast(kBlinkLed, OUTPUT);
  
  pjonNetwork.set_pin(PJON_NETWORK_PIN);
  pjonNetwork.begin();
  pjonNetwork.set_error(pjon_error_handler);
  pjonNetwork.set_receiver(pjon_receiver_function);
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
}

// Loop function
void loop() 
{
   pjonNetwork.receive(1500);
  
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
  //delay(10);
 
  //sendLapStatus();
  xBeeLapStatus();


   blinkLed();
  
  pjonNetwork.update();
 
}

// Returns if it has been more than interval (in ms) ago. Used for periodic actions
void blinkLed() {
  if (  millis() - prevBlinkTime > intervalOff ) {
    // Turn led off during halfway interval
    prevBlinkTime = millis();
    digitalWrite(kBlinkLed, LOW);
  } else if (  millis() - prevBlinkTime > intervalOn ) {
    // Turn led on at end of interval (if led state is on)
    digitalWrite(kBlinkLed, ledState?HIGH:LOW);
  } 
}

void xBeeLapStatus() {
  #define maxbuf 4
  static char buf[maxbuf+1];
  static byte bufCt=0;
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
  if (bufCt>0 && (bufCt == maxbuf || lastChar=='z' || lastChar=='Z' || lastXBeeRead < millis() )) {
      buf[bufCt]='\0';
      cmdMessenger.sendCmd(kLapStatus,buf);
      bufCt=0;
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
void pjon_receiver_function(uint8_t id, uint8_t *payload, uint8_t length) {
  const int maxMsg = 4;
  byte ct = min(length,maxMsg); 
  if (ct == 0) return;
  char msg[maxMsg+1];
  for(byte x = 0; x < ct; x++) msg[x] =  payload[x];
  msg[ct] = '\0';

  cmdMessenger.sendCmd(kSetCamMsg,msg);
  //Serial.print("pjon r ");
  //Serial.println(msg);
  //cmdMessenger.sendCmd(kAcknowledge,msg);
}
void pjon_error_handler(uint8_t code, uint8_t pjonId) {
  char buf[15];
  char num[4]; 
  byte idx=0;
  
 //itoa(pjonId,num,10);
 //strcat(buf,num);
   buf[idx++]=pjonId+'0';
   
  if(code == CONNECTION_LOST) {   
    buf[idx++]='L';
    buf[idx++]='_';  
    buf[idx++]='C';
  } else if (code == CONTENT_TOO_LONG ) {
      buf[idx++]='C';
      buf[idx++]='2';  
      buf[idx++]='L';
  } else if (code == MEMORY_FULL) {
      buf[idx++]='M';
      buf[idx++]='_';  
      buf[idx++]='F';
  } else if (code == PACKETS_BUFFER_FULL) {
      strcat(buf,"BUF");
      buf[idx++]='B';
      buf[idx++]='U';  
      buf[idx++]='F'; 
  } else {
      buf[idx++]='U';
      buf[idx++]='K';  
      buf[idx++]='N';
  }
  buf[idx]='\0';
  cmdMessenger.sendCmd(kSetCamMsg,buf);
  //cmdMessenger.sendCmd(kAcknowledge,buf); 
  
}

