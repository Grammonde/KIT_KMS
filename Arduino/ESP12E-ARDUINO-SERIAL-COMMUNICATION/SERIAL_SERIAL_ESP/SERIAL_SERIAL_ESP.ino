#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

char inChar;
const char* ssid = "KIT Campus"; // Set the wifi name
const char* password = NULL;  // Wifi password

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.println("Connecting..");
 
  }
  Serial.println("Connected to WiFi Network");
}
 
void loop() { // run over and over
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    inChar = Serial.read();
    
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://10.10.15.70:3000/api/get/?token=24EFhweWOOZ844AycXd7hH4mhLjZQSU0poVznIVXNvW&val=inChar");
    
//    int httpCode = http.GET(); //Send the request
// 
//    if (httpCode > 0) { //Check the returning code
//      
//      String payload = http.getString();   //Get the request response payload
//      Serial.println(payload); //Print the response payload
// 
//    }
//    else Serial.println(httpCode);
 
    http.end();   //Close connection
  }
}
