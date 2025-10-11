//Synchronized traffic lights (timer = 10 seconds)(counts up).
//If a traffic light's button is pushed when it's state is red, the timer increments by 5.
const long TIMER_LENGTH = 12;
const long OFFSET = 1;
const long NON_YELLOW_STATE_START = 1;
const long YELLOW_STATE_START = 10;

long synchronized_counter_n = 1;
long time_modifier = 0;
long time_at_n_minus_1 = 1;

bool activated = false;

enum TrafficLightState {
  RED_STATE,
  YELLOW_STATE,
  GREEN_STATE
};

struct TrafficLight {
  int red_out, yellow_out, green_out, input_pin;
  bool input_already_recieved;
  TrafficLightState tl_state;

  void deactivate_lights() {
    digitalWrite(red_out, LOW);
    digitalWrite(yellow_out, LOW);
    digitalWrite(green_out, LOW);
  }

  void to_red_state() {
    deactivate_lights();
    digitalWrite(red_out, HIGH);
    tl_state = RED_STATE;
  }

  void to_yellow_state() {
    deactivate_lights();
    digitalWrite(yellow_out, HIGH);
    tl_state = YELLOW_STATE;
  }

  void to_green_state() {
    deactivate_lights();
    digitalWrite(green_out, HIGH);
    tl_state = GREEN_STATE;
  }
};

TrafficLight traffic_light1 = { 4, 3, 2, 6, false, GREEN_STATE };
TrafficLight traffic_light2 = { 10, 9, 8, 5, false, RED_STATE };

TrafficLight* currenty_red = &traffic_light2;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  int output_pins[] = { traffic_light1.red_out, traffic_light1.yellow_out, traffic_light1.green_out, traffic_light2.red_out, traffic_light2.yellow_out, traffic_light2.green_out };
  for (int idx = 0; idx < 6; idx++) {
    pinMode(output_pins[idx], OUTPUT);
  }
  pinMode(traffic_light1.input_pin, INPUT);
  pinMode(traffic_light2.input_pin, INPUT);
  set_initial_states_of_traffic_lights();
  activated = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (activated)
    continue_timer();
  if (activated && has_time_changed()) {
    Serial.println(synchronized_counter_n);
    manage_state_machine_of(traffic_light1);
    manage_state_machine_of(traffic_light2);
  }
  read_input();
}

void set_initial_states_of_traffic_lights() {
  traffic_light1.to_green_state();
  traffic_light2.to_red_state();
}

void continue_timer() {
  synchronized_counter_n = ((millis() / 1000) + time_modifier) % TIMER_LENGTH + OFFSET;
}

bool has_time_changed() {
  bool it_has = false;
  if (synchronized_counter_n != time_at_n_minus_1) {
    time_at_n_minus_1 = synchronized_counter_n;
    it_has = true;
  }
  return it_has;
}

void manage_state_machine_of(TrafficLight& traffic_light) {
  switch (traffic_light.tl_state) {
    case RED_STATE:
      if (synchronized_counter_n == NON_YELLOW_STATE_START) {
        traffic_light.to_green_state();
        traffic_light.input_already_recieved = false;
      }
      break;
    case GREEN_STATE:
      if (synchronized_counter_n >= YELLOW_STATE_START) {
        traffic_light.to_yellow_state();
      }
      break;
    case YELLOW_STATE:
      if (synchronized_counter_n == NON_YELLOW_STATE_START) {
        traffic_light.to_red_state();
        currenty_red = &traffic_light;
      }
      break;
  }
}

void read_input() {
  if (digitalRead(currenty_red->input_pin) == HIGH) {
    process_input(currenty_red->input_already_recieved);
  }
}


void process_input(bool& input_already_recieved) {
  if (input_already_recieved == true || synchronized_counter_n > 5) {
    return;
  }
  time_modifier += 5;
  input_already_recieved = true;
}