#ifndef FROGGER_H
#define FROGGER_H

#include "config.h"

#define FROG_SIZE 4
#define ROAD_LANES 3
#define RIVER_LANES 3
#define MAX_CARS 8
#define MAX_LOGS 6
#define LANE_HEIGHT 8
#define SAFE_ZONE_HEIGHT 6

struct Frog {
  int x, y;
  bool onLog;
  int logIndex;
};

struct Car {
  int x, y;
  int speed;
  bool active;
  bool direction; // true = right, false = left
};

struct Log {
  int x, y;
  int width;
  int speed;
  bool active;
};

class FroggerGame {
private:
  Frog frog;
  Car cars[MAX_CARS];
  Log logs[MAX_LOGS];
  int score;
  int lives;
  bool gameOver;
  unsigned long lastUpdate;
  unsigned long lastSpawn;
  int highestY;
  
  void resetFrog();
  void updateFrog();
  void updateCars();
  void updateLogs();
  void spawnCar();
  void spawnLog();
  bool checkCarCollision();
  bool checkWaterCollision();
  void drawFrog();
  void drawCars();
  void drawLogs();
  void drawRoad();
  void drawUI();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return score; }
  const char* getName() { return "FROGGER"; }
};

extern FroggerGame froggerGame;

#endif