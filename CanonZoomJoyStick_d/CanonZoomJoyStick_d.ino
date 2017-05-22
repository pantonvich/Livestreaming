
//#define DEBUG
#define SERVO_PIN 5   //orange sig, brown ground, red 5+
//#define HDMI_SOURCE

  #define DPIN_PJON_ME_3 2
  #define DPIN_PJON_ME_4 3
  
#ifdef SERVO_PIN
  //#include <Servo.h> 
  //Servo MyServo;
  #include <SoftwareServo.h>
  SoftwareServo MyServo;
  
  byte AngleCenter = 33;    // variable to store the servo position 
  byte AngleIn = 20;
  byte AngleOut = 45;
  unsigned long StopZoom = -1;
  unsigned long StopZoomKick = -1;
  byte LastPayload = AngleCenter;

  #define ZOOM_KICK 25
  #define ZOOM_JOG 500

 // #define DPIN_IN 6
 // #define DPIN_OUT 7

  #define DPIN_TLT_UP 8
  #define DPIN_TLT_DN 11
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
      
  #define PAN_TLT_JOG 500
  
  unsigned long StopPtzUp = -1;
  unsigned long StopPtzDn = -1;
  unsigned long StopPtzLt = -1;
  unsigned long StopPtzRt = -1;
  short PanPos = -1;
  short TltPos = -1;
#endif

  #define APIN_PAN A0
  #define APIN_TLT A1
  #define DPIN_PTZ_SW 4
  

  #define APIN_ZOM A7
  //#define APIN_TLT A1
  #define DPIN_PTZ_SW2 6
  #define APIN_PAN_POS A2
  #define APIN_TLT_POS A3
  
  
#include <PJON.h>

#define MAX_ATTEMPTS 2
#define PACKET_MAX_LENGTH 10

#define PJON_SEND_STATUS 10000
#define PJON_NETWORK_PIN  12
#define PJON_1 1  //don't skip as we need to set - need to rethink this
#define PJON_2 2
#define PJON_3 3
#define PJON_4 4
#define PJON_5 5

long PjonReceiveTime = 250;
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
  byte HdmiSwitchOnCurMillis = 10;
  unsigned long HdmiSwitchCurMillis = 0;
  int CurrentSource = 0;
#endif

#ifdef DEBUG
  #define INPUT_STRING_SIZE 6
  

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
  PJON_IO_MODE(BLINK_LED, OUTPUT);

#ifdef DEBUG
  Serial.begin(115200);
  DEBUG_PRINT(F("CanonZoomJoyStick"),"d");
#endif  


#ifdef HDMI_SOURCE
   PJON_IO_MODE(HDMI_SWITCH_DPIN, OUTPUT);
   DEBUG_PRINT(F("HDMI_SOURCE PIN"),HDMI_SWITCH_DPIN);
#endif

#ifdef SERVO_PIN
  MyServo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object 
  MyServo.write(AngleCenter);

//  pinMode(DPIN_IN, INPUT_PULLUP);
//  pinMode(DPIN_OUT, INPUT_PULLUP);
//  DEBUG_PRINT(F("SERVO_PIN IN"),DPIN_IN);
//  DEBUG_PRINT(F("SERVO_PIN Out"),DPIN_OUT);

  PJON_IO_MODE(DPIN_TLT_UP, OUTPUT);
  PJON_IO_MODE(DPIN_TLT_DN, OUTPUT);
  PJON_IO_MODE(DPIN_PAN_LT, OUTPUT);
  PJON_IO_MODE(DPIN_PAN_RT, OUTPUT);
