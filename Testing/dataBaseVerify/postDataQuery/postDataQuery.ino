#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  


String token = "";
String data = "";
String verifies = "";
String setProcess;

int startAgain = 0;
int httpCode1;
int httpCode2;
int i = 0;
int j = 0;
int k = 0;
int nextProcess = 0;

boolean process = false;

void setup() {
 
  Serial.begin(115200);                 //Serial connection
  while(!Serial){
    ; // wait for serial port to connect. Needed for native USB port only
  
  }
   WiFiManager wifiManager;
   wifiManager.autoConnect("grammonde", "password123");
  Serial.println("Connected");
  
}
 
void loop() {

 if(startAgain == 0){

  setProcess = Serial.readString();

  Serial.println(setProcess);

  if(setProcess == "incoming"){

    process = true;

    Serial.println("Starting...");

    Serial.println("#");

    startAgain += 1;
    
  }
  
 }
  
 if(process == true){

  if(j == 0){
    
    verifies = Serial.readString();

    j++;
    
    if(verifies == "F3 78 2D 1B"){
      
      Serial.println("*");
  
      i++;
      
    }
  
  }
  
  if( i == 2 ){

    data = Serial.readString();

    nextProcess += 1;

    i++;
    
  }
  
   if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   
     HTTPClient http;    //Declare object of class HTTPClient
  
     Serial.println(verifies.length());
  
     Serial.println(verifies);

     Serial.println(data.length());

     Serial.println(data);
  
     if( i == 1 ){ 
      http.begin("http://10.0.100.150:3000/verify");      //Specify request destination
     
      http.addHeader("Content-Type", "text/plain");  //Specify content-type header
      
      httpCode1 = http.POST(verifies);     //Send the request

      if(httpCode1 > 0){

        token = http.getString();
        
        Serial.println("Verified Completed!");

        Serial.println(http.getString());

        if(token != ""){

          Serial.println(token);
          
          i++;
        
        }
        else{

          i = 0;
          j = 0;
          
        }
      
      }
        
      else Serial.println("Fail to verify!");

     }
     
     if( nextProcess == 1 ){

      String message = "token:" + token + "&" + "data:" + data;

      Serial.println(message);

      http.begin("http://10.0.100.150:3000/logs/post");      //Specify request destination
     
      http.addHeader("Content-Type", "text/plain");  //Specify content-type header
      
      httpCode2 = http.POST(message);

      if(httpCode2 > 0){
     
        String payload = http.getString();                  //Get the response payload
  
        Serial.println("Success!!!"); 
     
        Serial.println(payload);    //Print request response payload

        k++;      // Make the process END

        nextProcess++;
     
     }
     
     else{

      i++;
      
      Serial.println(httpCode2);   //Print HTTP return code
     }
     
    }
     
      http.end();   //Close connection

      if( k == 1 ){
        process = false;
        i = 0;
        j = 0;
        k = 0;
        nextProcess = 0;
        startAgain = 0;
      }
      
    }
   
    delay(5000);  //Send a request every 30 seconds
 }
}
