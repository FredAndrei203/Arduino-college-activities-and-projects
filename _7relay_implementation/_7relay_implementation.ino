#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {30, 32, 34, 36}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

String username = "ABCD";
String password = "1234";

enum Entering {USERNAME, PASSWORD, DONE};

int current_state = USERNAME;
int mistake_counter = 0;

String current_input;

int relay1 = 42;
int relay2 = 44;
int relay3 = 46;
int relay4 = 48;

void setup()
{
  Serial.begin(9600);
  Serial.println(mistake_counter);
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  for(int pin = 42; pin <= 48; pin += 2) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  prompt_username();
}

void loop() {
  char customKey = customKeypad.getKey();
  
  if(customKey) {
    process_input(customKey);
  }
}

void process_input(char customKey) {
  switch(customKey) {
    case '*':
      lcd.clear();
      current_input = "";
      break;
    case '#':
      check_input();
      break;
    default: 
      current_input += customKey;
      Serial.println(current_input);
      lcd.print(customKey);
  }
}

void prompt_username() {
  current_input = "";
  current_state = USERNAME;
  lcd.setCursor(0, 0);
  lcd.print("Username");
  lcd.setCursor(0, 1);
}

void prompt_password() {
  current_input = "";
  current_state = PASSWORD;
  lcd.setCursor(0, 0);
  lcd.print("Password");
  lcd.setCursor(0, 1);
}

void check_input() {
  switch(current_state) {
    case USERNAME:
      check_username();
      break;
    case PASSWORD:
      check_password();
      break;
  }
}

void check_username() {
  if(current_input == username) {
    lcd.clear();
    prompt_password();
  }
  else {
    mistake_counter++;
    output_mistake();
    if(mistake_counter >= 3) {
      unathorized();
      current_state = DONE;
      return;
    }
    prompt_username();
  }
}

void check_password() {
  if(current_input == password) {
    output_correct();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME");
    digitalWrite(relay1, LOW);
    turn_off_all_warnings();
  }
  else {
    mistake_counter++;
    output_mistake();
    if(mistake_counter >= 3) {
      unathorized();
      current_state = DONE;
      return;
    }
    prompt_password();
  }
}


void output_correct() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CORRECT");
  digitalWrite(relay1, LOW);
  delay(1000);
  lcd.clear();
}

void output_mistake() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WRONG");
  switch(mistake_counter) {
    case 1:
      digitalWrite(relay2, LOW);
      delay(1000);
      break;
    case 2:
      digitalWrite(relay3, LOW);
      delay(1000);
      break;
    case 3:
      digitalWrite(relay4, LOW);
  }
}

void unathorized() {
  lcd.setCursor(0, 0);
  lcd.print("UNATHORIZED");
}

void turn_off_all_warnings() {
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
}