zint R = 4;
int B = 3;

void setup()

{

 Serial.begin(115200);

 Serial.println("Arduino Examples - 7 Color Flash");

 pinMode(R, OUTPUT);
 pinMode(B, OUTPUT);
}

void loop()
{
  digitalWrite(R, HIGH);
  delay(200);
  digitalWrite(R, LOW);
  delay(200);
  
  digitalWrite(B, HIGH);
  delay(200);
  digitalWrite(B, LOW);
  delay(200);
}
