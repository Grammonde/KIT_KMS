#include<EEPROM.h>
#include<Ethernet2.h>

String string_Value;
String string_Read;
boolean isTrue = true;
boolean continues = false;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  string_Value = "Hello world";
  string_Value=string_Value+";";
  //EEPROM.begin(512);
}

void loop() {
  //To write the data into Flash memories
  if(isTrue == true){
    Serial.println(string_Value.length());
    Serial.println(string_Value);
    for(int n=0; n < string_Value.length(); n++){
      EEPROM.write(n, string_Value[n]);
      Serial.println(n);
    }
    Serial.println(EEPROM.length());
    isTrue = false;
    continues = true;
  }

  //To read from flash memories
  if(continues == true){
    for(int n = 0; n < 11; ++n){
      if(char(EEPROM.read(n)!=';')){
        string_Read += String(char(EEPROM.read(n)));
      }
    }
    Serial.println(string_Read);
    continues = false;
  }
}
