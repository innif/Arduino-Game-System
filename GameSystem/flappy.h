#ifndef FLAPPY_H
#define FLAPPY_H

#include "config.h"

#define BIRD_SIZE 3
#define PIPE_WIDTH 8
#define PIPE_GAP 20
#define MAX_PIPES 3

struct Bird {
  float x, y;
  float velocity;
};

struct Pipe {
  int x;
  int gapY;
  bool passed;
};

class FlappyGame {
private:
  Bird bird;
  Pipe pipes[MAX_PIPES];
  int score;
  bool gameOver;
  unsigned long lastUpdate;
  unsigned long lastPipeSpawn;
  int pipeSpawnInterval;
  float gravity;
  float jumpStrength;
  float gameSpeed;
  
  void spawnPipe();
  void updateBird();
  void updatePipes();
  bool checkCollisions();
  void drawBird();
  void drawPipes();
  void drawGround();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return score; }
  const char* getName() { return "FLAPPY BIRD"; }
};

extern FlappyGame flappyGame;

#endif
