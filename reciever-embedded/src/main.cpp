#include <ELECHOUSE_CC1101.h>
byte buffer[100] = {0};
const int ledPin = 6;
int flag = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Rx");
  ELECHOUSE_cc1101.Init(F_433);
  ELECHOUSE_cc1101.SetReceive();

  pinMode(ledPin, OUTPUT);
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
      flag = 1;
    } else {
      digitalWrite(ledPin, LOW);
      flag = 0;
    }
    
  }
}