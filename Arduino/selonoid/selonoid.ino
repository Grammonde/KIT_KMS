int solenoid1 = 4;
int solenoid2 = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(solenoid1, OUTPUT);
  pinMode(solenoid2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to r un repeatedly:
  digitalWrite(solenoid1, HIGH);
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(2000);
  digitalWrite(solenoid1, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);

  digitalWrite(solenoid2, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000); 
  digitalWrite(solenoid2, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
}
