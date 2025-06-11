#include "breakout.h"
#include "display.h"
#include "input.h"

BreakoutGame breakoutGame;

void BreakoutGame::init() {
  // Initialize paddle
  paddle.x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
  
  // Initialize bricks
  for (int row = 0; row < BRICK_ROWS; row++) {
    for (int col = 0; col < BRICK_COLS; col++) {
      bricks[row][col] = true;
    }
  }
  
  score = 0;
  lives = 3;
  gameOver = false;
  gameWon = false;
  lastUpdate = millis();
  
  resetBall();
}

void BreakoutGame::update() {
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastUpdate) / 16.0;
  lastUpdate = currentTime;
  
  if (gameOver || gameWon) return;
  
  handleInput(); // NOW called in update!
  updatePaddle();
  updateBall();
  
  // Check win condition
  if (countRemainingBricks() == 0) {
    gameWon = true;
    gameOver = true;
  }
  
  // Check lose condition
  if (lives <= 0) {
    gameOver = true;
  }
}

void BreakoutGame::draw() {
  clearDisplay();
  
  drawBricks();
  drawPaddle();
  drawBall();
  drawUI();
  
  if (gameWon) {
    drawCenteredText("YOU WIN!", 30, 1);
  }
  
  updateDisplay();
}

void BreakoutGame::handleInput() {
  // Paddle movement - continuous input for smooth control
  if (buttons.left && paddle.x > 0) {
    paddle.x -= 2;
  }
  if (buttons.right && paddle.x < SCREEN_WIDTH - PADDLE_WIDTH) {
    paddle.x += 2;
  }
}

void BreakoutGame::resetBall() {
  ball.x = SCREEN_WIDTH / 2;
  ball.y = PADDLE_Y - 10;
  
  // Random angle upward
  float angle = random(-45, 46) * PI / 180.0;
  ball.velX = sin(angle) * 2.0;
  ball.velY = -abs(cos(angle)) * 2.0; // Always upward
}

void BreakoutGame::updateBall() {
  ball.x += ball.velX;
  ball.y += ball.velY;
  
  // Wall collisions (left/right)
  if (ball.x <= 0 || ball.x >= SCREEN_WIDTH - BALL_SIZE) {
    ball.velX = -ball.velX;
    ball.x = constrain(ball.x, 0, SCREEN_WIDTH - BALL_SIZE);
  }
  
  // Top wall collision
  if (ball.y <= 0) {
    ball.velY = -ball.velY;
    ball.y = 0;
  }
  
  // Bottom collision (lose life)
  if (ball.y >= SCREEN_HEIGHT) {
    lives--;
    if (lives > 0) {
      resetBall();
    }
    return;
  }
  
  // Paddle collision
  if (checkBallPaddleCollision()) {
    ball.velY = -abs(ball.velY); // Always bounce upward
    
    // Add horizontal velocity based on where ball hits paddle
    float hitPos = (ball.x + BALL_SIZE/2 - paddle.x) / PADDLE_WIDTH;
    ball.velX = (hitPos - 0.5) * 3.0;
    
    // Limit maximum speed
    if (abs(ball.velX) > 2.5) ball.velX = (ball.velX > 0) ? 2.5 : -2.5;
  }
  
  // Brick collision
  checkBallBrickCollision();
}

void BreakoutGame::updatePaddle() {
  // Paddle is updated in handleInput()
}

bool BreakoutGame::checkBallPaddleCollision() {
  return (ball.x + BALL_SIZE >= paddle.x &&
          ball.x <= paddle.x + PADDLE_WIDTH &&
          ball.y + BALL_SIZE >= PADDLE_Y &&
          ball.y <= PADDLE_Y + PADDLE_HEIGHT &&
          ball.velY > 0); // Only when ball is moving down
}

bool BreakoutGame::checkBallBrickCollision() {
  int ballCenterX = ball.x + BALL_SIZE / 2;
  int ballCenterY = ball.y + BALL_SIZE / 2;
  
  for (int row = 0; row < BRICK_ROWS; row++) {
    for (int col = 0; col < BRICK_COLS; col++) {
      if (!bricks[row][col]) continue;
      
      int brickX = col * BRICK_WIDTH + (SCREEN_WIDTH - BRICK_COLS * BRICK_WIDTH) / 2;
      int brickY = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + 1);
      
      // Check collision
      if (ball.x + BALL_SIZE >= brickX &&
          ball.x <= brickX + BRICK_WIDTH &&
          ball.y + BALL_SIZE >= brickY &&
          ball.y <= brickY + BRICK_HEIGHT) {
        
        // Remove brick
        bricks[row][col] = false;
        score += (BRICK_ROWS - row) * 10; // Higher rows worth more points
        
        // Determine bounce direction
        float ballCenterX = ball.x + BALL_SIZE / 2;
        float ballCenterY = ball.y + BALL_SIZE / 2;
        float brickCenterX = brickX + BRICK_WIDTH / 2;
        float brickCenterY = brickY + BRICK_HEIGHT / 2;
        
        float deltaX = ballCenterX - brickCenterX;
        float deltaY = ballCenterY - brickCenterY;
        
        // Bounce based on which side was hit
        if (abs(deltaX) > abs(deltaY)) {
          ball.velX = -ball.velX; // Side collision
        } else {
          ball.velY = -ball.velY; // Top/bottom collision
        }
        
        return true;
      }
    }
  }
  
  return false;
}

void BreakoutGame::drawBall() {
  display.fillRect(ball.x, ball.y, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);
}

void BreakoutGame::drawPaddle() {
  display.fillRect(paddle.x, PADDLE_Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
}

void BreakoutGame::drawBricks() {
  for (int row = 0; row < BRICK_ROWS; row++) {
    for (int col = 0; col < BRICK_COLS; col++) {
      if (bricks[row][col]) {
        int x = col * BRICK_WIDTH + (SCREEN_WIDTH - BRICK_COLS * BRICK_WIDTH) / 2;
        int y = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + 1);
        
        display.fillRect(x, y, BRICK_WIDTH - 1, BRICK_HEIGHT, SSD1306_WHITE);
      }
    }
  }
}

void BreakoutGame::drawUI() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Score
  display.setCursor(0, 0);
  display.print(F("Score:"));
  display.print(score);
  
  // Lives
  display.setCursor(SCREEN_WIDTH - 45, 0);
  display.print(F("Lives:"));
  display.print(lives);
}

int BreakoutGame::countRemainingBricks() {
  int count = 0;
  for (int row = 0; row < BRICK_ROWS; row++) {
    for (int col = 0; col < BRICK_COLS; col++) {
      if (bricks[row][col]) count++;
    }
  }
  return count;
}