#endif

  PJON_IO_MODE(DPIN_PTZ_SW, INPUT);
  PJON_IO_WRITE(DPIN_PTZ_SW, HIGH);

  PJON_IO_MODE(DPIN_PTZ_SW2, INPUT);
  PJON_IO_WRITE(DPIN_PTZ_SW2, HIGH);

  if(!digitalRead(DPIN_PJON_ME_3)) PJON_ME = PJON_3;
  if(!digitalRead(DPIN_PJON_ME_4)) PJON_ME = PJON_4;
  if (digitalRead(DPIN_PJON_ME_3) && digitalRead(DPIN_PJON_ME_4)) PJON_ME = PJON_2;
  if (!digitalRead(DPIN_PJON_ME_3) && !digitalRead(DPIN_PJON_ME_4)) PJON_ME = PJON_5;
  
  PjonNetwork.set_id(PJON_ME);
  
  PjonNetwork.strategy.set_pin(PJON_NETWORK_PIN);
  
  //PjonNetwork.set_error(pjon_error_handler);
  PjonNetwork.set_receiver(pjon_receiver_function);
  PjonNetwork.begin();
  
  DEBUG_PRINT(F("PJON_NETWORK_PIN"), PJON_NETWORK_PIN);
  DEBUG_PRINT(F("PJON_ME"), PJON_ME);
/*
  //test direction
   short panPos = analogRead(APIN_PAN_POS);
   short tltPos = analogRead(APIN_TLT_POS);

   PJON_IO_WRITE(DPIN_TLT_DN, LOW);
   PJON_IO_WRITE(DPIN_TLT_UP, HIGH);
   PJON_IO_WRITE(DPIN_PAN_LT, LOW);
   PJON_IO_WRITE(DPIN_PAN_RT, HIGH);
   
   delay(500);
   PJON_IO_WRITE(DPIN_TLT_UP, LOW);
   PJON_IO_WRITE(DPIN_PAN_RT, LOW);
   short movUp = analogRead(APIN_TLT_POS);
   short movRt = analogRead(APIN_PAN_POS);
   
   PJON_IO_WRITE(DPIN_TLT_DN, HIGH);
   PJON_IO_WRITE(DPIN_PAN_RT, HIGH);
   delay(500);  
   PJON_IO_WRITE(DPIN_TLT_DN, LOW);
   PJON_IO_WRITE(DPIN_PAN_RT, LOW);
   short movDn = analogRead(APIN_TLT_POS);
   short movLt = analogRead(APIN_PAN_POS);

   DEBUG_PRINT("panPos",panPos);
   DEBUG_PRINT("tltPos",tltPos);
   DEBUG_PRINT("movRt",movRt);
   DEBUG_PRINT("movUp",movUp);
   DEBUG_PRINT("movLt",movLt);
   DEBUG_PRINT("movDn",movDn);
   */
} 

unsigned long CurMillis;


bool TltPosIsActive = false;
bool PanPosIsActive = false;
bool UseLocalJoyStick = true;
bool RemoteJoyStickZoom = false;



