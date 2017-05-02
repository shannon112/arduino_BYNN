/*
   >>>>This is BYNN main function<<<<
   機械工程實務_氣動車專題_全自動控制系統（循跡/顏色辨識）
   BYNN 吹得你不要不要的(Blow You No No)
   現場調整：
   需調顏色上下限！
   需調轉彎角度！
   需調前進速度！
*/
#include <Servo.h>

///////////////////////////////////////////////////////////////
////////// Pins /////////////////
///////////////////////////////////////////
//const int sensorLL =2;     //interrupt 0
//const int sensorRR = 3;     //interrupt 1
const int systemOpenPin = 4; //自動控制系統開關
const int BrushlessFPin = 6;
const int BrushlessBPin = 7;
const int myServoPin = 5;
/*
  const int TRIGPIN =5;
  const int ECHOPIN =7;
*/
int S0 = 8;//pinB
int S1 = 9;//pinA
int S2 = 12;//pinE
int S3 = 11;//pinF
int taosOutPin = 10;//pinC
int LED = 13;//pinD


///////////////////////////////////////////////////////////////
////////// variable/////////////////
///////////////////////////////////////////
//**********Motor********************
Servo BrushlessF;
Servo BrushlessB;
Servo myServo;
//**********State********************
boolean missionState = 0;
int IntIRState = 0;
String IRState = "";
int roundNumber = 0;
int systemOpenState = 0;
long heightFixd = 10;//當高度小於這個值，則任務完成
//**********Speed********************
int FnormalSpeed = 55;
int FlowSpeed = 50;
int FfastSpeed = 60;
int FbrustSpeed = 100;
int BnormalSpeed = 100;
int BfastSpeed = 150;
int BlowSpeed = 60;
int verybigDegree = 40;
int bigDegree = 15;
int smallDegree = 10;
int verysmallDegree = 10;
int middleDegree = 82;
//**********color********************
boolean colorState = 0;
int colorGapDown = 10;
int colorGapUp = 47;


///////////////////////////////////////////////////////////////
////////// Set up/////////////////
///////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  //**********receiveIR********************
  attachInterrupt(0, sensorLLGetBlack, LOW);
  attachInterrupt(1, sensorRRGetBlack, LOW);
  //**********Motor********************
  BrushlessF.attach(BrushlessFPin);
  BrushlessB.attach(BrushlessBPin);
  BrushlessF.write(0);
  BrushlessB.write(0);
  myServo.attach(myServoPin);
  directionControl(0);
  //**********color********************
  // TCS3200 初始化
  //initialize pins
  pinMode(LED, OUTPUT); //LED pinD
  //color mode selection
  pinMode(S2, OUTPUT); //S2 pinE
  pinMode(S3, OUTPUT); //S3 pinF
  //color response pin (only actual input from taos)
  pinMode(taosOutPin, INPUT); //taosOutPin pinC
  //communication freq (sensitivity) selection
  pinMode(S0, OUTPUT); //S0 pinB
  pinMode(S1, OUTPUT); //S1 pinA
  //**********system********************
  pinMode(systemOpenPin, INPUT_PULLUP);
  //**********HC-SR04********************
  /*
    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);
  */
}

///////////////////////////////////////////////////////////////
////////// Main function////////////////
///////////////////////////////////////////
void loop() {
  if (systemOpenState == 0) {
    systemOpen();
    goForward();
  } else {
    Serial.println("======================");
    directionControl(0);
    receiveColor();
    checkMission();
    Serial.println("======================");
  }
}


///////////////////////////////////////////////////////////////
//////////systemOpen////////////////
///////////////////////////////////////////
void systemOpen() {
  while (digitalRead(systemOpenPin) == 1) {
    Serial.println("waiting system open");
  }
  Serial.println("system open");
  systemInitialize();
  systemOpenState = 1;
}


///////////////////////////////////////////////////////////////
//////////systemInitialize////////////////
///////////////////////////////////////////
void systemInitialize() {
  Serial.print("Okay, Starting to initailize...\n");
  Serial.print("Setting high speed! and wait 2 sec! ");
  Serial.println("(hearing beep-beep)");
  BrushlessF.write(180);
  BrushlessB.write(180);
  delay(2000);
  Serial.print("Setting back to low speed! and wait 4 sec! ");
  Serial.println("(hearing beep-beep-beep)");
  BrushlessF.write(5);
  BrushlessB.write(5);
  delay(4000);
  Serial.print("MOTOR IS READY! ");
  Serial.println("(hearing regular beep---beep---beep--- )");
}


