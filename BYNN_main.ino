/*
   BYNN 吹得你不要不要的(Blow You No No)
   This is BYNN main function
   現場：
   需調顏色上下限！
   需條轉彎角度！
   需條前進速度！
*/
#include <TimerOne.h>
#include <Servo.h>
///////////////////////////////////////////////////////////////
////////// Pins /////////////////
///////////////////////////////////////////
const int sensorLL = 6;     //新的，一變黑就會有值
const int sensorL = 5;      //左感測器輸入腳，舊的要對到膠帶正中間才有
const int sensorM = 4;   //中感測器輸入腳
const int sensorR = 3;     //右感測器輸入腳
const int sensorRR = 14;     //新的，一變黑就會有值
const int BrushlessFPin = 9;
const int BrushlessBPin = 10;
const int myServoPin = 11;
const int S0 = 7;
const int S1 = 8;
const int S2 = 12;
const int S3 = 13;
const int OUT = 2;

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
int middleDegree = 90;
//**********color********************
int   g_count = 0;    // 頻率計算
int   g_array[3];     // 儲存 RGB 值
int   g_flag = 0;     // RGB 過濾順序
float g_SF[3];        // 儲存白平衡計算後之 RGB 補償係數
boolean colorState = 0;
int colorGapDown = 110;
int colorGapUp = 125;
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
  //**********color********************
  // TCS3200 初始化與輸出頻率設定
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  digitalWrite(S0, LOW);  // OUTPUT FREQUENCY SCALING 2%
  digitalWrite(S1, HIGH);
  Timer1.initialize();             // defaulte is 1s
  Timer1.attachInterrupt(TSC_Callback);
  attachInterrupt(0, TSC_Count, RISING);
  delay(2000);
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
void receiveColor() {
  g_flag = 0;
  Serial.println(g_array[0]);
  if ((g_array[0] > colorGapDown) && (g_array[0] < colorGapUp))
    colorState = 1;
  delay(300);
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


///////////////////////////////////////////////////////////////
//////////color////////////////
///////////////////////////////////////////
// 選擇過濾顏色
void TSC_FilterColor(int Level01, int Level02)
{
  if (Level01 != 0)
    Level01 = HIGH;

  if (Level02 != 0)
    Level02 = HIGH;

  digitalWrite(S2, Level01);
  digitalWrite(S3, Level02);
}

void TSC_Count()
{
  g_count ++ ;
}

void TSC_Callback()
{
  switch (g_flag)
  {
    case 0:
      Serial.println("->WB Start");
      TSC_WB(HIGH, HIGH);            // Green
      break;
    case 1:
      g_array[0] = g_count;
      break;
    default:
      g_count = 0;
      break;
  }
}

// 白平衡
void TSC_WB(int Level0, int Level1) {
  g_count = 0;
  g_flag ++;
  TSC_FilterColor(Level0, Level1);
  Timer1.setPeriod(50000);      // us; 每秒觸發
}
