#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "smartlock";
const char* password =  "smartlock";

void sendPOST(String input);
void sendGET();

void setup() {
 
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
  sendPOST("{\"status\": true}");
  // sendGET();

  delay(10000);
}

void sendPOST(String input) {

 if(WiFi.status()== WL_CONNECTED){
 
   HTTPClient http;   
 
   http.begin("https://sdsmartlock.com/api/locks"); 
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

void sendGET() {

    if ((WiFi.status() == WL_CONNECTED)) {
 
    HTTPClient http;
 
    http.begin("https://sdsmartlock.com/api/locks");
    int httpCode = http.GET();
 
    if (httpCode > 0) {
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
    return;
  }
  return;
 }