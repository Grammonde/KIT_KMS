boolean firstTime = true;
int proceed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
}

void loop(){
}

void serialEvent3() {
  // put your main code here, to run repeatedly:
  if(firstTime == true){
    Serial.println("1");
    Serial.println(firstTime);
    if(proceed == 1){
      Serial.println("First blood");
    }
    Serial.println(firstTime);
    Serial.println("middle man");
    if(proceed == 2){
      Serial.println("Rampage");
    }
    Serial.println("End");
    firstTime = false;
    Serial.println(firstTime);
  }
}
