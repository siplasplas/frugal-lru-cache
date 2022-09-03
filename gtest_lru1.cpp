//
// Created by Andrzej Borucki on 2022-09-03
//
#include "LRU.hpp"
#include "gtest/gtest.h"

const int NUM_OF_TEST2_RECORDS = 100;
const int TEST2_CACHE_CAPACITY = 50;

TEST(LRUTest, SimplePut) {
    cache::LRU<int, int> cache_lru(1);
    cache_lru.put(7, 777);
    EXPECT_TRUE(cache_lru.exists(7));
    EXPECT_EQ(cache_lru.get(7), 777);
    EXPECT_EQ(cache_lru.size(), 1);
}

TEST(LRUTest, MissingValue) {
    cache::LRU<int, int> cache_lru(1);
    EXPECT_THROW(cache_lru.get(7), std::range_error);
}

TEST(LRUTest1, KeepsAllValuesWithinCapacity) {
    cache::LRU<int, int> cache_lru(TEST2_CACHE_CAPACITY);

    for (int i = 0; i < NUM_OF_TEST2_RECORDS; ++i) {
        cache_lru.put(i, i);
    }

    for (int i = 0; i < NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; ++i) {
        EXPECT_FALSE(cache_lru.exists(i));
    }

    for (int i = NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; i < NUM_OF_TEST2_RECORDS; ++i) {
        EXPECT_TRUE(cache_lru.exists(i));
        EXPECT_EQ(cache_lru.get(i), i);
    }

    size_t size = cache_lru.size();
    EXPECT_EQ(size, TEST2_CACHE_CAPACITY);
}
