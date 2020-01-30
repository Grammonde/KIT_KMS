
/* A simple program to sequentially turn on and turn off 3 LEDs */ 

int LED1 = 13;

void setup() {
   pinMode(LED1, OUTPUT);
}
void loop() {
  digitalWrite(LED1, HIGH);    // turn on LED1 
  delay(200);                  // wait for 200ms
  digitalWrite(LED1, LOW);     // turn off LED1
  delay(200);                  // wait for 300ms
}
