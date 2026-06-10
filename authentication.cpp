#include "authentication.h"
#include <iostream>
#include <fstream>

// Constructor
AuthSystem::AuthSystem(const std::string& filePath) 
    : dataFilePath(filePath), nextPlayerId(1), currentPlayer(nullptr), usernameToPlayer(50), 
      requestCount(0) {
    
    // Initialize friend requests array
    pendingRequests = new FriendRequest[MAX_REQUESTS];
    
    loadPlayersFromFile();
    loadFriendRequests();
}

// Destructor
AuthSystem::~AuthSystem() {
    savePlayersToFile();
    saveFriendRequests();
    
    // Clean up player objects
    for (int i = 0; i < players.getSize(); i++) {
        delete players.get(i);
    }
    
    // Clean up friend requests array
    delete[] pendingRequests;
}

// Helper method to convert string to int
int AuthSystem::stringToInt(const std::string& str) const {
    int result = 0;
    for (char c : str) {
        if (c >= '0' && c <= '9') {
            result = result * 10 + (c - '0');
        } else {
            return 0; // Invalid number
        }
    }
    return result;
}
bool AuthSystem::loadPlayersFromFile() {
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "No existing player data found. Starting fresh." << std::endl;
        return false;
    }
    
    std::string line;
    int loadedCount = 0;
    int maxId = 0;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        Player* player = new Player();
        if (player->deserialize(line)) {
            players.append(player);
            usernameToPlayer.insert(player->getUsername(), player);
            
            // Load friend list from separate file
            loadFriendListFromFile(player->getUsername());
            
            // Update nextPlayerId
            if (player->getPlayerId() > maxId) {
                maxId = player->getPlayerId();
            }
            
            loadedCount++;
        } else {
            delete player; // Clean up if deserialization fails
            std::cout << "Warning: Failed to deserialize player data: " << line << std::endl;
        }
    }
    
    file.close();
    nextPlayerId = maxId + 1;
    std::cout << "Loaded " << loadedCount << " players from file." << std::endl;
    return true;
}


/* File operations
bool AuthSystem::loadPlayersFromFile() {
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "No existing player data found. Starting fresh." << std::endl;
        return false;
    }
    
    std::string line;
    int loadedCount = 0;
    int maxId = 0;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        Player* player = new Player();
        if (player->deserialize(line)) {
            players.append(player);
            usernameToPlayer.insert(player->getUsername(), player);
            
            // Update nextPlayerId
            if (player->getPlayerId() > maxId) {
                maxId = player->getPlayerId();
            }
            
            loadedCount++;
        } else {
            delete player; // Clean up if deserialization fails
            std::cout << "Warning: Failed to deserialize player data: " << line << std::endl;
        }
    }
    
    file.close();
    nextPlayerId = maxId + 1;
    std::cout << "Loaded " << loadedCount << " players from file." << std::endl;
    return true;
}
*/
bool AuthSystem::savePlayersToFile() {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "Error: Could not save player data to file." << std::endl;
        return false;
    }
    
    int savedCount = 0;
    for (int i = 0; i < players.getSize(); i++) {
        Player* player = players.get(i);
        if (player) {
            file << player->serialize() << std::endl;
            savedCount++;
        }
    }
    
    file.close();
    std::cout << "Saved " << savedCount << " players to file." << std::endl;
    return true;
}
/*
// Save specific player data to file (updates entire file with all players)
bool AuthSystem::savePlayerData(const std::string& username) {
    Player* player = getPlayer(username);
    if (!player) {
        std::cout << "Error: Player not found." << std::endl;
        return false;
    }
    
    // Save all players to maintain data integrity
    return savePlayersToFile();
}*/
// Save specific player data to file (updates entire file with all players)
// Save specific player data to file (updates entire file with all players)
bool AuthSystem::savePlayerData(const std::string& username) {
    Player* player = getPlayer(username);
    if (!player) {
        std::cout << "Error: Player not found." << std::endl;
        return false;
    }
    
    // Save all players to maintain data integrity
    bool success = savePlayersToFile();
    if (success) {
        std::cout << "Player data saved successfully for: " << username << std::endl;
        std::cout << "Updated stats - Games Played: " << player->getGamesPlayed() 
                  << ", Games Won: " << player->getGamesWon() 
                  << ", Highest Score: " << player->getHighestScore() << std::endl;
    }
    return success;
}
// Helper methods
bool AuthSystem::isUsernameTaken(const std::string& username) const {
    return usernameToPlayer.get(username) != nullptr;
}

int AuthSystem::generatePlayerId() {
    return nextPlayerId++;
}

