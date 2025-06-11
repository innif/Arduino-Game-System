#include "display.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initDisplay() {
  Wire.begin(PIN_SDA, PIN_SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.display();
}

void clearDisplay() {
  display.clearDisplay();
}

void updateDisplay() {
  display.display();
}

void drawCenteredText(const char* text, int y, int textSize) {
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(text);
}

void drawHighlightBox(int x, int y, int w, int h, bool inverted) {
  if (inverted) {
    display.fillRect(x, y, w, h, SSD1306_WHITE);
  } else {
    display.drawRect(x, y, w, h, SSD1306_WHITE);
  }
}