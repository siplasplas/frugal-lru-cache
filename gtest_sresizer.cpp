//
// Created by Andrzej Borucki on 2022-09-05
//
#include "gtest/gtest.h"
#include "SimpleResizer.hpp"
#include <algorithm>

using namespace std;

TEST(SimpleResizer, toBaseSize) {
    for (size_t kk=0; kk<=16; kk+=8) {
        SimpleResizer rl(kk);
        size_t n = 1;
        size_t maxn = 1L << 52;
        const int maxRange = 100;
        EXPECT_EQ(rl.toBaseSize(0), max((size_t)2,kk));
        while (n < maxn) {
            size_t n2 = 2 * n;
            size_t range = n2 - n - 1;
            if (range <= maxRange)
                for (size_t i = n + 1; i < n2; i++)
                    ASSERT_EQ(rl.toBaseSize(i), max(n2,kk));
            else {
                EXPECT_EQ(rl.toBaseSize(n + 1), max(n2,kk));
                EXPECT_EQ(rl.toBaseSize(n2 - 1), max(n2,kk));
                for (size_t i = 0; i < maxRange; i++) {
                    auto pos = size_t((long double) range / maxRange * i) + n + 1;
                    ASSERT_EQ(rl.toBaseSize(pos), max(n2,kk));
                }
            }
            EXPECT_EQ(rl.toBaseSize(n2), max(n2,kk));
            n = n2;
        }
    }
}

TEST(SimpleResizer, initCounter) {
    SimpleResizer rl(16);//no matter in this method
    EXPECT_THROW(rl.initCounter(0), std::range_error);
    EXPECT_EQ(rl.initCounter(1), 1);//must be >0
    EXPECT_EQ(rl.initCounter(2), 1);//else round down
    EXPECT_EQ(rl.initCounter(3), 2);
    EXPECT_EQ(rl.initCounter(4), 3);
    EXPECT_EQ(rl.initCounter(5), 4);
    EXPECT_EQ(rl.initCounter(6), 5);
    EXPECT_EQ(rl.initCounter(7), 6);
    EXPECT_EQ(rl.initCounter(8), 7);
    EXPECT_EQ(rl.initCounter(12), 10);
    EXPECT_EQ(rl.initCounter(16), 14);
    EXPECT_EQ(rl.initCounter(100), 90);
    EXPECT_EQ(rl.initCounter(128), 115);
    EXPECT_EQ(rl.initCounter(1628413597910449), 1465572238119404);//7^18 for round errors
    EXPECT_EQ(rl.initCounter(905824306333433), 815241875700089); //137^7 as any big number
    EXPECT_THROW(rl.initCounter(15134080077243536), std::range_error);
}

TEST(SimpleResizer, newCapacity) {
    SimpleResizer rl(16);

    EXPECT_EQ(rl.newCapacity(10,9), 16);//minimal resize

    EXPECT_EQ(rl.newCapacity(128,0), 256);
    EXPECT_EQ(rl.newCapacity(128,128), 128);

    EXPECT_EQ(rl.newCapacity(1024,0), 2048);
    EXPECT_EQ(rl.newCapacity(1024,100), 2048);
    EXPECT_EQ(rl.newCapacity(1024,307), 2048);
    EXPECT_EQ(rl.newCapacity(1024,308), 1024);
    EXPECT_EQ(rl.newCapacity(1024,900), 1024);
}
