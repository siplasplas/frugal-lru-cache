#ifndef CACHE_BINARYTREE_H
#define CACHE_BINARYTREE_H

struct Node {
    enum Color{ RED, BLACK, NIL };
    // also called "value" in a binary tree
    // also called "key" in a binary search tree
    int data;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr; // used in SimpleBinaryTree + red-black tree

    int height = 0; // used in AVL tree
    Color color = BLACK; // used in red-black tree

    /**
     * Constructs a new node with the given data.
     *
     * @param data the data to store in the node
     */
    Node(int data):data(data) {
    }

    int getData() const {
        return data;
    }
};

/**
 * Interface for various binary tree implementations.
 *
 * translated from @author <a href="sven@happycoders.eu">Sven Woltmann</a>
 */
class BinaryTree {
public:
    /**
     * Returns the binary tree's root node.
     *
     * @return the root node
     */
    virtual Node* getRoot() = 0;
};

#endif //CACHE_BINARYTREE_H
