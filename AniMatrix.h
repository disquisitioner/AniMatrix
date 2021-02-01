#ifndef AniMatrix_h
#define AniMatrix_h

// Layout orientations for a matrix that's actually a folded strip
#define MATRIX_TBRL 0
#define MATRIX_TBLR 1

#include "Arduino.h"
class AniMatrix
{
  public:
    AniMatrix(int num_leds, int width, int height, int layout_mode, CRGB leds[]);
    void setLED(int x, int y, CRGB color);
    void setColumn(int x, CRGB color);
    void setRow(int y, CRGB color);
    void setRect(int x, int y, int w, int h, CRGB color);
    void drawRow(int row, CRGB color);
    void drawColumn(int column, CRGB color);
  private:
    int _ledCount;
    CRGB *_leds;
    int _layoutMode;
    int _width;
    int _height;
    int xyToN(int x,int y);
};

#endif
