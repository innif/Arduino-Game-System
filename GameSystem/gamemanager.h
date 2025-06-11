#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "config.h"
#include "snake.h"
#include "tetris.h"
#include "flappy.h"
#include "game2048.h"
#include "breakout.h"
#include "frogger.h"
#include "helicopter.h"
#include "pacman.h"
#include "highscore.h"

class GameManager {
private:
  GameState currentState;
  int currentGame;
  int menuSelection;
  int menuScroll; // For scrolling menu
  const char* gameNames[MAX_GAMES] = {"SNAKE", "TETRIS", "FLAPPY BIRD", "2048", "BREAKOUT", "FROGGER", "HELICOPTER", "PAC-MAN"};
  static const int VISIBLE_MENU_ITEMS = 3;
  bool newHighscore; // Flag for new highscore
  
public:
  void init();
  void update();
  void setState(GameState newState);
  void setGame(int gameId);
  void showMenu();
  void handleMenuInput();
  void showGameOver();
  void showGameWon();
  void handleGameOverInput();
  void handleGameWonInput();
  int getCurrentGame() { return currentGame; }
  GameState getState() { return currentState; }
};

extern GameManager gameManager;

void initGameManager();
void updateGameManager();

#endif