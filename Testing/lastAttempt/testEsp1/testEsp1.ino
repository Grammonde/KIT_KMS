#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "BoreyR J-5";
const char* password = NULL;    

String data = "";
String verifies = "";
String token = "";

boolean setProcess = false;
boolean dbFail = false;
boolean logOut = false;
boolean logOut_condition = false;

int startAgain = 0 ;
int httpCode1;
int httpCode2;
int i = 0;
int j = 0;
int k = 0;
int nextProcess = 0;
int verify_time = 0;
int abnormal_time = 0;

boolean process = false;

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
 if(startAgain == 0){
  setProcess = Serial.find("*");  
  if(setProcess == true){
    process = true;
    Serial.println("Starting");
    startAgain += 1;
  }
  Serial.println(setProcess);
 }
 
 if(process == true){
  
  //Get tag_uid for verifying from Mega
  if(j == 0){
    verifies = Serial.readString();
    if(verifies != NULL){
      Serial.println("Tag_UID received");
      j++;
      i++; 
    }
  }

  //Get key data for Database querying from Mega
  if( i == 2 ){
    data = Serial.readString();
    if(data != NULL){
      nextProcess += 1;
    }
  }

  //Check for connection with WIFI
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   
     HTTPClient http;    //Declare object of class HTTPClient

     //Serial print the data read from Mega for debugging purpose
     Serial.println(verifies.length());  
     Serial.println(verifies);
     Serial.println(data.length());
     Serial.println(data);

     //Condition for Verifying route with API
     if( i == 1 ){ 
      http.begin("http://10.10.14.92:3000/verify");      //Specify request destination     
      http.addHeader("Content-Type", "text/plain");  //Specify content-type header      
      httpCode1 = http.POST(verifies);     //Send the request

      if(httpCode1 > 0){
        token = "";
        token = http.getString();        
        Serial.println(token);
        if(token == "Rfid not found"){
          Serial.println("Wrong");
          verify_time++;
          i = 0;
          j = 0;
          startAgain = 0;
          process = false;
          if(verify_time >= 5){
            Serial.println("Limited");
            verify_time = 0;
          }
        }
        else{
          Serial.println("Correct");
          i++;
          logOut_condition = true;
        }
      }
      else{
        i = 0;
        j = 0;
        startAgain = 0;
        process = false;
        Serial.println(httpCode1);
      }
     }

     if(logOut_condition == true){
        logOut = Serial.findUntil("Out", "In");  
        if(logOut == true){
         process = false;
         i = 0;
         j = 0;
         k = 0;
         nextProcess = 0;
         startAgain = 0;
         logOut = false;
         logOut_condition = false;
       }
     }

     //Condition for database query with API
     if( nextProcess == 1 ){
      String message = "token:" + token + "&" + "data:" + data;
      Serial.println(message);
      http.begin("http://10.10.14.92:3000/logs/post");      //Specify request destination
      http.addHeader("Content-Type", "text/plain");         //Specify content-type header
      httpCode2 = http.POST(message);
      if(httpCode2 > 0){
        String payload = http.getString();                  //Get the response payload
        Serial.println(payload);    //Print request response payload
        Serial.println("Success");
        k++;      // Make the process END
        nextProcess++;
      }
      else{
        Serial.println("Abnormal");   //Print HTTP return code
        i++;
        abnormal_time += 1;
        if(abnormal_time >= 3){
          Serial.println("Stop");
          
          abnormal_time = 0;
          process = false;
          i = 0;
          j = 0;
          nextProcess = 0;
          startAgain = 0;
          logOut_condition = false;
        }
      }
    }
      http.end();   //Close connection
      
      //Condition to end the process
      if( k == 1 ){
        process = false;
        i = 0;
        j = 0;
        k = 0;
        token = "";
        nextProcess = 0;
        abnormal_time = 0; 
        startAgain = 0;
        logOut_condition = false;
      }  
   }
    delay(2000);  //Send a request every 30 seconds
 }
}
 