// Authentication operations
bool AuthSystem::registerPlayer(const std::string& username, const std::string& password,
                               const std::string& nickname, const std::string& email) {
    
    // Validate input
    if (!Player::validateUsername(username)) {
        std::cout << "Error: Username must be 3-20 characters long and contain only letters, numbers, and underscores." << std::endl;
        return false;
    }
    
    if (!Player::validatePassword(password)) {
        std::cout << "Error: Password must be at least 6 characters long and contain both letters and numbers." << std::endl;
        return false;
    }
    
    if (!email.empty() && !Player::validateEmail(email)) {
        std::cout << "Error: Invalid email format." << std::endl;
        return false;
    }
    
    if (isUsernameTaken(username)) {
        std::cout << "Error: Username '" << username << "' is already taken." << std::endl;
        return false;
    }
    
    // Create new player
    int newId = generatePlayerId();
    std::string actualNickname = nickname.empty() ? username : nickname;
    
    Player* newPlayer = new Player(username, password, actualNickname, email, newId);
    players.append(newPlayer);
    usernameToPlayer.insert(username, newPlayer);
    
    // Save to file
    savePlayersToFile();
    
    std::cout << "Successfully registered player: " << username << " (ID: " << newId << ")" << std::endl;
    return true;
}

bool AuthSystem::login(const std::string& username, const std::string& password) {
    Player* player = usernameToPlayer.get(username);
    
    if (!player) {
        std::cout << "Error: Username not found." << std::endl;
        return false;
    }
    
    if (player->getPassword() != password) {
        std::cout << "Error: Incorrect password." << std::endl;
        return false;
    }
    
    if (currentPlayer != nullptr) {
        currentPlayer->setIsOnline(false);
    }
    
    currentPlayer = player;
    currentPlayer->setIsOnline(true);
    
    std::cout << "Successfully logged in as: " << username << std::endl;
    currentPlayer->displayProfile();
    return true;
}

void AuthSystem::logout() {
    if (currentPlayer) {
        std::string username = currentPlayer->getUsername();
        currentPlayer->setIsOnline(false);
        currentPlayer = nullptr;
        std::cout << "Successfully logged out: " << username << std::endl;
    } else {
        std::cout << "No user is currently logged in." << std::endl;
    }
}

bool AuthSystem::changePassword(const std::string& username, const std::string& oldPassword,
                               const std::string& newPassword) {
    
    Player* player = usernameToPlayer.get(username);
    if (!player) {
        std::cout << "Error: Username not found." << std::endl;
        return false;
    }
    
    if (player->getPassword() != oldPassword) {
        std::cout << "Error: Old password is incorrect." << std::endl;
        return false;
    }
    
    if (!Player::validatePassword(newPassword)) {
        std::cout << "Error: New password must be at least 6 characters long and contain both letters and numbers." << std::endl;
        return false;
    }
    
    player->setPassword(newPassword);
    savePlayersToFile();
    
    std::cout << "Password successfully changed for: " << username << std::endl;
    return true;
}

// Player management
Player* AuthSystem::getPlayer(const std::string& username) const {
    return usernameToPlayer.get(username);
}

Player* AuthSystem::getCurrentPlayer() const {
    return currentPlayer;
}

bool AuthSystem::updatePlayerProfile(const std::string& username, const std::string& newNickname,
                                   const std::string& newEmail) {
    
    Player* player = usernameToPlayer.get(username);
    if (!player) {
        std::cout << "Error: Username not found." << std::endl;
        return false;
    }
    
    if (!newEmail.empty() && !Player::validateEmail(newEmail)) {
        std::cout << "Error: Invalid email format." << std::endl;
        return false;
    }
    
    player->setNickname(newNickname);
    player->setEmail(newEmail);
    savePlayersToFile();
    
    std::cout << "Profile updated successfully for: " << username << std::endl;
    return true;
}

bool AuthSystem::deletePlayer(const std::string& username, const std::string& password) {
    Player* player = usernameToPlayer.get(username);
    if (!player) {
        std::cout << "Error: Username not found." << std::endl;
        return false;
    }
    
    if (player->getPassword() != password) {
        std::cout << "Error: Incorrect password." << std::endl;
        return false;
    }
    
    // Remove from all data structures
    usernameToPlayer.remove(username);
    
    // Find and remove from players array
    for (int i = 0; i < players.getSize(); i++) {
        if (players.get(i) == player) {
            players.remove(i);
            break;
        }
    }
    
    // If this player is currently logged in, logout
    if (currentPlayer == player) {
        currentPlayer = nullptr;
    }
    
    // Delete the player object
    delete player;
    
    savePlayersToFile();
    std::cout << "Player account deleted: " << username << std::endl;
    return true;
}

