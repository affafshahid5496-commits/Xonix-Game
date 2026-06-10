#pragma once

#include "LinkedList.h"
#include <string>

class Player
{
private:
    std::string username;
    std::string password; // In real implementation, this should be hashed
    std::string nickname;
    std::string email;
    int playerId;
    int totalScore;
    int gamesPlayed;
    int gamesWon;
    int currentThemeId;
    LinkedList friendsList; // Stores usernames of friends
    bool isOnline;          // Add to Player class private members:
    int highestScore;

    // Add to public methods:

public:
    // Constructors & Destructor
    Player();
    Player(const std::string &username, const std::string &password,
           const std::string &nickname, const std::string &email, int id);
    ~Player();

    // Getters
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getNickname() const;
    std::string getEmail() const;
    int getPlayerId() const;
    int getTotalScore() const;
    int getGamesPlayed() const;
    int getGamesWon() const;
    int getCurrentThemeId() const;
    bool getIsOnline() const;
    float getWinRate() const;
    LinkedList &getFriendsList();
    void setGamesPlayed(int n);
    int getHighestScore() const;
    void setHighestScore(int score);
    void setGamesWon(int wins);
    void updateHighestScore(int score); // New method to update if new score is higher
    // Setters
    void setPassword(const std::string &newPassword);
    void setNickname(const std::string &newNickname);
    void setEmail(const std::string &newEmail);
    void setTotalScore(int score);
    void setCurrentThemeId(int themeId);
    void setIsOnline(bool online);

    // Game statistics
    void addGamePlayed(bool won);
    void updateScore(int points);

    // Friend management
    bool addFriend(const std::string &friendUsername);
    bool removeFriend(const std::string &friendUsername);
    bool isFriend(const std::string &friendUsername) const;
    void displayFriends() const;

    // Validation
    static bool validateUsername(const std::string &username);
    static bool validatePassword(const std::string &password);
    static bool validateEmail(const std::string &email);

    // Serialization for file I/O
    std::string serialize() const;
    bool deserialize(const std::string &data);

    // Display
    void displayProfile() const;
};
