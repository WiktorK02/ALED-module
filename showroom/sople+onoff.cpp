#include <FastLED.h>
#include <math.h>
#define LED_PIN_1 5
#define LED_PIN_2 3
#define LED_PIN_3 9
#define NUM_LEDS 50
#define NUM_LEDS1 NUM_LEDS   // Number of LEDs for the first segment (full strip)
#define NUM_LEDS2 NUM_LEDS  // Number of LEDs for the second segment (2/3 of strip)
#define NUM_LEDS3 NUM_LEDS   // Number of LEDs for the third segment (1/3 of strip)

#define BUTTON_PIN 13 
#define BUTTON_PIN1 12

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

// Arrays for LED_PIN_1
uint8_t x1[NUM_LEDS1];
uint8_t y1[NUM_LEDS1];

// Arrays for LED_PIN_2
uint8_t x2[NUM_LEDS2];
uint8_t y2[NUM_LEDS2];

// Arrays for LED_PIN_3
uint8_t x3[NUM_LEDS3];
uint8_t y3[NUM_LEDS3];

// Palette
DEFINE_GRADIENT_PALETTE(indigoGP) {
  0, 0, 0, 0,
  100, 0, 0, 5,
  255, 255, 255, 247
};

CRGBPalette16 indigoPalett = indigoGP;

uint8_t gHue = 0;       // Global hue value for color shifting
bool reachedEnd1 = false; // Track if the end has been reached for segment 1
bool reachedEnd2 = false; // Track if the end has been reached for segment 2
bool reachedEnd3 = false; // Track if the end has been reached for segment 3
int currentStep1 = 0;    // Current step for segment 1
int currentStep2 = 0;    // Current step for segment 2
int currentStep3 = 0;    // Current step for segment 3

bool buttonPressed = false; // Flag to track if the button is pressed
bool lastButtonState = HIGH; // Last state of the button

bool buttonPressed1 = false; // Flag to track if the button is pressed
bool lastButtonState1 = HIGH; // Last state of the button

bool forwardDirection = true; // Flag to track current animation direction

const int totalSteps = 100;  // Total number of steps for the animation

bool effectEnabled = false;  // Flag to track if the effect is enabled

void setup() {
  Serial.begin(2000000);
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds2, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(leds3, NUM_LEDS);
  
  for (unsigned short i = 0; i < NUM_LEDS; i++) {
    unsigned char angle = (i * 256) / NUM_LEDS;
    
    // Initialize x1 and y1 for LED_PIN_1
    x1[i] = cos8(angle);
    y1[i] = sin8(angle);
    
    // Initialize x2 and y2 for LED_PIN_2 with a different pattern
    x2[i] = cos8(angle + 128); // example variation
    y2[i] = sin8(angle + 128); // example variation
    
    // Initialize x3 and y3 for LED_PIN_3 with another different pattern
    x3[i] = cos8(angle + 64); // another example variation
    y3[i] = sin8(angle + 64); // another example variation
  }
  // Initialize LEDs to off
  fill_solid(leds1, NUM_LEDS1, CRGB::Black);
  fill_solid(leds2, NUM_LEDS2, CRGB::Black);
  fill_solid(leds3, NUM_LEDS3, CRGB::Black);
  
  FastLED.show();
  FastLED.delay(2000);

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
  pinMode(BUTTON_PIN1, INPUT_PULLUP); 
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);
  bool buttonState1 = digitalRead(BUTTON_PIN1);
  buttonPressed = (buttonState == LOW && lastButtonState == HIGH);
  buttonPressed1 = (buttonState1 == LOW && lastButtonState1 == HIGH);
  
  // Toggle direction on button press
  if (buttonPressed) {
    changeDirection();
    effectEnabled = false;
    
  }

  // Toggle effect on button press
  if (buttonPressed1) {
    effectEnabled = !effectEnabled;
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
  
  if (effectEnabled) {   
    forwardDirection = false; // Flag to track current animation direction
    reachedEnd1 = false; // Track if the end has been reached for segment 1
    reachedEnd2 = false; // Track if the end has been reached for segment 2
    reachedEnd3 = false; // Track if the end has been reached for segment 3
    currentStep1 = 0;    // Current step for segment 1
    currentStep2 = 0;    // Current step for segment 2
    currentStep3 = 0; 

    effect(leds1, 40, x1, y1); // Effect for LED_PIN_1
    effect(leds2, 50, x2, y2); // Effect for LED_PIN_2 with a different speed
    effect(leds3, 60, x3, y3); // Effect for LED_PIN_3 with another different speed
  }
  
  lastButtonState = buttonState;
  lastButtonState1 = buttonState1;
  
  FastLED.show();
}

// Effect function (gets called in loop)
void effect(CRGB* leds, uint8_t speed, uint8_t* x, uint8_t* y) {
  const uint16_t scale = 150;
  
  // Adjust the multiplication factor for speed
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

// FastLED uses 4-bit interpolation. 8-bit looks far less janky.
// https://github.com/FastLED/FastLED/pull/202
CRGB ColorFromPaletteExtended(const CRGBPalette16& pal, uint16_t index, uint8_t brightness, TBlendType blendType) {
  // Extract the four most significant bits of the index as a palette index.
  uint8_t index_4bit = (index >> 12);
  // Calculate the 8-bit offset from the palette index.
  uint8_t offset = (uint8_t)(index >> 4);
  // Get the palette entry from the 4-bit index
  const CRGB* entry = &(pal[0]) + index_4bit;
  uint8_t red1   = entry->red;
  uint8_t green1 = entry->green;
  uint8_t blue1  = entry->blue;

  uint8_t blend = offset && (blendType != NOBLEND);
  if (blend) {
    if (index_4bit == 15) {
      entry = &(pal[0]);
    } else {
      entry++;
    }

    // Calculate the scaling factor and scaled values for the lower palette value.
    uint8_t f1 = 255 - offset;
    red1   = scale8_LEAVING_R1_DIRTY(red1,   f1);
    green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
    blue1  = scale8_LEAVING_R1_DIRTY(blue1,  f1);

    // Calculate the scaled values for the neighbouring palette value.
    uint8_t red2   = entry->red;
    uint8_t green2 = entry->green;
    uint8_t blue2  = entry->blue;
    red2   = scale8_LEAVING_R1_DIRTY(red2,   offset);
    green2 = scale8_LEAVING_R1_DIRTY(green2, offset);
    blue2  = scale8_LEAVING_R1_DIRTY(blue2,  offset);
    cleanup_R1();

    // These sums can't overflow, so no qadd8 needed.
    red1   += red2;
    green1 += green2;
    blue1  += blue2;
  }
  if (brightness != 255) {
    // nscale8x3_video(red1, green1, blue1, brightness);
    nscale8x3(red1, green1, blue1, brightness);
  }
  return CRGB(red1, green1, blue1);
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
    
    // Adjust the scaling to slow down the effect
    float scaledStep = pow((float)step / totalSteps, 2) * (totalSteps); // Increase the exponent (e.g., 1.2)

    float scale = (log(scaledStep + 1) - logMin) / (logMax - logMin);
    return (int)(scale * (numLeds));
}
