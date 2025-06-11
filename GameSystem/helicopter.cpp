#include "helicopter.h"
#include "display.h"
#include "input.h"

HelicopterGame helicopterGame;

void HelicopterGame::init() {
  heli.y = SCREEN_HEIGHT / 2;
  heli.velocity = 0;
  
  score = 0;
  gameOver = false;
  lastUpdate = millis();
  gameSpeed = 1.5;
  gravity = 0.15;
  lift = -1.0;
  caveOffset = 0;
  
  generateCave();
}

void HelicopterGame::update() {
  handleInput();
  
  unsigned long currentTime = millis();
  
  if (currentTime - lastUpdate > 30) { // ~33 FPS
    updateHelicopter();
    updateCave();
    
    if (checkCollisions()) {
      gameOver = true;
    }
    
    score++;
    
    // Gradually increase difficulty
    if (score % 500 == 0 && gameSpeed < 3.0) {
      gameSpeed += 0.2;
    }
    
    lastUpdate = currentTime;
  }
}

void HelicopterGame::draw() {
  clearDisplay();
  
  drawCave();
  drawHelicopter();
  drawUI();
  
  updateDisplay();
}

void HelicopterGame::handleInput() {
  if (buttons.up || buttons.right) {
    heli.velocity = lift;
  }
}

void HelicopterGame::generateCave() {
  int currentGap = 25;
  int currentTop = 15;
  
  for (int i = 0; i < CAVE_SEGMENTS; i++) {
    cave[i].gapHeight = currentGap + random(-3, 4);
    cave[i].gapHeight = constrain(cave[i].gapHeight, 18, 35);
    
    cave[i].topHeight = currentTop + random(-2, 3);
    cave[i].topHeight = constrain(cave[i].topHeight, 5, SCREEN_HEIGHT - cave[i].gapHeight - 5);
    
    cave[i].bottomHeight = SCREEN_HEIGHT - cave[i].topHeight - cave[i].gapHeight;
    
    currentGap = cave[i].gapHeight;
    currentTop = cave[i].topHeight;
  }
}

void HelicopterGame::updateHelicopter() {
  heli.velocity += gravity;
  heli.y += heli.velocity;
  
  // Keep helicopter on screen
  if (heli.y < 0) {
    heli.y = 0;
    heli.velocity = 0;
  }
  if (heli.y > SCREEN_HEIGHT - HELI_SIZE) {
    heli.y = SCREEN_HEIGHT - HELI_SIZE;
    heli.velocity = 0;
  }
}

void HelicopterGame::updateCave() {
  caveOffset += gameSpeed;
  
  // When we've scrolled one segment width, shift the cave
  if (caveOffset >= 4) {
    caveOffset = 0;
    
    // Shift all segments left
    for (int i = 0; i < CAVE_SEGMENTS - 1; i++) {
      cave[i] = cave[i + 1];
    }
    
    // Generate new segment at the end
    int lastIndex = CAVE_SEGMENTS - 1;
    cave[lastIndex].gapHeight = cave[lastIndex - 1].gapHeight + random(-3, 4);
    cave[lastIndex].gapHeight = constrain(cave[lastIndex].gapHeight, 18, 35);
    
    cave[lastIndex].topHeight = cave[lastIndex - 1].topHeight + random(-2, 3);
    cave[lastIndex].topHeight = constrain(cave[lastIndex].topHeight, 5, 
                                         SCREEN_HEIGHT - cave[lastIndex].gapHeight - 5);
    
    cave[lastIndex].bottomHeight = SCREEN_HEIGHT - cave[lastIndex].topHeight - cave[lastIndex].gapHeight;
  }
}

bool HelicopterGame::checkCollisions() {
  // Get the cave segment at helicopter position
  int segmentIndex = HELI_X / 4;
  if (segmentIndex >= CAVE_SEGMENTS) segmentIndex = CAVE_SEGMENTS - 1;
  
  // Check collision with top wall
  if (heli.y < cave[segmentIndex].topHeight) {
    return true;
  }
  
  // Check collision with bottom wall
  if (heli.y + HELI_SIZE > cave[segmentIndex].topHeight + cave[segmentIndex].gapHeight) {
    return true;
  }
  
  return false;
}

void HelicopterGame::drawHelicopter() {
  // Draw helicopter body
  display.fillRect(HELI_X, heli.y, HELI_SIZE, HELI_SIZE, SSD1306_WHITE);
  
  // Draw rotor
  display.drawLine(HELI_X - 1, heli.y - 1, HELI_X + HELI_SIZE + 1, heli.y - 1, SSD1306_WHITE);
  
  // Draw tail
  display.drawPixel(HELI_X + HELI_SIZE, heli.y + 2, SSD1306_WHITE);
}

void HelicopterGame::drawCave() {
  for (int i = 0; i < CAVE_SEGMENTS; i++) {
    int x = i * 4 - caveOffset;
    
    if (x >= -4 && x < SCREEN_WIDTH) {
      // Draw top wall
      display.fillRect(x, 0, 4, cave[i].topHeight, SSD1306_WHITE);
      
      // Draw bottom wall
      int bottomY = cave[i].topHeight + cave[i].gapHeight;
      display.fillRect(x, bottomY, 4, cave[i].bottomHeight, SSD1306_WHITE);
    }
  }
}

void HelicopterGame::drawUI() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Score (top right to avoid cave)
  display.setCursor(SCREEN_WIDTH - 30, 0);
  display.print(score / 10); // Divide by 10 for reasonable scoring
}