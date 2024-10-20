// liniowa funkcja 

#include <FastLED.h>

#define NUM_LEDS 32     // Total number of LEDs in the strip
#define DATA_PIN_1 5    // Data pin for the first segment
#define DATA_PIN_2 3    // Data pin for the second segment
#define DATA_PIN_3 9    // Data pin for the third segment
#define BUTTON_PIN 13   // Pin where the button is connected

#define NUM_LEDS1 NUM_LEDS   // Number of LEDs for the first segment (full strip)
#define NUM_LEDS2 (NUM_LEDS * 4 / 5 + 1)  // Number of LEDs for the second segment (2/3 of strip)
#define NUM_LEDS3 (NUM_LEDS * 3 / 5 + 6)  // Number of LEDs for the third segment (1/3 of strip)

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

uint8_t gHue = 0;       // Global hue value for color shifting
bool reachedEnd1 = false; // Track if the end has been reached for segment 1
bool reachedEnd2 = false; // Track if the end has been reached for segment 2
bool reachedEnd3 = false; // Track if the end has been reached for segment 3
int currentPos1 = 0;      // Current position for segment 1
int currentPos2 = 0;      // Current position for segment 2
int currentPos3 = 0;      // Current position for segment 3

bool buttonPressed = false; // Flag to track if the button is pressed
bool lastButtonState = HIGH; // Last state of the button

bool forwardDirection = true; // Flag to track current animation direction

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(leds1, NUM_LEDS1);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(leds2, NUM_LEDS2);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_3>(leds3, NUM_LEDS3);
  
  FastLED.setBrightness(50); // Set the brightness (0-255)

  // Initialize LEDs to off
  fill_solid(leds1, NUM_LEDS1, CRGB::Black);
  fill_solid(leds2, NUM_LEDS2, CRGB::Black);
  fill_solid(leds3, NUM_LEDS3, CRGB::Black);
  
  FastLED.show();

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);
  buttonPressed = (buttonState == LOW && lastButtonState == HIGH);

  if (buttonPressed) {
    changeDirection();
  }

  if (forwardDirection) {
    smoothLightUpForward(leds1, NUM_LEDS1, currentPos1, reachedEnd1);
    smoothLightUpForward(leds2, NUM_LEDS2, currentPos2, reachedEnd2);
    smoothLightUpForward(leds3, NUM_LEDS3, currentPos3, reachedEnd3);
  } else {
    smoothLightUpReverse(leds1, NUM_LEDS1, currentPos1, reachedEnd1);
    smoothLightUpReverse(leds2, NUM_LEDS2, currentPos2, reachedEnd2);
    smoothLightUpReverse(leds3, NUM_LEDS3, currentPos3, reachedEnd3);
  }

  FastLED.show();
  FastLED.delay(20);

  lastButtonState = buttonState;
}

void changeDirection() {
  reachedEnd1 = false;
  reachedEnd2 = false;
  reachedEnd3 = false;

  gHue = 0; // Reset hue for color shifting
  forwardDirection = !forwardDirection; // Toggle direction flag

  // Ensure starting positions are set correctly
  if (forwardDirection) {
    currentPos1 = 0;
    currentPos2 = 0;
    currentPos3 = 0;
  } else {
    currentPos1 = NUM_LEDS1 - 1;
    currentPos2 = NUM_LEDS2 - 1;
    currentPos3 = NUM_LEDS3 - 1;
  }
}

void smoothLightUpForward(CRGB* leds, int numLeds, int &currentPos, bool &reachedEnd) {
  fadeToBlackBy(leds, numLeds, 20);  // Fade the LEDs slightly
  
  if (!reachedEnd) {
    currentPos++;
    if (currentPos >= numLeds - 1) {
      reachedEnd = true;
    }
  }

  for (int i = 0; i <= currentPos; i++) {
    leds[i] += CHSV(gHue, 255, 192); // Set the color and brightness
  }

  gHue++;
}

void smoothLightUpReverse(CRGB* leds, int numLeds, int &currentPos, bool &reachedEnd) {
  fadeToBlackBy(leds, numLeds, 20);  // Fade the LEDs slightly


  if (!reachedEnd) {
    currentPos--;
    if (currentPos <= 0) {
      reachedEnd = true;
    }
  }

  for (int i = numLeds - 1; i >= currentPos; i--) {
    leds[i] = CRGB::Black;
  }

  gHue++;
}



// tangens hiperboliczny od 0 do pi/2

#include <FastLED.h>
#include <math.h>

