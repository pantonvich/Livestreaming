/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

//#define SERVO_PIN 5
//#define HDMI_SOURCE

#ifdef SERVO_PIN
  #include <Servo.h> 
  Servo myservo;
  byte angleCenter = 33;    // variable to store the servo position 
  byte angleIn = 20;
  byte angleOut =45;
  unsigned long stopZoom=0;
#endif

#include <PJON.h>

#define PJON_NETWORK_PIN  3
#define PJON_1 1  //don't skip as we need to set - need to rethink this
#define PJON_2 2
#define PJON_3 3
#define PJON_4 4
#define PJON_SEND_STATUS 30000

#define PJON_ME PJON_3
PJON<SoftwareBitBang> pjonNetwork(PJON_ME);

#define ledStatusCt 5

//#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINT(str0,str1)    \
     Serial.print(millis());     \
     Serial.print(": ");    \
     Serial.print(__PRETTY_FUNCTION__); \
     Serial.print(' ');      \
     Serial.print(__FILE__);     \
     Serial.print(':');      \
     Serial.print(__LINE__);     \
     Serial.print(' ');      \
     Serial.print(str0);      \
     Serial.print(' ');      \
     Serial.println(str1);
#else
    #define DEBUG_PRINT(str0,str1)
#endif

#ifdef HDMI_SOURCE
  #define PUSH_SWITCH_ATTEMPTS 11
  #define pushSwitchPin 2 //digi pin used as push switch
  byte pushSwitchAttempt = 0;
  byte pushSwitchState = LOW;
  byte pushSwitchOnMs = 10;
  unsigned long pushSwitchMs = 0;
  int currentSource = 0;
#endif

#define INPUT_STRING_SIZE 3
char inputString[INPUT_STRING_SIZE+1];         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
byte inputStringPos = 0;

void setup() 
{ 

#ifdef DEBUG
  Serial.begin(115200);
  DEBUG_PRINT(F("zoom"),"a");
#endif

#ifdef SERVO_PIN
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object 
  myservo.write(angleCenter);
#endif
  
  pjonNetwork.set_pin(PJON_NETWORK_PIN);
  pjonNetwork.begin();
  //pjonNetwork.set_error(pjon_error_handler);
  pjonNetwork.set_receiver(pjon_receiver_function);
} 

unsigned long ms;
unsigned long sendMs = 0;

void loop() 
{ 
  pjonNetwork.receive(1000);  
  ms=millis();

#ifdef SERVO_PIN
  if(stopZoom < ms)  {
    //Serial.println(F("zoomStop"));
    adjustServo(angleCenter);
    stopZoom=-1;
  }
#endif

#ifdef HDMI_SOURCE
  processSerialEvent();
  readLedStatusPins();
  checkSetSource();
  pushSwitch();
#endif

  if ( ms > sendMs ) {
    sendMs = ms + PJON_SEND_STATUS;
    serialPrintLedStatuses();  
  }

  pjonNetwork.update();
} 

#ifdef HDMI_SOURCE

byte ledStatusPins[] = {1,2,3,6,7};
unsigned int ledStatus[] = {0,0,0,0,0};
unsigned int ledStatusLtOnTrip = 900;  //if below this then it's the current source

bool isLedStatusOn(int ledPos){ 
  return ( ledPos > -1 && ledPos < ledStatusCt && ledStatus[ledPos] > 200 && ledStatus[ledPos] < ledStatusLtOnTrip);
}

#endif

void serialPrintLedStatuses() {
  static bool flag;
  char dig[] = "S000";
  byte digCt = 0;
  
  dig[digCt++] = '0' + PJON_ME;

#ifdef HDMI_SOURCE
  byte pinToSend = 0;
  if (pinToSend == ledStatusCt) pinToSend = 0; 
  dig[digCt++] = '0' + currentSource;
  
  for(byte x = 0; x < ledStatusCt; x++)
  {
    //Serial.print(ledStatus[x]);
    //Serial.print(" ");
    if (isLedStatusOn(x)) {
      dig[digCt++] = '1' + x;
      pinToSend = x;
      break;
    }
  }
#endif

  /*for(byte x = pinToSend+1; x < ledStatusCt; x++)
  {
    Serial.print(ledStatus[x]);
    Serial.print(" ");
  }
  */

  if (flag) dig[digCt++] = '*';
  flag = !flag;
  dig[digCt]='\0';
  DEBUG_PRINT("dig",dig);
  //Serial.print(" ");
  //Serial.print(ledStatus[pinToSend]/40);
  //Serial.print(" ");
  //Serial.print(ledStatus[pinToSend]);
  //Serial.println();

  pjonNetwork.send(PJON_1, dig, digCt);
}

