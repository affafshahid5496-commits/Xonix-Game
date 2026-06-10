#pragma once

#include <string>
#include <queue>
#include "Player.h"
#include "Leaderboard.h"
#include "Levels.h"

struct MatchPlayer {
    Player* player;
    int leaderboardRank;
    long long queueJoinTime;
    Levels::Difficulty difficulty;
    
    MatchPlayer(Player* p, int rank, long long joinTime, Levels::Difficulty diff = Levels::MEDIUM)
        : player(p), leaderboardRank(rank), queueJoinTime(joinTime), difficulty(diff) {}
    
    // For sorting in priority queue (lower rank = higher priority = better player)
    bool operator<(const MatchPlayer& other) const {
        // If ranks are different, lower rank has higher priority
        if (leaderboardRank != other.leaderboardRank) {
            return leaderboardRank > other.leaderboardRank; // Min-heap behavior
        }
        // If ranks are same, earlier join time has higher priority (FIFO)
        return queueJoinTime > other.queueJoinTime;
    }
};

struct GameRoom {
    int roomId;
    Player* player1;
    Player* player2;
    bool isActive;
    long long createdTime;
    Levels::Difficulty difficulty;  // Store the difficulty level for this room
    
    GameRoom(int id, Player* p1, Player* p2, long long time, Levels::Difficulty diff = Levels::MEDIUM)
        : roomId(id), player1(p1), player2(p2), isActive(true), createdTime(time), difficulty(diff) {}
};

class Matchmaking {
private:
    // Separate queues for each difficulty level
    std::priority_queue<MatchPlayer> easyQueue;
    std::priority_queue<MatchPlayer> mediumQueue;
    std::priority_queue<MatchPlayer> hardQueue;
    
    GameRoom* activeRooms[100];  // Store up to 100 active game rooms
    int roomCount;
    int nextRoomId;
    int lastCreatedRoomId;  // Track the last created room for easy access
    Leaderboard* leaderboard;
    
public:
    Matchmaking();
    ~Matchmaking();
    
    // Queue management
    void addPlayerToQueue(Player* player, int leaderboardRank, Levels::Difficulty difficulty = Levels::MEDIUM);
    bool removePlayerFromQueue(const std::string& username);
    int getQueueSize(Levels::Difficulty difficulty = Levels::MEDIUM) const;
    int getTotalQueueSize() const;
    bool isPlayerInQueue(const std::string& username) const;
    
    // Matchmaking
    void processMatchmaking();
    GameRoom* createGameRoom(Player* player1, Player* player2, Levels::Difficulty difficulty = Levels::MEDIUM);
    
    // Room management
    GameRoom* getGameRoom(int roomId) const;
    GameRoom* getLastCreatedRoom() const;  // Get the most recently created room
    bool closeGameRoom(int roomId);
    int getActiveRoomCount() const { return roomCount; }
    
    // Leaderboard integration
    void setLeaderboard(Leaderboard* lb) { leaderboard = lb; }
    int getPlayerLeaderboardRank(Player* player) const;
    
    // Utility
    void displayQueueStatus() const;
    void cleanup();
    
private:
    // Helper functions for accessing the correct queue
    std::priority_queue<MatchPlayer>& getQueue(Levels::Difficulty difficulty);
};
