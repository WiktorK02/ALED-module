const int analogInPin = A0;
const float smoothingFactor = 0.5;             

void setup() {
  Serial.begin(9600);
  pinMode(analogInPin, INPUT);
}

void loop() {
  int sensorValue = analogRead(analogInPin);
  int outputValue = SmoothAnalogValue(sensorValue, smoothingFactor);

  Serial.print("Smoothed Value: ");
  Serial.println(outputValue);
}

int SmoothAnalogValue(int sensorValue, float smoothingFactor) {
  static int filteredValue = 0;  
  filteredValue = (smoothingFactor * sensorValue) + ((1 - smoothingFactor) * filteredValue);
  int outputValue = map(filteredValue, 4, 684, 255, 0);
  if (outputValue < 0) outputValue = 0;
  
  return outputValue;
}
