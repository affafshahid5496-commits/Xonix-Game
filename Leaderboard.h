#pragma once
#include <string>
#include <fstream>
#include <iostream>

class LeaderboardNode {
public:
    int playerId;
    int score;
    std::string username;
    
    LeaderboardNode(int id = 0, int sc = 0, const std::string& uname = "") 
        : playerId(id), score(sc), username(uname) {}
};

class MaxHeap {
private:
    LeaderboardNode* heap;
    int capacity;
    int size;
    
    void heapifyUp(int index) {
        if (index && heap[parent(index)].score < heap[index].score) {
            std::swap(heap[index], heap[parent(index)]);
            heapifyUp(parent(index));
        }
    }
    
    void heapifyDown(int index) {
        int largest = index;
        int left = this->left(index);
        int right = this->right(index);
        
        if (left < size && heap[left].score > heap[largest].score)
            largest = left;
        if (right < size && heap[right].score > heap[largest].score)
            largest = right;
            
        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }
    
    int parent(int i) { return (i-1)/2; }
    int left(int i) { return (2*i + 1); }
    int right(int i) { return (2*i + 2); }
    
public:
    MaxHeap(int cap = 100) : capacity(cap), size(0) {
        heap = new LeaderboardNode[capacity];
    }
    
    ~MaxHeap() {
        delete[] heap;
    }
    
    void insert(int playerId, int score, const std::string& username = "") {
        if (size == capacity) {
            return;
        }
        
        heap[size] = LeaderboardNode(playerId, score, username);
        heapifyUp(size);
        size++;
    }
    
    void updateOrInsert(int playerId, int totalScore, const std::string& username = "") {
        // Check if player already exists and update score (cumulative)
        for (int i = 0; i < size; i++) {
            if (heap[i].playerId == playerId) {
                heap[i].score += totalScore;  // Add to cumulative score
                heap[i].username = username;
                // Re-heapify from this position
                heapifyUp(i);
                heapifyDown(i);
                return;
            }
        }
        // If player not found and heap has space, insert new player
        if (size < capacity) {
            insert(playerId, totalScore, username);
        }
    }
    
    LeaderboardNode getMax() const {
        if (size == 0) return LeaderboardNode();
        return heap[0];
    }
    
    bool isFull() const { return size == capacity; }
    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }
    
    LeaderboardNode* getHeapArray() const {
        return heap;
    }
    
    void sortForDisplay(LeaderboardNode* sortedArray) const {
        // Copy heap for sorting
        for (int i = 0; i < size; i++) {
            sortedArray[i] = heap[i];
        }
        
        // Sort in descending order by score
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (sortedArray[j].score < sortedArray[j + 1].score) {
                    std::swap(sortedArray[j], sortedArray[j + 1]);
                }
            }
        }
    }
};

class Leaderboard {
private:
    MaxHeap* topPlayers;
    std::string playersDataFile;
    std::string leaderboardFile;
    
    void clearHeap() {
        if (topPlayers) {
            delete topPlayers;
        }
    }
    
    void loadFromPlayersData() {
        std::ifstream file(playersDataFile);
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open " << playersDataFile << std::endl;
            return;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            // Parse player data: playerId|username|password|nickname|email|totalScore|gamesPlayed|gamesWon|currentThemeId|isOnline|highestScore
            std::string tokens[15];
            int tokenCount = 0;
            std::string currentToken;
            
            for (char c : line) {
                if (c == '|') {
                    if (tokenCount < 15) {
                        tokens[tokenCount] = currentToken;
                        tokenCount++;
                        currentToken.clear();
                    }
                } else {
                    currentToken += c;
                }
            }
            
            if (!currentToken.empty() && tokenCount < 15) {
                tokens[tokenCount] = currentToken;
                tokenCount++;
            }
            
            // Need at least: playerId(0), username(1), ..., totalScore(5)
            if (tokenCount >= 6) {
                // Parse playerId
                int playerId = 0;
                for (char c : tokens[0]) {
                    if (c >= '0' && c <= '9') playerId = playerId * 10 + (c - '0');
                }
                
                std::string username = tokens[1];
                
                // Parse totalScore (token 5)
                int totalScore = 0;
                for (char c : tokens[5]) {
                    if (c >= '0' && c <= '9') totalScore = totalScore * 10 + (c - '0');
                }
                
                // Insert into heap using total score
                topPlayers->insert(playerId, totalScore, username);
            }
        }
        
        file.close();
    }
    
    void saveToFile() {
        std::ofstream file(leaderboardFile);
        if (!file.is_open()) return;
        
        LeaderboardNode* sorted = new LeaderboardNode[topPlayers->getSize()];
        topPlayers->sortForDisplay(sorted);
        
        for (int i = 0; i < topPlayers->getSize(); i++) {
            file << (i + 1) << ". " << sorted[i].username 
                 << " - " << sorted[i].score << " pts" << std::endl;
        }
        
        delete[] sorted;
        file.close();
    }
    
public:
    Leaderboard(const std::string& playersFile = "players_data.txt", 
                const std::string& leaderFile = "leaderboard.txt")
        : playersDataFile(playersFile), leaderboardFile(leaderFile) {
        topPlayers = new MaxHeap(100);
        loadFromPlayersData();
        saveToFile();
    }
    
    ~Leaderboard() {
        saveToFile();
        clearHeap();
    }
    
    void reload() {
        clearHeap();
        topPlayers = new MaxHeap(100);
        loadFromPlayersData();
        saveToFile();
    }
    
    void updatePlayerScore(int playerId, int newHighestScore, const std::string& username = "") {
        // Find and update player in heap, or reload from file
        reload();
    }
    
    void displayTopPlayers() {
        LeaderboardNode* sorted = new LeaderboardNode[topPlayers->getSize()];
        topPlayers->sortForDisplay(sorted);
        
        std::cout << "=== LEADERBOARD (TOP " << topPlayers->getSize() << ") ===" << std::endl;
        for (int i = 0; i < topPlayers->getSize(); i++) {
            std::cout << (i + 1) << ". " << sorted[i].username 
                      << " - " << sorted[i].score << " points" << std::endl;
        }
        std::cout << "========================" << std::endl;
        
        delete[] sorted;
    }
    
    bool isTopPlayer(int score) const {
        return true;
    }
    
    int getTopPlayersCount() const {
        return topPlayers->getSize();
    }
    
    void getTopPlayersSorted(LeaderboardNode* outputArray) const {
        LeaderboardNode* sorted = new LeaderboardNode[topPlayers->getSize()];
        topPlayers->sortForDisplay(sorted);
        
        for (int i = 0; i < topPlayers->getSize(); i++) {
            outputArray[i] = sorted[i];
        }
        
        delete[] sorted;
    }
};
