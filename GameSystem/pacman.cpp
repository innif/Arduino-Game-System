#include "pacman.h"
#include "display.h"
#include "input.h"

PacManGame pacmanGame;

// Simple maze layout (0=wall, 1=dot, 2=empty, 3=pacman start, 4=ghost start)
const uint8_t mazeLayout[MAZE_HEIGHT][MAZE_WIDTH] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,1,0,1,0,0,1,0,0,1,0,0,1,0,1,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,1,1,1,4,1,1,3,1,1,1,1,1,1,1,0},
  {0,1,0,1,0,0,1,0,0,1,0,0,1,0,1,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

void PacManGame::init() {
  generateMaze();
  
  // Find Pac-Man and ghost start positions
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      if (mazeLayout[y][x] == 3) { // Pac-Man start
        pacman.x = x;
        pacman.y = y;
        pacman.direction = 0;
        pacman.nextDirection = 0;
        maze[y][x] = 2; // Empty space
      }
      else if (mazeLayout[y][x] == 4) { // Ghost start
        for (int i = 0; i < MAX_GHOSTS; i++) {
          if (!ghosts[i].active) {
            ghosts[i].x = x;
            ghosts[i].y = y;
            ghosts[i].direction = random(0, 4);
            ghosts[i].active = true;
            ghosts[i].lastMove = 0;
            maze[y][x] = 2; // Empty space
            break;
          }
        }
      }
    }
  }
  
  score = 0;
  dotsEaten = 0;
  gameOver = false;
  gameWon = false;
  lastMove = 0;
  lastGhostMove = 0;
  moveDelay = 300;
}

void PacManGame::update() {
  handleInput();
  
  unsigned long currentTime = millis();
  
  // Update Pac-Man
  if (currentTime - lastMove > moveDelay) {
    updatePacMan();
    lastMove = currentTime;
  }
  
  // Update ghosts (slower)
  if (currentTime - lastGhostMove > moveDelay + 100) {
    updateGhosts();
    lastGhostMove = currentTime;
  }
  
  // Check collisions
  if (checkGhostCollision()) {
    gameOver = true;
  }
  
  // Check win condition
  if (dotsEaten >= TOTAL_DOTS) {
    gameWon = true;
    gameOver = true;
  }
}

void PacManGame::draw() {
  clearDisplay();
  
  drawMaze();
  drawGhosts();
  drawPacMan();
  drawUI();
  
  updateDisplay();
}

void PacManGame::handleInput() {
  if (buttons.upPressed) {
    pacman.nextDirection = 3;
  }
  else if (buttons.downPressed) {
    pacman.nextDirection = 1;
  }
  else if (buttons.leftPressed) {
    pacman.nextDirection = 2;
  }
  else if (buttons.rightPressed) {
    pacman.nextDirection = 0;
  }
}

void PacManGame::generateMaze() {
  dotsEaten = 0;
  
  // Copy layout to maze
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      maze[y][x] = mazeLayout[y][x];
      if (maze[y][x] == 4) maze[y][x] = 2; // Convert ghost start to empty
    }
  }
  
  // Reset ghosts
  for (int i = 0; i < MAX_GHOSTS; i++) {
    ghosts[i].active = false;
  }
}

void PacManGame::updatePacMan() {
  // Try to change direction if requested
  int nextX = pacman.x;
  int nextY = pacman.y;
  
  switch (pacman.nextDirection) {
    case 0: nextX++; break; // right
    case 1: nextY++; break; // down
    case 2: nextX--; break; // left
    case 3: nextY--; break; // up
  }
  
  if (isValidMove(nextX, nextY)) {
    pacman.direction = pacman.nextDirection;
  }
  
  // Move in current direction
  switch (pacman.direction) {
    case 0: nextX = pacman.x + 1; nextY = pacman.y; break;
    case 1: nextX = pacman.x; nextY = pacman.y + 1; break;
    case 2: nextX = pacman.x - 1; nextY = pacman.y; break;
    case 3: nextX = pacman.x; nextY = pacman.y - 1; break;
  }
  
  if (isValidMove(nextX, nextY)) {
    pacman.x = nextX;
    pacman.y = nextY;
    
    // Eat dot
    if (maze[pacman.y][pacman.x] == 1) {
      maze[pacman.y][pacman.x] = 2;
      score += 10;
      dotsEaten++;
    }
  }
}

