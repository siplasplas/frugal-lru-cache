//
// Created by Andrzej Borucki on 2022-09-05
//

#ifndef CACHE_RESIZELOGIC_H
#define CACHE_RESIZELOGIC_H

#include "BitScanner.h"
#include <cstddef>

class ResizeLogic {
    const size_t MinSize = 16;
public:
    explicit ResizeLogic(size_t MinSize) : MinSize(MinSize) {}
    size_t toBaseSize(size_t size){
        if (size<MinSize) size = MinSize;
        if (size<=2) return 2;
        int bitpos = BitScanner::BitScanReverse(size);
        assert(bitpos>=1);
        bool atLeast3 = (size & (1L<<(bitpos-1) ))!=0;
        bool serie2 = (size & (1L<<bitpos )) == size;
        bool serie3 = (size & (3L<<(bitpos-1) )) == size;
        if (serie2 || serie3) return size;
        if (atLeast3)
            return 1L << (bitpos+1);
        else
            return 3L << (bitpos-1);
    }
};


#endif //CACHE_RESIZELOGIC_H
