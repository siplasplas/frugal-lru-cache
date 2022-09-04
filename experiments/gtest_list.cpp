//
// Created by Andrzej Borucki on 2022-09-03
//
#include "List.hpp"
#include <list>
#include "gtest/gtest.h"

const int FillCount = 3;

void fill(std::list<int> &list, cache::List<int> &mylist) {
    for (int v=0; v<FillCount; v++) {
        list.push_front(v);
        mylist.push_front(v);
    }
}

void compare(std::list<int> &list, cache::List<int> &mylist) {
    assert(FillCount>=1);
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;
    it = list.begin();
    myit = mylist.begin();
    while (it!=list.end() && myit!=mylist.end()) {
        EXPECT_EQ(*it, *myit);
        it++;
        myit++;
    }
    EXPECT_EQ(it, list.end());
    EXPECT_EQ(myit, mylist.end());

    do {
        it--;
        myit--;
        EXPECT_EQ(*it, *myit);
    } while (it!=list.begin() && myit!=mylist.begin());
    EXPECT_EQ(it, list.begin());
    EXPECT_EQ(myit, mylist.begin());
}

TEST(List, push_front) {
    std::list<int> list;
    cache::List<int> mylist;
    fill(list, mylist);
    compare(list, mylist);
}

TEST(List, pop_front) {
    std::list<int> list;
    cache::List<int> mylist;
    fill(list, mylist);
    list.pop_front();
    mylist.pop_front();
    compare(list, mylist);
}

TEST(List, pop_front_empty) {
    std::list<int> list;
    cache::List<int> mylist;
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

TEST(List, push_back) {
    std::list<int> list;
    cache::List<int> mylist;
    fill(list, mylist);
    list.push_back(100);
    mylist.push_back(100);
    compare(list, mylist);
}

TEST(List, pop_back) {
    std::list<int> list;
    cache::List<int> mylist;
    fill(list, mylist);
    list.pop_back();
    mylist.pop_back();
    compare(list, mylist);
}

TEST(List, pop_back_empty) {
    std::list<int> list;
    cache::List<int> mylist;
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

TEST(List, erase_empty) {
    cache::List<int> mylist;
    cache::List<int>::iterator myit;
    myit = mylist.begin();
    EXPECT_THROW(mylist.erase(myit), std::runtime_error);
}

TEST(List, erase_end) {
    cache::List<int> mylist;
    cache::List<int>::iterator myit;
    for (int i=0; i<FillCount; i++)
        mylist.push_front(i);
    myit = mylist.end();
    EXPECT_THROW(mylist.erase(myit), std::runtime_error);
}

TEST(List, erase_begin) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    fill(list, mylist);

    it = list.begin();
    myit = mylist.begin();
    list.erase(it);
    mylist.erase(myit);
    compare(list, mylist);
}

TEST(List, erase_second) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    fill(list, mylist);

    list.push_front(0);
    list.push_front(11);
    mylist.push_front(0);
    mylist.push_front(11);

    it = list.begin();
    myit = mylist.begin();
    it++;
    myit++;
    list.erase(it);
    mylist.erase(myit);
    compare(list, mylist);
}

TEST(List, erase_last) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    fill(list, mylist);

    it = list.end();
    myit = mylist.end();
    it--;
    myit--;
    list.erase(it);
    mylist.erase(myit);
    compare(list, mylist);
}

TEST(List, insert_second) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    fill(list, mylist);

    list.push_front(3);
    list.push_front(4);
    mylist.push_front(3);
    mylist.push_front(4);

    it = list.begin();
    myit = mylist.begin();
    it++;
    myit++;
    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}

TEST(List, insert_begin) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    fill(list, mylist);

    it = list.begin();
    myit = mylist.begin();

    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}

TEST(List, insert_empty) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    it = list.begin();
    myit = mylist.begin();

    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}

TEST(List, insert_end) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    fill(list, mylist);

    it = list.end();
    myit = mylist.end();

    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}


TEST(List, insert_last) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    fill(list, mylist);

    it = list.end();
    myit = mylist.end();
    it--;
    myit--;

    list.insert(it, 24);
    mylist.insert(myit, 24);
    compare(list, mylist);
}

TEST(List, move_to_front) {
    std::list<int> list;
    cache::List<int> mylist;
    std::list<int>::iterator it;
    cache::List<int>::iterator myit;

    fill(list, mylist);

    it = list.end();
    myit = mylist.end();
    it--;
    myit--;

    int n = *it;
    list.erase(it);
    list.push_front(n);
    mylist.moveToFront(myit);

    compare(list, mylist);
}
