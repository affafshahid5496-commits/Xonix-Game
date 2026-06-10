// Player.cpp
#include "Player.h"
#include <iostream>

// Constructor implementations
// Constructor implementations - ADD highestScore(0)
Player::Player() : playerId(0), totalScore(0), gamesPlayed(0), gamesWon(0), 
                   currentThemeId(1), isOnline(false), highestScore(0) {}

Player::Player(const std::string& username, const std::string& password,
               const std::string& nickname, const std::string& email, int id)
    : username(username), password(password), nickname(nickname), email(email),
      playerId(id), totalScore(0), gamesPlayed(0), gamesWon(0), 
      currentThemeId(1), isOnline(false), highestScore(0) {}

Player::~Player() {
    // Cleanup handled by std::string destructors
}

// Getter implementations
std::string Player::getUsername() const { return username; }
std::string Player::getPassword() const { return password; }
std::string Player::getNickname() const { return nickname; }
std::string Player::getEmail() const { return email; }
int Player::getPlayerId() const { return playerId; }
int Player::getTotalScore() const { return totalScore; }
int Player::getGamesPlayed() const { return gamesPlayed; }
int Player::getGamesWon() const { return gamesWon; }
int Player::getCurrentThemeId() const { return currentThemeId; }
bool Player::getIsOnline() const { return isOnline; }

float Player::getWinRate() const {
    if (gamesPlayed == 0) return 0.0f;
    return (static_cast<float>(gamesWon) / gamesPlayed) * 100.0f;
}

LinkedList& Player::getFriendsList() {
    return friendsList;
}

// Setter implementations
void Player::setPassword(const std::string& newPassword) {
    password = newPassword;
}

void Player::setNickname(const std::string& newNickname) {
    nickname = newNickname;
}

void Player::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Player::setTotalScore(int score) {
    totalScore = score;
}

void Player::setCurrentThemeId(int themeId) {
    currentThemeId = themeId;
}
void Player::setGamesPlayed(int n) {
    gamesPlayed = n;
}
void Player::setIsOnline(bool online) {
    isOnline = online;
}
int Player::getHighestScore() const { 
    return highestScore; 
}

void Player::setHighestScore(int score) {
    highestScore = score;
}

void Player::updateHighestScore(int score) {
    if (score > highestScore) {
        highestScore = score;
        gamesWon++;
    }
}
void Player::addGamePlayed(bool won) {
    
}
void Player::setGamesWon(int wins) {
    gamesWon = wins;
}
void Player::updateScore(int points) {
    totalScore += points;
    if (totalScore < 0) totalScore = 0; // Ensure score doesn't go negative
}

// Friend management
bool Player::addFriend(const std::string& friendUsername) {
    if (friendUsername.empty() || friendUsername == username) {
        return false;
    }
    
    if (friendsList.contains(friendUsername)) {
        return false; // Already friends
    }
    
    friendsList.append(friendUsername);
    return true;
}

bool Player::removeFriend(const std::string& friendUsername) {
    return friendsList.remove(friendUsername);
}

bool Player::isFriend(const std::string& friendUsername) const {
    return friendsList.contains(friendUsername);
}

void Player::displayFriends() const {
    std::cout << "Friends list for " << username << ":" << std::endl;
    friendsList.display();
}

