
#define DEBUG
#define SERVO_PIN 5   //orange sig, brown ground, red 5+
//#define HDMI_SOURCE

#ifdef SERVO_PIN
  //#include <Servo.h> 
  //Servo MyServo;
  #include <SoftwareServo.h>
  SoftwareServo MyServo;
  
  byte AngleCenter = 33;    // variable to store the servo position 
  byte AngleIn = 20;
  byte AngleOut = 45;
  unsigned long StopZoom = 0;
  unsigned long StopZoomKick = 0;
  byte LastPayload = AngleCenter;

  #define ZOOM_KICK 100
  #define ZOOM_JOG 150

  #define DPIN_PJON_ME_3 2
  #define DPIN_PJON_ME_4 3
  
  #define DPIN_IN 6
  #define DPIN_OUT 7

  #define DPIN_TILT_UP 8
  #define DPIN_TILT_DN 11
  #define DPIN_PAN_LT 10
  #define DPIN_PAN_RT 9

  #define PTZ_LT_DN '1'
  #define PTZ_CN_DN '2'
  #define PTZ_RT_DN '3'
  #define PTZ_LT_CN '4'
  #define PTZ_CN_CN '5'
  #define PTZ_RT_CN '6'
  #define PTZ_LT_UP '7'
  #define PTZ_CN_UP '8'
  #define PTZ_RT_UP '9'  
      
  #define PAN_TILT_JOG 150
  unsigned long StopPanTiltUp = 0;
  unsigned long StopPanTiltDn = 0;
  unsigned long StopPanTiltLt = 0;
  unsigned long StopPanTiltRt = 0;
#endif

#include <PJON.h>

#define MAX_ATTEMPTS 3
#define PACKET_MAX_LENGTH 10

#define PJON_SEND_STATUS 30000
#define PJON_NETWORK_PIN  12
#define PJON_1 1  //don't skip as we need to set - need to rethink this
#define PJON_2 2
#define PJON_3 3
#define PJON_4 4

uint8_t PJON_ME = PJON_2;

PJON<SoftwareBitBang> PjonNetwork(255);

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
  #define HDMI_LED_STATUS_CT 5
  #define PUSH_SWITCH_ATTEMPTS 11
  #define HDMI_SWITCH_DPIN 2 //digi pin used as push switch
  byte HdmiSwitchAttempt = 0;
  byte HdmiSwitchState = LOW;
  byte HdmiSwitchOnMs = 10;
  unsigned long HdmiSwitchMs = 0;
  int CurrentSource = 0;
#endif

#ifdef DEBUG
  #define INPUT_STRING_SIZE 3
  char InputString[INPUT_STRING_SIZE + 1];         // a string to hold incoming data
  boolean StringComplete = false;  // whether the string is complete
  byte InputStringPos = 0;
#endif

#define BLINK_LED 13

void setup() 
{ 
  //set pins to know what pjon id to use
  pinMode(DPIN_PJON_ME_3, INPUT_PULLUP);
  pinMode(DPIN_PJON_ME_4, INPUT_PULLUP);
  
  // set pin for blink LED
  pinModeFast(BLINK_LED, OUTPUT);

#ifdef DEBUG
  Serial.begin(115200);
  DEBUG_PRINT(F("CanonZoom"),"c");
#endif  


#ifdef HDMI_SOURCE
   pinModeFast(HDMI_SWITCH_DPIN, OUTPUT);
   DEBUG_PRINT(F("HDMI_SOURCE PIN"),HDMI_SWITCH_DPIN);
#endif

#ifdef SERVO_PIN
  MyServo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object 
  MyServo.write(AngleCenter);

  pinMode(DPIN_IN, INPUT_PULLUP);
  pinMode(DPIN_OUT, INPUT_PULLUP);
  DEBUG_PRINT(F("SERVO_PIN IN"),DPIN_IN);
  DEBUG_PRINT(F("SERVO_PIN Out"),DPIN_OUT);
#endif

  if(!digitalRead(DPIN_PJON_ME_3)) PJON_ME = PJON_3;
  if(!digitalRead(DPIN_PJON_ME_4)) PJON_ME = PJON_4;

  pinModeFast(DPIN_TILT_UP, OUTPUT);
  pinModeFast(DPIN_TILT_DN, OUTPUT);
  pinModeFast(DPIN_PAN_LT, OUTPUT);
  pinModeFast(DPIN_PAN_RT, OUTPUT);
  
  PjonNetwork.set_id(PJON_ME);
  
  PjonNetwork.strategy.set_pin(PJON_NETWORK_PIN);
  
  //PjonNetwork.set_error(pjon_error_handler);
  PjonNetwork.set_receiver(pjon_receiver_function);
  PjonNetwork.begin();
  
  DEBUG_PRINT(F("PJON_NETWORK_PIN"), PJON_NETWORK_PIN);
  DEBUG_PRINT(F("PJON_ME"), PJON_ME);

} 

