#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "SectionBsugoi";
const char* password = "SectionB";
String data = "";
 
void setup() {
 
  Serial.begin(115200);                 //Serial connection

  while(!Serial){
  
    ; // wait for serial port to connect. Needed for native USB port only
  
  }
  
  WiFi.begin(ssid, password);   //WiFi connection
  Serial.println("");
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(1000);
    
    Serial.println("Connecting");
 
  }

  verify = Serial.readString();
  
  data = Serial.readString();
  
  Serial.println("Connected"); 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient

   Serial.println(data.length());

   Serial.println(data);
 
   http.begin("http://10.10.13.77:3000/data/post");      //Specify request destination
   
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
   
   int httpCode = http.POST(data);   //Send the request
   
   if(httpCode > 0){
   
    String payload = http.getString();                  //Get the response payload

    Serial.println("Success!!!"); 
   
    Serial.println(payload);    //Print request response payload
   
   }
   
   else{
    
    Serial.println(httpCode);   //Print HTTP return code
   }
   
    http.end();   //Close connection
  
  }
 
  delay(10000);  //Send a request every 30 seconds
}
 
