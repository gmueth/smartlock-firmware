#include <ELECHOUSE_CC1101.h>
#include <Servo.h>

byte buffer[100] = {0};
const int ledPin = 4;
const int servoPin = 5;
int flag = 0;

Servo servo;

void setup()
{
  Serial.begin(9600);
  Serial.println("Rx");
  ELECHOUSE_cc1101.Init(F_433);
  ELECHOUSE_cc1101.SetReceive();

  pinMode(ledPin, OUTPUT);
  servo.attach(servoPin);
}

void loop()
{
  if (ELECHOUSE_cc1101.CheckReceiveFlag()) {
    
    ELECHOUSE_cc1101.ReceiveData(buffer);
    String str((char*) buffer);
    Serial.println(str);
    ELECHOUSE_cc1101.SetReceive();

    if (flag == 0) {
      digitalWrite(ledPin, HIGH);
      servo.write(180);
      flag = 1;
    } else {
      digitalWrite(ledPin, LOW);
      servo.write(0);
      flag = 0;
    }
  }
  // delay(100);
}