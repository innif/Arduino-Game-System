#ifndef BREAKOUT_H
#define BREAKOUT_H

#include "config.h"

#define PADDLE_WIDTH 16
#define PADDLE_HEIGHT 3
#define PADDLE_Y (SCREEN_HEIGHT - 8)
#define BALL_SIZE 2
#define BRICK_WIDTH 12
#define BRICK_HEIGHT 4
#define BRICK_ROWS 5
#define BRICK_COLS 10
#define BRICK_OFFSET_Y 10

struct Ball {
  float x, y;
  float velX, velY;
};

struct Paddle {
  int x;
};

class BreakoutGame {
private:
  Ball ball;
  Paddle paddle;
  bool bricks[BRICK_ROWS][BRICK_COLS];
  int score;
  int lives;
  bool gameOver;
  bool gameWon;
  unsigned long lastUpdate;
  
  void resetBall();
  void updateBall();
  void updatePaddle();
  bool checkBallPaddleCollision();
  bool checkBallBrickCollision();
  void drawBall();
  void drawPaddle();
  void drawBricks();
  void drawUI();
  int countRemainingBricks();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  bool isGameWon() { return gameWon; }
  int getScore() { return score; }
  const char* getName() { return "BREAKOUT"; }
};

extern BreakoutGame breakoutGame;

#endif