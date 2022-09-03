//
// Created by Andrzej Borucki on 2022-09-01
//

#include <iostream>
#include <forward_list>
#include "MemInfo.h"
#include "lrucache.hpp"

using namespace std;

const int COUNT = 10*1000*1000;

int main() {
    MemInfo mi;
    int a = mi.virtMem();
    int b = mi.virtMem();
    cache::lru_cache<int, int> cache_lru(COUNT);
    for (int i=0; i<COUNT; i++) {
        cache_lru.put(i, i);
    }
    cout << (mi.virtMem()-a)*1024.0/COUNT <<endl;
    cout << (mi.virtMem()-b)*1024.0/COUNT <<endl;
}