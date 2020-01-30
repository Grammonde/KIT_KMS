#include <MemoryFree.h> 
#include <EEPROM.h>

String inString;
int i = 0;
int j = 0;
String keynum = "12";
int nextProcess = 0;

String verify = "123456789";
String instring = "";

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
    if(inChar == '*'){
      nextProcess += 1;
      Serial.println("Hello");
    }
    // Write the data to serial Port Serial3
    Serial.write(inChar);
  }

  if (i == 1 && nextProcess == 1){
    Serial.println(nextProcess);
    Serial3.print("." + keynum);
    i++;
  }
  
  if(verify == "123456789" && j == 0){
    Serial3.print(verify);
    j++;
    i++;
  }
}
