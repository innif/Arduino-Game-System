#include "tetris.h"
#include "display.h"
#include "input.h"

TetrisGame tetrisGame;

// Tetris piece shapes (4x4 grid for each rotation)
const uint8_t tetrisPieces[7][4][4][4] = {
  // I piece
  {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
   {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
   {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},
   {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}},
  // O piece
  {{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
   {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
   {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
   {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}},
  // T piece
  {{{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
   {{0,0,0,0},{0,1,0,0},{0,1,1,0},{0,1,0,0}},
   {{0,0,0,0},{0,0,0,0},{1,1,1,0},{0,1,0,0}},
   {{0,0,0,0},{0,1,0,0},{1,1,0,0},{0,1,0,0}}},
  // S piece
  {{{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
   {{0,0,0,0},{0,1,0,0},{0,1,1,0},{0,0,1,0}},
   {{0,0,0,0},{0,0,0,0},{0,1,1,0},{1,1,0,0}},
   {{0,0,0,0},{1,0,0,0},{1,1,0,0},{0,1,0,0}}},
  // Z piece
  {{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
   {{0,0,0,0},{0,0,1,0},{0,1,1,0},{0,1,0,0}},
   {{0,0,0,0},{0,0,0,0},{1,1,0,0},{0,1,1,0}},
   {{0,0,0,0},{0,1,0,0},{1,1,0,0},{1,0,0,0}}},
  // J piece
  {{{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
   {{0,0,0,0},{0,1,1,0},{0,1,0,0},{0,1,0,0}},
   {{0,0,0,0},{0,0,0,0},{1,1,1,0},{0,0,1,0}},
   {{0,0,0,0},{0,1,0,0},{0,1,0,0},{1,1,0,0}}},
  // L piece
  {{{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}},
   {{0,0,0,0},{0,1,0,0},{0,1,0,0},{0,1,1,0}},
   {{0,0,0,0},{0,0,0,0},{1,1,1,0},{1,0,0,0}},
   {{0,0,0,0},{1,1,0,0},{0,1,0,0},{0,1,0,0}}}
};

void TetrisGame::init() {
  // Clear board
  for (int y = 0; y < TETRIS_HEIGHT; y++) {
    for (int x = 0; x < TETRIS_WIDTH; x++) {
      board[y][x] = 0;
    }
  }
  
  score = 0;
  level = 1;
  linesCleared = 0;
  dropSpeed = 500;
  lastDropTime = 0;
  gameOver = false;
  
  spawnNewPiece();
}

void TetrisGame::update() {
  handleInput();
  
  if (millis() - lastDropTime > dropSpeed) {
    if (!movePiece(0, 1, 0)) {
      placePiece();
      clearLines();
      spawnNewPiece();
      
      if (!isValidPosition(currentPiece.x, currentPiece.y, currentPiece.type, currentPiece.rotation)) {
        gameOver = true;
      }
    }
    lastDropTime = millis();
  }
}

void TetrisGame::draw() {
  clearDisplay();
  
  // Draw border
  display.drawRect(TETRIS_OFFSET_X - 1, TETRIS_OFFSET_Y - 1, 
                   TETRIS_FIELD_WIDTH + 2, TETRIS_FIELD_HEIGHT + 2, 
                   SSD1306_WHITE);
  
  // Draw board
  for (int y = 0; y < TETRIS_HEIGHT; y++) {
    for (int x = 0; x < TETRIS_WIDTH; x++) {
      if (board[y][x]) {
        int screenX = TETRIS_OFFSET_X + x * BLOCK_SIZE;
        int screenY = TETRIS_OFFSET_Y + y * BLOCK_SIZE;
        display.fillRect(screenX, screenY, BLOCK_SIZE, BLOCK_SIZE, SSD1306_WHITE);
      }
    }
  }
  
  // Draw current piece
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (tetrisPieces[currentPiece.type][currentPiece.rotation][py][px]) {
        int screenX = TETRIS_OFFSET_X + (currentPiece.x + px) * BLOCK_SIZE;
        int screenY = TETRIS_OFFSET_Y + (currentPiece.y + py) * BLOCK_SIZE;
        if (screenY >= TETRIS_OFFSET_Y) {
          display.fillRect(screenX, screenY, BLOCK_SIZE, BLOCK_SIZE, SSD1306_WHITE);
        }
      }
    }
  }
  
  // Draw score
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int scoreX = TETRIS_OFFSET_X + TETRIS_FIELD_WIDTH + 10;
  display.setCursor(scoreX, 5);
  display.println(F("Score:"));
  display.setCursor(scoreX, 15);
  display.println(score);
  
  display.setCursor(scoreX, 30);
  display.println(F("Level:"));
  display.setCursor(scoreX, 40);
  display.println(level);
  
  display.setCursor(scoreX, 55);
  display.print(F("L:"));
  display.print(linesCleared);
  
  updateDisplay();
}

void TetrisGame::handleInput() {
  if (buttons.leftPressed) {
    movePiece(-1, 0, 0);
  }
  else if (buttons.rightPressed) {
    movePiece(1, 0, 0);
  }
  else if (buttons.downPressed) {
    movePiece(0, 1, 0);
  }
  else if (buttons.upPressed) {
    movePiece(0, 0, 1);
  }
}

void TetrisGame::spawnNewPiece() {
  currentPiece.x = TETRIS_WIDTH / 2 - 2;
  currentPiece.y = 0;
  currentPiece.type = random(0, 7);
  currentPiece.rotation = 0;
}

bool TetrisGame::movePiece(int dx, int dy, int dr) {
  int newX = currentPiece.x + dx;
  int newY = currentPiece.y + dy;
  int newRotation = (currentPiece.rotation + dr) % 4;
  
  if (isValidPosition(newX, newY, currentPiece.type, newRotation)) {
    currentPiece.x = newX;
    currentPiece.y = newY;
    currentPiece.rotation = newRotation;
    return true;
  }
  return false;
}

bool TetrisGame::isValidPosition(int x, int y, int type, int rotation) {
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (tetrisPieces[type][rotation][py][px]) {
        int boardX = x + px;
        int boardY = y + py;
        
        if (boardX < 0 || boardX >= TETRIS_WIDTH || 
            boardY >= TETRIS_HEIGHT || 
            (boardY >= 0 && board[boardY][boardX])) {
          return false;
        }
      }
    }
  }
  return true;
}

void TetrisGame::placePiece() {
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (tetrisPieces[currentPiece.type][currentPiece.rotation][py][px]) {
        int boardX = currentPiece.x + px;
        int boardY = currentPiece.y + py;
        
        if (boardY >= 0) {
          board[boardY][boardX] = 1;
        }
      }
    }
  }
}

void TetrisGame::clearLines() {
  int linesClearedNow = 0;
  
  for (int y = TETRIS_HEIGHT - 1; y >= 0; y--) {
    bool fullLine = true;
    for (int x = 0; x < TETRIS_WIDTH; x++) {
      if (!board[y][x]) {
        fullLine = false;
        break;
      }
    }
    
    if (fullLine) {
      // Move all lines above down
      for (int moveY = y; moveY > 0; moveY--) {
        for (int x = 0; x < TETRIS_WIDTH; x++) {
          board[moveY][x] = board[moveY - 1][x];
        }
      }
      
      // Clear top line
      for (int x = 0; x < TETRIS_WIDTH; x++) {
        board[0][x] = 0;
      }
      
      linesClearedNow++;
      y++; // Check same line again
    }
  }
  
  if (linesClearedNow > 0) {
    score += linesClearedNow * 100 * level;
    linesCleared += linesClearedNow;
    
    // Increase level every 10 lines
    if (linesCleared >= level * 10) {
      level++;
      dropSpeed = max(50, dropSpeed - 50);
    }
  }
}