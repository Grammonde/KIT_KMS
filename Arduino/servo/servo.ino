#include <Servo.h>
Servo servo1;
int pos;

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
}

void loop() {
  pos = Serial.read();
  if(pos == 180){
    servo1.write(180);
  }
  if( pos == 0 ){
    servo1.write(90); 
  }
}
