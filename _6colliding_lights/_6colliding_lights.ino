int first_led_pin = 22;

const int left_button_pin = 2;
const int right_button_pin = 3;

bool leftbutton_pushed = false;
bool rightbutton_pushed = false;

int leftmost_pointer;
int rightmost_pointer;

const int left_pointer_mover = 1;
const int right_pointer_mover = -1;

void setup() {
  // put your setup code here, to run once:
  for(int light_pin = first_led_pin; light_pin < first_led_pin + 10; light_pin++)
    pinMode(light_pin, OUTPUT);
  pinMode(left_button_pin, INPUT);
  pinMode(right_button_pin, INPUT);
  leftmost_pointer = first_led_pin;
  rightmost_pointer = first_led_pin + 9;
}

void loop() {
  // put your main code here, to run repeatedly:
  check_input(left_button_pin, leftbutton_pushed, leftmost_pointer, left_pointer_mover);
  check_input(right_button_pin, rightbutton_pushed, rightmost_pointer, right_pointer_mover);
}

void check_input(int button_pin, bool& button_pushed, int& light_pointer, int pointer_mover) {
  if(digitalRead(button_pin) == HIGH && !button_pushed) {
    button_pushed = true;
    light_up_next(light_pointer, pointer_mover);
  }
  else if(digitalRead(button_pin) == LOW && button_pushed) {
    button_pushed = false;
  }
}

void light_up_next(int& light_pointer, int pointer_mover) {
  if(light_pointer >= first_led_pin && light_pointer <= first_led_pin + 9)
    digitalWrite(light_pointer, HIGH);
  light_pointer += pointer_mover;
}