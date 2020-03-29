// Include required libraries
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>
#include <MemoryFree.h> 
#include <EEPROM.h>

// Create instances
LiquidCrystal_I2C lcd(0x3F, 16, 2); 
MFRC522 mfrc522(53, 49); // MFRC522 mfrc522(SS_PIN, RST_PIN)

// Variable used for counter
uint8_t i = 0; 
uint8_t j = 0;
// uint8_t k = 0; 

//Secutrity purpose
int alert = 0;

//Condition for process
int startProcess = 0;   //Sign for the start of system processing
int secondProcess = 0;  //Forward to the second process
int nextProcess = 0;    //Confirmed whether the verify process is true or false
int token = 0;    //Verify the tag uid for user display whether the card is exist or not

// Initialize Pins for led
// Green LED is connected to 5V
constexpr uint8_t greenLed = 10;
constexpr uint8_t redLed = 11;

//Pin out for solenoid control
int key1 = 22;
int key2 = 23;
int key3 = 24;
int key4 = 25;
int key5 = 26;
int key6 = 27;

// Variable to store initial no of the Keys
char initial_keynum1[1] = {'1'};  
char initial_keynum2[1] = {'2'};
char initial_keynum3[1] = {'3'};
char initial_keynum4[1] = {'4'};
char initial_keynum5[1] = {'5'};
char initial_keynum6[1] = {'6'};

//Received ID from user
String scanned_id;

// Variable to store users input key
char input_key[3];  

// boolean to change modes
boolean RFIDMode = true; 

// Variable to store incoming keys
char key_pressed = 0;
char key_Enter = 0;
//char key_Back = 0;

// defining how many rows and columns our keypad have
const byte rows = 4;
const byte columns = 4;

// Keypad pin map
char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'R'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'O', '0', ' ', ' '}
}; 

// Initializing pins for keypad
byte row_pins[rows] = {9 ,8 ,7 ,14 };
byte column_pins[columns] = {5, 4, 3, 2};

// Create instance for keypad
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup() {

  //Begin Native port serial and normal serial port
  Serial.begin(115200);
  Serial3.begin(115200);
  
  // Arduino Pin configuration
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  //Configuration for Key numbers
  pinMode(key1, OUTPUT); 
  pinMode(key2, OUTPUT);
  pinMode(key3, OUTPUT);
  pinMode(key4, OUTPUT);
  pinMode(key5, OUTPUT);
  pinMode(key6, OUTPUT);

  lcd.begin();   // LCD screen
  lcd.backlight();
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522

  lcd.clear(); // Clear LCD screen
}

void loop() {
}

