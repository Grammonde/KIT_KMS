// Connect Vcc and Ground, SDA to A4, SCL to A5 on Arduino
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Set the LCD I2C address
#include <AddicoreRFID.h>
#include <SPI.h>
#define  uchar unsigned char
#define uint  unsigned int
uchar serNumA[5];
uchar fifobytes;
uchar fifoValue;
AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module
const int chipSelectPin = 100;
const int NRSTPD = 9; // RST Pin
#define MAX_LEN 30
void setup()  
{
  Serial.begin(9600);  
   SPI.begin();
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
    digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
    digitalWrite(NRSTPD, HIGH);
  myRFID.AddicoreRFID_Init();  
  lcd.begin(16,2);         

  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(500);
    lcd.setCursor(3,0);
    lcd.print ("Product of ");
    lcd.setCursor(2,1);
    lcd.print (" Arduino  Z ");
    lcd.noBacklight();
    delay(500);
    lcd.backlight();
  }
 delay (500);
 lcd.clear ();
 lcd.setCursor(2,0);
 lcd.print ("Scan Card");

}
void loop()   
{
    uchar i, tmp, checksum1;
  uchar status;
        uchar str[MAX_LEN];
        uchar RC_size;
        uchar blockAddr;  //Selection operation block address 0 to 63
        String mynum = "";
        str[1] = 0x4400;
  //Find tags, return tag type
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str); 
  if (status == MI_OK)
  {
            Serial.println("RFID tag detected");
          Serial.print(str[0],BIN);
            Serial.print(" , ");
          Serial.print(str[1],BIN);
            Serial.println(" ");
  }

  //Anti-collision, return tag serial number 4 bytes
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
            checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
            Serial.println("The tag's number is  : ");
          //Serial.print(2);
          Serial.print(str[0]);
            Serial.print(" , ");
          Serial.print(str[1],BIN);
            Serial.print(" , ");
          Serial.print(str[2],BIN);
            Serial.print(" , ");
          Serial.print(str[3],BIN);
            Serial.print(" , ");
          Serial.print(str[4],BIN);
            Serial.print(" , ");
            Serial.println(checksum1,BIN);
            
            // Should really check all pairs, but for now we'll just use the first
            if(str[0] == 56)                      //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
            {
                lcd.clear ();
                lcd.setCursor(0,0);
                lcd.print("Facebook :");
                delay (500);
                lcd.setCursor(0,1);
                lcd.print ("apple598"); 
            }  
            if(str[0] == 131) {             //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
                lcd.clear ();
                lcd.setCursor(0,0);
                lcd.print("Google :");
                delay (500);
                lcd.setCursor(0,1);
                lcd.print ("ilovegoogle598");   
            }

            if(str[0] == 84) {             //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
                lcd.clear ();
                lcd.setCursor(0,0);
                lcd.print("Ebay :");
                delay (500);
                lcd.setCursor(0,1);
                lcd.print ("naruto5987");      
            }
            
            Serial.println();
            delay(1000);
  }
    
        myRFID.AddicoreRFID_Halt();      //Command tag into hibernation              

}
