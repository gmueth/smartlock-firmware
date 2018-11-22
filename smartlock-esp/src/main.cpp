#include <WiFi.h>
#include <HTTPClient.h>
// #include <ArduinoJson.h>
 

const char* ssid = "Blips N Chitz";
const char* password =  "BEER_is_g00d&stuff";
int counter = 0;
const int ledPin = 12;

// const char* ssid = "smartlock";
// const char* password =  "smartlock";

void sendPOST(String input);
char sendGET();

void setup() {

  pinMode(ledPin, OUTPUT);
 
  Serial.begin(115200);
  delay(1000);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(10000);
    Serial.println("Connecting to WiFi..");
    WiFi.begin(ssid, password);
  }
 
  Serial.println("Connected to the WiFi network");
 
}

void loop() {


  if(counter%2 == 0) {
    // sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": false}");
  } else {
    // sendPOST("{\"userid\": \"5bd603af9dfa7d068ceb70dd\",\"lockid\": \"5bd603af9dfa7d068ceb70dd\",\"status\": true}");
  }
  counter++;
  char status = sendGET();
  Serial.println(status);
  if(status == 't') digitalWrite(ledPin, HIGH);
  else digitalWrite(ledPin, LOW);
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
      // Serial.println(payload[43];
    }

  else {
    Serial.println("Error on HTTP request");
  }
  http.end();
  return payload[43];
  }
  return payload[43];
 }