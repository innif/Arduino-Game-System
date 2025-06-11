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
    if (currentTime - lastSpawn > 1500) { // Spawn every 1.5 seconds
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
  if (frog.onLog && frog.logIndex >= 0 && frog.logIndex < MAX_LOGS && logs[frog.logIndex].active) {
    frog.x += logs[frog.logIndex].speed;
    
    // Check if frog falls off log
    if (frog.x + FROG_SIZE < logs[frog.logIndex].x || 
        frog.x > logs[frog.logIndex].x + logs[frog.logIndex].width) {
      frog.onLog = false;
      frog.logIndex = -1;
    }
  }
  
  // Keep frog on screen
  if (frog.x < 0) frog.x = 0;
  if (frog.x > SCREEN_WIDTH - FROG_SIZE) frog.x = SCREEN_WIDTH - FROG_SIZE;
  
  // Debug info
  Serial.print("Frog: ");
  Serial.print(frog.x);
  Serial.print(",");
  Serial.print(frog.y);
  Serial.print(" OnLog: ");
  Serial.print(frog.onLog);
  Serial.print(" LogIndex: ");
  Serial.println(frog.logIndex);
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
      // Cars only in road area: bottom safe zone up to river
      int roadStartY = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT - (ROAD_LANES * LANE_HEIGHT);
      int roadEndY = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT;
      cars[i].y = roadStartY + (random(0, ROAD_LANES) * LANE_HEIGHT);
      
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
      // Logs only in river area: safe zone to safe zone + river lanes
      logs[i].y = SAFE_ZONE_HEIGHT + (random(0, RIVER_LANES) * LANE_HEIGHT);
      logs[i].width = random(20, 40); // Longer logs for easier gameplay
      logs[i].speed = random(1, 2); // Slower speed
      logs[i].x = -logs[i].width;
      break;
    }
  }
}

bool FroggerGame::checkCarCollision() {
  // Check if frog is in road area
  int roadStartY = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT - (ROAD_LANES * LANE_HEIGHT);
  int roadEndY = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT;
  
  if (frog.y < roadStartY || frog.y >= roadEndY) {
    return false; // Not in road area
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
    
    // First check if frog is on a log
    frog.onLog = false;
    frog.logIndex = -1;
    
    for (int i = 0; i < MAX_LOGS; i++) {
      if (logs[i].active) {
        // More generous collision detection for logs
        if (frog.x + FROG_SIZE > logs[i].x && 
            frog.x < logs[i].x + logs[i].width &&
            frog.y + FROG_SIZE > logs[i].y && 
            frog.y < logs[i].y + 6) {
          frog.onLog = true;
          frog.logIndex = i;
          return false; // Safe on log, no drowning
        }
      }
    }
    
    // If we reach here, frog is in water but not on any log
    return true; // Drowns
  }
  
  return false; // Not in water area
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
  
  // Draw middle line between river and road
  int middleY = SAFE_ZONE_HEIGHT + (RIVER_LANES * LANE_HEIGHT);
  display.drawLine(0, middleY, SCREEN_WIDTH, middleY, SSD1306_WHITE);
  
  // Draw road lane dividers
  int roadStartY = SCREEN_HEIGHT - SAFE_ZONE_HEIGHT - (ROAD_LANES * LANE_HEIGHT);
  for (int i = 1; i < ROAD_LANES; i++) {
    int y = roadStartY + (i * LANE_HEIGHT);
    for (int x = 0; x < SCREEN_WIDTH; x += 8) {
      display.drawPixel(x, y, SSD1306_WHITE);
    }
  }
  
  // Draw river lane dividers
  for (int i = 1; i < RIVER_LANES; i++) {
    int y = SAFE_ZONE_HEIGHT + (i * LANE_HEIGHT);
    for (int x = 0; x < SCREEN_WIDTH; x += 12) {
      display.drawPixel(x, y, SSD1306_WHITE);
      display.drawPixel(x + 2, y, SSD1306_WHITE);
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
  display.setCursor(SCREEN_WIDTH - 45, 0);
  display.print(F("Lives:"));
  display.print(lives);
}