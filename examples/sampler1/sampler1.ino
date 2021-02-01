#include <FastLED.h>
#include "AniMatrix.h"

#define NUM_LEDS  256
#define DATA_PIN  10

CRGB leds[NUM_LEDS];

AniMatrix matrix(256,16,16,MATRIX_TBRL,leds);

void setup() {

  Serial.begin(115200);

  // Initialize the LED strip  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds,NUM_LEDS);
  FastLED.setBrightness(50);

  matrix.setLED(0,0,CRGB::Blue);
  matrix.setLED(15,0,CRGB::Red);
  matrix.setLED(0,15,CRGB::Yellow);
  matrix.setLED(15,15,CRGB::Green);
  FastLED.show();
  delay(2000);
}

unsigned int counter = 0;
void loop() {

  int i, j, s0, s;
  
  FastLED.clear();
  i = counter%16;
  j = counter/16;
  if( (j%2) == 0) {
    matrix.drawRow(i,CRGB::Purple);
    matrix.drawColumn(i,CRGB::Yellow);
    matrix.setLED(i,i,CRGB::Green);
  }
  else {
    i = 15-i;
    matrix.drawRow(i,CRGB::Purple);
    matrix.drawColumn(i,CRGB::Yellow);
    matrix.setLED(i,i,CRGB::Green);  
  }

  // Small blue dot that runs around the edge of the matrix
  updateBug(CRGB::Blue);

  // Animated square
  s0 = (counter%12)+1;
  if(s0 < 8) {
    s = s0;
  }
  else {
    s = 14 - s0;
  }
  matrix.setRect(8-s,8-s,2*s,2*s,CRGB::Red);
  
  FastLED.show();
  delay(60);
  counter++;
}

// Single pixel that runs around the edge of the matrix. For a
// 16x16 matrix that's 60 spots per complete cycle (don't want to
// draw the corners more than once), so figure out where to put the
// bug based on a 60-tick cycle.
void updateBug(CRGB color)
{
  int n = counter%60;
  if(n < 16) {
    matrix.setLED(n,0,color);
    return;
  }
  if(n < 31) {
    matrix.setLED(15,n-15,color);
    return;
  }
  if(n < 46) {
    matrix.setLED(45-n,15,color);
    return;
  }
  matrix.setLED(0,60-n,color);
  return;
}
