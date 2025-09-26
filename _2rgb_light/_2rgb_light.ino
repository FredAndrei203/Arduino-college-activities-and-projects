/*A project in which we learned how to set the color of an RGB LED.*/
int redPin = 5;
int greenPin = 6;
int bluePin = 7;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  setColor(255, 0, 0);
  delay(1000);
  setColor(0, 255, 0);  
  delay(1000);
  setColor(0, 0, 255);
  delay(1000);
  setColor(255, 255, 255);
  delay(1000);
  setColor(170, 0, 255);
  delay(1000);
  setColor(127, 127, 127);
  delay(1000);
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

