#include <FastLED.h>

#define LED_PIN 1 // Numer pinu GPIO podłączonego do LED
#define LED_COUNT 60 // Liczba diod LED w taśmie
#define BUTTON_PIN 2 // Numer pinu GPIO podłączonego do przycisku
#define ANALOG_PIN 3 // Numer pinu analogowego do sterowania jasnością
#define DIGITAL_PIN 4 // Numer pinu cyfrowego do zmiany koloru

CRGB leds[LED_COUNT]; 
int buttonState = 0; 
int analogValue = 0; 
int lastDigitalValue = 0; 

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  analogValue = analogRead(ANALOG_PIN); 
  int currentDigitalValue = digitalRead(DIGITAL_PIN);

  // Animacja włączania/wyłączania, sterowanie jasnością i zmiana koloru
  if (buttonState == HIGH) {
    for (int i = 0; i < LED_COUNT; i++) {
      leds[i] = CHSV(i * (256 / LED_COUNT), 255, analogValue / 255); // Ustawianie koloru i jasności
      FastLED.show();
      delay(10);
    }
  } else {
    // Wyłączanie diod LED
    for (int i = LED_COUNT - 1; i >= 0; i--) {
      leds[i] = CHSV(0, 0, 0); // Czarny kolor
      FastLED.show();
      delay(10);
    }
  }

  // Zmiana koloru przy zmianie wartości cyfrowej
  if (currentDigitalValue != lastDigitalValue) {
    if (currentDigitalValue == HIGH) {
      // Ustaw kolor na czerwony
      for (int i = 0; i < LED_COUNT; i++) {
        leds[i] = CRGB(255, 0, 0);
      }
    } else {
      // Ustaw kolor na zielony
      for (int i = 0; i < LED_COUNT; i++) {
        leds[i] = CRGB(0, 255, 0);
      }
    }
    FastLED.show();
    lastDigitalValue = currentDigitalValue;
  }
}
