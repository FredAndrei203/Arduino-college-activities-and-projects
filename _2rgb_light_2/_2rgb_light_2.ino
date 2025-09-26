/*A project in which we learn how to use Serial. We also learned how to read analog signals with a potentiometer.*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(1);
}
