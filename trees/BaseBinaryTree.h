#ifndef CACHE_BASEBINARYTREE_H
#define CACHE_BASEBINARYTREE_H

#include <string>
#include "IBinaryTree.h"

class BaseBinaryTree: public virtual IBinaryTree {
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
    void clear(Node* node) {
        if (node->left)
            clear(node->left);
        if (node->right)
            clear(node->right);
        delete node;
    }

    void clear() {
        clear(root);
    }
    virtual ~BaseBinaryTree() {
        clear();
    }
    Node* getRoot() override {
        return root;
    }

    std::string toString() {
        return appendNodeToStringRecursive(getRoot());
    }

};

#endif //CACHE_BASEBINARYTREE_H