//Built in function for Native port 3 while the port is available
void serialEvent3(){
  while (Serial3.available()) {
    //Read the data from the port serial3
    char inChar = Serial3.read(); 
    // Write the data to serial Port Serial3
    Serial.write(inChar);
  } 
  
  // System will first loxok for mode
  if (RFIDMode == true) {
    lcd.setCursor(0, 0);
    lcd.print("     Please   ");
    lcd.setCursor(0, 1);
    lcd.print(" Scan Your Tag ");

    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      Serial3.print("incoming");
      return;
    }

    //Call condition function in order to triger ESP verify route
    int startProcess = operation_start();
    Serial.println(startProcess); 

    //Reading from the card
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();

    scanned_id = tag.substring(1);
    Serial.println(scanned_id);

    if( startProcess == 1 && scanned_id != NULL){
      Serial3.print(scanned_id);
      startProcess++;
    }

    token = rfid_verify();

    //Checking the card
    if (token == 1)
    {
      // If UID of tag is matched.
      lcd.clear();
      lcd.print("Tag Matched");
      digitalWrite(greenLed, HIGH);
      delay(1000);   
      digitalWrite(greenLed, LOW);

      lcd.clear();
      lcd.print("Enter KEY PIN:");
      lcd.setCursor(0, 1);
      
      nextProcess++;
      
      RFIDMode = false; // Make RFID mode false
    }

    else
    {
      // If UID of tag is not matched.
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong Tag Shown");
      lcd.setCursor(0, 1);
      lcd.print("Access Denied");
      digitalWrite(redLed, HIGH);
      delay(1000);
      digitalWrite(redLed, LOW);
      lcd.clear();
    }
  }

  secondProcess = processTwo();

  // If RFID mode is false, it will look for keys from keypad
  if (RFIDMode == false) {
    key_pressed = keypad_key.getKey();   // Storing keys
    if (key_pressed)                                       
    {
      input_key[i++] = key_pressed; // Storing in password variable
      lcd.print(key_pressed);
      if (key_pressed == 'O'){  // Press 'O' to log out from the system
        RFIDMode = true;
      }
      else if(key_pressed == ' '){  // ' ' = Enter pressed = D button on keypad
        j++;
      }
//      else if(key_pressed == 'R'){
//        RFIDMode = false;
//      }
    }
    if (j == 1 && nextProcess == 1 && secondProcess == 1) // If 4 keys are completed
    {
      delay(200);
      if (!(strncmp(input_key, initial_keynum1, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key1, HIGH);
        delay(3000);
        digitalWrite(key1, LOW);
        lcd.clear();

        //Send data to esp8266 and to server for storing purpose
        Serial3.print(scanned_id + "." + input_key);

        startProcess = 0;
        nextProcess = 0;
        secondProcess = 0;
        token = 0;
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(input_key, initial_keynum2, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key2, HIGH);
        delay(3000);
        digitalWrite(key2, LOW);
        lcd.clear();

        //Send data to esp8266 and to server for storing purpose
        Serial3.print(scanned_id + "." + input_key);

        startProcess = 0;
        nextProcess = 0;
        secondProcess = 0;
        token = 0;        
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(input_key, initial_keynum3, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key3, HIGH);
        delay(3000);
        digitalWrite(key3, LOW);
        lcd.clear();

        //Send data to esp8266 and to server for storing purpose
        Serial3.print(scanned_id + "." + input_key);

        startProcess = 0;
        nextProcess = 0;
        secondProcess = 0;
        token = 0;        
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(input_key, initial_keynum4, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key4, HIGH);
        delay(3000);
        digitalWrite(key4, LOW);
        lcd.clear();

        
        //Send data to esp8266 and to server for storing purpose
        Serial3.print(scanned_id + "." + input_key);

        startProcess = 0;
        nextProcess = 0;
        secondProcess = 0;
        token = 0;        
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(input_key, initial_keynum5, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key5, HIGH);
        delay(3000); 
        digitalWrite(key5, LOW);

        //Send data to esp8266 and to server for storing purpose
        Serial3.print(scanned_id + "." + input_key);

        startProcess = 0;
        nextProcess = 0;
        secondProcess = 0;
        token = 0;        
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(input_key, initial_keynum6, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key6, HIGH);
        delay(3000);
        digitalWrite(key6, LOW);
        lcd.clear();


        //Send data to esp8266 and to server for storing purpose
        Serial3.print(scanned_id + "." + input_key);

        startProcess = 0;
        nextProcess = 0;
        secondProcess = 0;
        token = 0;        
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else{
        lcd.clear();
        lcd.print("  INVALID PIN  ");
        delay(3000);
        lcd.clear();
        alert++;
        if(alert >=3){      // No invalid input over 3 times
          startProcess = 0;
          nextProcess = 0;
          secondProcess = 0;
          token = 0;   
          i = 0;
          j = 0;
          RFIDMode = true; // Make RFID mode true
        }
        else{
          i = 0;
          j = 0;
        }
      }
    }
  }
}

int operation_start(){
  char inChar;
  int count = 0;
  while(Serial3.available()){
    inChar = Serial3.read();
    Serial.write(inChar);
    if(inChar == '#'){
      count++;
      break;
    }
  }
  return count;
}

int processTwo(){
  char inChar;
  int count = 0;
  while(Serial3.available()){
    inChar = Serial3.read();
    Serial.write(inChar);
    if(inChar == '*'){
      count++;
      break;
    }
  }
  return count;
}

int rfid_verify(){
  char inChar;
  int count = 0;
  while(Serial3.available()){
    inChar = Serial3.read();
    Serial.write(inChar);
    if(inChar == '+'){
      count++;
      break;
    }
    else if(inChar == '-'){
      count--;
      break;
    }
  }
  return count;
}
