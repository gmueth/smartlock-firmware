#include <ELECHOUSE_CC1101.h>

const int buttonPin1 = 3;    // the pin that the pushbutton is attached to
const int buttonPin2 = 4;
int i = 0;

void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  Serial.begin(9600);
  ELECHOUSE_cc1101.Init(F_433);
}

void loop() {
  
  if (!buttonPin1) {
    i++;
    String tx_message = "testing: " + String(i);
    int m_length = tx_message.length();
    byte txbyte[m_length];
    tx_message.getBytes(txbyte, m_length + 1);
    Serial.println((char *)txbyte);
    ELECHOUSE_cc1101.SendData(txbyte, m_length);
  }
  delay(100);
}