// System operations
void AuthSystem::displayAllPlayers() const {
    std::cout << "\n=== All Registered Players ===" << std::endl;
    if (players.getSize() == 0) {
        std::cout << "No players registered." << std::endl;
        return;
    }
    
    for (int i = 0; i < players.getSize(); i++) {
        Player* player = players.get(i);
        if (player) {
            std::cout << i + 1 << ". " << player->getUsername() 
                      << " (ID: " << player->getPlayerId() 
                      << ", Score: " << player->getTotalScore() 
                      << ", Status: " << (player->getIsOnline() ? "Online" : "Offline") 
                      << ")" << std::endl;
        }
    }
    std::cout << "Total: " << players.getSize() << " players" << std::endl;
}

int AuthSystem::getTotalPlayers() const {
    return players.getSize();
}

bool AuthSystem::saveSystemState() {
    return savePlayersToFile();
}

bool AuthSystem::loadSystemState() {
    return loadPlayersFromFile();
}

bool AuthSystem::removeFriend(const std::string& username, const std::string& friendUsername) {
    Player* player = getPlayer(username);
    Player* friendPlayer = getPlayer(friendUsername);
    
    if (!player || !friendPlayer) {
        std::cout << "Error: One or both players not found." << std::endl;
        return false;
    }
    
    bool success1 = player->removeFriend(friendUsername);
    bool success2 = friendPlayer->removeFriend(username);
    
    if (success1 || success2) {
        // Save updated friend lists to files
        saveFriendListToFile(username);
        saveFriendListToFile(friendUsername);
        
        savePlayersToFile();
        std::cout << "Friend removed: " << username << " and " << friendUsername << " are no longer friends." << std::endl;
        return true;
    }
    
    std::cout << "Error: Could not remove friend. They may not be friends." << std::endl;
    return false;
}
void AuthSystem::displayFriends(const std::string& username) const {
    Player* player = usernameToPlayer.get(username);
    if (!player) {
        std::cout << "Error: Player not found." << std::endl;
        return;
    }
    
    player->displayFriends();
}

// Utility methods
void AuthSystem::displayCurrentPlayer() const {
    if (currentPlayer) {
        currentPlayer->displayProfile();
    } else {
        std::cout << "No player is currently logged in." << std::endl;
    }
}

// Friend request system implementation

void AuthSystem::loadFriendRequests() {
    requestCount = 0;
    std::ifstream file(REQUESTS_FILE);
    if (!file.is_open()) return;
    
    std::string sender, receiver;
    while (file >> sender >> receiver && requestCount < MAX_REQUESTS) {
        pendingRequests[requestCount].sender = sender;
        pendingRequests[requestCount].receiver = receiver;
        requestCount++;
    }
    file.close();
}

void AuthSystem::saveFriendRequests() {
    std::ofstream file(REQUESTS_FILE, std::ios::trunc);
    if (!file.is_open()) return;
    
    for (int i = 0; i < requestCount; i++) {
        file << pendingRequests[i].sender << " " << pendingRequests[i].receiver << "\n";
    }
    file.close();
}

bool AuthSystem::sendFriendRequest(const std::string& fromUser, const std::string& toUser) {
    if (fromUser == toUser) {
        std::cout << "Cannot send friend request to yourself.\n";
        return false;
    }
    
    Player* sender = getPlayer(fromUser);
    Player* receiver = getPlayer(toUser);
    
    if (!sender || !receiver) {
        std::cout << "User does not exist.\n";
        return false;
    }
    
    if (sender->isFriend(toUser)) {
        std::cout << "Already friends with " << toUser << ".\n";
        return false;
    }
    
    loadFriendRequests();
    
    // Check if request already exists
    for (int i = 0; i < requestCount; i++) {
        if (pendingRequests[i].sender == fromUser && pendingRequests[i].receiver == toUser) {
            std::cout << "Friend request already sent to " << toUser << ".\n";
            return false;
        }
    }
    
    // Add new request if space available
    if (requestCount >= MAX_REQUESTS) {
        std::cout << "Friend request limit reached.\n";
        return false;
    }
    
    pendingRequests[requestCount].sender = fromUser;
    pendingRequests[requestCount].receiver = toUser;
    requestCount++;
    
    saveFriendRequests();
    std::cout << "Friend request sent to " << toUser << " successfully!\n";
    return true;
}
bool AuthSystem::acceptFriendRequest(const std::string& receiver, const std::string& sender) {
    loadFriendRequests();
    
    bool found = false;
    int foundIndex = -1;
    
    for (int i = 0; i < requestCount; i++) {
        if (pendingRequests[i].receiver == receiver && pendingRequests[i].sender == sender) {
            foundIndex = i;
            found = true;
            break;
        }
    }
    
    if (!found) {
        std::cout << "No pending friend request from " << sender << ".\n";
        return false;
    }
    
    // Remove from array by shifting
    for (int i = foundIndex; i < requestCount - 1; i++) {
        pendingRequests[i] = pendingRequests[i + 1];
    }
    requestCount--;
    
    saveFriendRequests();
    
    // Add as friends
    Player* receiverPlayer = getPlayer(receiver);
    Player* senderPlayer = getPlayer(sender);
    
    if (receiverPlayer && senderPlayer) {
        receiverPlayer->addFriend(sender);
        senderPlayer->addFriend(receiver);
        
        // Save friend lists to files
        saveFriendListToFile(receiver);
        saveFriendListToFile(sender);
        
        saveSystemState();
        std::cout << "Accepted friend request from " << sender << "!\n";
        return true;
    }
    
    return false;
}
bool AuthSystem::declineFriendRequest(const std::string& receiver, const std::string& sender) {
    loadFriendRequests();
    
    bool found = false;
    int foundIndex = -1;
    
    for (int i = 0; i < requestCount; i++) {
        if (pendingRequests[i].receiver == receiver && pendingRequests[i].sender == sender) {
            foundIndex = i;
            found = true;
            break;
        }
    }
    
    if (!found) {
        std::cout << "No pending friend request from " << sender << ".\n";
        return false;
    }
    
    // Remove from array by shifting
    for (int i = foundIndex; i < requestCount - 1; i++) {
        pendingRequests[i] = pendingRequests[i + 1];
    }
    requestCount--;
    
    saveFriendRequests();
    std::cout << "Declined friend request from " << sender << ".\n";
    return true;
}

