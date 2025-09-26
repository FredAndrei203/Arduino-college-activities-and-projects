//A project in which potentiometers are used to control the values of an RGB LED.

int redPin = 5;
int greenPin = 6;
int bluePin = 7;
int maxValue = 680;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue_red = analogRead(A0);
  int sensorValue_blue = analogRead(A1);
  int sensorValue_green = analogRead(A2);

  //Values for setColor
  sensorValue_red = map(sensorValue_red, 0, maxValue, 0, 255);
  sensorValue_blue = map(sensorValue_blue, 0, maxValue, 0, 255);
  sensorValue_green = map(sensorValue_green, 0, maxValue, 0, 255);
  setColor(sensorValue_red, sensorValue_green, sensorValue_blue);

  Serial.print("R: ");
  Serial.print(sensorValue_red);
  Serial.print(" G: ");
  Serial.print(sensorValue_green);
  Serial.print(" B: ");
  Serial.println(sensorValue_blue);
  delay(50);
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}