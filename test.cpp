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
    auto m1a =  mallinfo2();
    auto *lru_cache = new cache::lru_cache<int, int>(COUNT);
    for (int i=0; i<COUNT; i++) {
        lru_cache->put(i, i);
    }
    auto m1b =  mallinfo2();
    cout << "classic" << endl;
    printDiff(m1a, m1b);
    delete lru_cache;

    auto m0a =  mallinfo2();
    auto *ma = new SlotMap<int,int>(COUNT);
    for (int i=0; i<COUNT; i++)
        ma->put(i,i);

    auto m0b =  mallinfo2();
    cout << "slot map" << endl;
    printDiff(m0a, m0b);
}