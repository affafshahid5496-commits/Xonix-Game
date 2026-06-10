#include "Matchmaking.h"
#include <iostream>
#include <chrono>
#include <cstring>

Matchmaking::Matchmaking() : roomCount(0), nextRoomId(1), lastCreatedRoomId(-1), leaderboard(nullptr)
{
    // Initialize all room pointers to null
    for (int i = 0; i < 100; i++) {
        activeRooms[i] = nullptr;
    }
}

Matchmaking::~Matchmaking()
{
    cleanup();
}

// Helper function to get the correct queue based on difficulty
std::priority_queue<MatchPlayer>& Matchmaking::getQueue(Levels::Difficulty difficulty)
{
    switch (difficulty) {
        case Levels::EASY:
            return easyQueue;
        case Levels::MEDIUM:
            return mediumQueue;
        case Levels::HARD:
            return hardQueue;
        default:
            return mediumQueue;  // Default to medium
    }
}

void Matchmaking::addPlayerToQueue(Player* player, int leaderboardRank, Levels::Difficulty difficulty)
{
    if (!player) {
        std::cout << "Error: Cannot add null player to queue.\n";
        return;
    }
    
    // Check if player is already in queue
    if (isPlayerInQueue(player->getUsername())) {
        std::cout << "Player " << player->getUsername() << " is already in the queue.\n";
        return;
    }
    
    long long joinTime = std::chrono::system_clock::now().time_since_epoch().count();
    std::priority_queue<MatchPlayer>& targetQueue = getQueue(difficulty);
    targetQueue.push(MatchPlayer(player, leaderboardRank, joinTime, difficulty));
    
    std::string difficultyStr;
    switch (difficulty) {
        case Levels::EASY: difficultyStr = "EASY"; break;
        case Levels::MEDIUM: difficultyStr = "MEDIUM"; break;
        case Levels::HARD: difficultyStr = "HARD"; break;
    }
    
    std::cout << "Player " << player->getUsername() << " added to " << difficultyStr 
              << " matchmaking queue (Rank: " << leaderboardRank << ")\n";
}

bool Matchmaking::removePlayerFromQueue(const std::string& username)
{
    bool found = false;
    
    // Try to remove from each queue
    for (int d = 0; d <= 2; d++) {
        Levels::Difficulty difficulty = static_cast<Levels::Difficulty>(d);
        std::priority_queue<MatchPlayer>& targetQueue = getQueue(difficulty);
        
        // Create a temporary queue to store all except the player to remove
        std::priority_queue<MatchPlayer> tempQueue;
        
        while (!targetQueue.empty()) {
            MatchPlayer mp = targetQueue.top();
            targetQueue.pop();
            
            if (mp.player->getUsername() != username) {
                tempQueue.push(mp);
            } else {
                found = true;
            }
        }
        
        // Swap the queues
        targetQueue = tempQueue;
    }
    
    if (found) {
        std::cout << "Player " << username << " removed from matchmaking queue.\n";
    }
    return found;
}

int Matchmaking::getQueueSize(Levels::Difficulty difficulty) const
{
    switch (difficulty) {
        case Levels::EASY:
            return easyQueue.size();
        case Levels::MEDIUM:
            return mediumQueue.size();
        case Levels::HARD:
            return hardQueue.size();
        default:
            return 0;
    }
}

int Matchmaking::getTotalQueueSize() const
{
    return easyQueue.size() + mediumQueue.size() + hardQueue.size();
}

bool Matchmaking::isPlayerInQueue(const std::string& username) const
{
    // Check each difficulty queue
    for (int d = 0; d <= 2; d++) {
        Levels::Difficulty difficulty = static_cast<Levels::Difficulty>(d);
        
        // Get the appropriate queue
        const std::priority_queue<MatchPlayer>* targetQueue = nullptr;
        switch (difficulty) {
            case Levels::EASY:
                targetQueue = &easyQueue;
                break;
            case Levels::MEDIUM:
                targetQueue = &mediumQueue;
                break;
            case Levels::HARD:
                targetQueue = &hardQueue;
                break;
        }
        
        if (!targetQueue) continue;
        
        // We need to check without modifying the queue
        // Create a temporary copy to iterate
        std::priority_queue<MatchPlayer> tempQueue = *targetQueue;
        
        while (!tempQueue.empty()) {
            if (tempQueue.top().player->getUsername() == username) {
                return true;
            }
            tempQueue.pop();
        }
    }
    return false;
}

int Matchmaking::getPlayerLeaderboardRank(Player* player) const
{
    if (!leaderboard || !player) return -1;
    
    // Get player's score from leaderboard to determine rank
    // Assuming leaderboard has a method to get rank by username
    // For now, we'll use total score as a proxy for rank
    return player->getTotalScore();
}

void Matchmaking::processMatchmaking()
{
    // Process matchmaking for each difficulty level separately
    for (int d = 0; d <= 2; d++) {
        Levels::Difficulty difficulty = static_cast<Levels::Difficulty>(d);
        std::priority_queue<MatchPlayer>& targetQueue = getQueue(difficulty);
        
        // Match players from the same difficulty queue in pairs
        while (targetQueue.size() >= 2) {
            // Get two players with highest priority (best ranks)
            MatchPlayer player1 = targetQueue.top();
            targetQueue.pop();
            
            MatchPlayer player2 = targetQueue.top();
            targetQueue.pop();
            
            // Create a game room with the difficulty level
            GameRoom* room = createGameRoom(player1.player, player2.player, difficulty);
            
            if (room) {
                std::string difficultyStr;
                switch (difficulty) {
                    case Levels::EASY: difficultyStr = "EASY"; break;
                    case Levels::MEDIUM: difficultyStr = "MEDIUM"; break;
                    case Levels::HARD: difficultyStr = "HARD"; break;
                }
                
                std::cout << "Match created (" << difficultyStr << "): " 
                          << player1.player->getUsername() 
                          << " (Rank " << player1.leaderboardRank << ") vs " 
                          << player2.player->getUsername() 
                          << " (Rank " << player2.leaderboardRank << ") in Room " 
                          << room->roomId << "\n";
            }
        }
    }
}

