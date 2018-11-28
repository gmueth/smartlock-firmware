#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Mhelith";
const char* password =  "smartlock";
// const char* ssid = "Blips N Chitz";
// const char* password =  "BEER_is_g00d&stuff";
const int lockOutput = 12;
const int lockInput = 13;
const int buttonPin = 25;
const int ledPin = 27;
const int ledConnecting = 26;
const String token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJfaWQiOiI1YmU5YzM4NjRmZTlhYzMzYTNlMmY2ODQiLCJpYXQiOjE1NDMxNjkwNzl9.AtSLbRcKOZBVicBptGuxKwxQFTdGAqGNrCbmuoN_TxU";
volatile bool serverStatus = false;
volatile bool lockStatus = false;
volatile bool sendPost = false;
volatile bool waitForPost = false;
volatile bool lastGet;
volatile bool webChange = false;

void sendPOST(String input);
char sendGET();

void buttonISR() {
  if(sendPost) return;
  sendPost = true;
  if(serverStatus) digitalWrite(lockOutput, LOW);
  else digitalWrite(lockOutput, HIGH);
  waitForPost = true;
}

void setup() {

  pinMode(lockOutput, OUTPUT);
  pinMode(lockInput, INPUT);
  digitalWrite(ledPin, LOW);
  pinMode(ledPin, OUTPUT);
  pinMode(ledConnecting, OUTPUT);
 
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting...");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledConnecting, HIGH);
    Serial.println("Connecting to WiFi..");
    WiFi.begin(ssid, password);
    delay(10000);
    digitalWrite(ledConnecting, LOW);
  }
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);

  Serial.println("Connected to the WiFi network");
  sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": false}");
}

void loop() {

  if(digitalRead(lockInput) && !serverStatus && !sendPost && !webChange) {
    sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": true}");
    delay(5000);
    } 
  else if (!digitalRead(lockInput) && serverStatus && !sendPost && !webChange) {
    sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": false}");
    delay(5000);
  }

  if(WL_CONNECTED) digitalWrite(ledPin, HIGH);
  else digitalWrite(ledPin, LOW);

  if(sendPost == true && serverStatus == true) {
    sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": false}");
    sendPost = false;
  } 
  else if (sendPost == true && serverStatus == false) {
    sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": true}");
    sendPost = false;
  }
  delay(1000);

  webChange = false;


  noInterrupts();
  if(sendPost) {
    interrupts();
    return;
  }

  lastGet = serverStatus;
  char status = sendGET();
  Serial.println(status);
  if(status == 't' && !sendPost) { 
    digitalWrite(lockOutput, HIGH);
    serverStatus = true;
    webChange = true;
  } 
  else if (!sendPost) { 
    digitalWrite(lockOutput, LOW);
    serverStatus = false;
    webChange = true;
  }
  Serial.println(serverStatus);
  interrupts();

}

void sendPOST(String input) {

 if(WiFi.status()== WL_CONNECTED){
 
   HTTPClient http;   
 
   http.begin("https://sdsmartlock.com/api/events"); 
   http.addHeader("Content-Type", "application/json");
   http.addHeader("x-auth-token", token); 
 
   int httpResponseCode = http.POST(input);
 
   if(httpResponseCode>0){
 
    String response = http.getString();
 
    Serial.println(httpResponseCode);
    Serial.println(response);
 
   } else {
 
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();
 
 } else {
 
    Serial.println("Error in WiFi connection");   
 }
}

char sendGET() {

  String payload;

  if ((WiFi.status() == WL_CONNECTED)) {

  HTTPClient http;

  http.begin("https://sdsmartlock.com/api/locks/5bd603af9dfa7d068ceb70dd");
  int httpCode = http.GET();

  if (httpCode > 0) {

      payload = http.getString();

      // Serial.println(httpCode);
      // Serial.println(payload);
    }

  else {
    Serial.println("Error on HTTP request");
  }
  http.end();
  return payload[43];
  }
  return payload[43];
 }