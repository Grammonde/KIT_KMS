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
boolean availability = false; //After checking the availablity of the card in database
boolean rfid = false;
boolean log_out = false;
boolean db_send = false;
boolean send_key = false;
boolean process_status = false;
boolean inserted = false;
boolean fail_insert = false;

String key_selected = "";
String scanned_id = ""; //Store the user Tag_UID for verifying purpose
int token = 0; // Make sure that the token is returned from server after verifying for posting data purpose

int existence = 0;
int invalid_inp = 0;

constexpr uint8_t greenLed = 10;
constexpr uint8_t redLed = 11;
int key1 = 22;
int key2 = 23;
int key3 = 24;
int key4 = 25;
int key5 = 26;
int key6 = 27;

int unlocked_key;

char initial_key_pin1[1] = {'1'};  // Variable to store initial key_pin
char initial_key_pin2[1] = {'2'};
char initial_key_pin3[1] = {'3'};
char initial_key_pin4[1] = {'4'};
char initial_key_pin5[1] = {'5'};
char initial_key_pin6[1] = {'6'};

String tagUID = "F3 78 2D 1B";  // String to store UID of tag. Change it with your tag's UID

char key_pin[5];   // Variable to store users key_pin
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
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'O', '8', 'R', ' '},
  {'O', '0', 'R', ' '}
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
  if(firstTime == true){
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

  // If RFID mode is false, it will look for keys from keypad
  if (RFIDMode == false) {
    key_pressed = keypad_key.getKey();   // Storing keys
    if (key_pressed)                                       
    {
      key_pin[i++] = key_pressed; // Storing in key_pin variable
      lcd.print(key_pressed);
      if (key_pressed == 'O'){  // Press 'O' to log out from the system
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     Logging ");
        lcd.setCursor(0, 1);
        lcd.print("       Out   ");
        delay(3000);
        log_out = true;
        RFIDMode = true;
        firstTime = true;
      }
      else if(key_pressed == ' '){  // ' ' = Enter pressed = D button on keypad
        db_send = true;
        j++;
        Serial3.print("In");
      }
      else if(key_pressed == 'R'){  //Re-input
        //Clear screen for new input
        lcd.clear();
        lcd.print("Enter KEY PIN:");
        lcd.setCursor(0, 1);

        //Clear the char arrays using memset
        memset(key_pin, 0, sizeof key_pin);
      }
    }
    
    if (j == 1) // If 4 keys are completed
    {
      delay(200);
      if (!(strncmp(key_pin, initial_key_pin1, 1))) // If key_pin is matched
      {
        key_selected = '1';
        unlocked_key = key1;
        memset(key_pin, 0, sizeof key_pin);

        RFIDMode = true;
        send_key = true;
        i = 0;
        j = 0;
      }
      else if (!(strncmp(key_pin, initial_key_pin2, 1))) // If key_pin is matched
      {
        key_selected = '2';
        unlocked_key = key2;
        memset(key_pin, 0, sizeof key_pin);
        
        RFIDMode = true;
        send_key = true;
        i = 0;
        j = 0;
      }
      else if (!(strncmp(key_pin, initial_key_pin3, 1))) // If key_pin is matched
      {
        Serial.println(initial_key_pin3);
        key_selected = '3';
        unlocked_key = key3;
        memset(key_pin, 0, sizeof key_pin);

        RFIDMode = true;        
        send_key = true;
        i = 0;
        j = 0;
      }
      else if (!(strncmp(key_pin, initial_key_pin4, 1))) // If key_pin is matched
      {
        key_selected = '4';
        unlocked_key = key4;
        memset(key_pin, 0, sizeof key_pin);
        
        RFIDMode = true;        
        send_key = true; 
        i = 0;
        j = 0;
      }
      else if (!(strncmp(key_pin, initial_key_pin5, 1))) // If key_pin is matched
      {
        key_selected = '5';
        unlocked_key = key5;
        memset(key_pin, 0, sizeof key_pin);
        
        RFIDMode = true;        
        send_key = true;
        i = 0;
        j = 0;
      }
      else if (!(strncmp(key_pin, initial_key_pin6, 1))) // If key_pin is matched
      {
        key_selected = '6';
        unlocked_key = key6;
        memset(key_pin, 0, sizeof key_pin);
        
        RFIDMode = true;        
        send_key = true;
        i = 0;
        j = 0;
      }
      else{
        lcd.clear();
        lcd.print("  INVALID PIN  ");
        delay(3000);
        lcd.clear();
//        invalid_pin += 1;
//        if(invalid_pin >= 5){
//          lcd.clear();
//          lcd.setCursor(0, 0);
//          lcd.print("  Many Invalid PIN  ");
//          lcd.setCursor(0, 1);
//          lcd.print("Logging Out");
//
//          db_send = false;
//          i = 0;
//          j = 0;
//          RFIDMode = true;
//          firstTime = true;
//          invalid_pin = 0;
//        }
      }
    }
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
      Serial.println("Reached");
      if(isCorrect == true){
        token = 1;
        availability = true;
        dataVerify = false;
      }
      else if(isCorrect == false){
        token = -1;
        availability = true;
        dataVerify = false;
        if(time_limited == true){
          firstTime = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("    Reached   ");
          lcd.setCursor(0, 1);
          lcd.print("Limited Attempts ");
          delay(180000);
          
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

        token = 0;
        isCorrect = false;
        availability = false;
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

        token = 0;
        isCorrect = false;
        firstTime = true;
        availability = false;
      }
    }
  }
  if(db_send == true){
    if(send_key == true){
      Serial.println(scanned_id + "." + key_selected);
      Serial3.print(scanned_id + "." + key_selected);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Please wait  ");
      lcd.setCursor(0, 1);
      lcd.print(" Unlocking... ");
           
      process_status = true;
      send_key = false;
    }
    if(log_out == true){
      Serial3.print("Out");

      db_send = false;
      log_out = false;
      firstTime = true;
    }

    if(process_status == true){
      inserted = Serial3.findUntil("Inserted", "Abnormal");
      if(inserted == true){
        delay(500);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     Unlocked    ");
        lcd.setCursor(0, 1);
        lcd.print("  Thank You  ");
        digitalWrite(unlocked_key, HIGH);
        delay(3000);
        digitalWrite(unlocked_key, LOW);
        lcd.clear();

        db_send = false;
        inserted = false;
        process_status = false;
        firstTime = true;
        //invalid_pin = 0;
      }
      else{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    Trying...    ");
        lcd.setCursor(0, 1);
        lcd.print("   Please Wait   ");
        fail_insert = Serial3.findUntil("Stop", "Success");
        if(fail_insert == true){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("     Went Wrong");
          lcd.setCursor(0, 1);
          lcd.print(" Please Try again ");
          lcd.clear();

          fail_insert = false;
          firstTime = true;
          inserted = false;
          process_status = false;
          db_send = false;
        }
      }
    }
  }
}
  
