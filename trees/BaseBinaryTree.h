#ifndef CACHE_BASEBINARYTREE_H
#define CACHE_BASEBINARYTREE_H

#include <string>
#include "BinaryTree.h"

class BaseBinaryTree: public virtual BinaryTree {
protected:
    Node* root = nullptr;

    std::string  appendNodeToStringRecursive(Node* node) {
        std::string str = appendNodeToString(node);
        if (node->left) {
            str += " L{";
            str += appendNodeToStringRecursive(node->left);
            str += '}';
        }
        if (node->right != nullptr) {
            str += " R{";
            str += appendNodeToStringRecursive(node->right);
            str += '}';
        }
        return str;
    }

    std::string appendNodeToString(Node *node) {
        return std::to_string(node->data);
    }

public:
    Node* getRoot() override {
        return root;
    }

    std::string toString() {
        return appendNodeToStringRecursive(getRoot());
    }

};

#endif //CACHE_BASEBINARYTREE_H
