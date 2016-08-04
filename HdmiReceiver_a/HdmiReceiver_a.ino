#include <PJON.h>
#define blinkLedPin 13 //led for status
#define pushSwitchPin 2 //digi pin used as push switch
#define PJON_NETWORK_PIN 12
#define PJON_ME 2

#define PJON_1 1
#define PJON_2 2
#define PJON_3 3
#define PJON_4 4

// network(Arduino pin used, selected device id)
//PJON pjonNetwork(PJON_NETWORK_PIN, PJON_ME);
// network(Arduino pin used, selected device id)
PJON<SoftwareBitBang> pjonNetwork(PJON_ME);

int currentSource = 0;

#define ledStatusCt 5
#define PJON_SEND_STATUS 5000


byte ledStatusPins[] = {1,2,3,6,7};
unsigned int ledStatus[] = {0,0,0,0,0};
unsigned int ledStatusLtOnTrip = 900;  //if below this then it's the current source
//unsigned int ledStatusGtNA_Trip = 800; //if above this then there isn't a source
//unsigned long sourceNotFoundSendMs = 0;
//bool sourceNotFound = false;

#define PUSH_SWITCH_ATTEMPTS 11
byte pushSwitchAttempt = 0;
byte pushSwitchState = LOW;
byte pushSwitchOnMs = 10;
unsigned long pushSwitchMs = 0;
unsigned long ms = 0;
unsigned long sendMs = 0;

unsigned long blinkLedMs = 0;
unsigned int blinkLedOnMs = 150; //275
unsigned int blinkLedOffMs = 100; //150
unsigned int blinkLedBetweenBlinkMs = 3000;

byte blinkLedState = LOW;
byte blinkLedFlag = true;

bool testing = false;
bool test6 = false;

void setup() {
  
  pinModeFast(blinkLedPin, OUTPUT);
  digitalWriteFast(blinkLedPin, LOW);
  
  pinModeFast(pushSwitchPin, OUTPUT);
  digitalWriteFast(pushSwitchPin, LOW);
  
  Serial.begin(115200);
  Serial.println("HDMI Receiver va");
  Serial.print(" PJON_NETWORK_PIN: ");
  Serial.println(PJON_NETWORK_PIN);
  Serial.print(" Device Id (PJON_ME): ");
  Serial.println(PJON_ME);

    pjonNetwork.set_pin(PJON_NETWORK_PIN);
  pjonNetwork.begin();
  pjonNetwork.set_receiver(pjon_receiver_function);
};

