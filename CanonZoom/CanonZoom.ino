/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
byte pos = 33;    // variable to store the servo position 
unsigned long stopZoom=0;

#define INPUT_STRING_SIZE 3
char inputString[INPUT_STRING_SIZE+1];         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
byte inputStringPos = 0;

void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
    Serial.begin(115200);
    Serial.println("Servo");
    myservo.write(pos);
} 
 
void loop() 
{ 
  /*for(pos = 90; pos <= 120; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 120; pos>=90; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  */
  
  processSerialEvent();
  if(stopZoom < millis())  {
    Serial.println("zoomStop");
    adjustServo(pos);
    stopZoom=-1;
  }
} 



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
byte lastPayload=pos;
void processSerialReceive(){
  byte payload = inputString[0];
  byte input=0;
  if(inputStringPos==3 && payload == 'c') {
    payload = (inputString[1]  - '0') * 10 + (inputString[2] - '0');
    pos=payload;
    Serial.print(payload);
    Serial.println(" set center pos");
  } else if(inputStringPos>1) {
    payload = (payload  - '0') * 10 + (inputString[1] - '0');
  }
  
  adjustServo(payload);
  if(inputStringPos>2) {
    stopZoom = millis() + (inputString[2] - '0')*1000;
  }
  else
  stopZoom = millis() + 10000;
  
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(stopZoom);
   Serial.print(" ");
    Serial.print(millis());
  Serial.print(" ");
  Serial.println(stopZoom-millis());
}

void adjustServo(byte payload){
  byte payload2 = payload; 
  if (payload<180) {
    if ((lastPayload > payload2 || payload < pos ) && payload>10)
      payload -= 5;
    else if (lastPayload < payload2 || payload2>pos) payload += 5;
    
    myservo.write(payload);
    delay(100);
    myservo.write(payload2);
    Serial.print( payload);
    Serial.print( " ");
    Serial.print( payload2);
    Serial.println(" moved");
    
    lastPayload = payload2;
  }
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
      Serial.println(" end ");
    } else {
      // add it to the inputString:
      if(inputStringPos < INPUT_STRING_SIZE) inputString[inputStringPos++] = inChar;
    }
  }
}

