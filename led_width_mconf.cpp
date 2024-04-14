#include <FastLED.h>

#define LED_PIN 5
#define DATA_PIN LED_PIN
#define NUM_LEDS 30

#define ANALOG_PIN_WIDTH 1
#define ANALOG_PIN_POSITION 2

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void loop() {
  int widthValue = analogRead(ANALOG_PIN_WIDTH);
  int positionValue = analogRead(ANALOG_PIN_POSITION);

  float widthNormalized = (float)widthValue / (float)analogReadResolution * 255;
  float positionNormalized = (float)positionValue / (float)analogReadResolution * 255;

  int startIndex = (int)(positionNormalized * (NUM_LEDS - widthNormalized));
  int endIndex = startIndex + widthNormalized;

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < startIndex || i >= endIndex) {
      leds[i] = leds[i] * 0.95;
    }
  }

  for (int i = startIndex; i < endIndex; i++) {
    leds[i] = CRGB::ColorFromHex16(0xFFFF00);
  }

  FastLED.show();
}