GameRoom* Matchmaking::createGameRoom(Player* player1, Player* player2, Levels::Difficulty difficulty)
{
    if (!player1 || !player2) {
        std::cout << "Error: Cannot create room with null players.\n";
        return nullptr;
    }
    
    if (roomCount >= 100) {
        std::cout << "Error: Maximum number of game rooms reached.\n";
        return nullptr;
    }
    
    long long createdTime = std::chrono::system_clock::now().time_since_epoch().count();
    GameRoom* newRoom = new GameRoom(nextRoomId, player1, player2, createdTime, difficulty);
    lastCreatedRoomId = nextRoomId;  // Track the last created room ID
    nextRoomId++;
    
    activeRooms[roomCount] = newRoom;
    roomCount++;
    
    return newRoom;
}

GameRoom* Matchmaking::getGameRoom(int roomId) const
{
    for (int i = 0; i < roomCount; i++) {
        if (activeRooms[i] && activeRooms[i]->roomId == roomId) {
            return activeRooms[i];
        }
    }
    return nullptr;
}

GameRoom* Matchmaking::getLastCreatedRoom() const
{
    return getGameRoom(lastCreatedRoomId);
}

bool Matchmaking::closeGameRoom(int roomId)
{
    for (int i = 0; i < roomCount; i++) {
        if (activeRooms[i] && activeRooms[i]->roomId == roomId) {
            activeRooms[i]->isActive = false;
            delete activeRooms[i];
            activeRooms[i] = nullptr;
            
            // Shift remaining rooms
            for (int j = i; j < roomCount - 1; j++) {
                activeRooms[j] = activeRooms[j + 1];
            }
            roomCount--;
            
            std::cout << "Game room " << roomId << " closed.\n";
            return true;
        }
    }
    return false;
}

void Matchmaking::displayQueueStatus() const
{
    std::cout << "\n=== Matchmaking Queue Status ===\n";
    std::cout << "Total players waiting: " << getTotalQueueSize() << "\n";
    std::cout << "  - Easy: " << easyQueue.size() << "\n";
    std::cout << "  - Medium: " << mediumQueue.size() << "\n";
    std::cout << "  - Hard: " << hardQueue.size() << "\n";
    std::cout << "Active game rooms: " << roomCount << "\n";
    
    // Display waiting players by difficulty
    for (int d = 0; d <= 2; d++) {
        Levels::Difficulty difficulty = static_cast<Levels::Difficulty>(d);
        std::string difficultyStr;
        switch (difficulty) {
            case Levels::EASY: difficultyStr = "EASY"; break;
            case Levels::MEDIUM: difficultyStr = "MEDIUM"; break;
            case Levels::HARD: difficultyStr = "HARD"; break;
        }
        
        const std::priority_queue<MatchPlayer>* targetQueue = nullptr;
        switch (difficulty) {
            case Levels::EASY:
                targetQueue = &easyQueue;
                break;
            case Levels::MEDIUM:
                targetQueue = &mediumQueue;
                break;
            case Levels::HARD:
                targetQueue = &hardQueue;
                break;
        }
        
        if (targetQueue && targetQueue->size() > 0) {
            std::cout << "\n" << difficultyStr << " Queue (in priority order):\n";
            
            std::priority_queue<MatchPlayer> tempQueue = *targetQueue;
            int position = 1;
            
            while (!tempQueue.empty()) {
                MatchPlayer mp = tempQueue.top();
                tempQueue.pop();
                std::cout << "  " << position << ". " << mp.player->getUsername() 
                          << " (Rank: " << mp.leaderboardRank << ")\n";
                position++;
            }
        }
    }
    
    if (roomCount > 0) {
        std::cout << "\nActive game rooms:\n";
        for (int i = 0; i < roomCount; i++) {
            if (activeRooms[i]) {
                std::string difficultyStr;
                switch (activeRooms[i]->difficulty) {
                    case Levels::EASY: difficultyStr = "EASY"; break;
                    case Levels::MEDIUM: difficultyStr = "MEDIUM"; break;
                    case Levels::HARD: difficultyStr = "HARD"; break;
                }
                std::cout << "Room " << activeRooms[i]->roomId << " (" << difficultyStr << "): " 
                          << activeRooms[i]->player1->getUsername() << " vs " 
                          << activeRooms[i]->player2->getUsername() << "\n";
            }
        }
    }
    std::cout << "================================\n\n";
}

void Matchmaking::cleanup()
{
    // Clear all queues
    while (!easyQueue.empty()) {
        easyQueue.pop();
    }
    while (!mediumQueue.empty()) {
        mediumQueue.pop();
    }
    while (!hardQueue.empty()) {
        hardQueue.pop();
    }
    
    // Delete all active rooms
    for (int i = 0; i < roomCount; i++) {
        if (activeRooms[i]) {
            delete activeRooms[i];
            activeRooms[i] = nullptr;
        }
    }
    roomCount = 0;
    nextRoomId = 1;
}
