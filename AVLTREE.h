#pragma once
#include "Theme.h"

struct AVLNode {
    Theme theme;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const Theme& t)
        : theme(t), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);

    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);

    AVLNode* insertNode(AVLNode* node, const Theme& theme);
    AVLNode* searchNode(AVLNode* node, int id);

    void inorderFill(AVLNode* node, Theme* arr, int& index, int max);

public:
    AVLTree();
    void insert(const Theme& theme);
    Theme search(int id);

    // For displaying theme list
    void getInOrderList(Theme* arr, int max, int& count);
};