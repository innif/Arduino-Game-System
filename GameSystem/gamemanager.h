#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "config.h"
#include "snake.h"
#include "tetris.h"
#include "flappy.h"

class GameManager {
private:
  GameState currentState;
  int currentGame;
  int menuSelection;
  const char* gameNames[MAX_GAMES] = {"SNAKE", "TETRIS", "FLAPPY BIRD"};
  
public:
  void init();
  void update();
  void setState(GameState newState);
  void setGame(int gameId);
  void showMenu();
  void handleMenuInput();
  void showGameOver();
  void handleGameOverInput();
  int getCurrentGame() { return currentGame; }
  GameState getState() { return currentState; }
};

extern GameManager gameManager;

void initGameManager();
void updateGameManager();

#endif