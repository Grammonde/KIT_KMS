#include <MemoryFree.h> 
#include <EEPROM.h>

int i = 0;
int j = 0;
int k = 0;
int nextProcess = 0;

String keynum = "12";
String verify = "C3 78 2D 1B";

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
    }
    if(inChar == '#'){
      j++; 
    }
    // Write the data to serial Port Serial3
    Serial.write(inChar);
  }

  if( k == 0 ){
    Serial.println("Start!");
    Serial3.print("incoming");
    k++;
  }

  if (i == 1 && nextProcess == 1){
    Serial.println(nextProcess);
    Serial3.print(verify + "."  + keynum);
    i++;
  }
  
  if(verify == "F3 78 2D 1B" && j == 1){
    Serial3.print(verify);
    j++;
    i++;  
  }
}
