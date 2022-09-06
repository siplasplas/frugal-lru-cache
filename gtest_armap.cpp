//
// Created by Andrzej Borucki on 2022-09-06
//
#include "ArMap.hpp"
#include "gtest/gtest.h"
using namespace std;

TEST(ArMap, Add) {
    ArMap<int,int> map;
    for (int i = 0; i < 1000; ++i) {
        map.put(i, i * 10);
    }
    EXPECT_EQ(map.capacity(),1536);
}