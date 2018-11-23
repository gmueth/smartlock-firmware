#include <Arduino.h>
#include <Servo.h>

const int wifiInput = 7;
const int wifiOutput = 6;
const int ledPin = 2;
const int servoPin = 9;
const int buttonPin = 3;
volatile bool locked;
volatile bool waitForPost;
volatile bool buttonFlag = false;
Servo servo;

void buttonISR();
void unlock();
void lock();

void setup() {
  pinMode(wifiInput, INPUT);
  pinMode(wifiOutput, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
  servo.attach(servoPin);
}

void loop() {

  if(buttonFlag) {
    waitForPost = true;
    buttonFlag = false;
    if(locked) unlock();
    else lock();
  }

  if(waitForPost) {

    delay(5000);
    waitForPost = false;
  }

  if(digitalRead(wifiInput) == HIGH && !waitForPost) {
    unlock();
  } 
  else if(digitalRead(wifiInput) == LOW && !waitForPost) {
    lock();
  }

  delay(100);
}

void buttonISR() {
  if(waitForPost) return;
  buttonFlag = true;
}

void unlock() {
    servo.write(180);
    digitalWrite(wifiOutput, HIGH);
    digitalWrite(ledPin, HIGH);
    locked = false;
}

void lock() {
    servo.write(0);
    digitalWrite(wifiOutput, LOW);
    digitalWrite(ledPin, LOW);
    locked = true;
}