void AuthSystem::displayPendingRequests(const std::string& username) const {
    std::cout << "\n=== Pending Friend Requests ===\n";
    bool found = false;
    
    for (int i = 0; i < requestCount; i++) {
        if (pendingRequests[i].receiver == username) {
            std::cout << "- " << pendingRequests[i].sender << "\n";
            found = true;
        }
    }
    
    if (!found) {
        std::cout << "No pending friend requests.\n";
    }
    std::cout << "===============================\n";
}

void AuthSystem::getPendingRequests(const std::string& username, std::string* outRequests, int& outCount) const {
    outCount = 0;
    for (int i = 0; i < requestCount; i++) {
        if (pendingRequests[i].receiver == username) {
            if (outCount < 50) {  // Arbitrary max limit
                outRequests[outCount] = pendingRequests[i].sender;
                outCount++;
            }
        }
    }
}
bool AuthSystem::updatePlayerGameStats(const std::string& username, int score, bool won) {
    Player* player = getPlayer(username);
    if (!player) {
        std::cout << "Error: Player not found." << std::endl;
        return false;
    }
    
    // Update games played
    int currentGames = player->getGamesPlayed();
    player->setGamesPlayed(currentGames + 1);
    
    // Update games won if applicable
    if (won) {
        int currentWins = player->getGamesWon();
        player->setGamesWon(currentWins + 1);
    }
    
    // Update highest score if this is a new record
    if (score > player->getHighestScore()) {
        player->setHighestScore(score);
        std::cout << "New high score achieved: " << score << "!" << std::endl;
    }
    
    // Update total score
    player->updateScore(score);
    
    // Save to file
    return savePlayerData(username);
}



// Add these methods to authentication.cpp:

// Save a player's friend list to file
bool AuthSystem::saveFriendListToFile(const std::string& username) {
    Player* player = getPlayer(username);
    if (!player) {
        std::cout << "Error: Player not found." << std::endl;
        return false;
    }
    
    std::string filename = username + "_friends.txt";
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not save friend list to file." << std::endl;
        return false;
    }
    
    LinkedList& friends = player->getFriendsList();
    if (!friends.isEmpty()) {
        ListNode* current = friends.getHead();
        while (current != nullptr) {
            file << current->data << std::endl;
            current = current->next;
        }
    }
    
    file.close();
    std::cout << "Friend list saved for: " << username << std::endl;
    return true;
}

// Load a player's friend list from file
bool AuthSystem::loadFriendListFromFile(const std::string& username) {
    Player* player = getPlayer(username);
    if (!player) {
        std::cout << "Error: Player not found." << std::endl;
        return false;
    }
    
    std::string filename = username + "_friends.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        // It's okay if the file doesn't exist yet
        return false;
    }
    
    // Clear existing friends
    LinkedList& friends = player->getFriendsList();
    friends.clear();
    
    std::string friendName;
    while (std::getline(file, friendName)) {
        if (!friendName.empty()) {
            friends.append(friendName.c_str());
        }
    }
    
    file.close();
    std::cout << "Friend list loaded for: " << username << std::endl;
    return true;
}

// Save all players' friend lists
void AuthSystem::saveAllFriendLists() {
    for (int i = 0; i < players.getSize(); i++) {
        Player* player = players.get(i);
        if (player) {
            saveFriendListToFile(player->getUsername());
        }
    }
    std::cout << "All friend lists saved." << std::endl;
}