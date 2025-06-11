#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "config.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512
#define HIGHSCORE_MAGIC 0xABCD  // Magic number to verify valid data
#define HIGHSCORE_BASE_ADDR 0

struct HighscoreData {
  uint16_t magic;
  int scores[MAX_GAMES];
  uint16_t checksum;
};

class HighscoreManager {
private:
  HighscoreData data;
  bool initialized;
  
  uint16_t calculateChecksum();
  bool isValidData();
  void resetToDefaults();
  
public:
  void init();
  void saveHighscore(int gameId, int score);
  int getHighscore(int gameId);
  bool isNewHighscore(int gameId, int score);
  void saveToEEPROM();
  void loadFromEEPROM();
  void resetAllHighscores();
};

extern HighscoreManager highscoreManager;

void initHighscores();
int getGameHighscore(int gameId);
bool checkNewHighscore(int gameId, int score);
void saveGameHighscore(int gameId, int score);

#endif