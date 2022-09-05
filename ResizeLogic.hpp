//
// Created by Andrzej Borucki on 2022-09-05
//

#ifndef CACHE_RESIZELOGIC_H
#define CACHE_RESIZELOGIC_H

#include "BitScanner.h"

class ResizeLogic {
    const size_t MinSize;
    [[nodiscard]] std::pair<size_t,int> toBaseSizeEx(size_t size) const {
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
public:
    explicit ResizeLogic(size_t MinSize) : MinSize(MinSize) {}

    [[nodiscard]] size_t toBaseSize(size_t size) const {
        return toBaseSizeEx(size).first;
    }
};


#endif //CACHE_RESIZELOGIC_H
