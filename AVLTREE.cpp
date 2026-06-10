#include "AVLTREE.h"
#include <iostream>

AVLTree::AVLTree() { root = nullptr; }

int AVLTree::getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int AVLTree::getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));

    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));

    return y;
}

AVLNode* AVLTree::insertNode(AVLNode* node, const Theme& theme) {
    if (!node)
        return new AVLNode(theme);

    if (theme.id < node->theme.id)
        node->left = insertNode(node->left, theme);
    else if (theme.id > node->theme.id)
        node->right = insertNode(node->right, theme);
    else
        return node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && theme.id < node->left->theme.id)
        return rightRotate(node);

    if (balance < -1 && theme.id > node->right->theme.id)
        return leftRotate(node);

    if (balance > 1 && theme.id > node->left->theme.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && theme.id < node->right->theme.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insert(const Theme& theme) {
    root = insertNode(root, theme);
}

AVLNode* AVLTree::searchNode(AVLNode* node, int id) {
    if (!node) return nullptr;
    if (id == node->theme.id) return node;
    if (id < node->theme.id) return searchNode(node->left, id);
    return searchNode(node->right, id);
}

Theme AVLTree::search(int id) {
    AVLNode* n = searchNode(root, id);
    if (n) return n->theme;
    return Theme();
}

void AVLTree::inorderFill(AVLNode* node, Theme* arr, int& index, int max) {
    if (!node || index >= max) return;
    inorderFill(node->left, arr, index, max);
    arr[index++] = node->theme;
    inorderFill(node->right, arr, index, max);
}

void AVLTree::getInOrderList(Theme* arr, int max, int& count) {
    count = 0;
    inorderFill(root, arr, count, max);
}