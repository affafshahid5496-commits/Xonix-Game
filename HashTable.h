#pragma once
#include <string>

class Player; // Forward declaration

struct HashNode {
    std::string key; // Username
    Player* value;
    HashNode* next;
    
    HashNode(const std::string& k, Player* v);
    // No destructor needed - std::string handles memory
};

class HashTable {
private:
    HashNode** table;
    int capacity;
    int size;

    int hashFunction(const std::string& key) const;
    void resize();

public:
    HashTable(int initialCapacity = 16);
    ~HashTable();
    
    // Basic operations
    void insert(const std::string& key, Player* value);
    Player* get(const std::string& key) const;
    bool remove(const std::string& key);
    bool contains(const std::string& key) const;
    
    // Utility functions
    int getSize() const;
    int getCapacity() const;
    void display() const;
    
private:
    // Disable copying
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
};