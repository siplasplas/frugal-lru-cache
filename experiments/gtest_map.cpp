//
// Created by Andrzej Borucki on 2022-09-03
//
#include <unordered_map>
#include "gtest/gtest.h"
#include "HashMap.hpp"

using namespace std;

TEST(Map, Add) {
    unordered_map<int, int> map;
    cache::HashMap<int, int> hmap;
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

TEST(Map, Find) {
    unordered_map<int, int> map;
    cache::HashMap<int, int> hmap;
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
    auto myit = hmap.find(2);
    EXPECT_NE(it, map.end());
    EXPECT_NE(myit, hmap.end());
    it = map.find(3);
    myit = hmap.find(3);
    EXPECT_EQ(it, map.end());
    EXPECT_EQ(myit, hmap.end());
}

TEST(Map, Iterate) {
    unordered_map<int, int> map;
    cache::HashMap<int, int> hmap;
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(hmap.size(), 0);
    for (int i=1; i<=10; i++) {
        map[i]= 1000*i;
        hmap.put(i, 1000*i);
    }
    EXPECT_EQ(map.size(), 10);
    EXPECT_EQ(hmap.size(), 10);
    int sum = 0;
    for (auto pair: map) {
        sum += pair.second;
    }
    EXPECT_EQ(sum, 55000);
    int mysum = 0;
    for (auto pair: hmap) {
        mysum += pair.second;
    }
    EXPECT_EQ(mysum, 55000);
}

TEST(Map, Erase) {
    unordered_map<int, int> map;
    cache::HashMap<int, int> hmap;
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
    auto myit = hmap.find(2);
    EXPECT_NE(it, map.end());
    EXPECT_NE(myit, hmap.end());
    it = map.find(3);
    myit = hmap.find(3);
    EXPECT_EQ(it, map.end());
    EXPECT_EQ(myit, hmap.end());
    map.erase(2);
    hmap.erase(2);
    EXPECT_EQ(map.size(), 1);
    it = map.find(2);
    EXPECT_EQ(it, map.end());
    map.erase(2);
    EXPECT_EQ(map.size(), 1);
}