#ifndef PACMAN_H
#define PACMAN_H

#include "config.h"

#define MAZE_WIDTH 16
#define MAZE_HEIGHT 8
#define CELL_SIZE 8
#define MAX_GHOSTS 2
#define TOTAL_DOTS 66

struct PacMan {
  int x, y;
  int direction; // 0=right, 1=down, 2=left, 3=up
  int nextDirection;
};

struct Ghost {
  int x, y;
  int direction;
  bool active;
  unsigned long lastMove;
};

class PacManGame {
private:
  PacMan pacman;
  Ghost ghosts[MAX_GHOSTS];
  uint8_t maze[MAZE_HEIGHT][MAZE_WIDTH];
  int score;
  int dotsEaten;
  bool gameOver;
  bool gameWon;
  unsigned long lastMove;
  unsigned long lastGhostMove;
  int moveDelay;
  
  void generateMaze();
  void updatePacMan();
  void updateGhosts();
  bool isValidMove(int x, int y);
  bool checkGhostCollision();
  void drawMaze();
  void drawPacMan();
  void drawGhosts();
  void drawUI();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  bool isGameWon() { return gameWon; }
  int getScore() { return score; }
  const char* getName() { return "PAC-MAN"; }
};

extern PacManGame pacmanGame;

#endif
