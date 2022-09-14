//
// Created by Andrzej Borucki on 2022-09-01
//

#include <iostream>
#include <forward_list>
#include <malloc.h>
#include <chrono>
#include "lrucache.hpp"
#include "SlotMap.hpp"
#include "trees/AvlTree.h"
#include "AVLSlotTree.hpp"

using namespace std;

class StopWatch
{
public:
    chrono::time_point<chrono::high_resolution_clock> a, b;
    void start() { a = chrono::high_resolution_clock::now(); }
    void stop() { b = chrono::high_resolution_clock::now(); }
    double duration()
    {
        chrono::duration<double> elapsed_seconds = b - a;
        return elapsed_seconds.count();
    }
};

const int COUNT = 1000*1000;

void printDiff(struct mallinfo2 &ma, struct mallinfo2 &mb) {
    cout << double((mb.uordblks+mb.hblkhd)-(ma.uordblks+ma.hblkhd))/COUNT <<endl;
}

int main() {

/*    auto m1a =  mallinfo2();
    auto *lru_cache = new cache::lru_cache<int, int>(COUNT);
    for (int i=0; i<COUNT; i++) {
        lru_cache->put(i, i);
    }
    auto m1b =  mallinfo2();
    cout << "classic" << endl;
    printDiff(m1a, m1b);
    delete lru_cache;

    auto m0a =  mallinfo2();
    auto *ma = new SlotMap<int,int>(COUNT,COUNT);
    for (int i=0; i<COUNT; i++)
        ma->put(i,i);

    auto m0b =  mallinfo2();
    cout << "slot map" << endl;
    printDiff(m0a, m0b);*/

    auto m2a =  mallinfo2();
    auto *avl = new AvlTree;
    StopWatch sw;
    sw.start();
    for (int i=0; i<COUNT; i++) {
        avl->insertNode(i);
    }
    sw.stop();
    cout << "AVL" << endl;
    cout << "time=" << sw.duration() <<endl;
    auto m2b =  mallinfo2();
    printDiff(m2a, m2b);
    delete avl;


    auto m3a =  mallinfo2();
    auto *savl = new AVLSlotTree;
    sw.start();
    for (int i=0; i<COUNT; i++) {
        savl->insertNode(i);
    }
    sw.stop();
    cout << "slot AVL" << endl;
    cout << "time=" << sw.duration() <<endl;
    auto m3b =  mallinfo2();
    printDiff(m3a, m3b);
    delete savl;
}