String data = "";
String msg = "";
boolean firstTime = true;
boolean secondTime;

void setup() { 
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial){
    ;
  }
}

void loop() {
  if(firstTime == true){
    delay(10000);
    Serial.println("Hello");
    secondTime = Serial.find("hey");
    firstTime = false;
  }
  if(secondTime == true){
    Serial.println("Completed");
  }
}
