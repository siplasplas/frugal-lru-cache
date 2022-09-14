#ifndef CACHE_AVLTREE_H
#define CACHE_AVLTREE_H

#include "BinarySearchTreeRecursive.h"

class AvlTree : public BinarySearchTreeRecursive {
    Node* insertNodeTo(int key, Node* node) override {
        node = BinarySearchTreeRecursive::insertNodeTo(key, node);
        updateHeight(node);
        return rebalance(node);
    }

    Node* deleteNodeTo(int key, Node* node) override {
        node = BinarySearchTreeRecursive::deleteNodeTo(key, node);

        // Node* is nullptr if the tree doesn't contain the key
        if (node == nullptr) {
            return nullptr;
        }

        updateHeight(node);

        return rebalance(node);
    }
private:
    void updateHeight(Node* node) {
        int leftChildHeight = height(node->left);
        int rightChildHeight = height(node->right);
        node->height = std::max(leftChildHeight, rightChildHeight) + 1;
    }

    Node* rebalance(Node* node) {
        int bFactor = balanceFactor(node);

        // Left-heavy?
        if (bFactor < -1) {
            if (balanceFactor(node->left) <= 0) {
                // Rotate right
                node = rotateRight(node);
            } else {
                // Rotate left-right
                node->left = rotateLeft(node->left);
                node = rotateRight(node);
            }
        }

        // Right-heavy?
        if (bFactor > 1) {
            if (balanceFactor(node->right) >= 0) {
                // Rotate left
                node = rotateLeft(node);
            } else {
                // Rotate right-left
                node->right = rotateRight(node->right);
                node = rotateLeft(node);
            }
        }

        return node;
    }

    Node* rotateRight(Node* node) {
        Node* leftChild = node->left;

        node->left = leftChild->right;
        leftChild->right = node;

        updateHeight(node);
        updateHeight(leftChild);

        return leftChild;
    }

    Node* rotateLeft(Node* node) {
        Node* rightChild = node->right;

        node->right = rightChild->left;
        rightChild->left = node;

        updateHeight(node);
        updateHeight(rightChild);

        return rightChild;
    }

    int balanceFactor(Node* node) {
        return height(node->right) - height(node->left);
    }

    int height(Node* node) {
        return node != nullptr ? node->height : -1;
    }

protected:
    std::string appendNodeToString(Node* node) {
        return std::to_string(node->data)
            +"[H=" + std::to_string(height(node))
            +", BF="+std::to_string(balanceFactor(node))
            +"]";
    }
};


#endif //CACHE_AVLTREE_H
