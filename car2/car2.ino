#include <SoftwareSerial.h>// либа для блютуза
#include "AFMotor.h" // ллиба для машины

AF_DCMotor motor1(1); //
AF_DCMotor motor2(2); //

// верно для мотор шилда, для самой ардуино + 1 на номера п0ртов
// RX - цифровой вывод 9 (необходимо соединить с выводом TX другого устройства)
// TX - цифровой вывод 10 (необходимо соединить с выводом RX другого устройства)
SoftwareSerial bluetoothSerial(9, 10);

char bluetoothData[7] = {}; // полностью считанная стока.
char shitProtectionBuffer[7] = {}; // Тут будут храниться предыдущие валидные данных, в случае битой цифры будет использоваться соотвествующая отсюда.
int routePart = 0;
int speedPart = 0;
int speedDefoult = 0;
int speedLeftMotor = 0;
int speedRightMotor = 0;

void setup() {
  Serial.begin(9600); // скорость отправки на комп сообщений (для отладки)
  bluetoothSerial.begin(9600); // скорость считывания данных с блютуза
  Serial.println("started");
  putin();
}

void loop() {
  readIntegerFromBluetooth();
//  shitProtection();

//  routePart = charToInt(bluetoothData[0]) * 100 + charToInt(bluetoothData[1]) * 10 + charToInt(bluetoothData[2]);
//  speedPart = charToInt(bluetoothData[3]) * 1000 + charToInt(bluetoothData[4]) * 100 + charToInt(bluetoothData[5]) * 10 + charToInt(bluetoothData[6]);

//  route();
  //  Serial.println(bluetoothData);
  delay(5);
  //  Serial.println("end");
}

// Управление -------------------------------------------------------------------------------------------

void route() {
  calcSpeed();
  choiceCirclePart();
}

void calcSpeed() {
  speedDefoult = (int)( ((float)speedPart / (float)1000) * (float)255 );
  speedLeftMotor = speedDefoult;
  speedRightMotor = speedDefoult;
}

void choiceCirclePart() {
  if (routePart <= 90) {
    circlePartZero();
  } else {
    if (routePart <= 180) {
      circlePartOne();
    } else {
      if (routePart <= 270) {
        circlePartTwo();
      } else {
        if (routePart <= 360) {
          circlePartThree();
        }
      }
    }
  }
  
  Serial.print("скорость моотора 1 ");
  vivodIntVsosnole(speedLeftMotor);
  Serial.println("");
  motor1.setSpeed(speedLeftMotor);
  
  Serial.print("скорость моотора 2 ");
  vivodIntVsosnole(speedRightMotor);
  Serial.println("");
  motor2.setSpeed(speedRightMotor);
}

void circlePartZero() {
  motor1.run(FORWARD);
  Serial.println("мотор 1 FORWARD");
  motor2.run(FORWARD);
  Serial.println("мотор 2 FORWARD");


  speedRightMotor = (int)( ((float)routePart / (float)90) * (float)speedRightMotor );
}

void circlePartOne() {
  motor1.run(FORWARD);
  Serial.println("мотор 1 FORWARD");
  motor2.run(FORWARD);
  Serial.println("мотор 2 FORWARD");

  speedLeftMotor = (int)( ((float)(180 - routePart) / (float)90) * (float)speedRightMotor );
}

void circlePartTwo() {
  motor1.run(BACKWARD);
  Serial.println("мотор 1 BACKWARD");
  motor2.run(BACKWARD);
  Serial.println("мотор 1 BACKWARD");

  speedLeftMotor = (int)( ((float)(routePart - 180) / (float)90) * (float)speedRightMotor );
}

void circlePartThree() {
  motor1.run(BACKWARD);
  Serial.println("мотор 1 BACKWARD");
  motor2.run(BACKWARD);
  Serial.println("мотор 1 BACKWARD");

  speedRightMotor = (int)( ((float)(360 - routePart) / (float)90) * (float)speedRightMotor );
}

// Все что связано со считыванием -----------------------------------------------------------------------
// метод для считывания 7-ми значного числа с блютуза
// если блюттуз отвалилися - 0 00 0000 - машина должна остановиться
//void readIntegerFromBluetooth() {
//  for (int i = 0; i < 7; i++ ) {
//    if (bluetoothSerial.available()) {
//      bluetoothData[i] = bluetoothSerial.read();
//    } else {
//      // если что то не получилось вычитать - надо остановить машину
//      putin();
//    }
//  }
//}

void readIntegerFromBluetooth() {
  if (bluetoothSerial.available()) {
    int i = 0;
    int p = 0;
    
    int bluetoothDataElement = bluetoothSerial.read();

    while ((bluetoothDataElement != '#') && (p != 100)) {
      if (bluetoothSerial.available()) {
        bluetoothData[i] = bluetoothDataElement;
        Serial.print(bluetoothData[i]);
        
        ++i;
        bluetoothDataElement = bluetoothSerial.read();
      } else {
        // защита от вечного цикла
        ++p;
      }  
    }
    Serial.println();
  } else {
    putin();      
  }
}

// ну типо путин - обнуление))0
void putin() {
  for (int j = 0; j < 7; j++ ) {
    bluetoothData[j] = '0';
    shitProtectionBuffer[j] = '0';
  }
}

// Защищает от не корректных данных из блютуза (появляются когда теребишьь пальчиком)
void shitProtection() {
  shitProtectionForZero();
  shitProtectionForOne();

  shitProtectionForDigital(2);

  shitProtectionForThree();

  shitProtectionForDigital(4);
  shitProtectionForDigital(5);
  shitProtectionForDigital(6);
}

// Защита от не корректных данных из блютуза для 0 элемента
void shitProtectionForZero() {
  switch (bluetoothData[0])
  {
    case '0':
    case '1':
    case '2':
    case '3':
      break;

    default:
      bluetoothData[0] = shitProtectionBuffer[0];
      break;
  }
}

// Защита от не корректных данных из блютуза для 1 элемента
void shitProtectionForOne() {
  switch (bluetoothData[1])
  {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
      break;

    case '6':
    case '7':
    case '8':
    case '9':
      if (bluetoothData[0] == '3') {
        bluetoothData[1] = shitProtectionBuffer[1];
      }

      break;

    default:
      bluetoothData[1] = shitProtectionBuffer[1];
      break;
  }
}

// Защита от не корректных данных из блютуза для элемениа который должен быть просто цифровым (0 - 9)
void shitProtectionForDigital(int i) {
  switch (bluetoothData[i])
  {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;

    default:
      bluetoothData[i] = shitProtectionBuffer[i];
      break;
  }
}

// Защита от не корректных данных из блютуза для 3 элемента
void shitProtectionForThree() {
  switch (bluetoothData[3])
  {
    case '0':
    case '1':
      break;

    default:
      bluetoothData[3] = shitProtectionBuffer[3];
      break;
  }
}

void copyBluetoothDataToshitProtectionBuffer () {
  for (int j = 0; j < 7; j++ ) {
    shitProtectionBuffer[j] = bluetoothData[j];
  }
}


// Утилиты -----------------------------------------------------------
int charToInt(char ch) {
  return ch - '0';
}

// выводит в консоль числа мать их
void vivodIntVsosnole (int chisloy) {

  int ciferka = chisloy % 10;
  chisloy = chisloy / 10;

  if (chisloy > 0) {
    vivodIntVsosnole(chisloy);
  }

  char ch  = ciferka + '0';
  Serial.print(ch);
}
