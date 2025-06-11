#include "frogger.h"
#include "display.h"
#include "input.h"

FroggerGame froggerGame;

void FroggerGame::init() {
  resetFrog();
  
  // Initialize cars
  for (int i = 0; i < MAX_CARS; i++) {
    cars[i].active = false;
  }
  
  // Initialize logs
  for (int i = 0; i < MAX_LOGS; i++) {
    logs[i].active = false;
  }
  
  score = 0;
  lives = 3;
  gameOver = false;
  lastUpdate = millis();
  lastSpawn = millis();
  highestY = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT;
}

void FroggerGame::update() {
  handleInput();
  
  unsigned long currentTime = millis();
  
  if (currentTime - lastUpdate > 100) { // 10 FPS for smooth movement
    updateCars();
    updateLogs();
    updateFrog();
    
    // Spawn new obstacles
    if (currentTime - lastSpawn > 2000) {
      spawnCar();
      spawnLog();
      lastSpawn = currentTime;
    }
    
    lastUpdate = currentTime;
  }
  
  // Check collisions
  if (checkCarCollision() || checkWaterCollision()) {
    lives--;
    if (lives <= 0) {
      gameOver = true;
    } else {
      resetFrog();
    }
  }
  
  // Check win condition (reached top)
  if (frog.y <= SAFE_ZONE_HEIGHT) {
    score += 100;
    if (frog.y < highestY) {
      score += 50;
      highestY = frog.y;
    }
    resetFrog();
  }
}

void FroggerGame::draw() {
  clearDisplay();
  
  drawRoad();
  drawCars();
  drawLogs();
  drawFrog();
  drawUI();
  
  updateDisplay();
}

void FroggerGame::handleInput() {
  if (buttons.upPressed && frog.y > 0) {
    frog.y -= LANE_HEIGHT;
    frog.onLog = false;
  }
  else if (buttons.downPressed && frog.y < SCREEN_HEIGHT - FROG_SIZE) {
    frog.y += LANE_HEIGHT;
    frog.onLog = false;
  }
  else if (buttons.leftPressed && frog.x > 0) {
    frog.x -= FROG_SIZE;
  }
  else if (buttons.rightPressed && frog.x < SCREEN_WIDTH - FROG_SIZE) {
    frog.x += FROG_SIZE;
  }
}

void FroggerGame::resetFrog() {
  frog.x = SCREEN_WIDTH / 2 - FROG_SIZE / 2;
  frog.y = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT;
  frog.onLog = false;
  frog.logIndex = -1;
}

void FroggerGame::updateFrog() {
  // If frog is on a log, move with it
  if (frog.onLog && frog.logIndex >= 0 && logs[frog.logIndex].active) {
    frog.x += logs[frog.logIndex].speed;
    
    // Check if frog falls off log
    if (frog.x < logs[frog.logIndex].x || 
        frog.x > logs[frog.logIndex].x + logs[frog.logIndex].width) {
      frog.onLog = false;
    }
  }
  
  // Keep frog on screen
  if (frog.x < 0) frog.x = 0;
  if (frog.x > SCREEN_WIDTH - FROG_SIZE) frog.x = SCREEN_WIDTH - FROG_SIZE;
}

void FroggerGame::updateCars() {
  for (int i = 0; i < MAX_CARS; i++) {
    if (cars[i].active) {
      if (cars[i].direction) {
        cars[i].x += cars[i].speed;
        if (cars[i].x > SCREEN_WIDTH) cars[i].active = false;
      } else {
        cars[i].x -= cars[i].speed;
        if (cars[i].x < -8) cars[i].active = false;
      }
    }
  }
}

void FroggerGame::updateLogs() {
  for (int i = 0; i < MAX_LOGS; i++) {
    if (logs[i].active) {
      logs[i].x += logs[i].speed;
      if (logs[i].x > SCREEN_WIDTH || logs[i].x < -logs[i].width) {
        logs[i].active = false;
      }
    }
  }
}

void FroggerGame::spawnCar() {
  for (int i = 0; i < MAX_CARS; i++) {
    if (!cars[i].active) {
      cars[i].active = true;
      cars[i].y = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT - (random(1, ROAD_LANES + 1) * LANE_HEIGHT);
      cars[i].direction = random(0, 2);
      cars[i].speed = random(1, 3);
      
      if (cars[i].direction) {
        cars[i].x = -8;
      } else {
        cars[i].x = SCREEN_WIDTH;
      }
      break;
    }
  }
}

