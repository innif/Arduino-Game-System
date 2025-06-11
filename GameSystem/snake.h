#ifndef SNAKE_H
#define SNAKE_H

#include "config.h"

#define GRID_SIZE 4
#define GRID_WIDTH (SCREEN_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / GRID_SIZE)
#define MAX_SNAKE_LENGTH 50

struct Point {
  int x, y;
};

class SnakeGame {
private:
  Point snake[MAX_SNAKE_LENGTH];
  int snakeLength;
  Point food;
  int direction;
  int nextDirection;
  bool directionChanged;
  int score;
  unsigned long lastMoveTime;
  int gameSpeed;
  bool gameOver;
  
  void generateFood();
  void moveSnake();
  bool checkCollisions();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return score; }
  const char* getName() { return "SNAKE"; }
};

extern SnakeGame snakeGame;

#endif