#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
     
const char* ssid = "vKirirom"; // Set the wifi name
const char* password = NULL;  // Wifi password

ESP8266WebServer server(80);
MDNSResponder mdns;
     
String webPage = "";
     
int led_pin = 13;
     
void setup(void){
     
  //Set the pinOUT
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  //Board's Data
  Serial.println("");
  Serial.println("ESP8266 board info:");
  Serial.print("\tChip ID: ");
  Serial.println(ESP.getFlashChipId());
  Serial.print("\tCore Version: ");
  Serial.println(ESP.getCoreVersion());
  Serial.print("\tChip Real Size: ");
  Serial.println(ESP.getFlashChipRealSize());
  Serial.print("\tChip Flash Size: ");
  Serial.println(ESP.getFlashChipSize());
  Serial.print("\tChip Flash Speed: ");
  Serial.println(ESP.getFlashChipSpeed());
  Serial.print("\tChip Speed: ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.print("\tChip Mode: ");
  Serial.println(ESP.getFlashChipMode());
  Serial.print("\tSketch Size: ");
  Serial.println(ESP.getSketchSize());
  Serial.print("\tSketch Free Space: ");
  Serial.println(ESP.getFreeSketchSpace());

  //Buidling the HTML webpage for server
  webPage += "<h1>ESP8266 Web Server</h1>";
  webPage += "<p>Chip ID: ";
  webPage += ESP.getFlashChipId();
  webPage += "</p>";
  webPage += "<p>Core Version: ";
  webPage += ESP.getCoreVersion();
  webPage += "</p>";
  webPage += "<p>Chip Real Size: ";
  webPage += ESP.getFlashChipRealSize()/1024;
  webPage += " Kbit</p>";
  webPage += "<p>Chip Size: ";
  webPage += ESP.getFlashChipSize()/1024;
  webPage += " Kbit</p>";
  webPage += "<p>Chip Flash Speed: ";
  webPage += ESP.getFlashChipSpeed()/1000000;
  webPage += " MHz</p>";
  webPage += "<p>Chip Work Speed: ";
  webPage += ESP.getCpuFreqMHz();
  webPage += " MHz</p>";
  webPage += "<p>Chip Mode: ";
  webPage += ESP.getFlashChipMode();
  webPage += "</p>";
  webPage += "<p>LED state <a href=\"LedON\"><button>ON</button></a>&nbsp;<a href=\"LedOFF\"><button>OFF</button></a></p>";


  WiFi.begin(ssid, password); // If the password and the name match with the connceted WIFI of the board
  Serial.println("");
     

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
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
  
  server.on("/LedON", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(led_pin, HIGH);
    Serial.println("[ON]");
    delay(1000);
  });
  
  server.on("/LedOFF", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(led_pin, LOW);
    Serial.println("[OFF]");
    delay(1000);
  });

    server.on("/Led?????", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(led_pin, LOW);
    Serial.println("[OFF]");
    delay(1000);
  });
  
  server.begin();
  Serial.println("HTTP server started");
  
}
     
void loop(void){
  server.handleClient();
}
