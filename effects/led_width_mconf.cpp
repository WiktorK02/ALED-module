
#include <FastLED.h>

#define LED_PIN                 1
#define NUM_LEDS                51
#define BRIGHTNESS              255

CRGB leds[NUM_LEDS];
int led_range = 0;
const int analogInPin1 = 9;
const int analogInPin2 = 8;
const int analogInPin3 = 7;
const int analogInPin4 = 2;

const int numReadings = 30;  // Number of readings to average
int sensorReadings1[numReadings];
int sensorReadings2[numReadings];
int sensorReadings3[numReadings];
int sensorReadings4[numReadings];
int currentIndex = 0;  // Renamed from 'index'
void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  analogReadResolution(8);

  pinMode(analogInPin1, INPUT_PULLUP);
  pinMode(analogInPin2, INPUT_PULLUP);
  pinMode(analogInPin3, INPUT_PULLUP);
  pinMode(analogInPin4, INPUT_PULLUP);
}

void loop() {
  int sensorValue1 = 255 - analogRead(analogInPin1); //color
  int sensorValue2 = 252 - analogRead(analogInPin2); //brightness
  int sensorValue3 = 112 - analogRead(analogInPin3); //range
  int sensorValue4 = 79 - analogRead(analogInPin4); //position

  // Add the new readings to the array
  sensorReadings1[currentIndex] = sensorValue1;
  sensorReadings2[currentIndex] = sensorValue2;
  sensorReadings3[currentIndex] = sensorValue3;
  sensorReadings4[currentIndex] = sensorValue4;

  // Increment the currentIndex or wrap around
  currentIndex = (currentIndex + 1) % numReadings;

  // Calculate the average
  int total1 = 0, total2 = 0, total3 = 0, total4 = 0;
  for (int i = 0; i < numReadings; i++) {
    total1 += sensorReadings1[i];
    total2 += sensorReadings2[i];
    total3 += sensorReadings3[i];
    total4 += sensorReadings4[i];
  }
  int hueValue = total1 / numReadings;
  int mapped_analog_brightness = total2 / numReadings;
  int led_range = total3 / numReadings;
  int mappedValue = total4 / numReadings;
  if (mapped_analog_brightness < 0) mapped_analog_brightness = 0;
  FastLED.setBrightness(mapped_analog_brightness);
  int red = 0;
  int green = 0;
  int blue = 0;

  if (hueValue < 85) {
    red = hueValue * 3;
    green = 255 - hueValue * 3;
    blue = 0;
  } else if (hueValue < 170) {
    hueValue -= 85;
    red = 255 - hueValue * 3;
    green = 0;
    blue = hueValue * 3;
  } else {
    hueValue -= 170;
    red = 0;
    green = hueValue * 3;
    blue = 255 - hueValue * 3;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(5);  
  }

  for (int i = mappedValue - led_range; i <= mappedValue + led_range; i++) {
    int ledIndex = constrain(i, 0, NUM_LEDS - 1);
    int brightnessIncrement = 5;

    leds[ledIndex].red = min(leds[ledIndex].red + brightnessIncrement, red);
    leds[ledIndex].green = min(leds[ledIndex].green + brightnessIncrement, green);
    leds[ledIndex].blue = min(leds[ledIndex].blue + brightnessIncrement, blue);
  }
  Serial.println(sensorValue1);
  FastLED.delay(1);
  FastLED.show();
}
