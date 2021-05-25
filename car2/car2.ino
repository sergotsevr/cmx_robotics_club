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
  
  Serial.println("started");
}

void loop() {
  Serial.print(F("Free RAM = ")); 
  Serial.println(freeMemory(), DEC);  // print how much RAM is available.
  readIntegerFromBluetooth();
  Serial.println(bluetoothData);
  routePart = charToInt(bluetoothData[0]) * 100 + charToInt(bluetoothData[1]) * 10 + charToInt(bluetoothData[2]);
  calcLeftSpeed();
  Serial.print("speedLeftMotor  ");
  Serial.println(speedLeftMotor);
  delay(1000);
  /*
  readIntegerFromBluetooth();

  routePart = charToInt(bluetoothData[0]) * 100 + charToInt(bluetoothData[1]) * 10 + charToInt(bluetoothData[2]);
  speedPart = 200;
  route();*/
}

// Управление -------------------------------------------------------------------------------------------


void calcLeftSpeed() {
  Serial.print("routePart  ");
  Serial.println(routePart);
  double coef = sin (routePart * PI / 180.0) + 1;
  Serial.print("left coef  ");
  Serial.println(coef);
  speedLeftMotor = coef*speedMax;
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
