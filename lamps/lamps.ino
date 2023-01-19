#define NO_CLOCK_CORRECTION 1

#include <FastLED.h>
     
#define DATA_PIN 2  // Pin connected to the first led input
#define NUM_LEDS 1  // The number of leds in the chain

CRGB leds[NUM_LEDS];
CRGB prevColor;
CHSV color;

void setup() {
   FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
}
     
void loop() {
    color.s = random8(6) == 0 ? 0 : 255;
    color.v = 150;
    color.h = random8();

    CRGB newColor;  
    hsv2rgb_rainbow(color, newColor);

    CrossFade(newColor, 15);

    delay(60000);
}

void CrossFade(CRGB color, int wait) {
  for (int step = 1; step <= 255; step++) {
    CRGB temp;

    temp.r = lerp8by8(prevColor.r, color.r, step);
    temp.g = lerp8by8(prevColor.g, color.g, step);
    temp.b = lerp8by8(prevColor.b, color.b, step);

    leds[0] = temp; 
    FastLED.show(); 
    delay(wait);
  }
  prevColor = color; 
}