void FroggerGame::spawnLog() {
  for (int i = 0; i < MAX_LOGS; i++) {
    if (!logs[i].active) {
      logs[i].active = true;
      logs[i].y = SAFE_ZONE_HEIGHT + (random(0, RIVER_LANES) * LANE_HEIGHT);
      logs[i].width = random(16, 32);
      logs[i].speed = random(1, 3);
      logs[i].x = -logs[i].width;
      break;
    }
  }
}

bool FroggerGame::checkCarCollision() {
  // Check if frog is in road area
  if (frog.y < SCREEN_HEIGHT - SAFE_ZONE_HEIGHT - (ROAD_LANES * LANE_HEIGHT) ||
      frog.y >= SCREEN_HEIGHT - SAFE_ZONE_HEIGHT) {
    return false;
  }
  
  for (int i = 0; i < MAX_CARS; i++) {
    if (cars[i].active) {
      if (frog.x + FROG_SIZE > cars[i].x && frog.x < cars[i].x + 8 &&
          frog.y + FROG_SIZE > cars[i].y && frog.y < cars[i].y + 6) {
        return true;
      }
    }
  }
  return false;
}

bool FroggerGame::checkWaterCollision() {
  // Check if frog is in water area
  if (frog.y < SAFE_ZONE_HEIGHT + (RIVER_LANES * LANE_HEIGHT) &&
      frog.y >= SAFE_ZONE_HEIGHT) {
    
    // Check if frog is on a log
    frog.onLog = false;
    for (int i = 0; i < MAX_LOGS; i++) {
      if (logs[i].active) {
        if (frog.x + FROG_SIZE > logs[i].x && frog.x < logs[i].x + logs[i].width &&
            abs(frog.y - logs[i].y) < 4) {
          frog.onLog = true;
          frog.logIndex = i;
          break;
        }
      }
    }
    
    return !frog.onLog; // Drowns if not on log
  }
  
  return false;
}

void FroggerGame::drawFrog() {
  display.fillRect(frog.x, frog.y, FROG_SIZE, FROG_SIZE, SSD1306_WHITE);
  // Draw simple eyes
  display.drawPixel(frog.x + 1, frog.y + 1, SSD1306_BLACK);
  display.drawPixel(frog.x + 3, frog.y + 1, SSD1306_BLACK);
}

void FroggerGame::drawCars() {
  for (int i = 0; i < MAX_CARS; i++) {
    if (cars[i].active) {
      display.fillRect(cars[i].x, cars[i].y, 8, 6, SSD1306_WHITE);
      // Draw windows
      display.drawPixel(cars[i].x + 2, cars[i].y + 2, SSD1306_BLACK);
      display.drawPixel(cars[i].x + 6, cars[i].y + 2, SSD1306_BLACK);
    }
  }
}

void FroggerGame::drawLogs() {
  for (int i = 0; i < MAX_LOGS; i++) {
    if (logs[i].active) {
      display.fillRect(logs[i].x, logs[i].y, logs[i].width, 6, SSD1306_WHITE);
      // Draw log texture
      for (int x = logs[i].x; x < logs[i].x + logs[i].width; x += 4) {
        display.drawPixel(x, logs[i].y + 1, SSD1306_BLACK);
        display.drawPixel(x + 2, logs[i].y + 4, SSD1306_BLACK);
      }
    }
  }
}

void FroggerGame::drawRoad() {
  // Draw safe zones
  display.drawLine(0, SAFE_ZONE_HEIGHT, SCREEN_WIDTH, SAFE_ZONE_HEIGHT, SSD1306_WHITE);
  display.drawLine(0, SCREEN_HEIGHT - SAFE_ZONE_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - SAFE_ZONE_HEIGHT, SSD1306_WHITE);
  
  // Draw lane dividers
  for (int i = 1; i < ROAD_LANES; i++) {
    int y = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT - (i * LANE_HEIGHT);
    for (int x = 0; x < SCREEN_WIDTH; x += 8) {
      display.drawPixel(x, y, SSD1306_WHITE);
    }
  }
}

void FroggerGame::drawUI() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Score
  display.setCursor(0, 0);
  display.print(F("Score:"));
  display.print(score);
  
  // Lives
  display.setCursor(SCREEN_WIDTH - 30, 0);
  display.print(F("Lives:"));
  display.print(lives);
}