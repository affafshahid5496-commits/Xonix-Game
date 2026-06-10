#pragma  once 
#include "Leaderboard.h"

class MinHeap {
private:
    LeaderboardNode* heap;
    int capacity;
    int size;
    
    void heapifyUp(int index) {
        if (index && heap[parent(index)].score > heap[index].score) {
            std::swap(heap[index], heap[parent(index)]);
            heapifyUp(parent(index));
        }
    }
    
    void heapifyDown(int index) {
        int smallest = index;
        int left = this->left(index);
        int right = this->right(index);
        
        if (left < size && heap[left].score < heap[smallest].score)
            smallest = left;
        if (right < size && heap[right].score < heap[smallest].score)
            smallest = right;
            
        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }
    
    int parent(int i) { return (i-1)/2; }
    int left(int i) { return (2*i + 1); }
    int right(int i) { return (2*i + 2); }
    
public:
    MinHeap(int cap = 10) : capacity(cap), size(0) {
        heap = new LeaderboardNode[capacity];
    }
    
    ~MinHeap() {
        delete[] heap;
    }
    
    void insert(int playerId, int score) {
        if (size == capacity) {
            return; // Use replaceMin instead when full
        }
        
        heap[size] = LeaderboardNode(playerId, score);
        heapifyUp(size);
        size++;
    }
    
    void replaceMin(int playerId, int score) {
        if (size == 0) return;
        
        heap[0] = LeaderboardNode(playerId, score);
        heapifyDown(0);
    }
    
    LeaderboardNode getMin() const {
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
        // Copy heap to temporary array
        MinHeap tempHeap = *this;
        
        // Extract elements in sorted order (ascending - but we'll reverse for display)
        for (int i = 0; i < size; i++) {
            sortedArray[i] = tempHeap.getMin();
            // In a real implementation, we'd remove min, but for simplicity we'll just sort the copy
        }
        
        // Simple bubble sort for the copy (since we can't easily remove from our heap)
        // This is just for display purposes
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (sortedArray[j].score < sortedArray[j + 1].score) {
                    std::swap(sortedArray[j], sortedArray[j + 1]);
                }
            }
        }
    }
};