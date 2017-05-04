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
//#include "sortingAndActionNoMove.h"
//#include "sortingAndActionSharp.h"
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
int FverylowSpeed = 50;
int FlowSpeed = 60;
int FnormalSpeed = 65;
int FfastSpeed = 70;
int FbrustSpeed = 80;
int BbrustSpeed = 10;
int verybigDegree = 25;
int bigDegree = 15;
int smallDegree = 12;
int verysmallDegree = 10;
int middleDegree = 82;
int breakcounter=0;
//**********color********************
boolean colorState = 0;
int colorGapDown = 1680;
int colorGapUp = 2030;
int colornumber = 0;


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
  //**********initialize********************
  delay(1000);
  systemInitialize();
}


///////////////////////////////////////////////////////////////
////////// Main function////////////////
///////////////////////////////////////////
void loop() {
  //Serial.println("======================");
  //Serial.println("executing mission...");
  receiveIR();
  sortingAndAction();
  receiveColor();
  //Serial.println("======================");
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
void receiveColor() {
  float green = colorRead(taosOutPin);
  //Serial.print("color index: ");
  //Serial.println(green);
  if ((green > colorGapDown) && (green < colorGapUp)) {
    colornumber++;
    if (colornumber > 5) {
      colorState = 1;
      checkMission();
    }
  }
}


///////////////////////////////////////////////////////////////
//////////checkMission////////////////
///////////////////////////////////////////
void checkMission() {
  //Serial.println("mission complete !");
  //Serial.println("waiting for shoutdown...");
  sortingAndActionNoMove();
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
  //Serial.println(ddegree);
}
