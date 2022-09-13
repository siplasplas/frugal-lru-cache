//
// Created by Andrzej Borucki on 2022-09-13
//
#include "HashMap.hpp"
#include "gtest/gtest.h"
using namespace std;

TEST(HashMap, Add) {
    unordered_map<int, int> map;
    HashMap<int, int> hmap(1000, 1000);
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(hmap.size(), 0);
    map[1]= 1000;
    hmap.put(1, 1000);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(hmap.size(), 1);
    map[2]= 2000;
    hmap.put(2, 2000);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(hmap.size(), 2);
    map[2]= 200;
    hmap.put(2, 200);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(hmap.size(), 2);
}

TEST(HashMap, Find) {
    unordered_map<int, int> map;
    HashMap<int, int> hmap(1000, 100);
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(hmap.size(), 0);
    map[1]= 1000;
    hmap.put(1, 1000);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(hmap.size(), 1);
    map[2]= 2000;
    hmap.put(2, 2000);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(hmap.size(), 2);
    map[2]= 200;
    hmap.put(2, 200);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(hmap.size(), 2);
    auto it = map.find(2);
    auto link = hmap.find(2);
    EXPECT_NE(it, map.end());
    EXPECT_NE(link, nullptr);
    it = map.find(3);
    link = hmap.find(3);
    EXPECT_EQ(it, map.end());
    EXPECT_EQ(link, nullptr);
}

TEST(HashMap, Erase1) {
    for (int i=0; i<100; i++)
    {
        HashMap<int, int> hmap(20,20);
        for (int j=0; j<20; j++)
            hmap.put(i+j,i+j);
        for (int j=0; j<20; j++)
            EXPECT_NE(hmap.find(i+j), nullptr);
        hmap.erase(i+4);
        for (int j=0; j<20; j++) {
            if (j==4)
                EXPECT_EQ(hmap.find(i+j), nullptr);
            else
                EXPECT_NE(hmap.find(i+j), nullptr);
        }
    }
}

TEST(HashMap, Erase) {
    unordered_map<int, int> map;
    HashMap<int, int> hmap(1000,1000);
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(hmap.size(), 0);
    map[1]= 1000;
    hmap.put(1, 1000);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(hmap.size(), 1);
    map[2]= 2000;
    hmap.put(2, 2000);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(hmap.size(), 2);
    map[2]= 200;
    hmap.put(2, 200);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(hmap.size(), 2);
    auto it = map.find(2);
    auto link = hmap.find(2);
    EXPECT_NE(it, map.end());
    EXPECT_NE(link, nullptr);
    it = map.find(3);
    link = hmap.find(3);
    EXPECT_EQ(it, map.end());
    EXPECT_EQ(link, nullptr);
    map.erase(2);
    hmap.erase(2);
    EXPECT_EQ(map.size(), 1);
    it = map.find(2);
    EXPECT_EQ(it, map.end());
    map.erase(2);
    EXPECT_EQ(map.size(), 1);
}