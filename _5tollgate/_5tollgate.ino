#include <Wire.h>               // Required for I2C communication
#include <LiquidCrystal_I2C.h>  // LCD library
#include <Servo.h>              // Servo library
#include <Keypad.h>



enum state {
  NO_VEHICLE,
  HAS_VEHICLE_UNPAYED,
  HAS_VEHICLE_PAYED,
  VEHICLE_LEAVING
};

int toll_gate_state = NO_VEHICLE;

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 22, 24, 26, 28 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 30, 32, 34, 36 };  //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

bool key_pad_enabled = false;

int fee = 50;

// === Initialize the LCD ===
// Common I2C addresses: 0x27 or 0x3F — try 0x27 first
LiquidCrystal_I2C lcd(0x27, 16, 2);

// === Pin Definitions ===
#define trigPin 6
#define echoPin 5
#define greenLed 11
#define redLed 10
#define servoPin 2

// === Objects ===
Servo ServoniRay;

// === Variables ===
long duration;
int distance;
          // track gate state
unsigned long lastOpenTime = 0;  // track last time gate was open

String current_input;

void setup() {
  Serial.begin(9600);

  // === LCD Setup ===
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();

  // === Welcome Message ===
  lcd.setCursor(0, 0);
  lcd.print("Toll gate");
  lcd.setCursor(0, 1);
  lcd.print("NOW ACTIVATED");
  delay(1000);
  lcd.clear();

  // === Pin Modes ===
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  // === Servo Setup ===
  ServoniRay.attach(servoPin);

  // === Default States ===
  change_state(NO_VEHICLE);

  // // === LCD Labels ===
  // lcd.setCursor(0, 0);
  // lcd.print("Distance: ");
  // lcd.setCursor(0, 1);
  // lcd.print("Gate: CLOSED");
}

void loop() {
  // === Ultrasonic Pulse ===
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // === Measure Echo ===
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;  // Convert to cm

  // // === Serial Monitor Output ===
  // if (distance >= 200 || distance <= 0) {
  //   Serial.println("Out of range");
  // } else {
  //   Serial.print(distance);
  //   Serial.println(" cm");
  // }

  // // === Update LCD (no flicker) ===
  // lcd.setCursor(0, 0);
  // if (distance >= 200 || distance <= 0) {
  //   lcd.print("Out   ");
  // } else {
  //   lcd.print(distance);
  //   lcd.print("cm   ");
  // }

  // === Gate and LED Logic ===
  if (distance < 10 && toll_gate_state == NO_VEHICLE) {
    change_state(HAS_VEHICLE_UNPAYED);
  }
  else if (distance > 10 && toll_gate_state == VEHICLE_LEAVING) {
    delay(1500);
    change_state(NO_VEHICLE);
  }
  Serial.println(distance);
  Serial.print("STATE: ");
  Serial.println(toll_gate_state);
  char key_input = customKeypad.getKey();
  if(key_pad_enabled && key_input) {
    process_input(key_input);
  }

  delay(300);
}

void prompt_driver_pay() {
  current_input = "";
  toll_gate_state = HAS_VEHICLE_UNPAYED;
  toll_message_pay_up();
  key_pad_enabled = true;
}

void change_state(state next_state) {
  switch(next_state) {
    case NO_VEHICLE:
      toll_gate_state = NO_VEHICLE;
      change_gate_state(false);
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("GATE AVAILABLE");
      ServoniRay.write(90);
      break;
    case HAS_VEHICLE_UNPAYED:
      toll_gate_state = HAS_VEHICLE_UNPAYED;
      prompt_driver_pay();
      break;
    case HAS_VEHICLE_PAYED:
      toll_gate_state = HAS_VEHICLE_PAYED;
      check_payment();
      break;
    case VEHICLE_LEAVING:
      toll_gate_state = VEHICLE_LEAVING;
      ServoniRay.write(0);
      key_pad_enabled = false;
      current_input = "";
  }
}

void process_input(char customKey) {
  switch(customKey) {
    case '*':
      toll_message_pay_up();
      current_input = "";
      break;
    case '#':
      change_state(HAS_VEHICLE_PAYED);
      break;
    default: 
      current_input += customKey;
      Serial.println(current_input);
      lcd.print(customKey);
  }
}

void toll_message_pay_up() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Toll fee: 50");
  lcd.setCursor(0, 1);
}

void check_payment() {
  int current_payment = current_input.toInt();
  if(current_payment < fee) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("INSUFFICIENT");
    delay(1000);
    change_state(HAS_VEHICLE_UNPAYED);
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RECIEVED: ");
    lcd.print(current_input);

    int change = current_payment - fee;

    lcd.setCursor(0, 1);
    lcd.print("CHANGE: ");
    lcd.print(change);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    delay(1000);
    lcd.clear();
    lcd.print("PLEASE PROCEED");
    change_state(VEHICLE_LEAVING);
  }
}


void change_gate_state(bool state) {
  if(state) {
    ServoniRay.write(0);
  }
  else {
    ServoniRay.write(90);
  }
}