unsigned long Ms;
unsigned long SendMs = 0;

void loop() 
{ 
  
  Ms=millis();
  PjonNetwork.receive(500);  

  if(Ms > StopPanTiltUp) {digitalWriteFast(DPIN_TILT_UP, LOW); StopPanTiltUp = -1;}
  if(Ms > StopPanTiltDn) {digitalWriteFast(DPIN_TILT_DN, LOW); StopPanTiltDn = -1;}
  if(Ms > StopPanTiltLt) {digitalWriteFast(DPIN_PAN_LT, LOW); StopPanTiltLt = -1;}
  if(Ms > StopPanTiltRt) {digitalWriteFast(DPIN_PAN_RT, LOW); StopPanTiltRt = -1;}

#ifdef SERVO_PIN
  int pinInValue = digitalRead(DPIN_IN);
  int pinOutValue = digitalRead(DPIN_OUT);
  static int pinValueLast = 0;
  if (pinInValue == 1 && pinOutValue == 1 && pinValueLast != 0){
    adjustServo(AngleCenter);
    StopZoom=-1;
    pinValueLast = 0;
  } else if (!pinInValue) {
    if (pinValueLast > -1 ) adjustServo(AngleIn);   
    StopZoom=Ms+200;
    pinValueLast = -1;
  } else if (!pinOutValue) {
    if (pinValueLast < 1 ) adjustServo(AngleOut);
    StopZoom=Ms+200;
    pinValueLast = 1;
  }
  //DEBUG_PRINT("pinValueLast",pinValueLast);
  //DEBUG_PRINT("pinInValue",pinInValue);
  //DEBUG_PRINT("pinOutValue",pinOutValue);

  if(StopZoomKick < Ms) {
    adjustServo(LastPayload);
    StopZoomKick=-1;
  }
  if(StopZoom < Ms)  {
    //Serial.println(F("zoomStop"));
    adjustServo(AngleCenter);
    StopZoom=-1;
  }
  SoftwareServo::refresh();
#endif

#ifdef HDMI_SOURCE
  processSerialEvent();
  readHdmiLedStatusPins();
  checkSetSource();
  HdmiSwitch();
#endif

  if ( Ms > SendMs ) {
    SendMs = Ms + PJON_SEND_STATUS;
    serialPrintHdmiLedStatuses();  
  }

  PjonNetwork.update();
} 

#ifdef HDMI_SOURCE

byte HdmiLedStatusPins[] = {1,2,3,6,7};
unsigned int HdmiLedStatus[] = {0,0,0,0,0};
unsigned int HdmiLedStatusLtOnTrip = 900;  //if below this then it's the current source

bool isHdmiLedStatusOn(int ledPos){ 
  return ( ledPos > -1 && ledPos < HDMI_LED_STATUS_CT && HdmiLedStatus[ledPos] > 200 && HdmiLedStatus[ledPos] < HdmiLedStatusLtOnTrip);
}

#endif

