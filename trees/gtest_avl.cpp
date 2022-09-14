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
