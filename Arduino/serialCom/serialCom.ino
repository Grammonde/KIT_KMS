#include <MemoryFree.h>
#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  Serial.write("Mega8266");
  delay(2000);
}
