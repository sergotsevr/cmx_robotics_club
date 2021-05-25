#include <SoftwareSerial.h>// либа для блютуза
#include "AFMotor.h" // ллиба для машины
#include <math.h>
#include <MemoryFree.h> 

AF_DCMotor motor1(1); //
AF_DCMotor motor2(2); //

// верно для мотор шилда, для самой ардуино + 1 на номера п0ртов
// RX - цифровой вывод 9 (необходимо соединить с выводом TX другого устройства)
// TX - цифровой вывод 10 (необходимо соединить с выводом RX другого устройства)
SoftwareSerial bluetoothSerial(9, 10);

char bluetoothData[7] = {}; // полностью считанная стока.
double routePart = 0;
int speedMax = 125;
double speedLeftMotor = 0;
int speedRightMotor = 0;

int address = 10; // адрес первой ячейки для записи

void setup() {
  Serial.begin(9600); // скорость отправки на комп сообщений (для отладки)
  bluetoothSerial.begin(9600); // скорость считывания данных с блютуза
  for (int i=0; i<361; i++){
    testLeftSin(i);
    testRightSin(i);
  }
  bluetoothData[0]='0';
  bluetoothData[1]='0';
  bluetoothData[2]='0';
  bluetoothData[3]='0';
  bluetoothData[4]='0';
  bluetoothData[5]='0';
  routePart = charToInt(bluetoothData[0]) * 100 + charToInt(bluetoothData[1]) * 10 + charToInt(bluetoothData[2]);
  calcLeftSpeed();
  calcRightSpeed();
  Serial.print("speedLeftMotor  ");
  Serial.println(speedLeftMotor);
  Serial.print("speedRightMotor  ");
  Serial.println(speedRightMotor);
  putin();
  Serial.println("started");
}

void loop() {
  boolean stopper = true;
  Serial.print(F("Free RAM = ")); 
  Serial.println(freeMemory(), DEC);  // print how much RAM is available.
  readIntegerFromBluetooth();
  if(charToInt(bluetoothData[3]) == 0 &&  charToInt(bluetoothData[4])  == 0 && charToInt(bluetoothData[5]) == 0 ){
    stopper = true;
  }
  else{
    stopper = false;
  }
  if (!stopper){
   Serial.println(bluetoothData);
   routePart = charToInt(bluetoothData[0]) * 100 + charToInt(bluetoothData[1]) * 10 + charToInt(bluetoothData[2]);
   calcLeftSpeed();
   calcRightSpeed();
   Serial.print("speedLeftMotor  ");
   Serial.println(speedLeftMotor);
   Serial.print("speedRightMotor  ");
   Serial.println(speedRightMotor);
   route();
  }
  delay(500);
}

// Управление -------------------------------------------------------------------------------------------
void testLeftSin(int s){
double  param = s;
double isBack = 1;
if(s>180){
  isBack=-1;
}
  double result = (sin((param+90)*PI/180)+1)*isBack;
  Serial.print (s);
  Serial.print ("     ");
   Serial.println (result);
}

void testRightSin(int s){
double  param = s;
double isBack = 1;
if(s>180){
  isBack=-1;
}
  double result = (sin((param-90)*PI/180)+1)*isBack;
  Serial.print (s);
  Serial.print ("     ");
   Serial.println (result);
}


void calcLeftSpeed() {
  Serial.print("routePart  ");
  Serial.println((int)routePart);
  double isBack = 1;
  if(routePart>180){
    isBack=-1;
  }
  double coef = (sin ((routePart-90)*PI/180)+1)*isBack;
  Serial.print("left coef  ");
  Serial.println(coef);
  speedLeftMotor = coef*speedMax;
}

void calcRightSpeed() {
  Serial.print("routePart  ");
  Serial.println((int)routePart);
  double isBack = 1;
  if(routePart>180){
    isBack=-1;
  }
  double coef = (sin ((routePart-90)*PI/180)+1)*isBack;
  Serial.print("right coef  ");
  Serial.println(coef);
  speedRightMotor = coef*speedMax;
}

void route(){
  motor1.setSpeed(speedLeftMotor);
  motor2.setSpeed(speedRightMotor);
}

void readIntegerFromBluetooth2(){
  if (bluetoothSerial.available()) {
    for(int i =0; i<7; i++){
       bluetoothData[i] = bluetoothSerial.read();
    }
  }
}

void readIntegerFromBluetooth() {
  if (bluetoothSerial.available()) {
    int i = 0;
    int p = 0;
    
    int bluetoothDataElement = bluetoothSerial.read();

    while ((bluetoothDataElement != '#') && (p != 100)) {
      if (bluetoothSerial.available()) {
        bluetoothData[i] = bluetoothDataElement;
        ++i;
        bluetoothDataElement = bluetoothSerial.read();
      } else {
        // защита от вечного цикла
        ++p;
      }  
    }
   // Serial.println();
  } else {
   putin(); 
  }
  
}

void putin() {
  for (int j = 0; j < 7; j++ ) {
    bluetoothData[j] = '0';
  }
}

// Утилиты -----------------------------------------------------------
int charToInt(char ch) {
  return ch - '0';
}
