#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "SectionBsugoi";
const char* password = "SectionB";  
String data = "";
String verifies = "";
int httpCode1;
int httpCode2;
int i = 0;
int j = 0;
int k = 0;
int nextProcess = 0;
boolean process = true;

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
  
  Serial.println("Connected");
  
}
 
void loop() {
  
 if(process == true){

  if(j == 0){
    
    verifies = Serial.readString();

    j++;
    
    if(verifies == "123456789"){
      
      Serial.println("*");
  
      i++;
      
    }
  
  }
  
  if( i == 2 ){

    data = Serial.readString();

    nextProcess += 1;
    
  }
  
   if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   
     HTTPClient http;    //Declare object of class HTTPClient
  
     Serial.println(verifies.length());
  
     Serial.println(verifies);

     Serial.println(data.length());

     Serial.println(data);
  
     if( i == 1 ){ 

      http.begin("http://10.10.13.77:3000/verify/post");      //Specify request destination
     
      http.addHeader("Content-Type", "text/plain");  //Specify content-type header
      
      httpCode1 = http.POST(verifies);     //Send the request

      if(httpCode1 > 0){
        
        Serial.println("Verified Completed!");

        i++;

        Serial.println(http.getString());
      
      }
        
      else Serial.println("Fail to verify!");
      
     }
     
     if( nextProcess == 1 ){

      http.begin("http://10.10.13.77:3000/data/post");      //Specify request destination
     
      http.addHeader("Content-Type", "text/plain");  //Specify content-type header
      
      httpCode2 = http.POST(data);

      if(httpCode2 > 0){
     
        String payload = http.getString();                  //Get the response payload
  
        Serial.println("Success!!!"); 
     
        Serial.println(payload);    //Print request response payload

        k++;      // Make the process END
     
     }
     
     else{
      
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
      }
      
    }
   
    delay(10000);  //Send a request every 30 seconds
 }
}
 
