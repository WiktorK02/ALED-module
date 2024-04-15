#include <FastLED.h>

#define LED_PIN           2
#define NUM_LEDS          51
#define BRIGHTNESS        255
#define ANALOG_PIN        A0
#define ANALOG_RANGE_PIN  A1

CRGB leds[NUM_LEDS];
int led_range = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(ANALOG_RANGE_PIN, INPUT);
}

void loop() {
  int analogValue = analogRead(ANALOG_PIN);
  int mappedValue = map(analogValue, 532, 666, 0, NUM_LEDS - 1);

  int rangeValue = analogRead(ANALOG_RANGE_PIN);
  led_range = map(rangeValue, 640, 666, 0, 10);
  mappedValue = constrain(mappedValue, led_range, NUM_LEDS - 1 - led_range);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(5);
  }

  for (int i = mappedValue - led_range; i <= mappedValue + led_range; i++) {
    int ledIndex = constrain(i, 0, NUM_LEDS - 1);
    leds[ledIndex].green = min(leds[ledIndex].green + 5, 255); 
  FastLED.show();
}