#define NUM_LEDS 51     // Total number of LEDs in the strip
#define DATA_PIN_1 5    // Data pin for the first segment
#define DATA_PIN_2 3    // Data pin for the second segment
#define DATA_PIN_3 9    // Data pin for the third segment
#define BUTTON_PIN 13   // Pin where the button is connected

#define NUM_LEDS1 NUM_LEDS   // Number of LEDs for the first segment (full strip)
#define NUM_LEDS2 NUM_LEDS  // Number of LEDs for the second segment (2/3 of strip)
#define NUM_LEDS3 NUM_LEDS  // Number of LEDs for the third segment (1/3 of strip)

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

uint8_t gHue = 0;       // Global hue value for color shifting
bool reachedEnd1 = false; // Track if the end has been reached for segment 1
bool reachedEnd2 = false; // Track if the end has been reached for segment 2
bool reachedEnd3 = false; // Track if the end has been reached for segment 3
float currentPos1 = 0;      // Current position for segment 1
float currentPos2 = 0;      // Current position for segment 2
float currentPos3 = 0;      // Current position for segment 3

bool buttonPressed = false; // Flag to track if the button is pressed
bool lastButtonState = HIGH; // Last state of the button

bool forwardDirection = true; // Flag to track current animation direction

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(leds1, NUM_LEDS1);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(leds2, NUM_LEDS2);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_3>(leds3, NUM_LEDS3);
  
  FastLED.setBrightness(50); // Set the brightness (0-255)

  // Initialize LEDs to off
  fill_solid(leds1, NUM_LEDS1, CRGB::Black);
  fill_solid(leds2, NUM_LEDS2, CRGB::Black);
  fill_solid(leds3, NUM_LEDS3, CRGB::Black);
  
  FastLED.show();

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);
  buttonPressed = (buttonState == LOW && lastButtonState == HIGH);

  if (buttonPressed) {
    changeDirection();
  }

  if (forwardDirection) {
    smoothLightUpForward(leds1, NUM_LEDS1, currentPos1, reachedEnd1);
    smoothLightUpForward(leds2, NUM_LEDS2, currentPos2, reachedEnd2);
    smoothLightUpForward(leds3, NUM_LEDS3, currentPos3, reachedEnd3);
  } else {
    smoothLightUpReverse(leds1, NUM_LEDS1, currentPos1, reachedEnd1);
    smoothLightUpReverse(leds2, NUM_LEDS2, currentPos2, reachedEnd2);
    smoothLightUpReverse(leds3, NUM_LEDS3, currentPos3, reachedEnd3);
  }

  FastLED.show();
  FastLED.delay(20);

  lastButtonState = buttonState;
}

void changeDirection() {
  reachedEnd1 = false;
  reachedEnd2 = false;
  reachedEnd3 = false;

  gHue = 0; // Reset hue for color shifting
  forwardDirection = !forwardDirection; // Toggle direction flag

  // Ensure starting positions are set correctly
}

void smoothLightUpForward(CRGB* leds, int numLeds, float &currentPos, bool &reachedEnd) {
  fadeToBlackBy(leds, numLeds, 20);  // Fade the LEDs slightly
  
  if (!reachedEnd) {
    currentPos += 0.1;
    if (currentPos >= 2.0) { // 2.0 corresponds to tanh reaching near 1
      reachedEnd = true;
    }
  }

  // Adjust the mapping from tanh() to fit the range 0 to numLeds - 1
  int ledPos = map(tanh(currentPos) * 500 + 500, 0, 1000, -1, numLeds);

  for (int i = 0; i <= ledPos; i++) {
    leds[i] += CHSV(gHue, 255, 192); // Set the color and brightness
  }

  gHue++;
}


void smoothLightUpReverse(CRGB* leds, int numLeds, float &currentPos, bool &reachedEnd) {
  fadeToBlackBy(leds, numLeds, 20);  // Fade the LEDs slightly

  if (!reachedEnd) {
    currentPos -= 0.1;
    if (currentPos <= -2.0) { // -2.0 corresponds to tanh reaching near -1
      reachedEnd = true;
    }
  }

  // Adjust the mapping from tanh() to fit the range 0 to numLeds - 1
  int ledPos = map(tanh(currentPos) * 500 + 500, 0, 1000, -1, numLeds);

  for (int i = 0; i < numLeds; i++) {
    if (i > ledPos) {
      leds[i] = CRGB::Black;  // Set LEDs past ledPos to black
    } else {
      leds[i] += CHSV(gHue, 255, 192); // Set the color and brightness
    }
  }

  gHue++;
}

/// funkcja logarytmiczna 

#include <FastLED.h>
#include <math.h>

