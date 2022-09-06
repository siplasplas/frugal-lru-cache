//
// Created by Andrzej Borucki on 2022-09-06
//

#ifndef CACHE_SIMPLERESIZER_HPP
#define CACHE_SIMPLERESIZER_HPP

#include "BitScanner.h"
#include "IResizer.h"
#include <utility>
#include <cassert>
#include <stdexcept>

class SimpleResizer : public IResizer {
    const int64_t MinSize;
    static int64_t toBaseSizeEx(int64_t size, int64_t MinSize) {
        if (size<MinSize) size = MinSize;
        if (size<=2) return 2;
        int bitpos = BitScanner::BitScanReverse(size);
        assert(bitpos>=1);
        if  ((1L<<bitpos ) == size)
            return size;
        else
            return 1L << (bitpos+1);
    }

    int64_t grow(int64_t size) {
        int bitpos = BitScanner::BitScanReverse(size);
        return 1L << (bitpos+1);
    }

public:
    explicit SimpleResizer(int64_t MinSize) : MinSize(MinSize) {}

    int64_t toBaseSize(int64_t size) override{
        return toBaseSizeEx(size, MinSize);
    }

    /* capacity  is signed to avoid error converting
     * negative number to huge positive and creating huge counter */
    int64_t initCounter(int64_t capacity) override {
        if (capacity<=0) throw std::range_error("capacity must be positive");
        if (capacity>=0x20000000000000) throw std::range_error("too large capacity");
        long double limitCoeff = 0.9L;
        auto result = (int64_t)(limitCoeff*capacity);
        if (result<1) result = 1;
        return result;
    }

    int64_t newCapacity(int64_t capacity, int64_t erased) override {
        if (capacity<=0) throw std::range_error("capacity must be positive");
        if (capacity>=0x20000000000000) throw std::range_error("too large capacity");
        int64_t baseSize = toBaseSizeEx(capacity,MinSize);
        long double limitCoeff = 0.3L;
        if (erased > (int64_t)(limitCoeff * baseSize))
            return baseSize;
        else
            return grow(baseSize);
    }
};


#endif //CACHE_SIMPLERESIZER_HPP
