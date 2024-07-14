//// CALE BIALE

#include <FastLED.h>

#define LED_PIN_1 5
#define LED_PIN_2 3
#define LED_PIN_3 9
#define NUM_LEDS 64

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];

// Arrays for LED_PIN_1
uint8_t x1[NUM_LEDS];
uint8_t y1[NUM_LEDS];

// Arrays for LED_PIN_2
uint8_t x2[NUM_LEDS];
uint8_t y2[NUM_LEDS];

// Arrays for LED_PIN_3
uint8_t x3[NUM_LEDS];
uint8_t y3[NUM_LEDS];

// Palette
DEFINE_GRADIENT_PALETTE(indigoGP) {
  0, 0, 0, 0,
  100, 0, 0, 5,
  255, 255, 255, 247
};

CRGBPalette16 indigoPalett = indigoGP;

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
}

void loop() {
  effect(leds1, 40, x1, y1); // Effect for LED_PIN_1
  effect(leds2, 50, x2, y2); // Effect for LED_PIN_2 with a different speed
  effect(leds3, 60, x3, y3); // Effect for LED_PIN_3 with another different speed
  
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


///// COLOROWE 

#include <FastLED.h>

#define LED_PIN_1 5
#define LED_PIN_2 3
#define LED_PIN_3 9
#define NUM_LEDS 64

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];

// Arrays for LED_PIN_1
uint8_t x1[NUM_LEDS];
uint8_t y1[NUM_LEDS];

// Arrays for LED_PIN_2
uint8_t x2[NUM_LEDS];
uint8_t y2[NUM_LEDS];

// Arrays for LED_PIN_3
uint8_t x3[NUM_LEDS];
uint8_t y3[NUM_LEDS];

// Gradient palette with placeholder colors that will be dynamically changed
DEFINE_GRADIENT_PALETTE(indigoGP) {
  0, 0, 0, 0,
  100, 0, 0, 5,
  255, 255, 255, 247 // This will be replaced dynamically
};

CRGBPalette16 indigoPalette = indigoGP;

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
}

void loop() {
  updatePalette();
  effect(leds1, 40, x1, y1); // Effect for LED_PIN_1
  effect(leds2, 50, x2, y2); // Effect for LED_PIN_2 with a different speed
  effect(leds3, 60, x3, y3); // Effect for LED_PIN_3 with another different speed
  
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

    leds[i] = ColorFromPaletteExtended(indigoPalette, index, 255, LINEARBLEND);
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

void updatePalette() {
  uint8_t hue = millis() / 10; // Change hue based on time
  CRGB color = CHSV(hue, 255, 255);
  
  // Update the entire palette dynamically
  indigoPalette = CRGBPalette16(
    CHSV(0, 0, 0),   // Black
    CHSV(0, 0, 5),   // Dark color
    color            // Dynamic color
  );
}
