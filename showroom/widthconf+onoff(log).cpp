#include <FastLED.h>
#include <math.h>

// Parametry taśmy LED
#define NUM_LEDS 50       // Całkowita liczba diod LED na taśmie
#define DATA_PIN_1 5      // Pin danych dla pierwszego segmentu
#define DATA_PIN_2 3      // Pin danych dla drugiego segmentu
#define DATA_PIN_3 9      // Pin danych dla trzeciego segmentu
#define BUTTON_PIN_12 12  // Pin do przełączania pierwszego efektu
#define BUTTON_PIN_13 13  // Pin do zmiany kierunku drugiego efektu

#define NUM_LEDS1 NUM_LEDS // Liczba diod LED dla pierwszego segmentu (pełna taśma)
#define NUM_LEDS2 NUM_LEDS // Liczba diod LED dla drugiego segmentu (2/3 taśmy)
#define NUM_LEDS3 NUM_LEDS // Liczba diod LED dla trzeciego segmentu (1/3 taśmy)

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

// Parametry pierwszego efektu
const int analogInPin1 = A0;
const int analogInPin2 = A1;
const int analogInPin3 = A2;
const int analogInPin4 = A3;
const int numReadings = 30;
int sensorReadings1[numReadings];
int sensorReadings2[numReadings];
int sensorReadings3[numReadings];
int sensorReadings4[numReadings];
int currentIndex = 0;

// Parametry drugiego efektu
uint8_t gHue = 0;
bool reachedEnd1 = false;
bool reachedEnd2 = false;
bool reachedEnd3 = false;
int currentStep1 = 0;
int currentStep2 = 0;
int currentStep3 = 0;
bool forwardDirection = false;
const int totalSteps = 100;

// Flagi i stany
bool buttonPressed12 = false;
bool lastButtonState12 = HIGH;
bool buttonPressed13 = false;
bool lastButtonState13 = HIGH;
bool firstEffectActive = false;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(leds1, NUM_LEDS1);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(leds2, NUM_LEDS2);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_3>(leds3, NUM_LEDS3);
  FastLED.setBrightness(50);

  fill_solid(leds1, NUM_LEDS1, CRGB::Black);
  fill_solid(leds2, NUM_LEDS2, CRGB::Black);
  fill_solid(leds3, NUM_LEDS3, CRGB::Black);
  FastLED.show();

  pinMode(BUTTON_PIN_12, INPUT_PULLUP);
  pinMode(BUTTON_PIN_13, INPUT_PULLUP);

  pinMode(analogInPin1, INPUT_PULLUP);
  pinMode(analogInPin2, INPUT_PULLUP);
  pinMode(analogInPin3, INPUT_PULLUP);
  pinMode(analogInPin4, INPUT_PULLUP);

  //analogReadResolution(8);
}

void loop() {
  // Odczyt stanów przycisków
  bool buttonState12 = digitalRead(BUTTON_PIN_12);
  bool buttonState13 = digitalRead(BUTTON_PIN_13);

  if (buttonState12 == LOW && lastButtonState12 == HIGH) {
    // Resetowanie flag i kroków
    reachedEnd1 = false;
    reachedEnd2 = false;
    reachedEnd3 = false;
    currentStep1 = 0;
    currentStep2 = 0;
    currentStep3 = 0;
    forwardDirection = false;
    
    gHue = 0;
    firstEffectActive = !firstEffectActive; // Przełączanie stanu efektu
  }
  lastButtonState12 = buttonState12;

  // Obsługa naciśnięcia przycisku 13
  if (buttonState13 == LOW && lastButtonState13 == HIGH) {
    if(isBriNotMax(leds1, NUM_LEDS1) || isBriNotMax(leds2, NUM_LEDS2) || isBriNotMax(leds3, NUM_LEDS3)){
      Serial.println("true");
      FastLED.setBrightness(255);
    }
    firstEffectActive = false;
    changeDirection();
  }
  lastButtonState13 = buttonState13;

  if (firstEffectActive) {
    runFirstEffect();
  } else {
    runSecondEffect();
  }

  FastLED.show();
}