void serialPrintHdmiLedStatuses() {
  static bool flag;
  char dig[] = "S000";
  byte digCt = 0;
  
  dig[digCt++] = '0' + PJON_ME;

#ifdef HDMI_SOURCE
  byte pinToSend = 0;
  if (pinToSend == HDMI_LED_STATUS_CT) pinToSend = 0; 
  dig[digCt++] = '0' + CurrentSource;
  
  for(byte x = 0; x < HDMI_LED_STATUS_CT; x++)
  {
    //Serial.print(HdmiLedStatus[x]);
    //Serial.print(" ");
    if (isHdmiLedStatusOn(x)) {
      dig[digCt++] = '1' + x;
      pinToSend = x;
      break;
    }
  }
#endif

  /*for(byte x = pinToSend+1; x < HDMI_LED_STATUS_CT; x++)
  {
    Serial.print(HdmiLedStatus[x]);
    Serial.print(" ");
  }
  */

  if (flag) dig[digCt++] = '*';
  flag = !flag;
  dig[digCt++] = '\0';
  DEBUG_PRINT("dig", dig);
  DEBUG_PRINT("digCt", digCt);
  //Serial.print(" ");
  //Serial.print(HdmiLedStatus[pinToSend]/40);
  //Serial.print(" ");
  //Serial.print(HdmiLedStatus[pinToSend]);
  //Serial.println();

  PjonNetwork.send(PJON_1, dig, digCt);
}

void pjon_receiver_function(uint8_t *payload, uint16_t lenMsg, const PacketInfo &packet_info) {

  char msg[10];
  byte ct = min(lenMsg,10);
  for(int x = 0; x < ct; x++) msg[x] = payload[x];
  msg[ct] = '\0';
  DEBUG_PRINT("prf", msg);
  DEBUG_PRINT("len", lenMsg);

#ifdef SERVO_PIN
    byte angle = 0;
    unsigned long curMillis = millis();
    if(lenMsg > 2) {        
      if(payload[1] == 'i') angle = AngleIn;
      else if(payload[1] == 'o') angle = AngleOut;
      else if(payload[1] == 'c') angle = AngleCenter; 
      else if(payload[1] == 'C') {
        byte value = (payload[2] - '0') * 10 + (payload[3] - '0');
        AngleCenter = value;
      } else if(payload[1] == 'I') {
        byte value = (payload[2]  - '0') * 10 + (payload[3] - '0');
        AngleIn = value;
      }  else if(payload[1] == 'O') {
        byte value = (payload[2]  - '0') * 10 + (payload[3] - '0');
        AngleOut = value;
      } else if(payload[1] == 'p') {   
        byte ptz = payload[2];
        if(ptz == PTZ_LT_UP || ptz == PTZ_CN_UP || ptz == PTZ_RT_UP) {
          DEBUG_PRINT("up", 0);
          digitalWriteFast(DPIN_TILT_UP, HIGH);
          StopPanTiltUp = curMillis + PAN_TILT_JOG;
        } 
        if(ptz == PTZ_LT_DN || ptz == PTZ_CN_DN || ptz == PTZ_RT_DN) {
          DEBUG_PRINT("dw", 0);
          digitalWriteFast(DPIN_TILT_DN, HIGH);
          StopPanTiltDn = curMillis + PAN_TILT_JOG;
        } 
        if(ptz == PTZ_RT_UP || ptz == PTZ_RT_CN || ptz == PTZ_RT_DN) {
          DEBUG_PRINT("rt", 0);
          digitalWriteFast(DPIN_PAN_RT, HIGH);
          StopPanTiltRt = curMillis + PAN_TILT_JOG;
        } 
        if(ptz == PTZ_LT_UP || ptz == PTZ_LT_CN || ptz == PTZ_LT_DN) {
          DEBUG_PRINT("lt", 0);
          digitalWriteFast(DPIN_PAN_LT, HIGH);
          StopPanTiltLt = curMillis + PAN_TILT_JOG;
        }
      }
    }

    if (angle>0) {
      adjustServo(angle);
      byte payload2 = payload[2] - '0';
      if(StopZoom > curMillis && payload2 > 0)  {
        StopZoomKick = curMillis + ZOOM_KICK;
        StopZoom = curMillis + payload2 * 1000;
      } else if (payload2 = 0){
        StopZoom = curMillis + ZOOM_JOG;
      }
    }
#endif
 
#ifdef HDMI_SOURCE
if(lenMsg > 2) {
  if(payload[1] == 'V') {
      byte value = (payload[2]  - '0');
      if (value > HDMI_LED_STATUS_CT) {
        DEBUG_PRINT(F("V"), value);
        DEBUG_PRINT(F(" error V>="), HDMI_LED_STATUS_CT);
        
        return;
      }
        HdmiSwitchAttempt = PUSH_SWITCH_ATTEMPTS;
        CurrentSource = value;
        DEBUG_PRINT(F("V"), value);
    } 
}    
#endif

    //PjonNetwork.send(PJON_1, msg, ct);
    //PjonNetwork.update();
}

