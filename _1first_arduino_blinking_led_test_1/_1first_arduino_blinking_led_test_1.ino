/*A project in which we created christmas lights using 10 LEDs. The running lights and colliding lights function look like so since it has been in a year since
  I have touched C++.*/
// the setup function runs once when you press reset or power the board
void setup() {
  for(int idx = 4; idx < 14; idx++) {
    pinMode(idx, OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  running_lights();
  for(int idx = 0; idx < 3; idx++) {
    colliding_lights();
  }
  left_to_right_to_left();
  for(int idx = 0; idx < 6; idx++) {
    alternating_pair();
  }
  delay(100);
  for(int idx = 4; idx < 14; idx++) {
    digitalWrite(idx, LOW);
  }
}

void running_lights() {
  digitalWrite(4, LOW);
  digitalWrite(13, HIGH);
  delay(100); 
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  delay(100);
  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  delay(100);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  delay(100);
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(5, HIGH);
  delay(100);
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);
  delay(100);
  digitalWrite(4, LOW);
  delay(100);
}

void colliding_lights() {
  digitalWrite(13, HIGH);
  digitalWrite(4, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  digitalWrite(4, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(5, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  digitalWrite(5, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(11, LOW);
  digitalWrite(6, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(10, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(100);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
}

void left_to_right_to_left() {
  for (int i = 4; i < 14; i++) {
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }
  for (int i = 13; i >= 4; i--) {
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }
}

void alternating_pair() {
  for (int i = 4; i < 14; i += 2) {
    digitalWrite(i, HIGH);
    digitalWrite(i + 1, LOW);
  }
  delay(300);
  for (int i = 4; i < 14; i += 2) {
    digitalWrite(i, LOW);
    digitalWrite(i + 1, HIGH);
  }
  delay(300);
}