void loop() 
{ 

  byte forceSend = 0;
  static short panPosF = 513;
  static short tltPosF = 513;


  CurMillis = millis();
  PjonNetwork.receive(PjonReceiveTime);  

  if (UseLocalJoyStick) localJoyStick();
  
  readSetFilter(APIN_PAN_POS, panPosF,1);
  readSetFilter(APIN_TLT_POS, tltPosF,1);

  //PjonNetwork.receive(PjonReceiveTime);  
  setPanPos(panPosF);
  //PjonNetwork.receive(PjonReceiveTime);  
  setTltPos(tltPosF);

  //static  long tiltC=analogRead(APIN_TILT);


  /*
  if (abs( panC - pan)>100 || abs( tiltC - tilt)>100) {
     DEBUG_PRINT("Pan ",panC );
     DEBUG_PRINT("Tlt ", tiltC);
     pan = panC;
     tilt = tiltC;  
  }*/
#ifdef SERVO_PIN
  if(CurMillis > StopPtzUp) {forceSend = 2; PJON_IO_WRITE(DPIN_TLT_UP, LOW); StopPtzUp = -1; PJON_IO_WRITE(BLINK_LED, LOW); DEBUG_PRINT("up","end");}
  if(CurMillis > StopPtzDn) {forceSend = 2; PJON_IO_WRITE(DPIN_TLT_DN, LOW); StopPtzDn = -1; PJON_IO_WRITE(BLINK_LED, LOW); DEBUG_PRINT("dn","end");}
  if(CurMillis > StopPtzLt) {forceSend = 1; PJON_IO_WRITE(DPIN_PAN_LT, LOW); StopPtzLt = -1; PJON_IO_WRITE(BLINK_LED, LOW); DEBUG_PRINT("lt","end");}
  if(CurMillis > StopPtzRt) {forceSend = 1; PJON_IO_WRITE(DPIN_PAN_RT, LOW); StopPtzRt = -1; PJON_IO_WRITE(BLINK_LED, LOW); DEBUG_PRINT("rt","end");}
  if(CurMillis > StopZoomKick) {adjustServo(LastPayload); StopZoomKick=-1; DEBUG_PRINT("zk","end");}
  if(CurMillis > StopZoom)  {adjustServo(AngleCenter); StopZoom=-1;  DEBUG_PRINT("zm","end");} 
  //PJON_IO_WRITE(BLINK_LED, ptzSwState);
  SoftwareServo::refresh();
  //PjonNetwork.receive(PjonReceiveTime);  
#endif

#ifdef DEBUG
  processSerialEvent();
#endif
  
#ifdef HDMI_SOURCE

  readHdmiLedStatusPins();
  checkSetSource();
  HdmiSwitch();
#endif

  PjonSendStatuses(panPosF, tltPosF,forceSend);  
  
  PjonNetwork.update();
} 

void localJoyStick(){
  static short panF = 513;
  static short tltF = 513;
  static short zomF = 513;
  static bool ptzSwState = false;
  static bool ptzSw2State = false;
  static bool ptzIsActive = false;

  bool ptzSW = debounceSwitch(DPIN_PTZ_SW);
  if (ptzSW) {
    ptzSwState = !ptzSwState;
    PJON_IO_WRITE(BLINK_LED, ptzSwState);
    DEBUG_PRINT("ptzSw",ptzSwState);
  }

  readSetFilter(APIN_PAN, panF,5);
  readSetFilter(APIN_TLT, tltF,5);
  readSetFilter(APIN_ZOM, zomF,5);
 
  if (ptzSwState) {
    zomF = panF;
    PJON_IO_WRITE(BLINK_LED, ptzSwState);
  
    if (zomF < 350) {
      setZoom(0,AngleOut);
    } else if (zomF > 650){
      setZoom(0,AngleIn);
    } else {
      setZoom(0,AngleCenter);
    }
  } else {  
     if (tltF < 350) {
        setPtz(PTZ_CN_UP);
        ptzIsActive = true;
     } else if (tltF > 650){
        setPtz(PTZ_CN_DN);
        ptzIsActive = true;
     }
     if (panF < 350) {
        setPtz(PTZ_RT_CN);
        ptzIsActive = true;
     } else if (panF > 650){
        setPtz(PTZ_LT_CN);
        ptzIsActive = true;
     }
     
     if (tltF >= 350 && tltF<= 650 && panF >= 350 && panF <= 650 && ptzIsActive == true) {
        setPtz(PTZ_CN_CN);
        ptzIsActive = false;
     }
  }
  
}

