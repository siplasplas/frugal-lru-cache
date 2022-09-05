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
