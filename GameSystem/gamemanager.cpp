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
  menuScroll = 0;
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
          
        case GAME_2048:
          game2048.update();
          game2048.draw();
          if (game2048.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_BREAKOUT:
          breakoutGame.update();
          breakoutGame.draw();
          if (breakoutGame.isGameOver()) {
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
    case GAME_2048:
      game2048.init();
      break;
    case GAME_BREAKOUT:
      breakoutGame.init();
      break;
  }
  
  setState(STATE_PLAYING);
}

void GameManager::showMenu() {
  clearDisplay();
  
  drawCenteredText("GAMES", 5, 2);
  
  display.setTextSize(1);
  
  // Calculate which items to show
  int startItem = menuScroll;
  int endItem = min(startItem + VISIBLE_MENU_ITEMS, MAX_GAMES);
  
  for (int i = 0; i < VISIBLE_MENU_ITEMS && (startItem + i) < MAX_GAMES; i++) {
    int gameIndex = startItem + i;
    int y = 25 + i * 15;
    
    if (menuSelection == gameIndex) {
      display.fillRect(5, y - 2, SCREEN_WIDTH - 10, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    
    display.setCursor(10, y);
    display.println(gameNames[gameIndex]);
  }
  
  // Show scroll indicators if needed
  if (menuScroll > 0) {
    // Up arrow
    display.setCursor(SCREEN_WIDTH - 10, 20);
    display.setTextColor(SSD1306_WHITE);
    display.print(F("^"));
  }
  
  if (menuScroll + VISIBLE_MENU_ITEMS < MAX_GAMES) {
    // Down arrow
    display.setCursor(SCREEN_WIDTH - 10, 55);
    display.setTextColor(SSD1306_WHITE);
    display.print(F("v"));
  }
  
  updateDisplay();
}

void GameManager::handleMenuInput() {
  if (buttons.upPressed) {
    menuSelection = (menuSelection - 1 + MAX_GAMES) % MAX_GAMES;
    
    // Adjust scroll for wrap-around
    if (menuSelection == MAX_GAMES - 1) {
      // Wrapped from first to last - scroll to show last items
      menuScroll = MAX_GAMES - VISIBLE_MENU_ITEMS;
    } else if (menuSelection < menuScroll) {
      // Normal upward scroll
      menuScroll = menuSelection;
    }
    
    showMenu();
  }
  else if (buttons.downPressed) {
    menuSelection = (menuSelection + 1) % MAX_GAMES;
    
    // Adjust scroll for wrap-around
    if (menuSelection == 0) {
      // Wrapped from last to first - scroll to top
      menuScroll = 0;
    } else if (menuSelection >= menuScroll + VISIBLE_MENU_ITEMS) {
      // Normal downward scroll
      menuScroll = menuSelection - VISIBLE_MENU_ITEMS + 1;
    }
    
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
    case GAME_2048: score = game2048.getScore(); break;
    case GAME_BREAKOUT: score = breakoutGame.getScore(); break;
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