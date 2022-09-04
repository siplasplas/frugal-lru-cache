//
// Created by Andrzej Borucki on 2022-09-01
//

#include <iostream>
#include <forward_list>
#include <malloc.h>
#include "lrucache.hpp"
#include "LRU.hpp"
#include "SlotLRU.hpp"

using namespace std;

const int COUNT = 4*1000*10;

int main() {
    auto m1a =  mallinfo2();
    auto *lru_cache = new cache::lru_cache<int, int>(COUNT);
    for (int i=0; i<COUNT; i++) {
        lru_cache->put(i, i);
    }
    auto m1b =  mallinfo2();
    cout << "classic" << endl;
    cout << double(m1b.uordblks-m1a.uordblks)/COUNT <<endl;
    delete lru_cache;

    auto m2a =  mallinfo2();
    auto *LRU = new cache::LRU<int, int>(COUNT);
    for (int i=0; i<COUNT; i++) {
        LRU->put(i, i);
    }
    auto m2b =  mallinfo2();
    cout << "with new List class" << endl;
    cout << double(m2b.uordblks-m2a.uordblks)/COUNT <<endl;
    delete LRU;

    auto m3a =  mallinfo2();
    auto *slotLRU = new cache::SlotLRU<uint16_t, uint16_t, uint16_t>(COUNT);
    for (int i=0; i<COUNT; i++) {
        slotLRU->put(i, i);
    }
    auto m3b =  mallinfo2();
    cout << "with slots" << endl;
    cout << double(m3b.uordblks-m3a.uordblks)/COUNT <<endl;
    delete slotLRU;
}