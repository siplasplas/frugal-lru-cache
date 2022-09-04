//
// Created by Andrzej Borucki on 2022-09-04
//
#include "SlotBits.hpp"
#include "gtest/gtest.h"

TEST(SlotBits, SlotPos) {
    typedef uint8_t slot_t;
    SlotBits<slot_t> *slb;
    for (slot_t i=1; i<=SlotBits<slot_t>::limit(); i++) {
        slb = new SlotBits<slot_t>(i);
        for (slot_t j=1; j<=i; j++) {
            auto p = slb->slotToPos(j);
            auto jj = slb->posToSlot(p);
            EXPECT_EQ(jj, j);
        }
    }
}

TEST(SlotBits, findSlotFrom) {
    typedef uint16_t slot_t;
    SlotBits<slot_t> slb(1023);
    slb.test_setOccupiedRange(1, 1023);
    slb.test_setAsAvailable(1023);//special case - at end of bitmap
    slb.test_setAsAvailable(500);
    slb.test_setAsAvailable(502);
    slb.test_setAsAvailable(700);
    slb.test_setAsAvailable(100);
    EXPECT_EQ(slb.findSlotFrom(100),100);
    EXPECT_EQ(slb.findSlotFrom(101),500);
    EXPECT_EQ(slb.findSlotFrom(501),502);
    EXPECT_EQ(slb.findSlotFrom(900),1023);
    EXPECT_EQ(slb.findSlotFrom(1023),1023);
    slb.setAsOccupied(1023);
    EXPECT_EQ(slb.findSlotFrom(900),0);
    EXPECT_EQ(slb.findSlotFrom(1023),0);
}

TEST(SlotBits, findSlotTo) {
    typedef uint16_t slot_t;
    SlotBits<slot_t> slb(1023);
    slb.test_setOccupiedRange(1, 1023);
    slb.test_setAsAvailable(1023);//special case - at end of bitmap
    slb.test_setAsAvailable(500);
    slb.test_setAsAvailable(502);
    slb.test_setAsAvailable(700);
    slb.test_setAsAvailable(100);
    EXPECT_EQ(slb.findSlotTo(99),0);
    EXPECT_EQ(slb.findSlotTo(100),0); //without ending range
    EXPECT_EQ(slb.findSlotTo(101),100);
    EXPECT_EQ(slb.findSlotTo(700),100);
    slb.setAsOccupied(100);
    EXPECT_EQ(slb.findSlotTo(700),500);
}

//search from current+1 with wrap
TEST(SlotBits, findNextSlot) {
    typedef uint16_t slot_t;
    SlotBits<slot_t> slb(1023);
    slb.test_setOccupiedRange(1, 1023);
    EXPECT_EQ(slb.findNextSlot(500),0);
    slb.test_setAsAvailable(500);
    EXPECT_EQ(slb.findNextSlot(499),500);
    EXPECT_EQ(slb.findNextSlot(500),0);
    EXPECT_EQ(slb.findNextSlot(501),500);
    slb.test_setAsAvailable(1023);
    EXPECT_EQ(slb.findNextSlot(1022),1023);
    EXPECT_EQ(slb.findNextSlot(1023),500);
    slb.setAsOccupied(500);
    EXPECT_EQ(slb.findNextSlot(1023),0);
}
