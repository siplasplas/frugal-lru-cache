//
// Created by Andrzej Borucki on 2022-09-03
//
#include "SlotList.hpp"
#include <list>
#include "gtest/gtest.h"

const int FillCount = 3;

typedef uint ptr_t;

void fill(std::list<int> &list, cache::SlotList<ptr_t, int> &mylist) {
    for (int v=0; v<FillCount; v++) {
        list.push_front(v);
        mylist.push_front(v);
    }
}

void compare(std::list<int> &list, cache::SlotList<ptr_t, int> &mylist) {
    assert(FillCount>=1);
    std::list<int>::iterator it;
    ptr_t myit;
    it = list.begin();
    myit = mylist.frontLink;
    while (it!=list.end() && myit!=0) {
        EXPECT_EQ(*it, mylist.data(myit));
        it++;
        mylist.inc(myit);
    }
    EXPECT_EQ(it, list.end());
    EXPECT_EQ(myit, 0);

    do {
        it--;
        mylist.dec(myit);
        EXPECT_EQ(*it, mylist.data(myit));
    } while (it!=list.begin() && myit!=mylist.frontLink);
    EXPECT_EQ(it, list.begin());
    EXPECT_EQ(myit, mylist.frontLink);
}

TEST(SlotList, push_front) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    fill(list, mylist);
    compare(list, mylist);
}

TEST(SlotList, pop_front) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    fill(list, mylist);
    list.pop_front();
    mylist.pop_front();
    compare(list, mylist);
}

TEST(SlotList, pop_front_empty) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    EXPECT_EQ(list.empty(), true);
    EXPECT_EQ(mylist.empty(), true);
    fill(list, mylist);
    EXPECT_EQ(list.empty(), false);
    EXPECT_EQ(mylist.empty(), false);
    for (int i=0; i<FillCount; i++) {
        list.pop_front();
        mylist.pop_front();
        compare(list, mylist);
    }
    EXPECT_EQ(list.empty(), true);
    EXPECT_EQ(mylist.empty(), true);
    EXPECT_THROW(mylist.pop_front(), std::runtime_error);
}

TEST(SlotList, push_back) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    fill(list, mylist);
    list.push_back(100);
    mylist.push_back(100);
    compare(list, mylist);
}

TEST(SlotList, pop_back) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    fill(list, mylist);
    list.pop_back();
    mylist.pop_back();
    compare(list, mylist);
}

TEST(SlotList, pop_back_empty) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    EXPECT_EQ(list.empty(), true);
    EXPECT_EQ(mylist.empty(), true);
    fill(list, mylist);
    EXPECT_EQ(list.empty(), false);
    EXPECT_EQ(mylist.empty(), false);
    for (int i=0; i<FillCount; i++) {
        list.pop_back();
        mylist.pop_back();
        compare(list, mylist);
    }
    EXPECT_EQ(list.empty(), true);
    EXPECT_EQ(mylist.empty(), true);
    EXPECT_THROW(mylist.pop_back(), std::runtime_error);
}

TEST(SlotList, erase_empty) {
    cache::SlotList<ptr_t, int> mylist(100);
    ptr_t myit;
    myit = mylist.frontLink;
    EXPECT_THROW(mylist.erase(myit), std::runtime_error);
}

TEST(SlotList, erase_end) {
    cache::SlotList<ptr_t, int> mylist(100);
    ptr_t myit;
    for (int i=0; i<FillCount; i++)
        mylist.push_front(i);
    myit = mylist.tailLink;
    EXPECT_THROW(mylist.erase(myit), std::runtime_error);
}

TEST(SlotList, erase_begin) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    fill(list, mylist);

    it = list.begin();
    myit = mylist.frontLink;
    list.erase(it);
    mylist.erase(myit);
    compare(list, mylist);
}

TEST(SlotList, erase_second) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    fill(list, mylist);

    list.push_front(0);
    list.push_front(11);
    mylist.push_front(0);
    mylist.push_front(11);

    it = list.begin();
    myit = mylist.frontLink;
    it++;
    mylist.inc(myit);
    list.erase(it);
    mylist.erase(myit);
    compare(list, mylist);
}

TEST(SlotList, erase_last) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    fill(list, mylist);

    it = list.end();
    myit = mylist.tailLink;
    it--;
    mylist.dec(myit);
    list.erase(it);
    mylist.erase(myit);
    compare(list, mylist);
}

TEST(SlotList, insert_second) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    fill(list, mylist);

    list.push_front(3);
    list.push_front(4);
    mylist.push_front(3);
    mylist.push_front(4);

    it = list.begin();
    myit = mylist.frontLink;
    it++;
    mylist.inc(myit);
    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}

TEST(SlotList, insert_begin) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    fill(list, mylist);

    it = list.begin();
    myit = mylist.frontLink;

    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}

TEST(SlotList, insert_empty) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    it = list.begin();
    myit = mylist.frontLink;

    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}

TEST(SlotList, insert_end) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    fill(list, mylist);

    it = list.end();
    myit = mylist.tailLink;

    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}


TEST(SlotList, insert_last) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    fill(list, mylist);

    it = list.end();
    myit = mylist.tailLink;
    it--;
    mylist.dec(myit);

    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}

TEST(SlotList, move_to_front) {
    std::list<int> list;
    cache::SlotList<ptr_t, int> mylist(100);
    std::list<int>::iterator it;
    ptr_t myit;

    fill(list, mylist);

    it = list.end();
    myit = mylist.tailLink;
    it--;
    mylist.dec(myit);

    int n = *it;
    list.erase(it);
    list.push_front(n);
    mylist.moveToFront(myit);

    compare(list, mylist);
}
