//
// Created by Andrzej Borucki on 2022-09-14
//

#ifndef CACHE_AVLSLOTTREE_H
#define CACHE_AVLSLOTTREE_H

#include "SlotTree.hpp"

class AVLSlotTree : public SlotTree {
public:
    AVLSlotTree(slot_t capacity): SlotTree(capacity) {}
protected:
    slot_t insertNodeTo(int key, slot_t nodeSlot) override {
        nodeSlot = SlotTree::insertNodeTo(key, nodeSlot);
        updateHeight(nodeSlot);
        return rebalance(nodeSlot);
    }

    slot_t deleteNodeTo(int key, slot_t nodeSlot) override {
        nodeSlot = SlotTree::deleteNodeTo(key, nodeSlot);

        // Node* is nullptr if the tree doesn't contain the key
        if (nodeSlot == 0) {
            return 0;
        }
        updateHeight(nodeSlot);
        return rebalance(nodeSlot);
    }

private:
    void updateHeight(slot_t nodeSlot) {
        TreeSlot& node = slots[nodeSlot];
        int leftChildHeight = height(node.left);
        int rightChildHeight = height(node.right);
        node.height = std::max(leftChildHeight, rightChildHeight) + 1;
    }

    slot_t rebalance(slot_t nodeSlot) {
        int bFactor = balanceFactor(nodeSlot);

        TreeSlot& node = slots[nodeSlot];
        // Left-heavy?
        if (bFactor < -1) {
            if (balanceFactor(node.left) <= 0) {
                // Rotate right
                nodeSlot = rotateRight(nodeSlot);
            } else {
                // Rotate left-right
                node.left = rotateLeft(node.left);
                nodeSlot = rotateRight(nodeSlot);
            }
        }

        // Right-heavy?
        if (bFactor > 1) {
            if (balanceFactor(node.right) >= 0) {
                // Rotate left
                nodeSlot = rotateLeft(nodeSlot);
            } else {
                // Rotate right-left
                node.right = rotateRight(node.right);
                nodeSlot = rotateLeft(nodeSlot);
            }
        }

        return nodeSlot;
    }

    slot_t rotateRight(slot_t nodeSlot) {
        TreeSlot& node = slots[nodeSlot];
        slot_t leftChildSlot = node.left;

        TreeSlot& leftChild = slots[leftChildSlot];
        node.left = leftChild.right;
        leftChild.right = nodeSlot;

        updateHeight(nodeSlot);
        updateHeight(leftChildSlot);

        return leftChildSlot;
    }

    slot_t rotateLeft(slot_t nodeSlot) {
        TreeSlot& node = slots[nodeSlot];
        slot_t rightChildSlot = node.right;

        TreeSlot& rightChild = slots[rightChildSlot];
        node.right = rightChild.left;
        rightChild.left = nodeSlot;

        updateHeight(nodeSlot);
        updateHeight(rightChildSlot);

        return rightChildSlot;
    }

    int balanceFactor(slot_t nodeSlot) {
        TreeSlot& node = slots[nodeSlot];
        return height(node.right) - height(node.left);
    }

    int height(slot_t nodeSlot) {
        return nodeSlot != 0 ? slots[nodeSlot].height : -1;
    }
};


#endif //CACHE_AVLSLOTTREE_H
