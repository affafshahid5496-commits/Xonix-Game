#include "SaveGame.h"
#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <ctime>

SaveGameManager::SaveGameManager(const std::string& directory)
    : savesDirectory(directory) {
    // Create saves directory if it doesn't exist
    mkdir(savesDirectory.c_str(), 0755);
}

std::string SaveGameManager::getSaveFilePath(const std::string& username) const {
    return savesDirectory + "/" + username + "_save.dat";
}

bool SaveGameManager::saveGame(const GameSave& save) {
    std::string filepath = getSaveFilePath(save.username);
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        std::cout << "Error: Could not save game for " << save.username << std::endl;
        return false;
    }
    
    file << save.serialize() << std::endl;
    file.flush();  // Ensure data is written to disk
    file.close();
    
    std::cout << "Game saved successfully for " << save.username << std::endl;
    return true;
}

bool SaveGameManager::loadGame(const std::string& username, GameSave& outSave) {
    std::string filepath = getSaveFilePath(username);
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cout << "No saved game found for " << username << std::endl;
        return false;
    }
    
    std::string line;
    if (std::getline(file, line)) {
        file.close();
        return outSave.deserialize(line);
    }
    
    file.close();
    return false;
}

bool SaveGameManager::deleteSave(const std::string& username) {
    std::string filepath = getSaveFilePath(username);
    if (std::remove(filepath.c_str()) == 0) {
        std::cout << "Save game deleted for " << username << std::endl;
        return true;
    }
    return false;
}

bool SaveGameManager::hasSave(const std::string& username) const {
    std::string filepath = getSaveFilePath(username);
    std::ifstream file(filepath);
    return file.good();
}

bool SaveGameManager::saveGameRecord(const std::string& username, int score, int difficulty) {
    // Save to records file: {username}_records.txt
    std::string recordsPath = savesDirectory + "/" + username + "_records.txt";
    
    // Get current timestamp
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    char timestamp[80];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M", timeinfo);
    
    // Convert difficulty to string
    std::string difficultyStr;
    switch (difficulty) {
        case 0: difficultyStr = "Easy"; break;
        case 1: difficultyStr = "Medium"; break;
        case 2: difficultyStr = "Hard"; break;
        default: difficultyStr = "Unknown"; break;
    }
    
    // Append record: username|score|difficulty|timestamp|savefile
    std::ofstream file(recordsPath, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Error: Could not save game record for " << username << std::endl;
        return false;
    }
    
    std::string savefile = username + "_save.dat";
    file << username << "|" << score << "|" << difficultyStr << "|" 
         << timestamp << "|" << savefile << std::endl;
    file.flush();  // Ensure data is written to disk
    file.close();
    
    std::cout << "Game record saved for " << username << std::endl;
    return true;
}

bool SaveGameManager::deleteGameRecord(const std::string& username) {
    std::string recordsPath = savesDirectory + "/" + username + "_records.txt";
    std::ifstream inputFile(recordsPath);
    
    if (!inputFile.is_open()) {
        std::cout << "No records file found for " << username << std::endl;
        return false;
    }
    
    // Create temporary file to store all records except the last one
    std::string tempPath = recordsPath + ".tmp";
    std::ofstream tempFile(tempPath);
    
    if (!tempFile.is_open()) {
        std::cout << "Error: Could not create temporary records file" << std::endl;
        inputFile.close();
        return false;
    }
    
    std::string line;
    std::string lastLine;
    
    // Read all lines except the last one (most recent save)
    while (std::getline(inputFile, line)) {
        if (!line.empty()) {
            if (!lastLine.empty()) {
                tempFile << lastLine << std::endl;
            }
            lastLine = line;
        }
    }
    
    inputFile.close();
    tempFile.close();
    
    // Replace original file with temp file (removing the last record)
    if (std::remove(recordsPath.c_str()) == 0) {
        if (std::rename(tempPath.c_str(), recordsPath.c_str()) == 0) {
            std::cout << "Game record deleted for " << username << std::endl;
            return true;
        }
    }
    
    // Clean up temp file if rename failed
    std::remove(tempPath.c_str());
    return false;
}

