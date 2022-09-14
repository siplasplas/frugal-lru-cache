#ifndef CACHE_BINARYSEARCHTREE_H
#define CACHE_BINARYSEARCHTREE_H

#include "IBinaryTree.h"

/**
 * Interface for various BST implementations.
 *
 * translated from @author <a href="sven@happycoders.eu">Sven Woltmann</a>
 */
class IBinarySearchTree : public virtual IBinaryTree {
    /**
     * Searches for a node with the given search key.
     *
     * @param key the search key
     * @return the node or <code>null</code> if no node with the given search key exists
     */
    virtual Node* searchNode(int key) = 0;
    /**
     * Inserts a node with the given key.
     *
     * @param key the key of the node to be inserted
     */
    virtual void insertNode(int key) = 0;
    /**
     * Deletes the node with the given key.
     *
     * @param key the key of the node to be deleted
     */
    virtual void deleteNode(int key) = 0;

};

#endif //CACHE_BINARYSEARCHTREE_H
