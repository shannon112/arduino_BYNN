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
const int sensorLL = 18;     //新的，一變黑就會有值
const int sensorL = 17;      //左感測器輸入腳，舊的要對到膠帶正中間才有
const int sensorM = 16;   //中感測器輸入腳
const int sensorR = 15;     //右感測器輸入腳
const int sensorRR = 14;     //新的，一變黑就會有值
const int BrushlessFPin = 3;
const int BrushlessBPin = 4;
const int myServoPin = 5;
int S0 = 8;//pinB
int S1 = 9;//pinA
int S2 = 12;//pinE
int S3 = 11;//pinF
int taosOutPin = 10;//pinC
int LED = 13;//pinD
const int systemOpenPin = 2; //自動控制系統開關


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
//**********Speed********************
int FnormalSpeed = 65;
int FlowSpeed = 55;
int FfastSpeed = 80;
int FbrustSpeed = 100;
int BnormalSpeed = 100;
int BfastSpeed = 150;
int BlowSpeed = 60;
int verybigDegree = 25;
int bigDegree = 15;
int smallDegree = 10;
int verysmallDegree = 10;
int middleDegree = 82;
//**********color********************
boolean colorState = 0;
int colorGapDown = 2400;
int colorGapUp = 2700;


///////////////////////////////////////////////////////////////
////////// Set up/////////////////
///////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  //**********receiveIR********************
  //pinMode(sensorLL, INPUT); //定義最左感測器
  pinMode(sensorL, INPUT); //定義左感測器
  pinMode(sensorM, INPUT);//定義中感測器
  pinMode(sensorR, INPUT); //定義右感測器
  //pinMode(sensorRR, INPUT); //定義最右感測器//類比輸入
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
}

///////////////////////////////////////////////////////////////
////////// Main function////////////////
///////////////////////////////////////////
void loop() {
  if (systemOpenState == 0) {
    systemOpen();
  } else {
    //Serial.println("======================");
    receiveIR();
    sortingAndAction();
    receiveColor();
    checkMission();
    //Serial.println("======================");
  }
}


///////////////////////////////////////////////////////////////
//////////systemOpen////////////////
///////////////////////////////////////////
void systemOpen() {
  while (digitalRead(systemOpenPin) == 1) {
    //    Serial.println("waiting system open");
  }
  //  Serial.println("system open");
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
//////////receiveIR////////////////
///////////////////////////////////////////
//**********Variable********************
int stateLL = 1;  //最左感測器狀態
int stateL = 1;  //左感測器狀態
int stateM = 1;  //中感測器狀態
int stateR = 1;  //右感測器狀態
int stateRR = 1;  //最右感測器狀態
//**********Function********************
void receiveIR() { //0代表黑色 1代表淺色
  if (analogRead(sensorLL) > 300) {
    stateLL = 1;
  } else {
    stateLL = 0;
    directionControl(verybigDegree);
    speedControl(FlowSpeed, 10);
  };
  //  stateL = digitalRead(sensorLL);
  stateL = digitalRead(sensorL);
  stateM = digitalRead(sensorM);
  stateR = digitalRead(sensorR);
  //  stateL = digitalRead(sensorRR);
  if (analogRead(sensorRR) > 300) {
    stateRR = 1;
  } else {
    stateRR = 0;
    directionControl(-1 * verybigDegree);
    speedControl(FlowSpeed, 10);
  };

  IRState = String(stateLL) + String(stateL) + String(stateM) + String(stateR) + String(stateRR);
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
  }
}

///////////////////////////////////////////////////////////////
//////////sortingAndAction////////////////
///////////////////////////////////////////
void sortingAndAction() {
  Serial.println(IRState);
  if (colorState == 0) {
    IntIRState = IRState.toInt();
    switch (IntIRState) {
      case 11110 :
        //        Serial.println("need very big right turn");
        directionControl(-1 * verybigDegree);
        speedControl(FlowSpeed, 10);
        break;
      case 1111 : //01111
        //        Serial.println("need very big left turn");
        directionControl(verybigDegree);
        speedControl(FlowSpeed, 10);
        break;
      case 111 : //00111
        //        Serial.println("need big left turn");
        directionControl(bigDegree);
        speedControl(FnormalSpeed, 10);
        break;
      case 11100 :
        //        Serial.println("need big right turn");
        directionControl(-1 * bigDegree);
        speedControl(FnormalSpeed, 10);
        break;
      case 10111 :
        //        Serial.println("need small left turn");
        directionControl(smallDegree);
        speedControl(FnormalSpeed, 10);
        break;
      case 11101 :
        //        Serial.println("need small right turn");
        directionControl(-1 * smallDegree);
        speedControl(FnormalSpeed, 10);
        break;
      case 10011 :
        //        Serial.println("need very small left turn");
        directionControl(verysmallDegree);
        speedControl(FfastSpeed, 10);
        break;
      case 11011 :
        Serial.println("straight forward! ");
        directionControl(0);
        speedControl(FfastSpeed, 10);
        break;
      case 11001 :
        //        Serial.println("need very small right turn");
        directionControl(-1 * verysmallDegree);
        speedControl(FfastSpeed, 10);
        break;
      case 0:  //00000
        //        Serial.println("all black, change to default.");
        directionControl(0);
        speedControl(10, 10);
        break;
      case 11111:
        //        Serial.println("all white, go backward");
        speedControl(FnormalSpeed, 10);
        break;
      default:
        //        Serial.println("WTF, not in the sort! change to default.");
        speedControl(FnormalSpeed, 10);
    }
  } else {
    missionState = 1 ;
  }
}


///////////////////////////////////////////////////////////////
//////////checkMission////////////////
///////////////////////////////////////////
void checkMission() {
  if (missionState == 0) {
    //    Serial.println("executing mission...");
  } else {
    Serial.println("mission complete !");
    directionControl(0);
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
  //  Serial.println(ddegree);
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
  taosMode(3);

  //setting for a delay to let the sensor sit for a moment before taking a reading.
  int sensorDelay = 50;

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
/*
  void heightDetect() {
  long height = ping();
  Serial.println(height);
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
*/
