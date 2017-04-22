/*
   BYNN 吹得你不要不要的(Blow You No No)
   This is BYNN main function
*/
#include <Servo.h>  
///////////////////////////////////////////////////////////////
////////// Pins /////////////////
///////////////////////////////////////////
const int sensorLL = 6;     //新的，一變黑就會有值
const int sensorL = 5;      //左感測器輸入腳，舊的要對到膠帶正中間才有
const int sensorM = 4;   //中感測器輸入腳
const int sensorR = 3;     //右感測器輸入腳
const int sensorRR = 2;     //新的，一變黑就會有值
const int BrushlessFPin = 9;
const int BrushlessBPin = 10;
const int myServoPin = 11;


///////////////////////////////////////////////////////////////
////////// variable/////////////////
///////////////////////////////////////////
//**********Motor********************
Servo BrushlessF;
Servo BrushlessB;
Servo myServo;
//**********State********************
boolean missionState = 0;
//**********Speed********************
int FnormalSpeed = 30;
int FlowSpeed = 21;
int FfastSpeed = 60;
int BnormalSpeed = 60;
int BfastSpeed = 100;
int BlowSpeed = 30;
int verybigDegree = 20;
int bigDegree = 15;
int smallDegree = 10;
int verysmallDegree = 5;
int middleDegree=90;

///////////////////////////////////////////////////////////////
////////// Set up/////////////////
///////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  //**********receiveIR********************
  pinMode(sensorLL, INPUT); //定義最左感測器
  pinMode(sensorL, INPUT); //定義左感測器
  pinMode(sensorM, INPUT);//定義中感測器
  pinMode(sensorR, INPUT); //定義右感測器
  pinMode(sensorRR, INPUT); //定義最右感測器
  //**********Motor********************
  BrushlessF.attach(BrushlessFPin);
  BrushlessB.attach(BrushlessBPin);
  myServo.attach(myServoPin);
  directionControl(0);
}


///////////////////////////////////////////////////////////////
////////// Main function////////////////
///////////////////////////////////////////
void loop() {
  //initialize() //use another program to initialize
  receiveIR();
  receiveColor();
  sortingAndAction();
  checkMission();
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
String IRState = "";
//**********Function********************
void receiveIR() { //0代表黑色 1代表淺色
  stateLL = digitalRead(sensorLL);
  stateL = digitalRead(sensorL);
  stateM = digitalRead(sensorM);
  stateR = digitalRead(sensorR);
  stateRR = digitalRead(sensorRR);
  IRState = String(stateLL) + String(stateL) + String(stateM) + String(stateR) + String(stateRR);
}


///////////////////////////////////////////////////////////////
//////////receiveColor////////////////
///////////////////////////////////////////
//**********Variable********************
boolean colorState = 0;
//**********Function********************
void receiveColor() {

}


///////////////////////////////////////////////////////////////
//////////sortingAndAction////////////////
///////////////////////////////////////////
void sortingAndAction() {
  if (colorState == 0) {
    switch (IRState.toInt()) {
      case 00000:
        Serial.println("all black, change to default.");
        directionControl(0);
        speedControl(FnormalSpeed, 10);
        break;
      case 11111:
        Serial.println("all white, go backward");
        directionControl(0);
        speedControl(10, BlowSpeed);
        break;
      case 01111 :
        Serial.println("need very big left turn");
        directionControl(verybigDegree);
        speedControl(FlowSpeed, 10);
        break;
      case 00111 :
        Serial.println("need big left turn");
        directionControl(bigDegree);
        speedControl(FnormalSpeed, 10);
        break;
      case 10111 :
        Serial.println("need small left turn");
        directionControl(smallDegree);
        speedControl(FnormalSpeed, 10);
        break;
      case 10011 :
        Serial.println("need very small left turn");
        directionControl(verysmallDegree);
        speedControl(FfastSpeed, 10);
        break;
      case 11011 :
        Serial.println("straight forward! ");
        directionControl(0);
        speedControl(FfastSpeed, 10);
        break;
      case 11001 :
        Serial.println("need very small right turn");
        directionControl(-1 * verysmallDegree);
        speedControl(FfastSpeed, 10);
        break;
      case 11101 :
        Serial.println("need small right turn");
        directionControl(-1 * smallDegree);
        speedControl(FnormalSpeed, 10);
        break;
      case 11100 :
        Serial.println("need big right turn");
        directionControl(-1 * bigDegree);
        speedControl(FnormalSpeed, 10);
        break;
      case 11110 :
        Serial.println("need very big right turn");
        directionControl(-1 * verybigDegree);
        speedControl(FlowSpeed, 10);
        break;
      default:
        Serial.println("WTF, not in the sort! change to default.");
        directionControl(0);
        speedControl(FnormalSpeed, 10);
    }
  } else {
    speedControl(10, BnormalSpeed);
    delay(3000);
    speedControl(10, 10);
    missionState = 1 ;
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
void directionControl(int degree) { //左正右負
  myServo.write(middleDegree + degree);
}
