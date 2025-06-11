// GameSystem.ino - Hauptdatei
#include "config.h"
#include "display.h"
#include "input.h"
#include "gamemanager.h"

void setup() {
  Serial.begin(115200);
  initDisplay();
  initInput();
  initGameManager();
}

void loop() {
  updateInput();
  updateGameManager();
  delay(16); // ~60 FPS
}