void pjon_receiver_function(uint8_t id, uint8_t *payload, uint8_t length) {

  char msg[10];
  byte ct = min(length,10);
  for(int x = 0;x<ct;x++) msg[x]=payload[x];
  msg[ct] = '\0';
  DEBUG_PRINT("prf",msg);

#ifdef SERVO_PIN
    byte angle = 0;
    if(length>2) {        
      if(payload[1] == 'i') angle=angleIn;
      else if(payload[1] == 'o') angle=angleOut;
      else if(payload[1] == 'c') angle=angleCenter; 
      else if(payload[1] == 'C') {
        byte value = (payload[2]  - '0') * 10 + (payload[3]- '0');
        angleCenter=value;
      } else if(payload[1] == 'I') {
        byte value = (payload[2]  - '0') * 10 + (payload[3]- '0');
        angleIn=value;
      }  else if(payload[1] == 'O') {
        byte value = (payload[2]  - '0') * 10 + (payload[3] - '0');
        angleOut=value;
      }
    }
    if (angle>0) {
      adjustServo(angle);
      stopZoom = millis() + (payload[2] - '0')*1000;
    }
#endif

#ifdef HDMI_SOURCE
if(length>2) {
  if(payload[1] == 'V') {
      byte value = (payload[2]  - '0');
      if (value > ledStatusCt) {
        DEBUG_PRINT(F("V"),value);
        DEBUG_PRINT(F(" error V>="),ledStatusCt);
        
        return;
      }
        pushSwitchAttempt = PUSH_SWITCH_ATTEMPTS;
        currentSource = value;
        DEBUG_PRINT(F("V"),value);
    } 
}    
#endif

    pjonNetwork.send(PJON_1, msg, ct);
    pjonNetwork.update();
}

#ifdef HDMI_SOURCE
void readLedStatusPins() {
  for(int x=0;x<ledStatusCt;x++) {
    ledStatus[x] = analogRead(ledStatusPins[x]);
  } 
}

void checkSetSource() {

    if ( currentSource >= ledStatusCt  
    || pushSwitchState == HIGH
    || pushSwitchAttempt == 0
       ) return; 
       
  if (isLedStatusOn(currentSource-1) || pushSwitchAttempt == 1) {
    if(pushSwitchAttempt > 0) {
      sendMs = ms; //resend now
      pushSwitchAttempt = 0;
    }
    return;
  }

  //push switch
  pushSwitchState = HIGH;
  digitalWriteFast(pushSwitchPin, pushSwitchState);
  pushSwitchMs = ms + pushSwitchOnMs;
  pushSwitchAttempt--;
  DEBUG_PRINT(F("attempt High"),pushSwitchAttempt);

}

void pushSwitch() {
  if (ms > pushSwitchMs && pushSwitchState == HIGH) {
     pushSwitchState = LOW;
     digitalWrite(pushSwitchPin, pushSwitchState);
     DEBUG_PRINT(F("attempt Low"),pushSwitchAttempt);
     readLedStatusPins();
     //checkSetSource();
     serialPrintLedStatuses();
  }
}
#endif

void processSerialEvent(){
  if (stringComplete) {
    inputString[inputStringPos] = '\0';
    processSerialReceive();
    // clear the string:
    inputStringPos=0;
    //get ready for next string
    stringComplete = false;
  }
}


void processSerialReceive(){
#ifdef SERVO_PIN
  byte lastPayload=angleCenter;
  byte payload = inputString[0];
  byte input=0;
  if(inputStringPos==3 && payload == 'c') {
    payload = (inputString[1]  - '0') * 10 + (inputString[2] - '0');
    angleCenter=payload;
    
    DEBUG_PRINT(F("center"),payload);
  
  } else if(inputStringPos>1) {
    payload = (payload  - '0') * 10 + (inputString[1] - '0');
  }
  
  adjustServo(payload);
  if(inputStringPos>2) {
    stopZoom = millis() + (inputString[2] - '0')*1000;
  } else {
    stopZoom = millis() + 10000;
  }

#endif
}

#ifdef SERVO_PIN
void adjustServo(byte payload){
  byte payload2 = payload;
  char msg[5];
  
  if (payload<180) {
    if ((lastPayload > payload2 || payload < angleCenter ) && payload>10)
      payload -= 5;
    else if (lastPayload < payload2 || payload2>angleCenter) payload += 5;

    DEBUG_PRINT(F("p1"),payload);
    
    myservo.write(payload);

    msg[0] = '0' + PJON_ME;
    msg[1] = 'S';
    int t = payload2/10;
    msg[2] = '0' + t;
    msg[3] = '0' + (payload2 - t*10);
    pjonNetwork.send(PJON_1, msg, 4);  
    pjonNetwork.update();
    
    delay(100);
    myservo.write(payload2);   
    DEBUG_PRINT(F("p2"),payload2);   
    lastPayload = payload2;
  }
}
#endif

void serialEvent() {
#ifdef DEBUG
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
     Serial.print(inChar);
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n' || inChar == 'z') { 
      stringComplete = true;
      //Serial.println(F(" end "));
    } else {
      // add it to the inputString:
      if(inputStringPos < INPUT_STRING_SIZE) inputString[inputStringPos++] = inChar;
    }
  }
#endif
}

