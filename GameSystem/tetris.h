#ifndef TETRIS_H
#define TETRIS_H

#include "config.h"

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 16
#define BLOCK_SIZE 3
#define TETRIS_FIELD_WIDTH (TETRIS_WIDTH * BLOCK_SIZE)
#define TETRIS_FIELD_HEIGHT (TETRIS_HEIGHT * BLOCK_SIZE)
#define TETRIS_OFFSET_X ((SCREEN_WIDTH - TETRIS_FIELD_WIDTH - 50) / 2)
#define TETRIS_OFFSET_Y ((SCREEN_HEIGHT - TETRIS_FIELD_HEIGHT) / 2)

struct TetrisPiece {
  int x, y;
  int type;
  int rotation;
};

class TetrisGame {
private:
  uint8_t board[TETRIS_HEIGHT][TETRIS_WIDTH];
  TetrisPiece currentPiece;
  unsigned long lastDropTime;
  int dropSpeed;
  int score;
  int level;
  int linesCleared;
  bool gameOver;
  
  void spawnNewPiece();
  bool movePiece(int dx, int dy, int dr);
  bool isValidPosition(int x, int y, int type, int rotation);
  void placePiece();
  void clearLines();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return score; }
  const char* getName() { return "TETRIS"; }
};

extern TetrisGame tetrisGame;

#endif