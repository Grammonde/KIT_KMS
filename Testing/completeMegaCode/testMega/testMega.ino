// Include required libraries
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>

// Create instances
LiquidCrystal_I2C lcd(0x3F, 16, 2); 
MFRC522 mfrc522(53, 49); // MFRC522 mfrc522(SS_PIN, RST_PIN)

// Initialize Pins for led
// Green LED is connected to 5V

//Vairable for condition:
boolean firstTime = true;  //Accept card only one at a time
boolean begun = false;
boolean esp = false;
boolean espProgress = false;
boolean dataVerify = false;
boolean isCorrect = false;
boolean time_limited = false;
//boolean dataCorrect = false;
//boolean dataWrong = false;
boolean availability = false; //After checking the availablity of the card in database
boolean rfid = false;

String scanned_id = ""; //Store the user Tag_UID for verifying purpose
int token = 0; // Make sure that the token is returned from server after verifying for posting data purpose

int existence = 0;
int count1 = 0;
int count2 = 0;

constexpr uint8_t greenLed = 10;
constexpr uint8_t redLed = 11;
int key1 = 22;
int key2 = 23;
int key3 = 24;
int key4 = 25;
int key5 = 26;
int key6 = 27;

char initial_password1[1] = {'1'};  // Variable to store initial password
char initial_password2[1] = {'2'};
char initial_password3[1] = {'3'};
char initial_password4[1] = {'4'};
char initial_password5[1] = {'5'};
char initial_password6[1] = {'6'};

String tagUID = "F3 78 2D 1B";  // String to store UID of tag. Change it with your tag's UID

char password[1];   // Variable to store users password
boolean RFIDMode = true; // boolean to change modes
char key_pressed = 0; // Variable to store incoming keys
char key_Enter = 0;
//char key_Back = 0;
uint8_t i = 0;  // Variable used for counter
uint8_t j = 0;
// uint8_t k = 0; 

// defining how many rows and columns our keypad have
const byte rows = 4;
const byte columns = 4;

// Keypad pin map
char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'R'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', ' '},
  {'O', '0', ' ', ' '}
}; 

// Initializing pins for keypad
byte row_pins[rows] = {9 ,8 ,7 ,14 };
byte column_pins[columns] = {5, 4, 3, 2};

// Create instance for keypad
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup() {
  // Arduino Pin configuration
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(key1, OUTPUT); 
  pinMode(key2, OUTPUT);
  pinMode(key3, OUTPUT);
  pinMode(key4, OUTPUT);
  pinMode(key5, OUTPUT);
  pinMode(key6, OUTPUT);

  Serial.begin(115200);
  Serial3.begin(115200);

  lcd.begin();   // LCD screen
  lcd.backlight();
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522

  lcd.clear(); // Clear LCD screen
}
  
void loop() {
  while(firstTime == true){
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
      return;
    }

    Serial.println("Hi"); 
      
    //Reading from the card
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();  
    scanned_id = tag.substring(1);
    Serial3.print("*");
    Serial.println("Hello");
      
    firstTime = false;    //Set firstTime to false
    esp = true;           //Set confirmation of esp starting = false
  }
}

//Built in Function for checking whether the port is available or not
void serialEvent3(){
  // System will first look for mode
  if (RFIDMode == true) {
    if(esp == true){
      espProgress = Serial3.findUntil("Starting", "$");
      if(espProgress == true){
        begun = true;
        esp = false;
        espProgress = false;
      }
    }
    
    if (begun == true){
      //Send scanned ID to esp8266 for database verification
      Serial3.print(scanned_id);
      Serial.println(scanned_id);
            
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Verifying   ");
      lcd.setCursor(0, 1);
      lcd.print("  Please Wait ");

      begun = false;
      dataVerify = true;
    }

    if(dataVerify == true){
      isCorrect = Serial3.findUntil("Correct", "Wrong");
      time_limited = Serial3.findUntil("Limited", "$");
//      dataCorrect = Serial3.findUntil("Correct", "$");
//      dataWrong = Serial3.findUntil("Incorrect", "$");
      Serial.println("Reached");
      if(isCorrect == true){
        token += 1;
        availability = true;
        dataVerify = false;
      }
      else if(isCorrect == false){
        token -= 1;
        availability = true;
        dataVerify = false;
        if(time_limited == true){
          firstTime = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("      Reached   ");
          lcd.setCursor(0, 1);
          lcd.print("Limited Attempts ");
          delay(30000);
          time_limited = false;
        }
      }
    }
    
    if(availability == true){
      //Checking the card
      if(token == 1)
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

        token = -1;
        isCorrect = false;
//        dataCorrect = false;
//        dataWrong = false;
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

        token = -1;
        isCorrect = false;
//        dataWrong = false;
//        dataCorrect = false;
        firstTime = true;
        availability = false;
      }
    }
  }

  // If RFID mode is false, it will look for keys from keypad
  if (RFIDMode == false) {
    key_pressed = keypad_key.getKey();   // Storing keys
    if (key_pressed)                                       
    {
      password[i++] = key_pressed; // Storing in password variable
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
    if (j == 1) // If 4 keys are completed
    {
      delay(200);
      if (!(strncmp(password, initial_password1, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key1, HIGH);
        delay(3000);
        digitalWrite(key1, LOW);
        lcd.clear();
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(password, initial_password2, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key2, HIGH);
        delay(3000);
        digitalWrite(key2, LOW);
        lcd.clear();
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(password, initial_password3, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key3, HIGH);
        delay(3000);
        digitalWrite(key3, LOW);
        lcd.clear();
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(password, initial_password4, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key4, HIGH);
        delay(3000);
        digitalWrite(key4, LOW);
        lcd.clear();
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(password, initial_password5, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key5, HIGH);
        delay(3000); 
        digitalWrite(key5, LOW);
        lcd.clear();
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else if (!(strncmp(password, initial_password6, 1))) // If password is matched
      {
        lcd.clear();
        lcd.print("Unlocked");
        digitalWrite(key6, HIGH);
        delay(3000);
        digitalWrite(key6, LOW);
        lcd.clear();
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else{
        lcd.clear();
        lcd.print("  INVALID PIN  ");
        delay(3000);
        lcd.clear();
        i = 0;
        j = 0;
        RFIDMode = true; // Make RFID mode true
      }
    }
  }
  delay(3000);
}
  