void pjon_receiver_function(uint8_t length, uint8_t *payload) {
  static unsigned long testingTime;
  Serial.print("prf: ");
  char msg[10];
  byte ct = min(length,10);
  for(int x = 0;x<ct;x++) msg[x]=payload[x];
  msg[ct] = '\0';
  Serial.print(length);
  Serial.print(" ");
  Serial.println(msg );

  if (testing && (millis() - testingTime > 11000)) { 
    testing = false; 
    Serial.print("Testing timed out - : ");
    for (byte i = 0; i < length ; i++){
      Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("Done Testing!!!");
  }
  
  if (!testing && length == 2 && payload[0] == 'V' ) {
    test6 = (payload[1] == '6');
    byte temp = payload[1] - '0';
    if (temp > ledStatusCt - 1) {
      Serial.print(temp);
      Serial.print(" req source >= ");
      Serial.println(ledStatusCt);
      return;
    }
    pushSwitchAttempt = PUSH_SWITCH_ATTEMPTS;
    currentSource = temp;
    Serial.print("V: ");
    Serial.print(currentSource);
    Serial.println();
    //sendMs = ms; //resend now
    
  } else if (!testing && length == 3 && payload[0] == 'p' && payload[1] == '1') {
    test6 = false;
    pushSwitchOnMs = payload[2];
  } else if (!testing && length == 3 && payload[0] == 'b' && payload[1] == 'l') {
    test6 = false;
    blinkLedFlag = payload[2] - '0';
    Serial.print("bl ");
    Serial.println(blinkLedFlag);
  } else if (!testing && length == 3 && payload[0] == 'T' && payload[1] == 'S' && payload[2] == 'T') {
      test6 = false;
      testingTime = millis();
      testing = true; 
      Serial.println("Testing!!!");           
  } else if ( length == 5 && payload[0] == 'R' ) {
      testing = false;
      Serial.print("Testing Results are: ");
      for (byte i = 0; i < length ; i++){
        Serial.print((char)payload[i]);
      }
      Serial.println();
      Serial.println("Done Testing!!!");
  }

}
void readLedStatusPins() {
  for(int x=0;x<ledStatusCt;x++) {
    ledStatus[x] = analogRead(ledStatusPins[x]);
    //Serial.print(ledStatus[x]);
    //Serial.print(" ");
  } 
  //Serial.println(" aR ");
}

void checkSetSource() {

    if ( currentSource >= ledStatusCt  
    || pushSwitchState == HIGH
    || pushSwitchAttempt == 0
       ) return; 
       
  if (isLedStatusOn(currentSource) || pushSwitchAttempt == 1) {
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
  Serial.print(pushSwitchAttempt);
  Serial.println(" Push High");
}

void pushSwitch() {
  if (ms > pushSwitchMs && pushSwitchState == HIGH) {
     pushSwitchState = LOW;
     digitalWriteFast(pushSwitchPin, pushSwitchState);
     readLedStatusPins();
     //checkSetSource();
     serialPrintLedStatuses();
  }
}

void blinkLed() {
  static byte blinkLedCt = 0; 
  if (currentSource < 0) return;
  
  if (ms > blinkLedMs && blinkLedState == HIGH) {
     blinkLedState = LOW;
     blinkLedMs = ms + blinkLedOffMs;
     digitalWriteFast(blinkLedPin, blinkLedState);
     blinkLedCt--;
     //Serial.print(blinkLedCt);
     //Serial.print(" ");
     if (blinkLedCt == 0) {
       blinkLedCt = currentSource + 1;
       blinkLedMs += blinkLedBetweenBlinkMs;
     }
   } else if (ms > blinkLedMs && blinkLedState == LOW) {
       blinkLedState = HIGH;
       blinkLedMs = ms + blinkLedOnMs;
       digitalWriteFast(blinkLedPin, blinkLedState);
   }
}

void loop() {
  ms = millis();

  
  //if ( ms > sendMs ) {
  readLedStatusPins();
  checkSetSource();
  //}
  pushSwitch();

  if (blinkLedFlag) blinkLed();
  if ( ms > sendMs ) {
    sendMs = ms + PJON_SEND_STATUS;
    serialPrintLedStatuses();
   //pjonNetwork.update();
  }
   //pjonNetwork.receive(1000);
   pjonNetwork.update();
  
}

void serialPrintLedStatuses() {
    //pjonNetwork.send(1,"23", 2);
    //return;
  char dig[] = "S000";
  byte digCt = 0;
  static byte pinToSend = 0;
  
  static byte test = 10;
  if (pinToSend == ledStatusCt) pinToSend = 0;
  
  if (test6) {
    if (test == 255 ) test = 25;
    ledStatus[pinToSend] = (test--) * 40;
  } 
  dig[digCt++] = '0' + PJON_ME;
  dig[digCt++] = '1' + currentSource;
  
  for(byte x = 0; x <ledStatusCt; x++)
  {
    Serial.print(ledStatus[x]);
    Serial.print(" ");
    if (isLedStatusOn(x)) {
      dig[digCt++] = '1' + x;
      pinToSend = x;
      break;
    }
  }
  dig[digCt]='\0';
Serial.print("dig:  ");
  
  //dig[digCt++] = '1' + pinToSend;
  //dig[digCt++] = 'A' + ledStatus[pinToSend++]/40;
  
 //   if (currentSource < ledStatusCt && isLedStatusOn(currentSource)) {
 //     dig[digCt++] = '1' + currentSource;
 //   } else {
 //     dig[digCt++] = 'A' + currentSource;
 //   }
  Serial.print(dig);
  Serial.print(" ");
  Serial.print(ledStatus[pinToSend-1]/40);
  Serial.print(" ");
  Serial.print(ledStatus[pinToSend-1]);
  Serial.println();

  pjonNetwork.send(PJON_1, dig, digCt);
  //pjonNetwork.send(1,"23", 2);
}

bool isLedStatusOn(int ledPos){ 
  return ( ledPos > -1 && ledPos < ledStatusCt && ledStatus[ledPos] > 200 && ledStatus[ledPos] < ledStatusLtOnTrip);
}
