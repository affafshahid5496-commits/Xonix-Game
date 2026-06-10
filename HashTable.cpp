#include "HashTable.h"
#include "Player.h"
#include <iostream>

// HashNode implementation
HashNode::HashNode(const std::string& k, Player* v) 
    : key(k), value(v), next(nullptr) {
    // std::string constructor handles the copying
}

// HashTable implementation
HashTable::HashTable(int initialCapacity) : capacity(initialCapacity), size(0) {
    table = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < capacity; i++) {
        HashNode* current = table[i];
        while (current) {
            HashNode* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] table;
}

int HashTable::hashFunction(const std::string& key) const {
    unsigned long hash = 5381;
    
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash % capacity;
}

void HashTable::resize() {
    int oldCapacity = capacity;
    capacity *= 2;
    HashNode** newTable = new HashNode*[capacity];
    
    for (int i = 0; i < capacity; i++) {
        newTable[i] = nullptr;
    }
    
    // Rehash all elements
    for (int i = 0; i < oldCapacity; i++) {
        HashNode* current = table[i];
        while (current) {
            HashNode* next = current->next;
            int newIndex = hashFunction(current->key);
            
            // Insert at head of new bucket
            current->next = newTable[newIndex];
            newTable[newIndex] = current;
            
            current = next;
        }
    }
    
    delete[] table;
    table = newTable;
}

void HashTable::insert(const std::string& key, Player* value) {
    // Resize if load factor > 0.7
    if (size >= capacity * 0.7) {
        resize();
    }
    
    int index = hashFunction(key);
    
    // Check if key already exists
    HashNode* current = table[index];
    while (current) {
        if (current->key == key) {
            // Update existing value
            current->value = value;
            return;
        }
        current = current->next;
    }
    
    // Insert new node at head of chain
    HashNode* newNode = new HashNode(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    size++;
}

Player* HashTable::get(const std::string& key) const {
    int index = hashFunction(key);
    HashNode* current = table[index];
    
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return nullptr;
}

bool HashTable::remove(const std::string& key) {
    int index = hashFunction(key);
    HashNode* current = table[index];
    HashNode* prev = nullptr;
    
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;
}

bool HashTable::contains(const std::string& key) const {
    return get(key) != nullptr;
}

int HashTable::getSize() const {
    return size;
}

int HashTable::getCapacity() const {
    return capacity;
}

void HashTable::display() const {
    for (int i = 0; i < capacity; i++) {
        std::cout << "Bucket " << i << ": ";
        HashNode* current = table[i];
        while (current) {
            std::cout << "[" << current->key << " -> Player]";
            if (current->next) std::cout << " -> ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}