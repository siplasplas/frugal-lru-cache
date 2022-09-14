#ifndef CACHE_BINARYSEARCHTREERECURSIVE_H
#define CACHE_BINARYSEARCHTREERECURSIVE_H

#include "IBinarySearchTree.h"
#include "BaseBinaryTree.h"
#include <stdexcept>

/**
 * A recursive binary search tree implementation with <code>int</code> keys.
 *
 * translate from @author <a href="sven@happycoders.eu">Sven Woltmann</a>
 */
class BinarySearchTree : public BaseBinaryTree, public IBinarySearchTree {

    Node* searchNode(int key, Node* node) {
        if (node == nullptr) {
            return nullptr;
        }

        if (key == node->data) {
            return node;
        } else if (key < node->data) {
            return searchNode(key, node->left);
        } else {
            return searchNode(key, node->right);
        }
    }
    void deleteNodeWithTwoChildren(Node* node) {
        // Find minimum node of right subtree ("inorder successor" of current node)
        Node* inOrderSuccessor = findMinimum(node->right);

        // Copy inorder successor's data to current node
        node->data = inOrderSuccessor->data;

        // Delete inorder successor recursively
        node->right = deleteNodeTo(inOrderSuccessor->data, node->right);
    }

    Node* findMinimum(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
protected:
    virtual Node* insertNodeTo(int key, Node* node) {
        // No node at current position --> store new node at current position
        if (node == nullptr) {
            node = new Node(key);
        }

            // Otherwise, traverse the tree to the left or right depending on the key
        else if (key < node->data) {
            node->left = insertNodeTo(key, node->left);
        } else if (key > node->data) {
            node->right = insertNodeTo(key, node->right);
        } else {
            throw std::runtime_error("BST already contains a node with key " + std::to_string(key));
        }

        return node;
    }

    virtual Node* deleteNodeTo(int key, Node* node) {
        // No node at current position --> go up the recursion
        if (node == nullptr) {
            return nullptr;
        }

        // Traverse the tree to the left or right depending on the key
        if (key < node->data) {
            node->left = deleteNodeTo(key, node->left);
        } else if (key > node->data) {
            node->right = deleteNodeTo(key, node->right);
        }

            // At this point, "node" is the node to be deleted

            // Node* has no children --> just delete it
        else if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }

            // Node* has only one child --> replace node by its single child
        else if (node->left == nullptr) {
            Node *tmp = node;
            node = node->right;
            delete tmp;
        } else if (node->right == nullptr) {
            Node *tmp = node;
            node = node->left;
            delete tmp;
        }

            // Node* has two children
        else {
            deleteNodeWithTwoChildren(node);
        }

        return node;
    }

public:
    Node* searchNode(int key) {
        return searchNode(key, root);
    }


public:
    void insertNode(int key) override {
        root = insertNodeTo(key, root);
    }


    void deleteNode(int key) override {
        root = deleteNodeTo(key, root);
    }
};


#endif //CACHE_BINARYSEARCHTREERECURSIVE_H
