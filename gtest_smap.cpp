//
// Created by Andrzej Borucki on 2022-09-04
//
#include "SlotMap.hpp"
#include "gtest/gtest.h"
using namespace std;

TEST(SlotMap, Add) {
    unordered_map<int, int> map;
    SlotMap<int, int> smap(1000);
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(smap.size(), 0);
    map[1]= 1000;
    smap.put(1, 1000);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(smap.size(), 1);
    map[2]= 2000;
    smap.put(2, 2000);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(smap.size(), 2);
    map[2]= 200;
    smap.put(2, 200);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(smap.size(), 2);
}

TEST(SlotMap, Find) {
    unordered_map<int, int> map;
    SlotMap<int, int> smap(1000);
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(smap.size(), 0);
    map[1]= 1000;
    smap.put(1, 1000);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(smap.size(), 1);
    map[2]= 2000;
    smap.put(2, 2000);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(smap.size(), 2);
    map[2]= 200;
    smap.put(2, 200);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(smap.size(), 2);
    auto it = map.find(2);
    auto link = smap.find(2);
    EXPECT_NE(it, map.end());
    EXPECT_NE(link, nullptr);
    it = map.find(3);
    link = smap.find(3);
    EXPECT_EQ(it, map.end());
    EXPECT_EQ(link, nullptr);
}

TEST(SlotMap, Erase1) {
    for (int i=0; i<100; i++)
    {
        SlotMap<int, int> smap(20);
        for (int j=0; j<20; j++)
            smap.put(i+j,i+j);
        for (int j=0; j<20; j++)
            EXPECT_NE(smap.find(i+j), nullptr);
        smap.erase(i+4);
        for (int j=0; j<20; j++) {
            if (j==4)
                EXPECT_EQ(smap.find(i+j), nullptr);
            else
                EXPECT_NE(smap.find(i+j), nullptr);
        }
    }
}

TEST(SlotMap, Erase) {
    unordered_map<int, int> map;
    SlotMap<int, int> smap(1000);
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(smap.size(), 0);
    map[1]= 1000;
    smap.put(1, 1000);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(smap.size(), 1);
    map[2]= 2000;
    smap.put(2, 2000);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(smap.size(), 2);
    map[2]= 200;
    smap.put(2, 200);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(smap.size(), 2);
    auto it = map.find(2);
    auto link = smap.find(2);
    EXPECT_NE(it, map.end());
    EXPECT_NE(link, nullptr);
    it = map.find(3);
    link = smap.find(3);
    EXPECT_EQ(it, map.end());
    EXPECT_EQ(link, nullptr);
    map.erase(2);
    smap.erase(2);
    EXPECT_EQ(map.size(), 1);
    it = map.find(2);
    EXPECT_EQ(it, map.end());
    map.erase(2);
    EXPECT_EQ(map.size(), 1);
}