#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
     
int PIN_NUM = 2;  // Pin connected to the first led input
int LED_CNT = 6;  // The number of leds in the chain

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_CNT, PIN_NUM, NEO_GRB + NEO_KHZ800);

// Set initial color
uint8_t  prevR = 0;
uint8_t  prevG = 0;
uint8_t  prevB = 0;

void setup() {
  // Randomize the random numbers generator
  int id = EEPROM.read(0);
  randomSeed(id);
  EEPROM.write(0, id+random(10));
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
     
void loop() {
  uint8_t colors[3];
  GetWheelPos(random(256), colors);
  CrossFade(colors, 15);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
void GetWheelPos(byte wheelPos, uint8_t colors[]) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    colors[0] = 255 - wheelPos * 3;
    colors[1] = 0;
    colors[2] = wheelPos * 3;
    return;
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    colors[0] = 0;
    colors[1] = wheelPos * 3;
    colors[2] = 255 - wheelPos * 3;
    return;
  }
  wheelPos -= 170;
  colors[0] = wheelPos * 3;
  colors[1] = 255 - wheelPos * 3;
  colors[2] = 0;
}

void setColor(uint8_t r, uint8_t  g, uint8_t b) {
    for(uint16_t i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
}

uint8_t  calculateVal(uint8_t prev, uint8_t next, int step) {
  if (next > prev) {
    return prev + (uint8_t)((int)next - prev)*step / 255; 
  } else if (next < prev) {
    return prev - (uint8_t)((int)prev - next)*step / 255;     
  } else {
    return next;
  }
}

void CrossFade(int8_t color[3], int wait) {
  for (int i = 0; i <= 255; i++) {
    uint8_t redVal = calculateVal(prevR, color[0], i);
    uint8_t grnVal = calculateVal(prevG, color[1], i);
    uint8_t bluVal = calculateVal(prevB, color[2], i);

    setColor(redVal, grnVal, bluVal);
    delay(wait); // Pause for 'wait' milliseconds before resuming the loop
  }
  // Update current values for next loop
  prevR = color[0]; 
  prevG = color[1]; 
  prevB = color[2];
}