void PacManGame::updateGhosts() {
  for (int i = 0; i < MAX_GHOSTS; i++) {
    if (!ghosts[i].active) continue;
    
    // Simple AI: try to move towards Pac-Man, but sometimes random
    int bestDirection = ghosts[i].direction;
    
    if (random(0, 4) == 0) { // 25% chance of random movement
      bestDirection = random(0, 4);
    } else {
      // Move towards Pac-Man
      int dx = pacman.x - ghosts[i].x;
      int dy = pacman.y - ghosts[i].y;
      
      if (abs(dx) > abs(dy)) {
        bestDirection = (dx > 0) ? 0 : 2; // right or left
      } else {
        bestDirection = (dy > 0) ? 1 : 3; // down or up
      }
    }
    
    // Try the best direction first
    int nextX = ghosts[i].x;
    int nextY = ghosts[i].y;
    
    switch (bestDirection) {
      case 0: nextX++; break;
      case 1: nextY++; break;
      case 2: nextX--; break;
      case 3: nextY--; break;
    }
    
    if (isValidMove(nextX, nextY)) {
      ghosts[i].x = nextX;
      ghosts[i].y = nextY;
      ghosts[i].direction = bestDirection;
    } else {
      // Try other directions
      for (int dir = 0; dir < 4; dir++) {
        nextX = ghosts[i].x;
        nextY = ghosts[i].y;
        
        switch (dir) {
          case 0: nextX++; break;
          case 1: nextY++; break;
          case 2: nextX--; break;
          case 3: nextY--; break;
        }
        
        if (isValidMove(nextX, nextY)) {
          ghosts[i].x = nextX;
          ghosts[i].y = nextY;
          ghosts[i].direction = dir;
          break;
        }
      }
    }
  }
}

bool PacManGame::isValidMove(int x, int y) {
  if (x < 0 || x >= MAZE_WIDTH || y < 0 || y >= MAZE_HEIGHT) {
    return false;
  }
  return maze[y][x] != 0; // Not a wall
}

bool PacManGame::checkGhostCollision() {
  for (int i = 0; i < MAX_GHOSTS; i++) {
    if (ghosts[i].active && ghosts[i].x == pacman.x && ghosts[i].y == pacman.y) {
      return true;
    }
  }
  return false;
}

void PacManGame::drawMaze() {
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      int screenX = x * CELL_SIZE;
      int screenY = y * CELL_SIZE;
      
      if (maze[y][x] == 0) { // Wall
        display.fillRect(screenX, screenY, CELL_SIZE, CELL_SIZE, SSD1306_WHITE);
      }
      else if (maze[y][x] == 1) { // Dot
        display.fillCircle(screenX + CELL_SIZE/2, screenY + CELL_SIZE/2, 1, SSD1306_WHITE);
      }
    }
  }
}

void PacManGame::drawPacMan() {
  int screenX = pacman.x * CELL_SIZE + CELL_SIZE/2;
  int screenY = pacman.y * CELL_SIZE + CELL_SIZE/2;
  
  display.fillCircle(screenX, screenY, 3, SSD1306_WHITE);
  
  // Draw mouth based on direction
  switch (pacman.direction) {
    case 0: // right
      display.drawLine(screenX, screenY, screenX + 2, screenY - 1, SSD1306_BLACK);
      display.drawLine(screenX, screenY, screenX + 2, screenY + 1, SSD1306_BLACK);
      break;
    case 1: // down
      display.drawLine(screenX, screenY, screenX - 1, screenY + 2, SSD1306_BLACK);
      display.drawLine(screenX, screenY, screenX + 1, screenY + 2, SSD1306_BLACK);
      break;
    case 2: // left
      display.drawLine(screenX, screenY, screenX - 2, screenY - 1, SSD1306_BLACK);
      display.drawLine(screenX, screenY, screenX - 2, screenY + 1, SSD1306_BLACK);
      break;
    case 3: // up
      display.drawLine(screenX, screenY, screenX - 1, screenY - 2, SSD1306_BLACK);
      display.drawLine(screenX, screenY, screenX + 1, screenY - 2, SSD1306_BLACK);
      break;
  }
}

void PacManGame::drawGhosts() {
  for (int i = 0; i < MAX_GHOSTS; i++) {
    if (ghosts[i].active) {
      int screenX = ghosts[i].x * CELL_SIZE + CELL_SIZE/2;
      int screenY = ghosts[i].y * CELL_SIZE + CELL_SIZE/2;
      
      // Draw ghost body
      display.fillCircle(screenX, screenY - 1, 3, SSD1306_WHITE);
      display.fillRect(screenX - 3, screenY, 6, 3, SSD1306_WHITE);
      
      // Draw ghost bottom (wavy)
      display.drawPixel(screenX - 2, screenY + 3, SSD1306_WHITE);
      display.drawPixel(screenX, screenY + 2, SSD1306_WHITE);
      display.drawPixel(screenX + 2, screenY + 3, SSD1306_WHITE);
      
      // Draw eyes
      display.drawPixel(screenX - 1, screenY - 2, SSD1306_BLACK);
      display.drawPixel(screenX + 1, screenY - 2, SSD1306_BLACK);
    }
  }
}

void PacManGame::drawUI() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Score (bottom area)
  display.setCursor(0, SCREEN_HEIGHT - 8);
  display.print(F("Score: "));
  display.print(score);
  
  // Dots remaining
  display.setCursor(70, SCREEN_HEIGHT - 8);
  display.print(F("Dots: "));
  display.print(TOTAL_DOTS - dotsEaten);
}