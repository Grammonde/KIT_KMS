#include<>

  constexpr uint8_t greenLed = 10;
  constexpr uint8_t redLed = 11;
  int key = 22;

  char user_code[6];
  char code = {''};
  
  char initial_pass = {'1','2','3','4'};
  char password =[4];
  
  char key_pin = {'1'};
  char enter_pin[1];

void setup() {
  // put your setup code here, to run once:
  pinMode(key, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!(strncmp(user_code, code, 6)))
  {
    digitalWrite(greenLED, HIGH);
    delay(3000);
    digitalWrite(greenLED, LOW);
    if(!(strncmp(user_code, code, 6)))
    {
       digitalWrite(greenLED, HIGH);
       delay(3000);
       digitalWrite(greenLED, LOW);
       if(!(strncmp(user_code, code, 6)))
       {
        digitalWrite(key, HIGH);
        delay(3000);
        digitalWrite(key, LOW);
       }
       else
       {
        digitalWrite(redLED, HIGH);
        delay(3000);
        digitalWrite(redLED, LOW);
       }
    }
    else
    {
      digitalWrite(redLED, HIGH);
      delay(3000);
      digitalWrite(redLED, LOW);
    }
  }
  else
  {
    digitalWrite(redLED, HIGH);
    delay(3000);
    digitalWrite(redLED, LOW);
  }
}
