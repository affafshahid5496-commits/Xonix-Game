#pragma once
#include "Player.h"
#include "DArray.h"
#include "HashTable.h"
#include <string>
#include <fstream>

// Move constants outside the class
const int MAX_REQUESTS = 50;
const std::string REQUESTS_FILE = "friend_requests.txt";

class AuthSystem
{
private:
    DynamicArray players;       // Stores all player objects
    HashTable usernameToPlayer; // Fast lookup by username
    std::string dataFilePath;
    int nextPlayerId;
    Player *currentPlayer; // Currently logged in player

    struct FriendRequest
    {
        std::string sender;
        std::string receiver;
    };

    // Fixed-size array for pending requests
    FriendRequest *pendingRequests;
    int requestCount;

    // File operations
    bool loadPlayersFromFile();
    bool savePlayersToFile();
    void loadFriendRequests();
    void saveFriendRequests();

    // Helper methods
    bool isUsernameTaken(const std::string &username) const;
    int generatePlayerId();
    int stringToInt(const std::string &str) const;

public:
    AuthSystem(const std::string &filePath = "players_data.txt");
    ~AuthSystem();

    // Authentication operations
    bool registerPlayer(const std::string &username, const std::string &password,
                        const std::string &nickname = "", const std::string &email = "");
    bool login(const std::string &username, const std::string &password);
    void logout();
    bool changePassword(const std::string &username, const std::string &oldPassword,
                        const std::string &newPassword);

    // Player management
    Player *getPlayer(const std::string &username) const;
    Player *getCurrentPlayer() const;
    bool updatePlayerProfile(const std::string &username, const std::string &newNickname,
                             const std::string &newEmail);
    bool deletePlayer(const std::string &username, const std::string &password);

    // System operations
    void displayAllPlayers() const;
    int getTotalPlayers() const;
    bool saveSystemState();
    bool loadSystemState();
    bool savePlayerData(const std::string &username); // Save specific player to file
    bool updatePlayerGameStats(const std::string &username, int score, bool won);
    bool saveFriendListToFile(const std::string &username);
    bool loadFriendListFromFile(const std::string &username);
    void saveAllFriendLists();
    // Friend system operations
    bool sendFriendRequest(const std::string &fromUsername, const std::string &toUsername);
    bool removeFriend(const std::string &username, const std::string &friendUsername);
    void displayFriends(const std::string &username) const;
    bool acceptFriendRequest(const std::string &receiver, const std::string &sender);
    bool declineFriendRequest(const std::string &receiver, const std::string &sender);
    void displayPendingRequests(const std::string &username) const;
    void getPendingRequests(const std::string &username, std::string *outRequests, int &outCount) const;

    // Utility methods
    void displayCurrentPlayer() const;
};