void runFirstEffect() {
  int sensorValue2 = 252 - analogRead(analogInPin2); // jasność
  int sensorValue3 = 112 - analogRead(analogInPin3); // zasięg
  int sensorValue4 = 79 - analogRead(analogInPin4); // pozycja

  // Dodanie nowych odczytów do tablicy
  sensorReadings2[currentIndex] = sensorValue2;
  sensorReadings3[currentIndex] = sensorValue3;
  sensorReadings4[currentIndex] = sensorValue4;

  // Inkremetowanie currentIndex lub owijanie
  currentIndex = (currentIndex + 1) % numReadings;

  // Obliczanie średniej
  int total2 = 0, total3 = 0, total4 = 0;
  for (int i = 0; i < numReadings; i++) {
    total2 += sensorReadings2[i];
    total3 += sensorReadings3[i];
    total4 += sensorReadings4[i];
  }
  int mapped_analog_brightness = total2 / numReadings;
  int led_range = total3 / numReadings;
  int mappedValue = total4 / numReadings;

  if (mapped_analog_brightness < 0) mapped_analog_brightness = 0;
  FastLED.setBrightness(mapped_analog_brightness);

  // Zapewnienie minimalnej długości efektu to 1 LED
  led_range = max(led_range, 1);

  // Ograniczenie mappedValue do zakresu tablicy LED
  mappedValue = constrain(mappedValue, 0, NUM_LEDS - 1);

  // Zapewnienie, że led_range nie powoduje przekroczenia liczby diod LED
  int startIndex = constrain(mappedValue - led_range, 0, NUM_LEDS - 1);
  int endIndex = constrain(mappedValue + led_range, 0, NUM_LEDS - 1);

  // Dostosowanie startIndex i endIndex, aby zapewnić aktywność co najmniej 1 LED
  if (endIndex - startIndex < 1) {
    if (startIndex > 0) {
      endIndex = startIndex + 1;
    } else {
      startIndex = endIndex - 1;
    }
  }

  // Wygaszenie wszystkich LED przed zastosowaniem nowego efektu
  for (int i = 0; i < NUM_LEDS; i++) {
    leds1[i].fadeToBlackBy(5); 
    leds2[i].fadeToBlackBy(5); 
    leds3[i].fadeToBlackBy(5);  
  }

  // Ustawienie zakresu LED na biały
  for (int i = startIndex; i <= endIndex; i++) {
    leds1[i] = CRGB::White;
    leds2[i] = CRGB::White;
    leds3[i] = CRGB::White;
  }
}

void runSecondEffect() {
  if (forwardDirection) {
    smoothLightUpForward(leds1, NUM_LEDS1, currentStep1, reachedEnd1);
    smoothLightUpForward(leds2, NUM_LEDS2, currentStep2, reachedEnd2);
    smoothLightUpForward(leds3, NUM_LEDS3, currentStep3, reachedEnd3);
  } else {
    smoothLightUpReverse(leds1, NUM_LEDS1, currentStep1, reachedEnd1);
    smoothLightUpReverse(leds2, NUM_LEDS2, currentStep2, reachedEnd2);
    smoothLightUpReverse(leds3, NUM_LEDS3, currentStep3, reachedEnd3);
  }
}

void changeDirection() {
  reachedEnd1 = false;
  reachedEnd2 = false;
  reachedEnd3 = false;

  gHue = 0;
  forwardDirection = !forwardDirection;
}

void smoothLightUpForward(CRGB* leds, int numLeds, int &currentStep, bool &reachedEnd) {
  fadeToBlackBy(leds, numLeds, 20);

  if (!reachedEnd) {
    currentStep++;
    if (currentStep >= totalSteps - 1) {
      reachedEnd = true;
    }
  }

  int currentPos = logMapping(currentStep, totalSteps, numLeds);

  for (int i = 0; i <= currentPos; i++) {
    leds[i] += CRGB::White;
  }

  gHue++;
}

void smoothLightUpReverse(CRGB* leds, int numLeds, int &currentStep, bool &reachedEnd) {
  fadeToBlackBy(leds, numLeds, 20);

  if (!reachedEnd) {
    currentStep--;
    if (currentStep <= 0) {
      reachedEnd = true;
    }
  }

  int currentPos = logMapping(currentStep, totalSteps, numLeds);

  for (int i = 0; i <= currentPos; i++) {
    leds[i] += CRGB::White;
  }

  gHue++;
}

int logMapping(int step, int totalSteps, int numLeds) {
  float logMin = log(2);
  float logMax = log(totalSteps);
  float scaledStep = pow((float)step / totalSteps, 2) * totalSteps;
  float scale = (log(scaledStep + 1) - logMin) / (logMax - logMin);
  return (int)(scale * numLeds);
}

bool isBriNotMax(CRGB* leds, int numLeds) {
  for (int i = 0; i < numLeds; i++) {
    if (leds[i].r < 255 || leds[i].g < 255 || leds[i].b < 255) {
      return true;
    }
  }
  return false;
}