#ifdef HDMI_SOURCE
void readHdmiLedStatusPins() {
  for(int x = 0; x < HDMI_LED_STATUS_CT; x++) {
    HdmiLedStatus[x] = analogRead(HdmiLedStatusPins[x]);
  } 
}

void checkSetSource() {

  if ( CurrentSource >= HDMI_LED_STATUS_CT  
    || HdmiSwitchState == HIGH
    || HdmiSwitchAttempt == 0
       ) return; 
       
  if (isHdmiLedStatusOn(CurrentSource - 1) || HdmiSwitchAttempt == 1) {
    if(HdmiSwitchAttempt > 0) {
      SendMs = Ms; //resend now
      HdmiSwitchAttempt = 0;
    }
    return;
  }

  //push switch
  HdmiSwitchState = HIGH;
  digitalWriteFast(HDMI_SWITCH_DPIN, HdmiSwitchState);
  HdmiSwitchMs = Ms + HdmiSwitchOnMs;
  HdmiSwitchAttempt--;
  DEBUG_PRINT(F("attempt High"), HdmiSwitchAttempt);

}

void HdmiSwitch() {
  if (Ms > HdmiSwitchMs && HdmiSwitchState == HIGH) {
     HdmiSwitchState = LOW;
     digitalWrite(HDMI_SWITCH_DPIN, HdmiSwitchState);
     DEBUG_PRINT(F("attempt Low"), HdmiSwitchAttempt);
     readHdmiLedStatusPins();
     //checkSetSource();
     serialPrintHdmiLedStatuses();
  }
}
#endif

void processSerialEvent(){
  if (StringComplete) {
    InputString[InputStringPos] = '\0';
    processSerialReceive();
    // clear the string:
    InputStringPos = 0;
    //get ready for next string
    StringComplete = false;
  }
}

void processSerialReceive(){
#ifdef SERVO_PIN
  
  byte payload = InputString[0];
  byte input = 0;
  if(InputStringPos == 3 && payload == 'c') {
    payload = (InputString[1]  - '0') * 10 + (InputString[2] - '0');
    AngleCenter = payload;
    
    DEBUG_PRINT(F("center"), payload);
  
  } else if(InputStringPos > 1) {
    payload = (payload  - '0') * 10 + (InputString[1] - '0');
  }
  
  adjustServo(payload);
  
  if(InputStringPos > 2) {
    StopZoom = Ms + (InputString[2] - '0') * 1000;
  } else {
    StopZoom = Ms + 10000;
  }

#endif
}

#ifdef SERVO_PIN
void adjustServo(byte payload){
  byte payload2 = payload;
  //char msg[5];
  
  if (payload < 180) {

    digitalWriteFast(BLINK_LED, payload != AngleCenter);
    
    if ((LastPayload > payload2 || payload < AngleCenter ) && payload > 10)
      payload -= 5;
    else if (LastPayload < payload2 || payload2 > AngleCenter) payload += 5;

    DEBUG_PRINT(F("p1"), payload);
    
    MyServo.write(payload);
    
    //delay(100);
    //MyServo.write(payload2);   
    //DEBUG_PRINT(F("p2"),payload2);   
    LastPayload = payload2;
  }
}
#endif

#ifdef DEBUG
void serialEvent() {

  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
     Serial.print(inChar);
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n' || inChar == 'z') { 
      StringComplete = true;
      //Serial.println(F(" end "));
    } else {
      // add it to the InputString:
      if(InputStringPos < INPUT_STRING_SIZE) InputString[InputStringPos++] = inChar;
    }
  }

}
#endif
