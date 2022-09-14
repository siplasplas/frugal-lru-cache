//
// Created by Andrzej Borucki on 2022-09-14
//
#include "gtest/gtest.h"
#include "AvlTree.h"

using namespace std;

TEST(AVL, search) {
    AvlTree avl;
    for (int i=0; i<40; i+=2)
        avl.insertNode(i);
    for (int i=0; i<40; i+=2) {
        auto node = avl.searchNode(i);
        EXPECT_EQ(node->data, i);
        EXPECT_EQ(avl.searchNode(i+1), nullptr);
    }
}

TEST(AVL, search_le) {
    AvlTree avl;
    for (int i=0; i<40; i+=2)
        avl.insertNode(i);
    for (int i=0; i<40; i++)
    {
        auto node = avl.searchNode(i, BinarySearchTree::skLE);
        if (i%2)
            EXPECT_EQ(node->data, i-1);
        else
            EXPECT_EQ(node->data, i);
    }
    auto node = avl.searchNode(-1, BinarySearchTree::skLE);
    EXPECT_EQ(node, nullptr);
    node = avl.searchNode(40, BinarySearchTree::skLE);
    EXPECT_EQ(node->data, 38);
    node = avl.searchNode(41, BinarySearchTree::skLE);
    EXPECT_EQ(node->data, 38);
}


TEST(AVL, search_ge) {
    AvlTree avl;
    for (int i=0; i<40; i+=2)
        avl.insertNode(i);
    for (int i=0; i<=38; i++)
    {
        auto node = avl.searchNode(i, BinarySearchTree::skGE);
        if (i%2)
            EXPECT_EQ(node->data, i+1);
        else
            EXPECT_EQ(node->data, i);
    }
    auto node = avl.searchNode(-1, BinarySearchTree::skGE);
    EXPECT_EQ(node->data, 0);
    node = avl.searchNode(39, BinarySearchTree::skGE);
    EXPECT_EQ(node, nullptr);
    node = avl.searchNode(40, BinarySearchTree::skGE);
    EXPECT_EQ(node, nullptr);
}
