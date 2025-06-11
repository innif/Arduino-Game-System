#include "game2048.h"
#include "display.h"
#include "input.h"

Game2048 game2048;

void Game2048::init() {
  // Clear grid
  for (int y = 0; y < GRID_SIZE_2048; y++) {
    for (int x = 0; x < GRID_SIZE_2048; x++) {
      grid[y][x] = 0;
    }
  }
  
  score = 0;
  gameOver = false;
  hasWon = false;
  moved = false;
  
  // Add two initial tiles
  addRandomTile();
  addRandomTile();
}

void Game2048::update() {
  handleInput(); // Add input handling to update!
  
  if (moved) {
    addRandomTile();
    moved = false;
    
    if (!canMove()) {
      gameOver = true;
    }
  }
}

void Game2048::draw() {
  clearDisplay();
  
  // Draw score (top left, above the grid)
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Score:"));
  display.setCursor(0, 10);
  display.print(score);
  
  // Draw grid
  for (int y = 0; y < GRID_SIZE_2048; y++) {
    for (int x = 0; x < GRID_SIZE_2048; x++) {
      drawTile(x, y, grid[y][x]);
    }
  }
  
  // Draw win message (right side)
  if (hasWon) {
    display.setCursor(0, 25);
    display.print(F("WIN!"));
  }
  
  updateDisplay();
}

void Game2048::handleInput() {
  if (buttons.leftPressed) {
    moveLeft();
  }
  else if (buttons.rightPressed) {
    moveRight();
  }
  else if (buttons.upPressed) {
    moveUp();
  }
  else if (buttons.downPressed) {
    moveDown();
  }
}

void Game2048::addRandomTile() {
  // Find empty cells
  int emptyCells[16][2];
  int count = 0;
  
  for (int y = 0; y < GRID_SIZE_2048; y++) {
    for (int x = 0; x < GRID_SIZE_2048; x++) {
      if (grid[y][x] == 0) {
        emptyCells[count][0] = x;
        emptyCells[count][1] = y;
        count++;
      }
    }
  }
  
  if (count > 0) {
    int index = random(0, count);
    int x = emptyCells[index][0];
    int y = emptyCells[index][1];
    
    // 90% chance for 2, 10% chance for 4
    grid[y][x] = (random(0, 10) < 9) ? 2 : 4;
  }
}

bool Game2048::canMove() {
  // Check for empty cells
  for (int y = 0; y < GRID_SIZE_2048; y++) {
    for (int x = 0; x < GRID_SIZE_2048; x++) {
      if (grid[y][x] == 0) return true;
    }
  }
  
  // Check for possible merges
  for (int y = 0; y < GRID_SIZE_2048; y++) {
    for (int x = 0; x < GRID_SIZE_2048; x++) {
      int value = grid[y][x];
      if ((x < GRID_SIZE_2048 - 1 && grid[y][x + 1] == value) ||
          (y < GRID_SIZE_2048 - 1 && grid[y + 1][x] == value)) {
        return true;
      }
    }
  }
  
  return false;
}

void Game2048::moveLeft() {
  bool hasMoved = false;
  
  for (int y = 0; y < GRID_SIZE_2048; y++) {
    int line[GRID_SIZE_2048];
    for (int x = 0; x < GRID_SIZE_2048; x++) {
      line[x] = grid[y][x];
    }
    
    if (mergeLine(line, GRID_SIZE_2048)) {
      hasMoved = true;
      for (int x = 0; x < GRID_SIZE_2048; x++) {
        grid[y][x] = line[x];
      }
    }
  }
  
  moved = hasMoved;
}

void Game2048::moveRight() {
  bool hasMoved = false;
  
  for (int y = 0; y < GRID_SIZE_2048; y++) {
    int line[GRID_SIZE_2048];
    for (int x = 0; x < GRID_SIZE_2048; x++) {
      line[x] = grid[y][GRID_SIZE_2048 - 1 - x];
    }
    
    if (mergeLine(line, GRID_SIZE_2048)) {
      hasMoved = true;
      for (int x = 0; x < GRID_SIZE_2048; x++) {
        grid[y][GRID_SIZE_2048 - 1 - x] = line[x];
      }
    }
  }
  
  moved = hasMoved;
}

void Game2048::moveUp() {
  bool hasMoved = false;
  
  for (int x = 0; x < GRID_SIZE_2048; x++) {
    int line[GRID_SIZE_2048];
    for (int y = 0; y < GRID_SIZE_2048; y++) {
      line[y] = grid[y][x];
    }
    
    if (mergeLine(line, GRID_SIZE_2048)) {
      hasMoved = true;
      for (int y = 0; y < GRID_SIZE_2048; y++) {
        grid[y][x] = line[y];
      }
    }
  }
  
  moved = hasMoved;
}

void Game2048::moveDown() {
  bool hasMoved = false;
  
  for (int x = 0; x < GRID_SIZE_2048; x++) {
    int line[GRID_SIZE_2048];
    for (int y = 0; y < GRID_SIZE_2048; y++) {
      line[y] = grid[GRID_SIZE_2048 - 1 - y][x];
    }
    
    if (mergeLine(line, GRID_SIZE_2048)) {
      hasMoved = true;
      for (int y = 0; y < GRID_SIZE_2048; y++) {
        grid[GRID_SIZE_2048 - 1 - y][x] = line[y];
      }
    }
  }
  
  moved = hasMoved;
}

bool Game2048::mergeLine(int line[], int size) {
  bool moved = false;
  
  // Compress (move all non-zero elements to the left)
  int writePos = 0;
  for (int i = 0; i < size; i++) {
    if (line[i] != 0) {
      if (writePos != i) {
        line[writePos] = line[i];
        line[i] = 0;
        moved = true;
      }
      writePos++;
    }
  }
  
  // Merge
  for (int i = 0; i < size - 1; i++) {
    if (line[i] != 0 && line[i] == line[i + 1]) {
      line[i] *= 2;
      line[i + 1] = 0;
      score += line[i];
      moved = true;
      
      // Check for win condition
      if (line[i] == 2048) {
        hasWon = true;
      }
    }
  }
  
  // Compress again after merging
  writePos = 0;
  for (int i = 0; i < size; i++) {
    if (line[i] != 0) {
      if (writePos != i) {
        line[writePos] = line[i];
        line[i] = 0;
        moved = true;
      }
      writePos++;
    }
  }
  
  return moved;
}

void Game2048::drawTile(int x, int y, int value) {
  int screenX = BOARD_OFFSET_X + x * (TILE_SIZE + TILE_MARGIN);
  int screenY = BOARD_OFFSET_Y + y * (TILE_SIZE + TILE_MARGIN);
  
  // Draw tile background
  if (value == 0) {
    display.drawRect(screenX, screenY, TILE_SIZE, TILE_SIZE, SSD1306_WHITE);
  } else {
    display.fillRect(screenX, screenY, TILE_SIZE, TILE_SIZE, SSD1306_WHITE);
    
    // Draw value
    display.setTextSize(1);
    display.setTextColor(SSD1306_BLACK);
    
    // Center the text
    int textWidth = getTileWidth(value);
    int textX = screenX + (TILE_SIZE - textWidth) / 2;
    int textY = screenY + (TILE_SIZE - 8) / 2;
    
    display.setCursor(textX, textY);
    if (value >= 1000) {
      display.print(value / 1000);
      display.print(F("k"));
    } else {
      display.print(value);
    }
  }
}

int Game2048::getTileWidth(int value) {
  if (value < 10) return 6;
  if (value < 100) return 12;
  if (value < 1000) return 18;
  return 12; // For "1k", "2k", etc.
}