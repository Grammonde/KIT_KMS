#include <MemoryFree.h> 
#include <EEPROM.h>

String inString;
int i = 0;
void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
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
  }
  if (i == 0){
    Serial3.print("BBBBBBBBBB120.1");
  }
  i++;
}
