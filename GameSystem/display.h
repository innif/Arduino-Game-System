#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"

void initDisplay();
void clearDisplay();
void updateDisplay();
void drawCenteredText(const char* text, int y, int textSize = 1);
void drawHighlightBox(int x, int y, int w, int h, bool inverted = false);

#endif