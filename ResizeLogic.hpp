//
// Created by Andrzej Borucki on 2022-09-05
//

#ifndef CACHE_RESIZELOGIC_H
#define CACHE_RESIZELOGIC_H

#include "BitScanner.h"

class ResizeLogic {
    const int64_t MinSize;
    static std::pair<int64_t,int> toBaseSizeEx(int64_t size, int64_t MinSize) {
        if (size<MinSize) size = MinSize;
        if (size<=2) return std::make_pair(2,2);
        int bitpos = BitScanner::BitScanReverse(size);
        assert(bitpos>=1);
        bool atLeast3 = (size & (1L<<(bitpos-1) ))!=0;
        bool serie2 = (1L<<bitpos ) == size;
        bool serie3 = (3L<<(bitpos-1) ) == size;
        assert(!(serie2 && serie3));
        if (serie2 || serie3) return std::make_pair(size,serie2 ? 2:3);
        if (atLeast3)
            return std::make_pair(1L << (bitpos+1), 2);
        else
            return std::make_pair(3L << (bitpos-1), 3);
    }

    int64_t grow(const std::pair<int64_t, int> &p) {
        if (p.second==2)
            return p.first + p.first/2;
        else {
            int bitpos = BitScanner::BitScanReverse(p.first);
            return 1L << (bitpos+1);
        }
    }
public:
    explicit ResizeLogic(int64_t MinSize) : MinSize(MinSize) {}

    int64_t toBaseSize(int64_t size) {
        return toBaseSizeEx(size, MinSize).first;
    }

    /* capacity  is signed to avoid error converting
     * negative number to huge positive and creating huge couner */
    static int64_t initCounter(int64_t capacity) {
        if (capacity<=0) throw std::range_error("capacity must be positive");
        if (capacity>=0x20000000000000) throw std::range_error("too large capacity");
        int kind = toBaseSizeEx(capacity,0).second;
        assert(kind==2 || kind==3);
        long double limitCoeff = kind==2? 0.9L : 0.8L;
        auto result = (int64_t)(limitCoeff*capacity);
        if (result<1) result = 1;
        return result;
    }

    int64_t newCapacity(int64_t capacity, int64_t erased) {
        if (capacity<=0) throw std::range_error("capacity must be positive");
        if (capacity>=0x20000000000000) throw std::range_error("too large capacity");
        std::pair<int64_t, int> p = toBaseSizeEx(capacity,MinSize);
        int kind = p.second;
        assert(kind==2 || kind==3);
        long double limitCoeff = kind==2? 0.3L : 0.2L;
        if (erased>(int64_t)(limitCoeff*p.first))
            return p.first;
        else
            return grow(p);
    }
};


#endif //CACHE_RESIZELOGIC_H