void setPanPos(short posF) {
  static short lstPos = posF;
  static short strtPos = posF;
  static unsigned long strtAt = 0;
  static unsigned long endAt = 0;
  static bool stopSign = false;

  
  if (PanPos > 0) {
    if (!PanPosIsActive) {
      stopSign = ((PanPos - posF) > 0);
      PanPosIsActive = true;
      strtPos = posF; 
      strtAt = CurMillis;
      DEBUG_PRINT("endAt strt",endAt); 
      endAt = CurMillis + PAN_TLT_JOG*2;  
    } else if ( stopSign != ((PanPos - posF) > 0) || endAt < CurMillis ) {

        DEBUG_PRINT("endAt stopsign",stopSign != ((PanPos - posF) > 0));
        DEBUG_PRINT("endAt time",endAt < CurMillis);
        
        StopPtzRt = CurMillis;
        StopPtzLt = CurMillis; 
        PanPos = -1;
        PanPosIsActive = false;
        DEBUG_PRINT("endAt","ended");
    } else if ((CurMillis - strtAt)/100 > abs(strtPos - posF)) {
      //runaway stop
        //DEBUG_PRINT("pan runaway?",abs(strtPos - posF));
    } else if (abs(lstPos - posF) > 1 ) {
        //only add more time if we moved   
        endAt = CurMillis + PAN_TLT_JOG*2;
        DEBUG_PRINT("endAt add",abs(lstPos - posF));
        DEBUG_PRINT("strtPos",strtPos);
        DEBUG_PRINT("lstPos",lstPos);
        lstPos = posF;
    } 
    if (PanPos > 0) {
      if (stopSign)  {
        setPtz(PTZ_RT_CN);
      } else {
        setPtz(PTZ_LT_CN);
      }
    }
  } 
}

void setTltPos(short posF) {
  static short lstPos = posF;
  static short strtPos = posF;
  static unsigned long strtAt = 0;
  static unsigned long endAt = 0;
  static bool stopSign = false;
  
  if (TltPos > 0) {
    if (!TltPosIsActive) {
      stopSign = ((TltPos - posF) > 0);
      TltPosIsActive = true; 
      strtAt = CurMillis;
      strtPos = posF;
      DEBUG_PRINT("endAt strt",endAt);
      endAt = CurMillis + PAN_TLT_JOG*5;   
    } else if ( stopSign != ((TltPos - posF) > 0) || endAt < CurMillis ) {

        DEBUG_PRINT("endAt stopsign",stopSign != ((TltPos - posF) > 0));
        DEBUG_PRINT("endAt time",endAt < CurMillis);
        StopPtzUp = CurMillis;
        StopPtzDn = CurMillis; 
        TltPos = -1;
        TltPosIsActive = false;
     } else if ((CurMillis - strtAt)/50 > abs(strtPos - posF)) {
      //runaway stop
        //DEBUG_PRINT("tilt runaway?",abs(strtPos - posF)); 
    } else if (abs(lstPos - posF) > 1 ) {
        //only add more time if we moved   
        endAt = CurMillis + PAN_TLT_JOG*5;
        DEBUG_PRINT("endAt add",endAt);
        DEBUG_PRINT("lstPos - posF",(lstPos - posF));
        DEBUG_PRINT("stopSign",stopSign);
        DEBUG_PRINT("strtPos",strtPos);
        DEBUG_PRINT("lstPos",lstPos);
        lstPos = posF;
    } 
    if (TltPos > 0) {
      if (stopSign)  {
        setPtz(PTZ_CN_UP);
      } else {
        setPtz(PTZ_CN_DN);
      }
    }
  } 
}


void readSetFilter(byte apin,short &value,short filter) {
  value += (( analogRead(apin)- value)/filter);
}
    
#ifdef HDMI_SOURCE

byte HdmiLedStatusPins[] = {1,2,3,6,7};
unsigned int HdmiLedStatus[] = {0,0,0,0,0};
unsigned int HdmiLedStatusLtOnTrip = 900;  //if below this then it's the current source

bool isHdmiLedStatusOn(int ledPos){ 
  return ( ledPos > -1 && ledPos < HDMI_LED_STATUS_CT && HdmiLedStatus[ledPos] > 200 && HdmiLedStatus[ledPos] < HdmiLedStatusLtOnTrip);
}

#endif


