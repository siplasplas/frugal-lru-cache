//
// Created by Andrzej Borucki on 2022-09-04
//
#include "SlotMap.hpp"
#include "gtest/gtest.h"
using namespace std;
TEST(SlotMap, Add) {
    SlotMap smap;
    for (int i=0; i<500; i++)
        smap.put(i,i);
}
