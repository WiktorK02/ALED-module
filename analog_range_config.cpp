const int analogInPin1 = 9;
const int analogInPin2 = 8;          
const int analogInPin3 = 7;
const int analogInPin4 = 2;

void setup() {
  Serial.begin(115200);
  pinMode(analogInPin1, INPUT_PULLUP);
  pinMode(analogInPin2, INPUT_PULLUP);
  pinMode(analogInPin3, INPUT_PULLUP);
  pinMode(analogInPin4, INPUT_PULLUP);
}

void loop() {

  int sensorValue1 = analogRead(analogInPin1);
  int sensorValue2 = analogRead(analogInPin2);
  int sensorValue3 = analogRead(analogInPin3);
  int sensorValue4 = analogRead(analogInPin4);

  Serial.print(sensorValue1);
  Serial.print(" ");
  Serial.print(sensorValue2);
  Serial.print(" ");
  Serial.print(sensorValue3);
  Serial.print(" ");
  Serial.print(sensorValue4);
  Serial.println(" ");
}
