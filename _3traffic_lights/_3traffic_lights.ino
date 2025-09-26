/*A project in which we created 2 traffic lights that are synchronized. Both have a button that works only when the light is red, and the 10-second counter is still below 6
  seconds. When pushed, the counter is increased by 5 seconds. It can only be pressed once per instance of red light.*/

enum LightState {
  RED_LIGHT,
  YELLOW_LIGHT,
  GREEN_LIGHT
};

struct TrafficLight {
  int red_pin, yellow_pin, green_pin, button_pin;
  LightState state;
  bool button_pushed;

  void _turn_of_all_lights() {
    digitalWrite(red_pin, LOW);
    digitalWrite(yellow_pin, LOW);
    digitalWrite(green_pin, LOW);
  }

  void light_up_green() {
    state = GREEN_LIGHT;
    _turn_of_all_lights();
    digitalWrite(green_pin, HIGH);
  }

  void light_up_yellow() {
    state = YELLOW_LIGHT;
    _turn_of_all_lights();
    digitalWrite(yellow_pin, HIGH);
  }

  void light_up_red() {
    state = RED_LIGHT;
    _turn_of_all_lights();
    digitalWrite(red_pin, HIGH);
  }
};

TrafficLight t_light1 = {4, 3, 2, 6, GREEN_LIGHT, false};
TrafficLight t_light2 = {10, 9, 8, 5, RED_LIGHT, false};

TrafficLight* currenty_red = &t_light2;

long processed_timer = 1;
long time_modifier = 0;
long counter = 1;
const long CYCLE_LENGTH = 10;
const long OFFSET = 1;

bool activated = false;

const long GREEN_RED_START = 1;
const long YELLOW_START = 9;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  int output_pins[] = {t_light1.red_pin, t_light1.yellow_pin, t_light1.green_pin, t_light2.red_pin, t_light2.yellow_pin, t_light2.green_pin};
  for(int idx = 0; idx < 6; idx++) {
    pinMode(output_pins[idx], OUTPUT);
  }
  pinMode(t_light1.button_pin, INPUT);
  pinMode(t_light2.button_pin, INPUT);
  initialize_states();
  activated = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(activated) run_timer();
  if(activated && time_has_advanced()) {
    Serial.println(processed_timer);
    check_state_switching_of(t_light1);
    check_state_switching_of(t_light2);
  }
  check_input();
}

void initialize_states() {
  t_light1.light_up_green();
  t_light2.light_up_red();
}

void run_timer() {
  processed_timer = ((millis() / 1000) + time_modifier) % CYCLE_LENGTH + OFFSET;
}

bool time_has_advanced() {
  bool needs_to = false;
  if(processed_timer != counter) {
    counter = processed_timer;
    needs_to = true;
  }
  return needs_to;
}

void check_state_switching_of(TrafficLight& traffic_light) {
    switch(traffic_light.state) {
    case RED_LIGHT:
      if(processed_timer == GREEN_RED_START) {
        traffic_light.light_up_green();
        traffic_light.button_pushed = false;
      }
      break;
    case GREEN_LIGHT:
      if(processed_timer >= YELLOW_START) {
        traffic_light.light_up_yellow();
      }
      break;
    case YELLOW_LIGHT:
      if(processed_timer == GREEN_RED_START) {
        traffic_light.light_up_red();
        currenty_red = &traffic_light;
      }
      break;
  }
}

void check_input() {
  if (digitalRead(currenty_red -> button_pin) == HIGH) {
    button_pushed(currenty_red -> button_pushed);
  }
}


void button_pushed(bool& button_pushed) {
  if(button_pushed == true || processed_timer > 5) {
    return;
  }
  time_modifier += 5;
  button_pushed = true;
}