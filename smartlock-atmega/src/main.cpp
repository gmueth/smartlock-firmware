#include <Arduino.h>
#include <ELECHOUSE_CC1101.h>
#include <Servo.h>

byte buffer[100] = {0};
const int wifiInput = 7;
const int wifiOutput = 6;
const int ledPin = 4;
const int servoPin = 5;
const int buttonPin = 3;
volatile bool locked = true;
volatile bool waitForPost = false;
volatile bool buttonFlag = false;
volatile int flag = 0;
Servo servo;

void buttonISR();
void unlock();
void lock();

void setup() {
  Serial.begin(9600);
  Serial.println("Rx");
  ELECHOUSE_cc1101.Init(F_433);
  ELECHOUSE_cc1101.SetReceive();

  pinMode(wifiInput, INPUT);
  pinMode(wifiOutput, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
  servo.attach(servoPin);
}

void loop() {

  if (ELECHOUSE_cc1101.CheckReceiveFlag()) {
    
    ELECHOUSE_cc1101.ReceiveData(buffer);
    String str((char*) buffer);
    Serial.println(str);
    ELECHOUSE_cc1101.SetReceive();

    if (flag == 0) {
      unlock();
      flag = 1;
      waitForPost = true;
    } else {
      lock();
      flag = 0;
      waitForPost = true;
    }
  }

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

  // delay(100);
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