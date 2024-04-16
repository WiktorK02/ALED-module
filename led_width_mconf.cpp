#include <FastLED.h>

#define LED_PIN                 2
#define NUM_LEDS                51
#define BRIGHTNESS              255
#define ANALOG_PIN              A0
#define ANALOG_RANGE_PIN        A1
#define ANALOG_BRIGHTNESS_PIN   A2
#define ANALOG_COLOR_PIN        A3  

CRGB leds[NUM_LEDS];
int led_range = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(ANALOG_RANGE_PIN, INPUT);
  pinMode(ANALOG_BRIGHTNESS_PIN, INPUT);
  pinMode(ANALOG_COLOR_PIN, INPUT);  
}

void loop() {
  int analogValue = analogRead(ANALOG_PIN);
  int mappedValue = map(analogValue, 532, 666, 0, NUM_LEDS - 1);

  int rangeValue = analogRead(ANALOG_RANGE_PIN);
  led_range = map(rangeValue, 640, 666, 0, 10);
  mappedValue = constrain(mappedValue, led_range, NUM_LEDS - 1 - led_range);

  int analog_brightness = analogRead(ANALOG_BRIGHTNESS_PIN);
  int mapped_analog_brightness = map(analog_brightness, 0, 666, 255, 0);
  if (mapped_analog_brightness < 0) mapped_analog_brightness = 0;
  FastLED.setBrightness(mapped_analog_brightness);

  int hueValue = analogRead(ANALOG_COLOR_PIN);  
  hueValue = map(hueValue, 0, 666, 0, 255);

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

  FastLED.show();
}
