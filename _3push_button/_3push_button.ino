/*A project in which:
  -On button press: LED1 lights up.
  -On button release: LED1 shuts off and LED2 toggles (lights up or shuts off).*/

int pushButton = 2;
int led1 = 3;
int led2 = 4;


int pushed = LOW;
bool led2_activated = false;
bool was_pushed = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pushButton, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  pushed = digitalRead(pushButton);
  activate_led1();
  activate_led2();
}

void serial_read_input() {
  int buttonState = digitalRead(pushButton);
  Serial.println(buttonState);
  delay(100);
}

void activate_led1() {
  if(pushed == HIGH) {
    digitalWrite(led1, HIGH);
    was_pushed = true;
  }
  else {
    digitalWrite(led1, LOW);
  }
}

void activate_led2() {
  if(pushed == HIGH) {
    return;
  }

  if(pushed == LOW && was_pushed) {
    led2_activated = !led2_activated;
    if(led2_activated) {
      digitalWrite(led2, HIGH);
    }
    else {
      digitalWrite(led2, LOW);
    }
    was_pushed = false;
  }
}