void PjonSendStatuses(short panPosF, short tltPosF, byte forceSend) {
  static bool flag;
  static unsigned long sendCurMillis = 0;
  if ( CurMillis < sendCurMillis && forceSend==0) return;
  
  sendCurMillis = CurMillis + PJON_SEND_STATUS;   
  flag = !flag;
  
  if (flag && forceSend != 2) { 
    pjonPosSend(panPosF, 'p'); 
    DEBUG_PRINT("panP",panPosF); 
  } else { 
    pjonPosSend(tltPosF, 't'); 
    DEBUG_PRINT("tltP",tltPosF); 
  }
    
#ifdef HDMI_SOURCE
  static flag = true;
  if (flag) dig[digCt++] = '*';
  flag = !flag;
  dig[digCt++] = '\0';
  DEBUG_PRINT("dig", dig);
  DEBUG_PRINT("digCt", digCt);

  PjonNetwork.send(PJON_1, dig, digCt);
  byte pinToSend = 0;
  if (pinToSend == HDMI_LED_STATUS_CT) pinToSend = 0; 
  dig[digCt++] = '0' + CurrentSource;
  
  for(byte x = 0; x < HDMI_LED_STATUS_CT; x++)
  {
    if (isHdmiLedStatusOn(x)) {
      dig[digCt++] = '1' + x;
      pinToSend = x;
      break;
    }
  }

  if (flag) dig[digCt++] = '*';
  flag = !flag;
  dig[digCt++] = '\0';
  DEBUG_PRINT("dig", dig);
  DEBUG_PRINT("digCt", digCt);

  PjonNetwork.send(PJON_1, dig, digCt);
#endif

}

void pjon_receiver_function(uint8_t *payload, uint16_t lenMsg, const PJON_Packet_Info &packet_info) {

  char msg[10];
  byte ct = min(lenMsg,10);
  for(int x = 0; x < ct; x++) msg[x] = payload[x];
  msg[ct] = '\0';
  DEBUG_PRINT("prf", msg);
  
  process_message(payload, lenMsg);
  
}

void process_message(uint8_t *payload, uint16_t lenMsg) {

//DEBUG_PRINT("payload",*payload);
//DEBUG_PRINT("lenMsg",lenMsg);

#ifdef SERVO_PIN
    byte angle = 0;
    if(lenMsg > 2) {
      if(payload[1] == 's') {
        StopZoom = StopPtzUp = StopPtzDn = StopPtzLt = StopPtzRt = CurMillis; 
        TltPos = -1;
        PanPos = -1;
        PanPosIsActive = false; 
        TltPosIsActive = false; 
        adjustServo(AngleCenter);   
        DEBUG_PRINT("stop","stop");  
      }
      else if(payload[1] == 'L') {UseLocalJoyStick = (payload[2] - '0');DEBUG_PRINT("JoyStick",UseLocalJoyStick); }
      else if(payload[1] == 'i') angle = AngleIn;
      else if(payload[1] == 'o') angle = AngleOut;
      else if(payload[1] == 'c') angle = AngleCenter; 
      else if(payload[1] == 'C') {
          byte value = (payload[2] - '0') * 10 + (payload[3] - '0');
          AngleCenter = value;
      } else if(payload[1] == 'I') {
          byte value = (payload[2]  - '0') * 10 + (payload[3] - '0');
          AngleIn = value;
      } else if(payload[1] == 'O') {
          byte value = (payload[2]  - '0') * 10 + (payload[3] - '0');
          AngleOut = value;
      } else if(payload[1] == 'j') {   
          setPtz(payload[2]);
      } else if(payload[1] == 'p' && lenMsg > 3) {
          //PanMov = true;   
          PanPos = (payload[2] - '0') * 100 + (payload[3] - '0') * 10 + (payload[4] - '0');
          PanPosIsActive = false;  
          DEBUG_PRINT("PanPos",PanPos);
      } else if(payload[1] == 't' && lenMsg > 3) { 
           
          TltPos = (payload[2] - '0') * 100 + (payload[3] - '0') * 10 + (payload[4] - '0'); 
          TltPosIsActive = false;  
          DEBUG_PRINT("TltPos",TltPos);
      }
    }

    if (angle>0) setZoom(payload[2] - '0', angle);
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
      SendCurMillis = CurMillis; //resend now
      HdmiSwitchAttempt = 0;
    }
    return;
  }

  //push switch
  HdmiSwitchState = HIGH;
  PJON_IO_WRITE(HDMI_SWITCH_DPIN, HdmiSwitchState);
  HdmiSwitchCurMillis = CurMillis + HdmiSwitchOnCurMillis;
  HdmiSwitchAttempt--;
  DEBUG_PRINT(F("attempt High"), HdmiSwitchAttempt);

}

