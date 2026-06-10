#pragma once

#include <string>
#include <fstream>

struct GameSave {
    std::string username;
    int score;
    int playerX;
    int playerY;
    int playerDX;
    int playerDY;
    int powerUps;
    int enemyCount;
    int difficulty;  // 0 = Easy, 1 = Medium, 2 = Hard
    
    // Enemy positions (store up to 10 enemies)
    int enemyX[10];
    int enemyY[10];
    
    // Grid data (simplified - store as string)
    std::string gridData;
    
    GameSave() : username(""), score(0), playerX(0), playerY(0), playerDX(0), 
                 playerDY(0), powerUps(0), enemyCount(0), difficulty(1), gridData("") {
        for (int i = 0; i < 10; i++) {
            enemyX[i] = 0;
            enemyY[i] = 0;
        }
    }
    
    std::string serialize() const {
        std::string result = username + "|";
        result += std::to_string(score) + "|";
        result += std::to_string(playerX) + "|";
        result += std::to_string(playerY) + "|";
        result += std::to_string(playerDX) + "|";
        result += std::to_string(playerDY) + "|";
        result += std::to_string(powerUps) + "|";
        result += std::to_string(enemyCount) + "|";
        result += std::to_string(difficulty) + "|";
        
        // Serialize enemy positions
        for (int i = 0; i < 10; i++) {
            result += std::to_string(enemyX[i]) + ",";
            result += std::to_string(enemyY[i]) + "|";
        }
        
        result += gridData;
        return result;
    }
    
    bool deserialize(const std::string& data) {
        size_t pos = 0;
        
        // Parse pipe-delimited fields for basic game state
        for (int i = 0; i < 9; i++) {
            size_t nextPos = data.find('|', pos);
            if (nextPos == std::string::npos && i < 8) return false;
            
            std::string field = data.substr(pos, (nextPos != std::string::npos ? nextPos : data.length()) - pos);
            
            try {
                switch(i) {
                    case 0: username = field; break;
                    case 1: score = std::stoi(field); break;
                    case 2: playerX = std::stoi(field); break;
                    case 3: playerY = std::stoi(field); break;
                    case 4: playerDX = std::stoi(field); break;
                    case 5: playerDY = std::stoi(field); break;
                    case 6: powerUps = std::stoi(field); break;
                    case 7: enemyCount = std::stoi(field); break;
                    case 8: difficulty = std::stoi(field); break;
                }
            } catch(...) {
                return false;
            }
            
            if (nextPos != std::string::npos) {
                pos = nextPos + 1;
            } else {
                break;
            }
        }
        
        // Parse enemy positions (format: x1,y1|x2,y2|...)
        for (int i = 0; i < 10; i++) {
            size_t nextPipe = data.find('|', pos);
            if (nextPipe == std::string::npos) break;
            
            std::string enemyData = data.substr(pos, nextPipe - pos);
            size_t commaPos = enemyData.find(',');
            
            if (commaPos != std::string::npos) {
                try {
                    enemyX[i] = std::stoi(enemyData.substr(0, commaPos));
                    enemyY[i] = std::stoi(enemyData.substr(commaPos + 1));
                } catch(...) {
                    return false;
                }
            }
            
            pos = nextPipe + 1;
        }
        
        // Remaining data is gridData
        if (pos < data.length()) {
            gridData = data.substr(pos);
        }
        
        return !username.empty();
    }
};

class SaveGameManager {
private:
    std::string savesDirectory;
    
public:
    SaveGameManager(const std::string& directory = "saves");
    
    bool saveGame(const GameSave& save);
    bool loadGame(const std::string& username, GameSave& outSave);
    bool deleteSave(const std::string& username);
    bool hasSave(const std::string& username) const;
    bool saveGameRecord(const std::string& username, int score, int difficulty);
    bool deleteGameRecord(const std::string& username);
    std::string getSaveFilePath(const std::string& username) const;
};

