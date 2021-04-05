#include <SoftwareSerial.h>// import the serial library
#include "AFMotor.h"
AF_DCMotor motor1(1);  
AF_DCMotor motor2(2);  


SoftwareSerial mySerial(9, 10); // RX, TX

int ledpin = 13; // led on D13 will show blink on / off

int BluetoothData; // the data given from Computer

void setup() {

  Serial.begin(4800);

  Serial.println("Type AT commands!"); // put your setup code here, to run once:

  mySerial.begin(9600);

  Serial.println("Bluetooth On please press 1 or 0 blink LED ..");

  pinMode(ledpin, OUTPUT);
  motor1.run(FORWARD);  
  motor2.run(FORWARD);  
}

void loop() {

  // put your main code here, to run repeatedly:

  if (mySerial.available()) {

    BluetoothData = mySerial.read();

    if (BluetoothData == '1') { // if number 1 pressed ….
 
      motor1.setSpeed(255);  
      motor2.setSpeed(255);
      digitalWrite(ledpin,1);

    }

    if (BluetoothData == '0') { // if number 0 pressed ….

      motor1.setSpeed(0);  
      motor2.setSpeed(0);
      
      digitalWrite(ledpin,1);
      digitalWrite(ledpin,0);
    }

  }

 
  delay(100);// prepare for next data …

}
