#include "gamemanager.h"
#include "display.h"
#include "input.h"

GameManager gameManager;

void initGameManager() {
  gameManager.init();
}

void updateGameManager() {
  gameManager.update();
}

void GameManager::init() {
  currentState = STATE_MENU;
  currentGame = 0;
  menuSelection = 0;
  showMenu();
}

void GameManager::update() {
  switch (currentState) {
    case STATE_MENU:
      handleMenuInput();
      break;
      
    case STATE_PLAYING:
      switch (currentGame) {
        case GAME_SNAKE:
          snakeGame.update();
          snakeGame.draw();
          if (snakeGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_TETRIS:
          tetrisGame.update();
          tetrisGame.draw();
          if (tetrisGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_FLAPPY:
          flappyGame.update();
          flappyGame.draw();
          if (flappyGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
      }
      break;
      
    case STATE_GAME_OVER:
      handleGameOverInput();
      break;
  }
}

void GameManager::setState(GameState newState) {
  currentState = newState;
  
  switch (newState) {
    case STATE_MENU:
      showMenu();
      break;
    case STATE_GAME_OVER:
      showGameOver();
      break;
  }
}

void GameManager::setGame(int gameId) {
  currentGame = gameId;
  menuSelection = gameId;
  
  switch (gameId) {
    case GAME_SNAKE:
      snakeGame.init();
      break;
    case GAME_TETRIS:
      tetrisGame.init();
      break;
    case GAME_FLAPPY:
      flappyGame.init();
      break;
  }
  
  setState(STATE_PLAYING);
}

void GameManager::showMenu() {
  clearDisplay();
  
  drawCenteredText("GAMES", 5, 2);
  
  display.setTextSize(1);
  
  for (int i = 0; i < MAX_GAMES; i++) {
    int y = 25 + i * 15;
    
    if (menuSelection == i) {
      display.fillRect(5, y - 2, SCREEN_WIDTH - 10, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    
    display.setCursor(10, y);
    display.println(gameNames[i]);
  }
  
  updateDisplay();
}

void GameManager::handleMenuInput() {
  if (buttons.upPressed) {
    menuSelection = (menuSelection - 1 + MAX_GAMES) % MAX_GAMES;
    showMenu();
  }
  else if (buttons.downPressed) {
    menuSelection = (menuSelection + 1) % MAX_GAMES;
    showMenu();
  }
  else if (buttons.rightPressed) {
    setGame(menuSelection);
  }
}

void GameManager::showGameOver() {
  clearDisplay();
  
  drawCenteredText("GAME OVER", 5, 2);
  
  // Show score
  int score = 0;
  switch (currentGame) {
    case GAME_SNAKE: score = snakeGame.getScore(); break;
    case GAME_TETRIS: score = tetrisGame.getScore(); break;
    case GAME_FLAPPY: score = flappyGame.getScore(); break;
  }
  
  char scoreText[20];
  sprintf(scoreText, "Score: %d", score);
  drawCenteredText(scoreText, 28, 1);
  
  drawCenteredText("UP: Menu", 45, 1);
  drawCenteredText("RIGHT: Restart", 55, 1);
  
  updateDisplay();
}

void GameManager::handleGameOverInput() {
  if (buttons.upPressed) {
    setState(STATE_MENU);
  }
  else if (buttons.rightPressed) {
    setGame(currentGame);
  }
}