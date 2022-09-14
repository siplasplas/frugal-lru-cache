//
// Created by Andrzej Borucki on 2022-09-14
//

#ifndef CACHE_SLOTTREE_H
#define CACHE_SLOTTREE_H

#include <cstdint>
#include <stdexcept>
#include <cassert>

using slot_t = uint32_t;

#pragma pack(push, 1)
struct TreeSlot {
    int data;
    slot_t left;
    slot_t right;
    uint8_t height = 0;
};
#pragma pack(pop)

class SlotTree {
    slot_t capacity;
    slot_t size = 0;
    slot_t erased = 0;
    TreeSlot *slots;
    slot_t ptr;
    slot_t root = 0;
public:
    SlotTree(int capacity):capacity(capacity){
        slots = new TreeSlot[capacity+1];
        ptr = 1;
    }

    ~SlotTree(){
        delete []slots;
    }

    virtual void insertNode(int key) {
        root = insertNodeTo(key, root);
    }

protected:
    virtual slot_t insertNodeTo(int key, slot_t parentSlot) {
        // No node at current position -. store new node at current position
        if (!parentSlot) {
            TreeSlot& slot = slots[ptr];
            slot.data = key;
            slot.left = slot.right = 0;
            slot.height = 0;
            ptr++;
            size++;
            return ptr-1;
        }
        TreeSlot& node = slots[parentSlot];
        // Otherwise, traverse the tree to the left or right depending on the key
        if (key < node.data) {
            node.left = insertNodeTo(key, node.left);
        } else if (key > node.data) {
            node.right = insertNodeTo(key, node.right);
        } else {
            throw std::runtime_error("BST already contains a node with key " + std::to_string(key));
        }
        return parentSlot;
    }

    virtual slot_t deleteNodeTo(int key, slot_t nodeSlot) {
        // No node at current position -. go up the recursion
        if (!nodeSlot) return 0;
        // Traverse the tree to the left or right depending on the key

        TreeSlot& node = slots[nodeSlot];
        if (key < node.data) {
            node.left = deleteNodeTo(key, node.left);
        } else if (key > node.data) {
            node.right = deleteNodeTo(key, node.right);
        }
        // At this point, "node" is the node to be deleted

        // Node* has no children --> just delete it
        else if (node.left == 0 && node.right == 0) {
            size--;
            erased++;
            return 0;
        }

        // Node* has only one child --> replace node by its single child
        else if (node.left == 0) {
            size--;
            erased++;
            return node.right;
        } else if (node.right == 0) {
            size--;
            erased++;
            return node.left;
        }

        // Node* has two children
        else {
            deleteNodeWithTwoChildren(nodeSlot);
            return nodeSlot;
        }
        assert(false);
        return 0;
    }

    void deleteNodeWithTwoChildren(slot_t nodeSlot) {
        TreeSlot& node = slots[nodeSlot];
        // Find minimum node of right subtree ("inorder successor" of current node)
        slot_t inOrderSuccessorSlot = findMinimum(node.right);

        TreeSlot& inOrderSuccessor = slots[inOrderSuccessorSlot];
        // Copy inorder successor's data to current node
        node.data = inOrderSuccessor.data;

        // Delete inorder successor recursively
        node.right = deleteNodeTo(inOrderSuccessor.data, node.right);
    }

    slot_t findMinimum(slot_t nodeSlot) {
        assert(nodeSlot);
        TreeSlot& node = slots[nodeSlot];
        while (node.left != 0) {
            nodeSlot = node.left;
            node = slots[nodeSlot];
        }
        return nodeSlot;
    }

    slot_t searchNodeFrom(int key, slot_t nodeSlot) {
        while (nodeSlot) {
            TreeSlot& node = slots[nodeSlot];
            if (key == node.data) {
                return nodeSlot;
            } else if (key < node.data) {
                nodeSlot = node.left;
            } else {
                nodeSlot = node.right;
            }
        }
        return 0;
    }
};


#endif //CACHE_SLOTTREE_H
