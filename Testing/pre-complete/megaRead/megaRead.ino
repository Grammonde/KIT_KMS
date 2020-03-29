#include<MemoryFree.h>
#include<EEPROM.h>

boolean firstTime = true;
boolean secondTime;
boolean next;
boolean none = NULL;

String data = "";
String msg = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void serialEvent3(){
  while(Serial3.available()){
    char inChar;
    inChar = Serial3.read();
    Serial.print(inChar);
  }
  if(firstTime == true){
    Serial3.print("hey");
  }
}
