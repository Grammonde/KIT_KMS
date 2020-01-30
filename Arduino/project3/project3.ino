int LED1 = 30;
//int LED2 = 12;
//int LED3 = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
//  pinMode(LED2, OUTPUT);
//  pinMode(LED3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED1, HIGH);
  delay(300);
//  digitalWrite(LED2, HIGH);
//  delay(300);
//  digitalWrite(LED3, HIGH);
//  delay(300);
  
  digitalWrite(LED1, LOW);
  delay(300);
//  digitalWrite(LED2, LOW);
//  delay(300);
//  digitalWrite(LED3, LOW);
//  delay(300);
}