void HdmiSwitch() {
  if (CurMillis > HdmiSwitchCurMillis && HdmiSwitchState == HIGH) {
     HdmiSwitchState = LOW;
     digitalWrite(HDMI_SWITCH_DPIN, HdmiSwitchState);
     DEBUG_PRINT(F("attempt Low"), HdmiSwitchAttempt);
     readHdmiLedStatusPins();
     //checkSetSource();
     serialPrintHdmiLedStatuses();
  }
}
#endif

#ifdef DEBUG

  void processSerialEvent(){
    if (StringComplete) {
      InputString[InputStringPos] = '\0';
      //processSerialReceive()
      DEBUG_PRINT("PSE",InputString);
      process_message((uint8_t*)InputString,InputStringPos);
      // clear the string:
      InputStringPos = 0;
      //get ready for next string
      StringComplete = false;
    }
  }
#endif

#ifdef SERVO_PIN

void setZoom(byte zoomTime, byte angle){  
  //static byte last0 = 0;
  //static byte last1 = 0;
  //static byte last2 = 0;
  static byte ctCn = 0;

  if (angle != AngleCenter && ctCn > 0) ctCn--;

  //if (angle != AngleCenter &&  (last0 == AngleCenter || last1 == AngleCenter) ){
  //  last2 = last1;
  //  last1 = last0;    
  //  last0 = angle;
 if(angle != AngleCenter && ctCn > 0 && zoomTime == 0) {
    DEBUG_PRINT("skz", angle);
    return;
  }

 if (angle == AngleCenter) ctCn=4;
    
  adjustServo(angle);
  if(StopZoom > CurMillis && zoomTime > 0)  {
    StopZoomKick = CurMillis + ZOOM_KICK;
    StopZoom = CurMillis + zoomTime * 1000;
  } else if (zoomTime == 0){
    StopZoomKick = CurMillis + ZOOM_KICK;
    StopZoom = CurMillis + ZOOM_JOG;
  }
  //DEBUG_PRINT("CurMillis",CurMillis);
 //DEBUG_PRINT("szk",StopZoomKick);
 // DEBUG_PRINT("sz",StopZoom);
 // DEBUG_PRINT("szj",ZOOM_JOG);
 // DEBUG_PRINT("szt",zoomTime);
}

