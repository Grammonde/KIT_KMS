#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "KIT Campus II";
const char* password = NULL;
String data = "";
//String datas = "bonjour";

void setup () {
 
  Serial.begin(115200); 
  while(!Serial){
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  WiFi.begin(ssid, password);
  Serial.println("");
   
  while (WiFi.status() != WL_CONNECTED) {
   
  delay(1000);
  Serial.println("Connecting..");
   
  }
    
    data = Serial.readString();
        
    Serial.println("Connected");
    
}

void loop() {

if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient
  
    Serial.println(data.length());

    Serial.println(data);
   
  //http.begin("http://10.10.13.77:3000/api/get?token=24EFhweWOOZ844AycXd7hH4mhLjZQSU0poVznIVXNvW&data="+data);  //Specify request destination
  
  //http.begin("http://jsonplaceholder.typicode.com/users/1");

    http.begin("http://10.10.13.77:3000/mony/get?data="+data);
  
    int httpCode = http.GET();                     //Send the request
   
    if (httpCode > 0) { //Check the returning code
    
    String payload = http.getString();   //Get the request response payload
        
    Serial.println(payload); //Print the response payload
    
    Serial.println("Success!");
    
  } 
  else
    Serial.println(httpCode);
    
    http.end();   //Close connection
   
  }
   
  delay(10000);    //Send a request every 30 seconds
 
}
