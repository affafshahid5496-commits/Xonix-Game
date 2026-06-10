#pragma once

#include <string>

class ListNode {
public:
    std::string data;
    ListNode* next;
    
    ListNode(const std::string& value);
    // No destructor needed - std::string handles its own memory
};

class LinkedList {
private:
    ListNode* head;
    ListNode* tail;
    int size;

public:
    // Constructor & Destructor
    LinkedList();
    ~LinkedList();
    
    // Basic operations
    void append(const std::string& value);
    void prepend(const std::string& value);
    bool insert(int index, const std::string& value);
    bool remove(int index);
    bool remove(const std::string& value);
    bool contains(const std::string& value) const;
    std::string get(int index) const;
    void set(int index, const std::string& value);
    void clear();
    
    // Utility functions
    int getSize() const;
    bool isEmpty() const;
    void display() const;
    ListNode* getHead() const;
};