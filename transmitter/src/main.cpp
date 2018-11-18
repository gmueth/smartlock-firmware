#include <Arduino.h>
#include <ELECHOUSE_CC1101.h>

const byte ledPin = 13;
const byte interruptPin = 3;
volatile byte state = LOW;
volatile int i = 0;
volatile int flag = 0;

void buttonISR();
void transmit();

void setup() {


  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonISR, FALLING);

  Serial.begin(9600);
  ELECHOUSE_cc1101.Init(F_433);

}

void loop() {

  Serial.println(flag);
  if(flag == 1) {
    Serial.println("Transmitting...");
    transmit();
    flag = 0;
  }
  delay(500);

  digitalWrite(ledPin, state);
}

void buttonISR() {
  state = !state;
  flag = 1;
  // Must remove Serial.print too slow for interrupt
  Serial.println("Interupt Triggered.");
  return;
}

void transmit() {
  i++;
  String tx_message = "testing: " + String(i);
  int m_length = tx_message.length();
  byte txbyte[m_length];
  tx_message.getBytes(txbyte, m_length + 1);
  Serial.println((char *)txbyte);
  ELECHOUSE_cc1101.SendData(txbyte, m_length);
  return;
}