/*
 * AniMatrix -- a utility library designed to make it easy to 
 * generate patterns and animations on LED matrix displays, particularly
 * those that are folded linear strips of LEDs rather than addressable 
 * two dimension matrix devices.  (E.g., a 16x16 LED matrix that is actually 
 * a folded strip of 256 linearly-connected LEDs.)  Makes use of 
 * the excellent FastLED library to handle low level command and
 * control of a wide variety of LEDs.
 *
 * Generally there are two types of public functions.   Those named "set*(...)"
 * operate in a virtual 2D (x,y) space allowing any LED in the matrix to be
 * addressed by its (x,y) coordinates in the matrix (even though electrically the
 * matrix is one long strip of x*y LEDs).  The "set*(...)" functions provide
 * an interface that is oblivious to how the LED strip is wired and folded into
 * the matrix, relying on a private "xyToN()" function to map (x,y) coordinates
 * to identify the actual Nth LED in the strip.  These functions are easy to
 * write and use, but they're inefficient in that they must perform the
 * (x,y) -> N mapping every time they address an LED.
 *
 * There are also functions named "draw*(...)", which operate in the actual
 * 0 to N linear space of the matrix as an LED strip.  These functions should 
 * be more performant as they avoid the (x,y) -> N mapping for each manipulated
 * LED and they can take advantage of direct knowledge of the layout of the
 * strip, e.g. to easily draw a row of LEDs in the matrix given any row is
 * likely to be a consecutive group of LEDs in the strip.
 * 
 * Author: David Bryant <david@disquisitioner.com>
 * Version: 0.1 (still in active development)
 * Date: 1 February 2021
 * 
 */

#include "Arduino.h"
#include <FastLED.h>
#include "AniMatrix.h"

AniMatrix::AniMatrix(int num_leds, int width, int height, int layout_mode, CRGB leds[])
{
  _ledCount = num_leds;
  _leds = leds;
  _layoutMode = layout_mode;
  _width = width;
  _height = height;
}

void AniMatrix::setLED(int x, int y, CRGB color)
{
  int n;
  n = xyToN(x,y);
  _leds[n] = color;
}

void AniMatrix::setColumn(int x, CRGB color)
{
  int i;
  for(i=0;i<_height;i++) {
    setLED(x,i,color);
  }
}

void AniMatrix::setRow(int y, CRGB color)
{
  int j;
  for(j=0;j<_width;j++) {
    setLED(j,y,color);
  }
}

void AniMatrix::setRect(int x, int y, int w, int h, CRGB color)
{
  int i, j;
  // Top and bottom
  for(i=0;i<w;i++) {
    setLED(x+i,y,color);
    setLED(x+i,y+h-1,color);
  }
  // Right and left
  for(j=1;j<h-1;j++) {
    setLED(x,y+j,color);
    setLED(x+w-1,y+j,color);
  }
}

void AniMatrix::drawRow(int row, CRGB color)
{
  int i, n;

  switch(_layoutMode) {
    case MATRIX_TBRL:
      if( (row%2) == 0) {
        // right to left row, count up from end
        n = xyToN(_width-1,row);
      }
      else {
        // left to right row, count up from start
        n = xyToN(0,row);
      } 
      for(i=0;i<_width;i++) {
        _leds[n+i] = color;
      }
      break;
    default:
      _leds[0] = CRGB::White;  // for now...
  }
}

void AniMatrix::drawColumn(int column, CRGB color)
{
  int i, n;

  switch(_layoutMode) {
    case MATRIX_TBRL:
      n = xyToN(column,0);
      // Draw R-to-L rows
      for(i=0;i<_height;i+=2,n+=(2*_width)) {
        _leds[n] = color;
      }
      // Draw L-to-R rows
      n = xyToN(column,1);
      for(i=1;i<_height;i+=2,n+=(2*_width)) {
        _leds[n] = color;
      }
      break;
    default:
      _leds[0] = CRGB::White;  // for now...
  }
}

int AniMatrix::xyToN(int x, int y)
{
  int n;

  switch(_layoutMode) {
    case MATRIX_TBRL:
      if( (y%2) == 0) {
        n = (y*_height) + (_width - 1 -x);
      }
      else {
        n = (y * _height) + x;
      }
      break;
    default:
      n = 0;
      break;
  }
  return(n);
}