///////////////////////////////////////////////////////////////
//////////systemInitialize////////////////
///////////////////////////////////////////
void goForward() {
  directionControl(0);
  for (int i = 0; i < 4; i++) {
    speedControl(30 + (i * 25), 10);
    delay(500);
  }
}


///////////////////////////////////////////////////////////////
//////////receiveIR////////////////
///////////////////////////////////////////
void sensorLLGetBlack() {
  Serial.println("need very big left turn");
  directionControl(verybigDegree);
}

void sensorRRGetBlack() {
  Serial.println("need very big right turn");
  directionControl(-1 * verybigDegree);
}


///////////////////////////////////////////////////////////////
//////////receiveColor////////////////
///////////////////////////////////////////
void receiveColor() {
  float green = colorRead(taosOutPin);
  Serial.print("color index: ");
  Serial.println(green);

  if ((green > colorGapDown) && (green < colorGapUp)) {
    colorState = 1;
    missionState = 1;
  }
}


///////////////////////////////////////////////////////////////
//////////checkMission////////////////
///////////////////////////////////////////
void checkMission() {
  if (missionState == 0) {
    Serial.println("executing mission...");
  } else {
    Serial.println("mission complete !");
    directionControl(0);
    delay(3000);
    speedControl(10, 10);
    while (1) {
      Serial.println("waiting for shoutdown...");
      delay(2000);
    }
  }
}


///////////////////////////////////////////////////////////////
//////////speedControl////////////////
///////////////////////////////////////////
void speedControl(int speedFront, int speedBack) {
  BrushlessF.write(speedFront);
  BrushlessB.write(speedBack);
}


///////////////////////////////////////////////////////////////
//////////directionControl////////////////
///////////////////////////////////////////
void directionControl(int degree) { //左負右正
  int ddegree = middleDegree - 1 * degree;
  myServo.write(ddegree);
}


///////////////////////////////////////////////////////////////
//////////color////////////////
///////////////////////////////////////////
/*
  Eecher's TCS3200 program
  adapted from code found at reibot.org

  This section will return the pulseIn reading of the selected color.
  It will turn on the sensor at the start taosMode(1), and it will power off the sensor at the end taosMode(0)
  color codes: 0=white, 1=red, 2=blue, 3=green
  if LEDstate is 0, LED will be off. 1 and the LED will be on.
  taosOutPin is the ouput pin of the TCS3200.
*/

float colorRead(int taosOutPin) {
  //turn on sensor and use highest frequency/sensitivity setting
  taosMode(2);

  //setting for a delay to let the sensor sit for a moment before taking a reading.
  int sensorDelay = 100;

  //set the S2 and S3 pins to select the color to be sensed
  //(color == 3) {//green
  digitalWrite(S3, HIGH); //S3
  digitalWrite(S2, HIGH); //S2

  // create a var where the pulse reading from sensor will go
  float readPulse;

  //  turn LEDs on or off, as directed by the LEDstate var (IKKE Frydenlund)
  digitalWrite(LED, HIGH);

  // wait a bit for LEDs to actually turn on, as directed by sensorDelay var
  delay(sensorDelay);

  // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
  readPulse = pulseIn(taosOutPin, LOW, 80000);

  //if the pulseIn times out, it returns 0 and that throws off numbers. just cap it at 80k if it happens
  if (readPulse < .1) {
    readPulse = 80000;
  }

  //turn off color sensor and LEDs to save power
  taosMode(0);

  // return the pulse value back to whatever called for it...
  return readPulse;
}

// Operation modes area, controlled by hi/lo settings on S0 and S1 pins.
//setting mode to zero will put taos into low power mode. taosMode(0);

void taosMode(int mode) {
  if (mode == 0) {
    //power OFF mode-  LED off and both channels "low"
    digitalWrite(LED, LOW);
    digitalWrite(S0, LOW); //S0
    digitalWrite(S1, LOW); //S1
  } else if (mode == 1) {
    //this will put in 1:1, highest sensitivity
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, HIGH); //S1
  } else if (mode == 2) {
    //this will put in 1:5
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, LOW); //S1
  } else if (mode == 3) {
    //this will put in 1:50
    digitalWrite(S0, LOW); //S0
    digitalWrite(S1, HIGH); //S1
  }
}


///////////////////////////////////////////////////////////////
//////////HC-SR04////////////////
///////////////////////////////////////////
void heightDetect() {
  long height = ping();
  if (height < heightFixd) {
    missionState = 1;
  }
}


long ping() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  return pulseIn(ECHOPIN, HIGH) / 10;
}