// Validation methods
bool Player::validateUsername(const std::string& username) {
    if (username.empty() || username.length() < 3 || username.length() > 20) {
        return false;
    }
    
    // Check if username contains only alphanumeric characters
    for (char c : username) {
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool Player::validatePassword(const std::string& password) {
    // Minimum 6 characters, at least one letter and one number
    if (password.length() < 6) {
        return false;
    }
    
    bool hasLetter = false;
    bool hasDigit = false;
    
    for (char c : password) {
        if (isalpha(c)) hasLetter = true;
        if (isdigit(c)) hasDigit = true;
    }
    
    return hasLetter && hasDigit;
}

bool Player::validateEmail(const std::string& email) {
    if (email.empty()) return true; // Email is optional
    
    size_t at_pos = email.find('@');
    size_t dot_pos = email.find('.', at_pos);
    
    return (at_pos != std::string::npos && 
            dot_pos != std::string::npos && 
            dot_pos > at_pos + 1 && 
            dot_pos < email.length() - 1);
}
/*
std::string Player::serialize() const {
    std::string result;
    
    // Serialize all player fields using pipe delimiter - ADD highestScore
    result += std::to_string(playerId) + "|";
    result += username + "|";
    result += password + "|";
    result += nickname + "|";
    result += email + "|";
    result += std::to_string(totalScore) + "|";
    result += std::to_string(gamesPlayed) + "|";
    result += std::to_string(gamesWon) + "|";
    result += std::to_string(currentThemeId) + "|";
    result += std::string(isOnline ? "1" : "0") + "|";
    result += std::to_string(highestScore);  // Add highest score
    
    // Serialize friends list
    if (!friendsList.isEmpty()) {
        ListNode* current = friendsList.getHead();
        while (current != nullptr) {
            result += "|" + std::string(current->data);
            current = current->next;
        }
    }
    
    return result;
}

bool Player::deserialize(const std::string& data) {
    // Manual parsing without vector
    const int MAX_TOKENS = 50;
    std::string tokens[MAX_TOKENS];
    int tokenCount = 0;
    std::string currentToken;
    
    // Manual parsing
    for (char c : data) {
        if (c == '|') {
            if (tokenCount < MAX_TOKENS) {
                tokens[tokenCount] = currentToken;
                tokenCount++;
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }
    
    // Don't forget the last token
    if (!currentToken.empty() && tokenCount < MAX_TOKENS) {
        tokens[tokenCount] = currentToken;
        tokenCount++;
    }
    
    if (tokenCount < 11) {  // Changed from 10 to 11 for new field
        return false; // Invalid data format
    }
    
    // Parse all fields (existing code remains the same until...)
    playerId = 0;
    for (char c : tokens[0]) {
        if (c >= '0' && c <= '9') playerId = playerId * 10 + (c - '0');
        else return false;
    }
    
    username = tokens[1];
    password = tokens[2];
    nickname = tokens[3];
    email = tokens[4];
    
    totalScore = 0;
    for (char c : tokens[5]) {
        if (c >= '0' && c <= '9') totalScore = totalScore * 10 + (c - '0');
        else return false;
    }
    
    gamesPlayed = 0;
    for (char c : tokens[6]) {
        if (c >= '0' && c <= '9') gamesPlayed = gamesPlayed * 10 + (c - '0');
        else return false;
    }
    
    gamesWon = 0;
    for (char c : tokens[7]) {
        if (c >= '0' && c <= '9') gamesWon = gamesWon * 10 + (c - '0');
        else return false;
    }
    
    currentThemeId = 0;
    for (char c : tokens[8]) {
        if (c >= '0' && c <= '9') currentThemeId = currentThemeId * 10 + (c - '0');
        else return false;
    }
    
    isOnline = (tokens[9] == "1");
    
    // Parse the new highestScore field - ADD THIS SECTION
    highestScore = 0;
    for (char c : tokens[10]) {
        if (c >= '0' && c <= '9') highestScore = highestScore * 10 + (c - '0');
        else return false;
    }
    
    // Deserialize friends list (starting from token 11 now)
    friendsList.clear();
    for (int i = 11; i < tokenCount; i++) {
        if (!tokens[i].empty()) {
            friendsList.append(tokens[i]);
        }
    }
    
    return true;
}
*/
// Display profile - ADD highest score display
void Player::displayProfile() const {
    std::cout << "\n=== Player Profile ===" << std::endl;
    std::cout << "Username: " << username << std::endl;
    std::cout << "Nickname: " << nickname << std::endl;
    std::cout << "Email: " << (email.empty() ? "Not set" : email) << std::endl;
    std::cout << "Player ID: " << playerId << std::endl;
    std::cout << "Total Score: " << totalScore << std::endl;
    std::cout << "Highest Score: " << highestScore << std::endl;  // Add this line
    std::cout << "Games Played: " << getGamesPlayed() << std::endl;
    std::cout << "Games Won: " << gamesWon << std::endl;
    std::cout << "Win Rate: " << std::fixed << getWinRate() << "%" << std::endl;
    std::cout << "Current Theme: " << currentThemeId << std::endl;
    std::cout << "Status: " << (isOnline ? "Online" : "Offline") << std::endl;
    
    std::cout << "Friends: ";
    if (friendsList.isEmpty()) {
        std::cout << "No friends yet" << std::endl;
    } else {
        std::cout << friendsList.getSize() << " friend(s)" << std::endl;
        friendsList.display();
    }
    std::cout << "=====================" << std::endl;
}




std::string Player::serialize() const {
    std::string result;
    
    // Serialize all player fields using pipe delimiter
    result += std::to_string(playerId) + "|";
    result += username + "|";
    result += password + "|";
    result += nickname + "|";
    result += email + "|";
    result += std::to_string(totalScore) + "|";
    result += std::to_string(gamesPlayed) + "|";
    result += std::to_string(gamesWon) + "|";
    result += std::to_string(currentThemeId) + "|";
    result += std::string(isOnline ? "1" : "0") + "|";
    result += std::to_string(highestScore);  // Highest score
    
    
    return result;
}

bool Player::deserialize(const std::string& data) {
    // Manual parsing without vector
    const int MAX_TOKENS = 50;
    std::string tokens[MAX_TOKENS];
    int tokenCount = 0;
    std::string currentToken;
    
    // Manual parsing
    for (char c : data) {
        if (c == '|') {
            if (tokenCount < MAX_TOKENS) {
                tokens[tokenCount] = currentToken;
                tokenCount++;
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }
    
    // Don't forget the last token
    if (!currentToken.empty() && tokenCount < MAX_TOKENS) {
        tokens[tokenCount] = currentToken;
        tokenCount++;
    }
    
    // We need exactly 11 tokens (all basic fields, no friends)
    if (tokenCount != 11) {
        return false; // Invalid data format - expecting exactly 11 fields
    }
    
    // Parse all fields
    playerId = 0;
    for (char c : tokens[0]) {
        if (c >= '0' && c <= '9') playerId = playerId * 10 + (c - '0');
        else return false;
    }
    
    username = tokens[1];
    password = tokens[2];
    nickname = tokens[3];
    email = tokens[4];
    
    totalScore = 0;
    for (char c : tokens[5]) {
        if (c >= '0' && c <= '9') totalScore = totalScore * 10 + (c - '0');
        else return false;
    }
    
    gamesPlayed = 0;
    for (char c : tokens[6]) {
        if (c >= '0' && c <= '9') gamesPlayed = gamesPlayed * 10 + (c - '0');
        else return false;
    }
    
    gamesWon = 0;
    for (char c : tokens[7]) {
        if (c >= '0' && c <= '9') gamesWon = gamesWon * 10 + (c - '0');
        else return false;
    }
    
    currentThemeId = 0;
    for (char c : tokens[8]) {
        if (c >= '0' && c <= '9') currentThemeId = currentThemeId * 10 + (c - '0');
        else return false;
    }
    
    isOnline = (tokens[9] == "1");
    
    // Parse the highestScore field
    highestScore = 0;
    for (char c : tokens[10]) {
        if (c >= '0' && c <= '9') highestScore = highestScore * 10 + (c - '0');
        else return false;
    }
    
    // Friends list is NOT deserialized - clear any existing friends
    friendsList.clear();
    
    return true;
}