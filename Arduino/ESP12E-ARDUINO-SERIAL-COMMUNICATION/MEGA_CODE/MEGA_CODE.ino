String str = "hello"; 
void setup(){
 Serial.begin(115200);
 Serial3.begin(115200);
 Serial.print(char(169)); // Copyright Symbol 
 Serial.println("Myengineeringstuffs.com");
 delay(2000);
}
void loop(){
  Serial3.println(str);
  delay(5000);
}
