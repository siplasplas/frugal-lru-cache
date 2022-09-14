//
// Created by Andrzej Borucki on 2022-09-14
//
#include "gtest/gtest.h"
#include "RedBlackTree.h"

using namespace std;

TEST(RBTree, test) {
    RedBlackTree rbt;
    for (int i=0; i<1000; i++)
        rbt.insertNode(i);
}
