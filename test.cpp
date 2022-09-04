//
// Created by Andrzej Borucki on 2022-09-01
//

#include <iostream>
#include <forward_list>
#include <malloc.h>
#include "lrucache.hpp"
#include "experiments/LRU.hpp"
#include "SlotMap.hpp"
#include "experiments/SlotLRU.hpp"

using namespace std;

const int COUNT = 4*1000*1000;

void printDiff(struct mallinfo2 &ma, struct mallinfo2 &mb) {
    cout << double((mb.uordblks+mb.hblkhd)-(ma.uordblks+ma.hblkhd))/COUNT <<endl;
}

int main() {
    auto m0a =  mallinfo2();

    auto *ma = new SlotMap(COUNT);
    for (int i=0; i<COUNT; i++)
        ma->put(i,i);

    auto m0b =  mallinfo2();
    cout << "slot map" << endl;
    printDiff(m0a, m0b);

/*
    auto m1a =  mallinfo2();
    auto *lru_cache = new cache::lru_cache<int, int>(COUNT);
    for (int i=0; i<COUNT; i++) {
        lru_cache->put(i, i);
    }
    auto m1b =  mallinfo2();
    cout << "classic" << endl;
    printDiff(m1a, m1b);
    delete lru_cache;

    auto m2a =  mallinfo2();
    auto *LRU = new cache::LRU<int, int>(COUNT);
    for (int i=0; i<COUNT; i++) {
        LRU->put(i, i);
    }
    auto m2b =  mallinfo2();
    cout << "with new List class" << endl;
    printDiff(m2a, m2b);
    delete LRU;
*/
/*
    auto m3a =  mallinfo2();
    auto *slotLRU = new cache::SlotLRU<uint16_t, uint16_t, uint16_t>(COUNT);
    for (int i=0; i<COUNT; i++) {
        slotLRU->put(i, i);
    }
    auto m3b =  mallinfo2();
    cout << "with slots" << endl;
    printDiff(m3a, m3b
    delete slotLRU;*/
}