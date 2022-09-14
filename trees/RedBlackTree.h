#ifndef CACHE_REDBLACKTREE_H
#define CACHE_REDBLACKTREE_H

#include "BaseBinaryTree.h"
#include "IBinarySearchTree.h"
#include <stdexcept>

/**
 * A red-black tree implementation with <code>int</code> keys.
 *
 * @author <a href="sven@happycoders.eu">Sven Woltmann</a>
 */
class RedBlackTree : public BaseBinaryTree, public IBinarySearchTree {
public:
    Node* searchNode(int key) {
        Node* node = root;
        while (node != nullptr) {
            if (key == node->data) {
                return node;
            } else if (key < node->data) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return nullptr;
    }

    void fixRedBlackPropertiesAfterInsert(Node *node) {
        Node *parent = node->parent;

        // Case 1: Parent is nullptr, we've reached the root, the end of the recursion
        if (parent == nullptr) {
            // Uncomment the following line if you want to enforce black roots (rule 2):
            // node->color = BLACK;
            return;
        }

        // Parent is black --> nothing to do
        if (parent->color == Node::BLACK) {
            return;
        }

        // From here on, parent is red
        Node* grandparent = parent->parent;

        // Case 2:
        // Not having a grandparent means that parent is the root. If we enforce black roots
        // (rule 2), grandparent will never be nullptr, and the following if-then block can be
        // removed.
        if (grandparent == nullptr) {
            // As this method is only called on red nodes (either on newly inserted ones - or -
            // recursively on red grandparents), all we have to do is to recolor the root black.
            parent->color = Node::BLACK;
            return;
        }

        // Get the uncle (may be nullptr/nil, in which case its color is BLACK)
        Node *uncle = getUncle(parent);

        // Case 3: Uncle is red -> recolor parent, grandparent and uncle
        if (uncle != nullptr && uncle->color == Node::RED) {
            parent->color = Node::BLACK;
            grandparent->color = Node::RED;
            uncle->color = Node::BLACK;

            // Call recursively for grandparent, which is now red.
            // It might be root or have a red parent, in which case we need to fix more...
            fixRedBlackPropertiesAfterInsert(grandparent);
        }

            // Note on performance:
            // It would be faster to do the uncle color check within the following code. This way
            // we would avoid checking the grandparent-parent direction twice (once in getUncle()
            // and once in the following else-if). But for better understanding of the code,
            // I left the uncle color check as a separate step.

            // Parent is left child of grandparent
        else if (parent == grandparent->left) {
            // Case 4a: Uncle is black and node is left->right "inner child" of its grandparent
            if (node == parent->right) {
                rotateLeft(parent);

                // Let "parent" point to the new root node of the rotated sub-tree.
                // It will be recolored in the next step, which we're going to fall-through to.
                parent = node;
            }

            // Case 5a: Uncle is black and node is left->left "outer child" of its grandparent
            rotateRight(grandparent);

            // Recolor original parent and grandparent
            parent->color = Node::BLACK;
            grandparent->color = Node::RED;
        }

            // Parent is right child of grandparent
        else {
            // Case 4b: Uncle is black and node is right->left "inner child" of its grandparent
            if (node == parent->left) {
                rotateRight(parent);

                // Let "parent" point to the new root node of the rotated sub-tree.
                // It will be recolored in the next step, which we're going to fall-through to.
                parent = node;
            }

            // Case 5b: Uncle is black and node is right->right "outer child" of its grandparent
            rotateLeft(grandparent);

            // Recolor original parent and grandparent
            parent->color = Node::BLACK;
            grandparent->color = Node::RED;
        }
    }

    Node* getUncle(Node *parent) {
        Node *grandparent = parent->parent;
        if (grandparent->left == parent) {
            return grandparent->right;
        } else if (grandparent->right == parent) {
            return grandparent->left;
        } else {
            throw std::runtime_error("Parent is not a child of its grandparent");
        }
    }

    Node* deleteNodeWithZeroOrOneChild(Node *node) {
        // Node has ONLY a left child --> replace by its left child
        if (node->left != nullptr) {
            replaceParentsChild(node->parent, node, node->left);
            return node->left; // moved-up node
        }

            // Node has ONLY a right child --> replace by its right child
        else if (node->right != nullptr) {
            replaceParentsChild(node->parent, node, node->right);
            return node->right; // moved-up node
        }

            // Node has no children -->
            // * node is red --> just remove it
            // * node is black --> replace it by a temporary NIL node (needed to fix the R-B rules)
        else {
            Node* newChild;
            if (node->color == Node::BLACK) {
                newChild = new Node(0);
                newChild->color = Node::NIL;
            } else newChild = nullptr;
            replaceParentsChild(node->parent, node, newChild);
            return newChild;
        }
    }

    void replaceParentsChild(Node *parent, Node *oldChild, Node *newChild) {
        if (parent == nullptr) {
            root = newChild;
        } else if (parent->left == oldChild) {
            parent->left = newChild;
        } else if (parent->right == oldChild) {
            parent->right = newChild;
        } else {
            throw std::runtime_error("Node is not a child of its parent");
        }

        if (newChild != nullptr) {
            newChild->parent = parent;
        }
    }
    Node* findMinimum(Node *node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void fixRedBlackPropertiesAfterDelete(Node *node) {
        // Case 1: Examined node is root, end of recursion
        if (node == root) {
            // Uncomment the following line if you want to enforce black roots (rule 2):
            // node->color = BLACK;
            return;
        }

        Node* sibling = getSibling(node);

        // Case 2: Red sibling
        if (sibling->color == Node::RED) {
            handleRedSibling(node, sibling);
            sibling = getSibling(node); // Get new sibling for fall-through to cases 3-6
        }

        // Cases 3+4: Black sibling with two black children
        if (isBlack(sibling->left) && isBlack(sibling->right)) {
            sibling->color = Node::RED;

            // Case 3: Black sibling with two black children + red parent
            if (node->parent->color == Node::RED) {
                node->parent->color = Node::BLACK;
            }

                // Case 4: Black sibling with two black children + black parent
            else {
                fixRedBlackPropertiesAfterDelete(node->parent);
            }
        }

            // Case 5+6: Black sibling with at least one red child
        else {
            handleBlackSiblingWithAtLeastOneRedChild(node, sibling);
        }
    }

    void handleRedSibling(Node *node, Node *sibling) {
        // Recolor...
        sibling->color = Node::BLACK;
        node->parent->color = Node::RED;

        // ... and rotate
        if (node == node->parent->left) {
            rotateLeft(node->parent);
        } else {
            rotateRight(node->parent);
        }
    }
    void handleBlackSiblingWithAtLeastOneRedChild(Node *node, Node *sibling) {
        bool nodeIsLeftChild = node == node->parent->left;

        // Case 5: Black sibling with at least one red child + "outer nephew" is black
        // --> Recolor sibling and its child, and rotate around sibling
        if (nodeIsLeftChild && isBlack(sibling->right)) {
            sibling->left->color = Node::BLACK;
            sibling->color = Node::RED;
            rotateRight(sibling);
            sibling = node->parent->right;
        } else if (!nodeIsLeftChild && isBlack(sibling->left)) {
            sibling->right->color = Node::BLACK;
            sibling->color = Node::RED;
            rotateLeft(sibling);
            sibling = node->parent->left;
        }

        // Fall-through to case 6...

        // Case 6: Black sibling with at least one red child + "outer nephew" is red
        // --> Recolor sibling + parent + sibling's child, and rotate around parent
        sibling->color = node->parent->color;
        node->parent->color = Node::BLACK;
        if (nodeIsLeftChild) {
            sibling->right->color = Node::BLACK;
            rotateLeft(node->parent);
        } else {
            sibling->left->color = Node::BLACK;
            rotateRight(node->parent);
        }
    }

    Node* getSibling(Node *node) {
        Node* parent = node->parent;
        if (node == parent->left) {
            return parent->right;
        } else if (node == parent->right) {
            return parent->left;
        } else {
            throw std::runtime_error("Parent is not a child of its grandparent");
        }
    }

    bool isBlack(Node* node) {
        return node == nullptr || node->color == Node::BLACK;
    }

    /*static class NilNode extends Node {
        private NilNode() {
            super(0);
            this->color = BLACK;
        }
    }*/

    // -- Helpers for insertion and deletion ---------------------------------------------------------

    void rotateRight(Node *node) {
        Node* parent = node->parent;
        Node* leftChild = node->left;

        node->left = leftChild->right;
        if (leftChild->right != nullptr) {
            leftChild->right->parent = node;
        }

        leftChild->right = node;
        node->parent = leftChild;

        replaceParentsChild(parent, node, leftChild);
    }

    void rotateLeft(Node* node) {
        Node* parent = node->parent;
        Node* rightChild = node->right;

        node->right = rightChild->left;
        if (rightChild->left != nullptr) {
            rightChild->left->parent = node;
        }

        rightChild->left = node;
        node->parent = rightChild;

        replaceParentsChild(parent, node, rightChild);
    }


public:
    // -- Insertion ----------------------------------------------------------------------------------

    void insertNode(int key) {
        Node* node = root;
        Node* parent = nullptr;

        // Traverse the tree to the left or right depending on the key
        while (node != nullptr) {
            parent = node;
            if (key < node->data) {
                node = node->left;
            } else if (key > node->data) {
                node = node->right;
            } else {
                throw std::runtime_error("BST already contains a node with key " + std::to_string(key));
            }
        }

        // Insert new node
        Node *newNode = new Node(key);
        newNode->color = Node::RED;
        if (parent == nullptr) {
            root = newNode;
        } else if (key < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        newNode->parent = parent;

        fixRedBlackPropertiesAfterInsert(newNode);
    }




    // -- Deletion -----------------------------------------------------------------------------------


    void deleteNode(int key) {
        Node *node = root;

        // Find the node to be deleted
        while (node != nullptr && node->data != key) {
            // Traverse the tree to the left or right depending on the key
            if (key < node->data) {
                node = node->left;
            } else {
                node = node->right;
            }
        }

        // Node not found?
        if (node == nullptr) {
            return;
        }

        // At this point, "node" is the node to be deleted

        // In this variable, we'll store the node at which we're going to start to fix the R-B
        // properties after deleting a node->
        Node *movedUpNode;
        bool deletedNodeColor;

        // Node has zero or one child
        if (node->left == nullptr || node->right == nullptr) {
            movedUpNode = deleteNodeWithZeroOrOneChild(node);
            deletedNodeColor = node->color;
        }

            // Node has two children
        else {
            // Find minimum node of right subtree ("inorder successor" of current node)
            Node *inOrderSuccessor = findMinimum(node->right);

            // Copy inorder successor's data to current node (keep its color!)
            node->data = inOrderSuccessor->data;

            // Delete inorder successor just as we would delete a node with 0 or 1 child
            movedUpNode = deleteNodeWithZeroOrOneChild(inOrderSuccessor);
            deletedNodeColor = inOrderSuccessor->color;
        }

        if (deletedNodeColor == Node::BLACK) {
            fixRedBlackPropertiesAfterDelete(movedUpNode);

            // Remove the temporary NIL node
            if (movedUpNode->color == Node::NIL) {
                replaceParentsChild(movedUpNode->parent, movedUpNode, nullptr);
            }
        }
    }

protected:
    // -- For toString() -----------------------------------------------------------------------------
    std::string appendNodeToString(Node *node) {
        return std::to_string(node->data) + (node->color == Node::RED ? "[R]" : "[B]");
    }

};

#endif //CACHE_REDBLACKTREE_H
