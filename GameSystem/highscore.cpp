#include "highscore.h"

HighscoreManager highscoreManager;

void initHighscores() {
  highscoreManager.init();
}

int getGameHighscore(int gameId) {
  return highscoreManager.getHighscore(gameId);
}

bool checkNewHighscore(int gameId, int score) {
  return highscoreManager.isNewHighscore(gameId, score);
}

void saveGameHighscore(int gameId, int score) {
  highscoreManager.saveHighscore(gameId, score);
}

void HighscoreManager::init() {
  EEPROM.begin(EEPROM_SIZE);
  loadFromEEPROM();
  initialized = true;
  
  Serial.println("Highscore system initialized");
  for (int i = 0; i < MAX_GAMES; i++) {
    Serial.print("Game ");
    Serial.print(i);
    Serial.print(" highscore: ");
    Serial.println(data.scores[i]);
  }
}

void HighscoreManager::loadFromEEPROM() {
  // Read data from EEPROM
  EEPROM.get(HIGHSCORE_BASE_ADDR, data);
  
  // Verify data integrity
  if (!isValidData()) {
    Serial.println("Invalid EEPROM data, resetting highscores");
    resetToDefaults();
    saveToEEPROM();
  } else {
    Serial.println("Highscores loaded from EEPROM");
  }
}

void HighscoreManager::saveToEEPROM() {
  data.magic = HIGHSCORE_MAGIC;
  data.checksum = calculateChecksum();
  
  EEPROM.put(HIGHSCORE_BASE_ADDR, data);
  EEPROM.commit();
  EEPROM.end();
  
  Serial.println("Highscores saved to EEPROM");
}

bool HighscoreManager::isValidData() {
  // Check magic number
  if (data.magic != HIGHSCORE_MAGIC) {
    Serial.println("Invalid magic number");
    return false;
  }
  
  // Check checksum
  uint16_t savedChecksum = data.checksum;
  data.checksum = 0; // Temporarily clear for calculation
  uint16_t calculatedChecksum = calculateChecksum();
  data.checksum = savedChecksum; // Restore
  
  if (savedChecksum != calculatedChecksum) {
    Serial.println("Checksum mismatch");
    // return false;
  }
  
  // Check if scores are reasonable (not negative, not too high)
  for (int i = 0; i < MAX_GAMES; i++) {
    if (data.scores[i] < 0 || data.scores[i] > 999999) {
        Serial.print("Invalid score for game ");
      //return false;
    }
  }
  
  return true;
}

uint16_t HighscoreManager::calculateChecksum() {
  uint16_t checksum = 0;
  uint8_t* ptr = (uint8_t*)&data;
  
  // Calculate checksum over everything except the checksum field
  for (int i = 0; i < sizeof(HighscoreData) - sizeof(uint16_t); i++) {
    checksum += ptr[i];
  }
  
  return checksum;
}

void HighscoreManager::resetToDefaults() {
  data.magic = HIGHSCORE_MAGIC;
  
  // Set default highscores
  for (int i = 0; i < MAX_GAMES; i++) {
    data.scores[i] = 0;
  }
  
  data.checksum = 0;
}

int HighscoreManager::getHighscore(int gameId) {
  if (gameId >= 0 && gameId < MAX_GAMES) {
    return data.scores[gameId];
  }
  return 0;
}

bool HighscoreManager::isNewHighscore(int gameId, int score) {
  if (gameId >= 0 && gameId < MAX_GAMES) {
    return score > data.scores[gameId];
  }
  return false;
}

void HighscoreManager::saveHighscore(int gameId, int score) {
  if (gameId >= 0 && gameId < MAX_GAMES && score > data.scores[gameId]) {
    data.scores[gameId] = score;
    saveToEEPROM();
    
    Serial.print("New highscore for game ");
    Serial.print(gameId);
    Serial.print(": ");
    Serial.println(score);
  }
}

void HighscoreManager::resetAllHighscores() {
  resetToDefaults();
  saveToEEPROM();
  Serial.println("All highscores reset");
}