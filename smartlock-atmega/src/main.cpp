#include <Arduino.h>
#include <Servo.h>

const int wifiInput = 7;
const int ledPin = 2;
const int servoPin = 9;

Servo servo;

void unlock();
void lock();

void setup() {
  pinMode(wifiInput, INPUT);
  pinMode(ledPin, OUTPUT);
  servo.attach(servoPin);
}

void loop() {
  if(digitalRead(wifiInput) == HIGH) {
    unlock();
  } else {
    lock();
  }
}

void unlock() {
    digitalWrite(ledPin, HIGH);
    servo.write(180);
}

void lock() {
    digitalWrite(ledPin, LOW);
    servo.write(0);
}