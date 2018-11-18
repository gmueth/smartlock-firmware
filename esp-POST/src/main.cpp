#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "AndroidAPAE8F";
const char* password =  "zhgj5063";
 
void setup() {
 
  Serial.begin(115200);
  delay(2000);   //Delay needed before calling the WiFi.begin
 
 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(5000);
    Serial.println("Connecting to WiFi..");
    WiFi.begin(ssid, password); 
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;   
 
   http.begin("https://sdsmartlock.com/api/locks");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "application/json");             //Specify content-type header
 
   int httpResponseCode = http.POST("\"status\": \"true\"");   //Send the actual POST request
 
   if(httpResponseCode>0){
 
    String response = http.getString();                       //Get the response to the request
 
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
 
   }else{
 
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();  //Free resources
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(10000);  //Send a request every 10 seconds
 
}