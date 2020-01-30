#include <MemoryFree.h>
#include <EEPROM.h>

#define PIN_LED 13    // PINOUT set 
String inString;

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}

void loop() {
}

// Checking with the port Serial3
void serialEvent3() {
  while (Serial3.available()) {
    //Read the data from the port serial3
    char inChar = Serial3.read();
    // Write the data to serial Port Serial3
    Serial.write(inChar);
    inString += inChar;
    if (inChar == ']') {
      if (inString.indexOf("[ON]")>0) {
        digitalWrite(PIN_LED, HIGH);
      }
      else if (inString.indexOf("[OFF]")>0) {
        digitalWrite(PIN_LED, LOW);
      }
      else
      {
        Serial.println("Wrong command");
      }
      inString = "";
    }
  }
}
