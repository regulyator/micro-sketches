#include <Adafruit_NeoPixel.h>

#define PIXELS        5
#define LED_PIN_1     0
#define LED_PIN_2     5
#define PEAK_FALL     500
#define VUBRIGHTNESS  90

Adafruit_NeoPixel strip_1 = Adafruit_NeoPixel(PIXELS, LED_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_2 = Adafruit_NeoPixel(PIXELS, LED_PIN_2, NEO_GRB + NEO_KHZ800);

int currentIndex = 0;
int trailLength = 1;
unsigned long previousMillis = 0;
unsigned long delayTime = 250;  // Adjust this value to control the speed
bool forwardDirection = true;

void setup() {
  strip_1.begin();
  strip_1.show();  // Initialize all pixels to off
  strip_2.begin();
  strip_2.show();  // Initialize all pixels to off
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= delayTime) {
    previousMillis = currentMillis;

    // Clear all pixels
    for (int i = 0; i < PIXELS; i++) {
      strip_1.setPixelColor(i, 0);
      strip_2.setPixelColor(i, 0);
    }

    // Set the head pixel, main pixel, and trail pixels
    for (int i = currentIndex + 1; i >= currentIndex - trailLength; i--) {
      if (i >= 0 && i < PIXELS) {
        // Calculate color for the current pixel
        uint32_t color = CandyColor(i);

        strip_1.setPixelColor(i, color);
        strip_2.setPixelColor(i, color);

        // Fade the trailing pixels
        if (i != currentIndex) {
          int fadeIndex = currentIndex - i;
          uint32_t fadeColor = FadeColor(color, fadeIndex);

          strip_1.setPixelColor(i, fadeColor);
          strip_2.setPixelColor(i, fadeColor);
        }
      }
    }

    // Interpolate between adjacent pixels
    for (int i = currentIndex + 1; i > currentIndex - trailLength; i--) {
      if (i >= 0 && i < PIXELS && i - 1 >= 0 && i - 1 < PIXELS) {
        // Calculate intermediate color between adjacent pixels
        uint32_t color1 = strip_1.getPixelColor(i);
        uint32_t color2 = strip_1.getPixelColor(i - 1);
        uint32_t interpolatedColor = InterpolateColor(color1, color2);

        strip_1.setPixelColor(i - 1, interpolatedColor);
        strip_2.setPixelColor(i - 1, interpolatedColor);
      }
    }

    strip_1.show();
    strip_2.show();

    if (forwardDirection) {
      currentIndex++;
      if (currentIndex > PIXELS + trailLength) {
        currentIndex = PIXELS - 1;
        forwardDirection = false;
      }
    } else {
      currentIndex--;
      if (currentIndex < -trailLength) {
        currentIndex = 0;
        forwardDirection = true;
      }
    }

    delay(40);
  }
}

// Function to generate candy colors based on pixel index
uint32_t CandyColor(byte index) {
  if (index % 2 == 0) {
    // Pink color with decreased brightness
    return strip_1.Color(255 / 100, 105 / 10, 180 / 10);  // Decrease RGB values for pink
  } else {
    // Purple color with decreased brightness
    return strip_1.Color(138 / 10, 43 / 10, 226 / 10);  // Decrease RGB values for purple
  }
}

// Function to fade the color based on trail distance
uint32_t FadeColor(uint32_t color, int trailDistance) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  r = (r * trailDistance) / (trailLength + 1);
  g = (g * trailDistance) / (trailLength + 1);
  b = (b * trailDistance) / (trailLength + 1);

  return (r << 16) | (g << 8) | b;
}

// Function to interpolate between two colors
uint32_t InterpolateColor(uint32_t color1, uint32_t color2) {
  uint8_t r1 = (color1 >> 16) & 0xFF;
  uint8_t g1 = (color1 >> 8) & 0xFF;
  uint8_t b1 = color1 & 0xFF;

  uint8_t r2 = (color2 >> 16) & 0xFF;
  uint8_t g2 = (color2 >> 8) & 0xFF;
  uint8_t b2 = color2 & 0xFF;

  uint8_t r = (r1 + r2) / 2;
  uint8_t g = (g1 + g2) / 2;
  uint8_t b = (b1 + b2) / 2;

  return (r << 16) | (g << 8) | b;
}