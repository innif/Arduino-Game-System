#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "config.h"

#define HELI_SIZE 4
#define CAVE_SEGMENTS 32
#define HELI_X 15

struct Helicopter {
  float y;
  float velocity;
};

struct CaveSegment {
  int topHeight;
  int bottomHeight;
  int gapHeight;
};

class HelicopterGame {
private:
  Helicopter heli;
  CaveSegment cave[CAVE_SEGMENTS];
  int score;
  bool gameOver;
  unsigned long lastUpdate;
  float gameSpeed;
  float gravity;
  float lift;
  int caveOffset;
  
  void generateCave();
  void updateHelicopter();
  void updateCave();
  bool checkCollisions();
  void drawHelicopter();
  void drawCave();
  void drawUI();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return score; }
  const char* getName() { return "HELICOPTER"; }
};

extern HelicopterGame helicopterGame;

#endif