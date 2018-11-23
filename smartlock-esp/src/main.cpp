#include <WiFi.h>
#include <HTTPClient.h>

// const char* ssid = "smartlock";
// const char* password =  "smartlock";
const char* ssid = "Blips N Chitz";
const char* password =  "BEER_is_g00d&stuff";
const int lockOutput = 12;
const int lockInput = 13;
volatile bool serverStatus = false;
volatile bool lockStatus = false;
volatile bool sendPost = false;
volatile bool lastGet;

void sendPOST(String input);
char sendGET();

void setup() {

  pinMode(lockOutput, OUTPUT);
  pinMode(lockInput, INPUT);
 
  Serial.begin(115200);
  delay(1000);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(10000);
    Serial.println("Connecting to WiFi..");
    WiFi.begin(ssid, password);
  }

  Serial.println("Connected to the WiFi network");
  sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": false}");

}

void loop() {

  lastGet = serverStatus;
  char status = sendGET();
  Serial.println(status);
  if(status == 't') { 
    digitalWrite(lockOutput, HIGH);
    serverStatus = true;
  } else { 
    digitalWrite(lockOutput, LOW);
    serverStatus = false;
  }

  if(digitalRead(lockInput) == HIGH) {
    lockStatus = true;
  } else {
    lockStatus = false;
  }

  Serial.println(serverStatus);
  Serial.println(lockStatus);

  if(serverStatus != lockStatus && lastGet == serverStatus) {
    sendPost = true;
  }

  if(sendPost == true && lockStatus == false) {
    sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": false}");
    sendPost = false;
  } 
  else if (sendPost == true && lockStatus == true) {
    sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": true}");
    sendPost = false;
  }

  delay(1000);
}

void sendPOST(String input) {

 if(WiFi.status()== WL_CONNECTED){
 
   HTTPClient http;   
 
   http.begin("https://sdsmartlock.com/api/events"); 
   http.addHeader("Content-Type", "application/json"); 
 
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