void setPtz(byte ptz) { //,bool ptzSw) {
static byte last0 = 0;
//static byte last1 = 0;

  //if (ptz != PTZ_CN_CN &&  (last0 == PTZ_CN_CN || last1 == PTZ_CN_CN) ){
  //  last1 = last0;    
  //  last0 = ptz;
  //  DEBUG_PRINT("skp",(char)ptz);
  //  return;
  //}
    static byte ctCn = 0;

  if (ptz != PTZ_CN_CN && ctCn > 0) ctCn--;
  if(ptz != PTZ_CN_CN && ctCn > 0) {
    DEBUG_PRINT("skp", (char)ptz);
    return;
  }

 if (ptz == PTZ_CN_CN) ctCn=4;
 
      if(ptz == PTZ_LT_UP || ptz == PTZ_CN_UP || ptz == PTZ_RT_UP) {
        if(last0 != ptz ) {
          PJON_IO_WRITE(DPIN_TLT_DN, LOW);
          PJON_IO_WRITE(DPIN_TLT_UP, HIGH);
          }
          DEBUG_PRINT("up",(char)ptz);
        
        PJON_IO_WRITE(BLINK_LED, HIGH);
        StopPtzUp = CurMillis + PAN_TLT_JOG;
      } 
      if(ptz == PTZ_LT_DN || ptz == PTZ_CN_DN || ptz == PTZ_RT_DN) {
        if(last0 != ptz ) {
          PJON_IO_WRITE(DPIN_TLT_UP, LOW);
          PJON_IO_WRITE(DPIN_TLT_DN, HIGH);
          }
          DEBUG_PRINT("dn",(char)ptz);
        
        PJON_IO_WRITE(BLINK_LED, HIGH);
        StopPtzDn = CurMillis + PAN_TLT_JOG;
      } 
      if(ptz == PTZ_RT_UP || ptz == PTZ_RT_CN || ptz == PTZ_RT_DN) {
        if(last0 != ptz ) {
          PJON_IO_WRITE(DPIN_PAN_LT, LOW);
          PJON_IO_WRITE(DPIN_PAN_RT, HIGH);
          }
           DEBUG_PRINT("rt",(char)ptz);
        
        PJON_IO_WRITE(BLINK_LED, HIGH);
        StopPtzRt = CurMillis + PAN_TLT_JOG;
      } 
      if(ptz == PTZ_LT_UP || ptz == PTZ_LT_CN || ptz == PTZ_LT_DN) {
        if(last0 != ptz ) {
          PJON_IO_WRITE(DPIN_PAN_RT, LOW);
          PJON_IO_WRITE(DPIN_PAN_LT, HIGH);
          }
          DEBUG_PRINT("lt",(char)ptz);
        
        PJON_IO_WRITE(BLINK_LED, HIGH);
        StopPtzLt = CurMillis + PAN_TLT_JOG;
      }

      if (ptz == PTZ_CN_CN && last0 != PTZ_CN_CN) {
       StopPtzLt = CurMillis;
       StopPtzRt = CurMillis;
       StopPtzUp = CurMillis;
       StopPtzDn = CurMillis;
       //DEBUG_PRINT("cn",(char)last0);
      }
  //last1 = last0;    
  if (!TltPosIsActive && !PanPosIsActive) last0 = ptz;
}
#endif

#ifdef SERVO_PIN
void adjustServo(byte payload){
  byte payload2 = payload;

  if (payload < 180) {

    PJON_IO_WRITE(BLINK_LED, payload != AngleCenter);

    if (payload != AngleCenter) {
      if ((LastPayload > payload2 || payload < AngleCenter ) && payload > 10)
        payload -= 5;
      else if (LastPayload < payload2 || payload2 > AngleCenter) payload += 5;
    }

    if (LastPayload != payload2) {
      MyServo.write(payload);  
      LastPayload = payload2;
      DEBUG_PRINT(F("p1"), payload); 
      DEBUG_PRINT(F("p2"),payload2);
    }
  }
}
#endif

#ifdef DEBUG
void serialEvent() {

  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n' || inChar == 'z') { 
      //InputString[InputStringPos++] = '\0';
      StringComplete = true;
      Serial.println(F(" end "));
    } else {
      // add it to the InputString:
      if(InputStringPos < INPUT_STRING_SIZE) InputString[InputStringPos++] = inChar;
    }
  }

}
#endif

void pjonPosSend(short v, char ptz) {
    char value[] = "0t000";
    value[0] = '0' + PJON_ME;
    value[1] = ptz;
    byte t = 100;
    if (v > 999) v = 999;
    for(byte x = 2; x < 5; x++) {
      byte n =  (v / t);
       value[x] = '0' + n;
       v -= n * t;
       t /= 10;
    }
    DEBUG_PRINT("pjonPosSend",value);
    PjonNetwork.send(PJON_1,value,5);
}

bool debounceSwitch(byte pin)
{
//http://www.eng.utah.edu/~cs5780/debouncing.pdf
static uint8_t State = 0; // Current debounce status
State = (State << 1) | !PJON_IO_READ(pin) | 0xe0;
if (State == 0xf0) return true;
return false;
} 