#define NUM_LEDS 50     // Total number of LEDs in the strip
#define DATA_PIN_1 5    // Data pin for the first segment
#define DATA_PIN_2 3    // Data pin for the second segment
#define DATA_PIN_3 9    // Data pin for the third segment
#define BUTTON_PIN 13   // Pin where the button is connected

#define NUM_LEDS1 NUM_LEDS   // Number of LEDs for the first segment (full strip)
#define NUM_LEDS2 NUM_LEDS  // Number of LEDs for the second segment (2/3 of strip)
#define NUM_LEDS3 NUM_LEDS   // Number of LEDs for the third segment (1/3 of strip)

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

uint8_t gHue = 0;       // Global hue value for color shifting
bool reachedEnd1 = false; // Track if the end has been reached for segment 1
bool reachedEnd2 = false; // Track if the end has been reached for segment 2
bool reachedEnd3 = false; // Track if the end has been reached for segment 3
int currentStep1 = 0;    // Current step for segment 1
int currentStep2 = 0;    // Current step for segment 2
int currentStep3 = 0;    // Current step for segment 3

bool buttonPressed = false; // Flag to track if the button is pressed
bool lastButtonState = HIGH; // Last state of the button

bool forwardDirection = true; // Flag to track current animation direction

const int totalSteps = 100;  // Total number of steps for the animation

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(leds1, NUM_LEDS1);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(leds2, NUM_LEDS2);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_3>(leds3, NUM_LEDS3);
  
  FastLED.setBrightness(50); // Set the brightness (0-255)

  // Initialize LEDs to off
  fill_solid(leds1, NUM_LEDS1, CRGB::Black);
  fill_solid(leds2, NUM_LEDS2, CRGB::Black);
  fill_solid(leds3, NUM_LEDS3, CRGB::Black);
  
  FastLED.show();

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);
  buttonPressed = (buttonState == LOW && lastButtonState == HIGH);

  if (buttonPressed) {
    changeDirection();
  }

  if (forwardDirection) {
    smoothLightUpForward(leds1, NUM_LEDS1, currentStep1, reachedEnd1);
    smoothLightUpForward(leds2, NUM_LEDS2, currentStep2, reachedEnd2);
    smoothLightUpForward(leds3, NUM_LEDS3, currentStep3, reachedEnd3);
  } else {
    smoothLightUpReverse(leds1, NUM_LEDS1, currentStep1, reachedEnd1);
    smoothLightUpReverse(leds2, NUM_LEDS2, currentStep2, reachedEnd2);
    smoothLightUpReverse(leds3, NUM_LEDS3, currentStep3, reachedEnd3);
  }

  FastLED.show();
  FastLED.delay(20);

  lastButtonState = buttonState;
}

void changeDirection() {
  reachedEnd1 = false;
  reachedEnd2 = false;
  reachedEnd3 = false;

  gHue = 0; // Reset hue for color shifting
  forwardDirection = !forwardDirection; // Toggle direction flag
}

void smoothLightUpForward(CRGB* leds, int numLeds, int &currentStep, bool &reachedEnd) {
  fadeToBlackBy(leds, numLeds, 20);  // Fade the LEDs slightly

  if (!reachedEnd) {
    currentStep++;
    if (currentStep >= totalSteps - 1) {
      reachedEnd = true;
    }
  }

  int currentPos = logMapping(currentStep, totalSteps, numLeds);

  for (int i = 0; i <= currentPos; i++) {
    leds[i] += CHSV(gHue, 255, 192); // Set the color and brightness
  }

  gHue++;
}

void smoothLightUpReverse(CRGB* leds, int numLeds, int &currentStep, bool &reachedEnd) {
  fadeToBlackBy(leds, numLeds, 20);  // Fade the LEDs slightly

  if (!reachedEnd) {
    currentStep--;
    if (currentStep <= 0) {
      reachedEnd = true;
    }
  }

  int currentPos = logMapping(currentStep, totalSteps, numLeds);

  for (int i = 0; i <= currentPos; i++) {
    leds[i] += CHSV(gHue, 255, 192); // Set the color and brightness
  }

  gHue++;
}

int logMapping(int step, int totalSteps, int numLeds) {
    float logMin = log(2); // Adjust the minimum logarithmic base
    float logMax = log(totalSteps);
    
    // Apply a non-linear scaling to the step
    float scaledStep = pow((float)step / totalSteps, 2) * totalSteps;
    
    float scale = (log(scaledStep + 1) - logMin) / (logMax - logMin);
    return (int)(scale * (numLeds));
}

