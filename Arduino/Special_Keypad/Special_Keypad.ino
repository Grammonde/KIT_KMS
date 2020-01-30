#include <Keypad.h>

char key_pressed;
char key_Enter;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'#','0','*',' '}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad keypad_pressed = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup(){
 Serial.begin(9600);
}
 
void loop(){
  key_pressed = keypad_pressed.getKey();
  if(key_pressed){
    Serial.println("Por Favor");
    back:
    Serial.print(key_pressed);
  }
  key_Enter = keypad_pressed.getKey();
  if(key_Enter == ' '){
    Serial.println("Entered");
    goto back;
  }
}
