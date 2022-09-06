//
// Created by Andrzej Borucki on 2022-09-05
//
#include "gtest/gtest.h"
#include "ResizeLogic.hpp"
#include <algorithm>

using namespace std;

TEST(ResizeLogic, toBaseSize) {
    for (size_t kk=0; kk<=32; kk+=8) {
        ResizeLogic rl(kk);
        size_t n = 1;
        size_t n3 = 0L;
        size_t maxn = 1L << 52;
        const int maxRange = 1000;
        EXPECT_EQ(rl.toBaseSize(0), max((size_t)2,kk));
        while (n < maxn) {
            size_t n2 = 2 * n;
            size_t range = n2 - n3 - 1;
            if (range <= maxRange)
                for (size_t i = n3 + 1; i < n2; i++)
                    ASSERT_EQ(rl.toBaseSize(i), max(n2,kk));
            else {
                EXPECT_EQ(rl.toBaseSize(n3 + 1), max(n2,kk));
                EXPECT_EQ(rl.toBaseSize(n2 - 1), max(n2,kk));
                for (size_t i = 0; i < maxRange; i++) {
                    auto pos = size_t((long double) range / maxRange * i) + n3 + 1;
                    ASSERT_EQ(rl.toBaseSize(pos), max(n2,kk));
                }
            }
            EXPECT_EQ(rl.toBaseSize(n2), max(n2,kk));
            n3 = n2 + n;
            range = n3 - n2 - 1;
            if (range <= maxRange)
                for (size_t i = n2 + 1; i < n3; i++)
                    ASSERT_EQ(rl.toBaseSize(i), max(n3,kk));
            else {
                EXPECT_EQ(rl.toBaseSize(n2 + 1), max(n3,kk));
                EXPECT_EQ(rl.toBaseSize(n3 - 1), max(n3,kk));
                for (size_t i = 0; i < maxRange; i++) {
                    auto pos = size_t((long double) range / maxRange * i) + n2 + 1;
                    ASSERT_EQ(rl.toBaseSize(pos), max(n3,kk));
                }
            }
            EXPECT_EQ(rl.toBaseSize(n3), max(n3,kk));
            n = n2;
        }
    }
}

TEST(ResizeLogic, initCounter) {
    ResizeLogic rl(16);//no matter in this method
    EXPECT_THROW(rl.initCounter(0), std::range_error);
    EXPECT_EQ(rl.initCounter(1), 1);//must be >0
    EXPECT_EQ(rl.initCounter(2), 1);//else round down
    EXPECT_EQ(rl.initCounter(3), 2);
    EXPECT_EQ(rl.initCounter(4), 3);
    EXPECT_EQ(rl.initCounter(5), 4);
    EXPECT_EQ(rl.initCounter(6), 4);
    EXPECT_EQ(rl.initCounter(7), 6);//90%
    EXPECT_EQ(rl.initCounter(8), 7);
    EXPECT_EQ(rl.initCounter(12), 9);
    EXPECT_EQ(rl.initCounter(16), 14);
    EXPECT_EQ(rl.initCounter(95), 76);
    EXPECT_EQ(rl.initCounter(96), 76);
    EXPECT_EQ(rl.initCounter(97), 87);
    EXPECT_EQ(rl.initCounter(100), 90);
    EXPECT_EQ(rl.initCounter(128), 115);
    EXPECT_EQ(rl.initCounter(1628413597910449), 1302730878328359);//7^18 for round errors
    EXPECT_EQ(rl.initCounter(905824306333433), 815241875700089); //137^7
    EXPECT_THROW(rl.initCounter(15134080077243536), std::range_error);
}

TEST(ResizeLogic, newCapacity) {
    ResizeLogic rl(16);

    EXPECT_EQ(rl.newCapacity(10,9), 16);//minimal resize

    EXPECT_EQ(rl.newCapacity(128,0), 192);
    EXPECT_EQ(rl.newCapacity(128,128), 128);
    EXPECT_EQ(rl.newCapacity(192,0), 256);
    EXPECT_EQ(rl.newCapacity(192,192), 192);

    EXPECT_EQ(rl.newCapacity(96,0), 128);
    EXPECT_EQ(rl.newCapacity(96,19), 128);
    EXPECT_EQ(rl.newCapacity(96,20), 96);

    EXPECT_EQ(rl.newCapacity(1024,0), 1536);
    EXPECT_EQ(rl.newCapacity(1024,100), 1536);
    EXPECT_EQ(rl.newCapacity(1024,307), 1536);
    EXPECT_EQ(rl.newCapacity(1024,308), 1024);
    EXPECT_EQ(rl.newCapacity(1024,900), 1024);
}
