#pragma once
#include <string>

class Player; // Forward declaration

// Generic template-based Dynamic Array
template <typename T>
class DArray {
private:
    T* data;
    int capacity;
    int length;

    void resize(int newCapacity) {
        if (newCapacity <= capacity) return;
        
        T* newData = new T[newCapacity];
        for (int i = 0; i < length; i++) {
            newData[i] = data[i];
        }
        
        if (data != nullptr) {
            delete[] data;
        }
        
        data = newData;
        capacity = newCapacity;
    }

public:
    // Constructors & Destructor
    DArray() : data(nullptr), capacity(0), length(0) {}
    
    DArray(int initialCapacity) : capacity(initialCapacity), length(0) {
        if (initialCapacity > 0) {
            data = new T[initialCapacity];
        } else {
            data = nullptr;
        }
    }
    
    ~DArray() {
        if (data != nullptr) {
            delete[] data;
        }
    }
    
    // Basic operations
    void append(const T& value) {
        if (length >= capacity) {
            int newCapacity = (capacity == 0) ? 10 : capacity * 2;
            resize(newCapacity);
        }
        data[length] = value;
        length++;
    }
    
    void insert(int index, const T& value) {
        if (index < 0 || index > length) return;
        
        if (length >= capacity) {
            int newCapacity = (capacity == 0) ? 10 : capacity * 2;
            resize(newCapacity);
        }
        
        for (int i = length; i > index; i--) {
            data[i] = data[i - 1];
        }
        
        data[index] = value;
        length++;
    }
    
    T remove(int index) {
        if (index < 0 || index >= length) {
            return T();
        }
        
        T removedValue = data[index];
        
        for (int i = index; i < length - 1; i++) {
            data[i] = data[i + 1];
        }
        
        length--;
        return removedValue;
    }
    
    T get(int index) const {
        if (index < 0 || index >= length) {
            return T();
        }
        return data[index];
    }
    
    T& at(int index) {
        static T dummy;
        if (index < 0 || index >= length) {
            return dummy;
        }
        return data[index];
    }
    
    void set(int index, const T& value) {
        if (index >= 0 && index < length) {
            data[index] = value;
        }
    }
    
    int find(const T& value) const {
        for (int i = 0; i < length; i++) {
            if (data[i] == value) {
                return i;
            }
        }
        return -1;
    }
    
    // Utility functions
    int getSize() const {
        return length;
    }
    
    int size() const {
        return length;
    }
    
    int getCapacity() const {
        return capacity;
    }
    
    bool isEmpty() const {
        return length == 0;
    }
    
    void clear() {
        length = 0;
    }
    
    // Operator overloads
    T operator[](int index) const {
        return get(index);
    }
    
    T& operator[](int index) {
        return at(index);
    }
};

// Specialization for Player pointers (original DynamicArray behavior)
class DynamicArray {
private:
    DArray<Player*> playerArray;

public:
    DynamicArray() : playerArray(0) {}
    DynamicArray(int initialCapacity) : playerArray(initialCapacity) {}
    
    void append(Player* value) { playerArray.append(value); }
    void insert(int index, Player* value) { playerArray.insert(index, value); }
    Player* remove(int index) { return playerArray.remove(index); }
    Player* get(int index) const { return playerArray.get(index); }
    void set(int index, Player* value) { playerArray.set(index, value); }
    int find(Player* value) const { return playerArray.find(value); }
    
    int getSize() const { return playerArray.getSize(); }
    int getCapacity() const { return playerArray.getCapacity(); }
    bool isEmpty() const { return playerArray.isEmpty(); }
    void clear() { playerArray.clear(); }
};
