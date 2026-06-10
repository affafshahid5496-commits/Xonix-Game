#include "LinkedList.h"
#include <iostream>

// ListNode implementation
ListNode::ListNode(const std::string& value) : data(value), next(nullptr) {
    // std::string constructor handles the copying
}

// LinkedList implementation
LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

LinkedList::~LinkedList() {
    clear();
}

void LinkedList::append(const std::string& value) {
    ListNode* newNode = new ListNode(value);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

void LinkedList::prepend(const std::string& value) {
    ListNode* newNode = new ListNode(value);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

bool LinkedList::insert(int index, const std::string& value) {
    if (index < 0 || index > size) return false;
    
    if (index == 0) {
        prepend(value);
        return true;
    }
    if (index == size) {
        append(value);
        return true;
    }
    
    ListNode* current = head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    
    ListNode* newNode = new ListNode(value);
    newNode->next = current->next;
    current->next = newNode;
    size++;
    return true;
}

bool LinkedList::remove(int index) {
    if (index < 0 || index >= size || !head) return false;
    
    ListNode* toDelete;
    if (index == 0) {
        toDelete = head;
        head = head->next;
        if (!head) tail = nullptr;
    } else {
        ListNode* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        toDelete = current->next;
        current->next = toDelete->next;
        if (toDelete == tail) {
            tail = current;
        }
    }
    
    delete toDelete;
    size--;
    return true;
}

bool LinkedList::remove(const std::string& value) {
    if (!head) return false;
    
    if (head->data == value) {
        ListNode* toDelete = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete toDelete;
        size--;
        return true;
    }
    
    ListNode* current = head;
    while (current->next && current->next->data != value) {
        current = current->next;
    }
    
    if (current->next) {
        ListNode* toDelete = current->next;
        current->next = toDelete->next;
        if (toDelete == tail) {
            tail = current;
        }
        delete toDelete;
        size--;
        return true;
    }
    
    return false;
}

bool LinkedList::contains(const std::string& value) const {
    ListNode* current = head;
    while (current) {
        if (current->data == value) return true;
        current = current->next;
    }
    return false;
}

std::string LinkedList::get(int index) const {
    if (index < 0 || index >= size) {
        return ""; // Return empty string for invalid index
    }
    
    ListNode* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

void LinkedList::set(int index, const std::string& value) {
    if (index < 0 || index >= size) return;
    
    ListNode* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    current->data = value; // std::string assignment handles memory
}

void LinkedList::clear() {
    while (head) {
        ListNode* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    head = tail = nullptr;
    size = 0;
}

int LinkedList::getSize() const {
    return size;
}

bool LinkedList::isEmpty() const {
    return size == 0;
}

void LinkedList::display() const {
    ListNode* current = head;
    std::cout << "[";
    while (current) {
        std::cout << current->data;
        if (current->next) std::cout << " -> ";
        current = current->next;
    }
    std::cout << "]" << std::endl;
}

ListNode* LinkedList::getHead() const {
    return head;
}