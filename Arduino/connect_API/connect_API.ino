#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "vKirirom";
const char* password = NULL;
 
void setup () {
 
Serial.begin(115200); 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
 
delay(1000);
Serial.println("Connecting..");
 
}
  
  Serial.println("Connected");
 
}
 
void loop() {
 
if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status 
 
HTTPClient http;  //Declare an object of class HTTPClient
 
//http.begin("http://10.10.15.70:3000/api/get?token=24EFhweWOOZ844AycXd7hH4mhLjZQSU0poVznIVXNvW&data=hello");  //Specify request destination

http.begin("http://jsonplaceholder.typicode.com/users/1");

int httpCode = http.GET();                     //Send the request
 
if (httpCode > 0) { //Check the returning code
 
  String payload = http.getString();   //Get the request response payload
  Serial.println(payload);                     //Print the response payload
  
}
else
  Serial.println(httpCode);
 
http.end();   //Close connection
 
}
 
delay(30000);    //Send a request every 30 seconds
 
}
