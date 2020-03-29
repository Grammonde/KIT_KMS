#include<EEPROM.h>

String write_data = "F3 78 2D 1B.12";
String read_data;

boolean isTrue = true;
boolean continues = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  write_data = write_data+"/";
  //ESP8266 has only 512 bytes for flash memories
  EEPROM.begin(512);
}

void loop() {
  //Write data in EPS8266's flash memories
  if(isTrue == true){
    Serial.println(write_data);
    Serial.println(write_data.length());
    for(int i = 0; i < write_data.length(); i++){
      EEPROM.write(i, write_data[i]);   
    }
    EEPROM.commit();
    isTrue = false;
    continues = true;
  }
  if(continues == true){
    for(int j = 0; j < 15; ++j){
      if(char(EEPROM.read(j)) != '/'){
        read_data += String(char(EEPROM.read(j)));
      }
    }
    Serial.println(read_data);
    continues = false;
  }
}
