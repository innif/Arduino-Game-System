#ifndef GAME2048_H
#define GAME2048_H

#include "config.h"

#define GRID_SIZE_2048 4
#define TILE_SIZE 14
#define TILE_MARGIN 2
#define BOARD_OFFSET_X 32
#define BOARD_OFFSET_Y 0

class Game2048 {
private:
  int grid[GRID_SIZE_2048][GRID_SIZE_2048];
  int score;
  bool gameOver;
  bool hasWon;
  bool moved;
  
  void addRandomTile();
  bool canMove();
  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();
  bool mergeLine(int line[], int size);
  void rotateCW();
  void rotateCCW();
  void drawTile(int x, int y, int value);
  int getTileWidth(int value);
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  bool isGameWon() { return hasWon; }
  int getScore() { return score; }
  const char* getName() { return "2048"; }
};

extern Game2048 game2048;

#endif