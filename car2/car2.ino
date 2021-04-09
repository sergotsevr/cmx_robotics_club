#include <SoftwareSerial.h>// либа для блютуза

// верно для мотор шилда, для самой ардуино + 1 на номера п0ртов
// RX - цифровой вывод 9 (необходимо соединить с выводом TX другого устройства)
// TX - цифровой вывод 10 (необходимо соединить с выводом RX другого устройства)
SoftwareSerial bluetoothSerial(9, 10);

char bluetoothData[7] = {}; // полностью считанная стока.
char shitProtectionBuffer[7] = {}; // Тут будут храниться предыдущие валидные данных, в случае битой цифры будет использоваться соотвествующая отсюда.

void setup() {
  Serial.begin(9600); // скорость отправки на комп сообщений (для отладки) 
  bluetoothSerial.begin(9600); // скорость считывания данных с блютуза
  Serial.println("started");
}

void loop() {
  readIntegerFromBluetooth();
  Serial.println(bluetoothData);
  delay(100);
  Serial.println("end");
}

// метод для считывания 7-ми значного числа с блютуза
// если блюттуз отвалилися - 0 00 0000 - машина должна остановиться
void readIntegerFromBluetooth() {
  for (int i = 0; i < 7; i++ ) {
    if (bluetoothSerial.available()) {
      bluetoothData[i] = bluetoothSerial.read();
    } else {
      // если что то не получилось вычитать - надо остановить машину
      putin();
    }
  }
}

// ну типо путин - обнуление))0
void putin() {
  for (int j = 0; j < 7; j++ ) {
    bluetoothData[j] = '0';
  }
}

// Защищает от не корректных данных из блютуза (появляются когда теребишьь пальчиком)
void shitProtection() {
}
