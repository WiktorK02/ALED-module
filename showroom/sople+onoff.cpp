#include <FastLED.h>
#include <math.h>

// Pin Definitions
#define LED_PIN_1 5
#define LED_PIN_2 3
#define LED_PIN_3 9
#define NUM_LEDS 50

// Button Pins
#define BUTTON_PIN 13
#define BUTTON_PIN1 12

// Number of LEDs per strip
#define NUM_LEDS1 NUM_LEDS
#define NUM_LEDS2 NUM_LEDS
#define NUM_LEDS3 NUM_LEDS

// LED Arrays
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

// Position Arrays
uint8_t x1[NUM_LEDS1];
uint8_t y1[NUM_LEDS1];
uint8_t x2[NUM_LEDS2];
uint8_t y2[NUM_LEDS2];
uint8_t x3[NUM_LEDS3];
uint8_t y3[NUM_LEDS3];

// Gradient Palette
DEFINE_GRADIENT_PALETTE(indigoGP) {
  0, 0, 0, 0,
  100, 0, 0, 5,
  255, 255, 255, 247
};
CRGBPalette16 indigoPalett = indigoGP;

// Global Variables
float gHue = 0.0;

// State tracking for stepper motors
bool reachedEnd1 = false;
bool reachedEnd2 = false;
bool reachedEnd3 = false;
int currentStep1 = 0;
int currentStep2 = 0;
int currentStep3 = 0;

// Button states
bool buttonPressed = false;
bool lastButtonState = HIGH;
bool buttonPressed1 = false;
bool lastButtonState1 = HIGH;

// Direction control
bool forwardDirection = false;

// Stepper motor configuration
const int totalSteps = 400;

// Effect and color control
bool effectEnabled = false;
bool changeColors = false;

// Button press timing
unsigned long buttonPressStartTime = 0;
const unsigned long requiredHoldTime = 1000; // 1 second in milliseconds

// Miscellaneous
bool checkFlag = false;


void setup() {
  // Initialize LEDs
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds2, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(leds3, NUM_LEDS);

  // Calculate positions
  for (unsigned short i = 0; i < NUM_LEDS; i++) {
    unsigned char angle = (i * 256) / NUM_LEDS;
    x1[i] = cos8(angle);
    y1[i] = sin8(angle);
    x2[i] = cos8(angle + 128);
    y2[i] = sin8(angle + 200);
    x3[i] = cos8(angle + 84);
    y3[i] = sin8(angle - 64);
  }

  // Initialize LED arrays to black
  fill_solid(leds1, NUM_LEDS1, CRGB::Black);
  fill_solid(leds2, NUM_LEDS2, CRGB::Black);
  fill_solid(leds3, NUM_LEDS3, CRGB::Black);
  FastLED.show();
  FastLED.delay(20);

  // Setup button pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
}

void loop() {
  // Read button states
  bool buttonState = digitalRead(BUTTON_PIN);
  bool buttonState1 = digitalRead(BUTTON_PIN1);
  buttonPressed = (buttonState == LOW && lastButtonState == HIGH);
  buttonPressed1 = (buttonState1 == LOW && lastButtonState1 == HIGH);

  // Change direction on button press
  if (buttonPressed) {
    changeDirection();
    effectEnabled = false;
  }

  // Handle button 12 hold logic
  if (buttonState1 == LOW && lastButtonState1 == HIGH) {
    buttonPressStartTime = millis();
    checkFlag = true;
  }

  if (buttonState1 == HIGH && lastButtonState1 == LOW) {
    if (millis() - buttonPressStartTime < requiredHoldTime) {
      effectEnabled = !effectEnabled; // Toggle effect if pressed briefly
    }
  }

  if (buttonState1 == LOW && checkFlag) {
    if (millis() - buttonPressStartTime > requiredHoldTime) {
      changeColors = !changeColors; // Toggle color change on long hold
      if (!changeColors) {
        indigoPalett = indigoGP; // Reset to default palette
      }
      checkFlag = false;
    }
  }

  // Update palette if changing colors
  if (changeColors) {
    updatePalette();
  }

  // Handle LED effects
  if (effectEnabled) {
    resetEffectState();
    effect(leds1, 40, x1, y1);
    effect(leds2, 50, x2, y2);
    effect(leds3, 60, x3, y3);
  } else {
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

  // Update last button states
  lastButtonState = buttonState;
  lastButtonState1 = buttonState1;

  FastLED.show();
}

void effect(CRGB* leds, uint8_t speed, uint8_t* x, uint8_t* y) {
  const uint16_t scale = 100;
  uint32_t shift = millis() / speed;
  shift *= 150;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint32_t real_x = shift + x[i] * scale;
    uint32_t real_y = shift + y[i] * scale;

    uint16_t noise = inoise16(real_x, real_y, 4223);
    uint16_t index = noise * 3;

    leds[i] = ColorFromPaletteExtended(indigoPalett, index, 255, LINEARBLEND);
  }
}

CRGB ColorFromPaletteExtended(const CRGBPalette16& pal, uint16_t index, uint8_t brightness, TBlendType blendType) {
  uint8_t index_4bit = (index >> 12);
  uint8_t offset = (uint8_t)(index >> 4);
  const CRGB* entry = &(pal[0]) + index_4bit;
  uint8_t red1 = entry->red;
  uint8_t green1 = entry->green;
  uint8_t blue1 = entry->blue;

  uint8_t blend = offset && (blendType != NOBLEND);
  if (blend) {
    if (index_4bit == 15) {
      entry = &(pal[0]);
    } else {
      entry++;
    }

    uint8_t f1 = 255 - offset;
    red1 = scale8_LEAVING_R1_DIRTY(red1, f1);
    green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
    blue1 = scale8_LEAVING_R1_DIRTY(blue1, f1);

    uint8_t red2 = entry->red;
    uint8_t green2 = entry->green;
    uint8_t blue2 = entry->blue;
    red2 = scale8_LEAVING_R1_DIRTY(red2, offset);
    green2 = scale8_LEAVING_R1_DIRTY(green2, offset);
    blue2 = scale8_LEAVING_R1_DIRTY(blue2, offset);
    cleanup_R1();

    red1 += red2;
    green1 += green2;
    blue1 += blue2;
  }

  if (brightness != 255) {
    nscale8x3(red1, green1, blue1, brightness);
  }
  return CRGB(red1, green1, blue1);
}

void changeDirection() {
  reachedEnd1 = false;
  reachedEnd2 = false;
  reachedEnd3 = false;
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
}

int logMapping(int step, int totalSteps, int numLeds) {
  float logMin = log(2);
  float logMax = log(totalSteps);

  float scaledStep = pow((float)step / totalSteps, 2) * (totalSteps);

  float scale = (log(scaledStep + 1) - logMin) / (logMax - logMin);
  return (int)(scale * (numLeds));
}

void updatePalette() {
  uint8_t hue = millis() / 100; // Change hue based on time
  CRGB color = CHSV(hue, 255, 255);

  // Update the entire palette dynamically
  indigoPalett = CRGBPalette16(
    CHSV(0, 0, 0),   // Black
    CHSV(0, 0, 5),   // Dark color
    color            // Dynamic color
  );
}

void resetEffectState() {
  forwardDirection = false;
  reachedEnd1 = false;
  reachedEnd2 = false;
  reachedEnd3 = false;
  currentStep1 = 0;
  currentStep2 = 0;
  currentStep3 = 0;
}
