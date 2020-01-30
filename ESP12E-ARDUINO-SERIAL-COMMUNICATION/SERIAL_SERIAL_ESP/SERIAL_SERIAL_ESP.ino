#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
     
const char* ssid = "SectionBsugoi"; // Set the wifi name
const char* password = "SectionB";  // Wifi password

ESP8266WebServer server(80);
MDNSResponder mdns;
     
String webPage = "";
String data = "";
     
void setup(void){
     
  //Set the pinOUT
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  WiFi.begin(ssid, password); // If the password and the name match with the connceted WIFI of the board
  Serial.println("");
     

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  data = Serial.readString(); 
  webPage += data;
  Serial.println("");
  Serial.print("Connected to "); 
  Serial.println(ssid);
  Serial.print("IP address: ");  //  Random IP will appear
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  //Turning on the server
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  
  server.begin();
  Serial.println("HTTP server started");
  
}
     
void loop(void){
  server.handleClient();
}
