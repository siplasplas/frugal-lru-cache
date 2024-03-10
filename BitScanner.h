//
// Created by Andrzej Borucki on 2022-09-05
//

#ifndef BITSCANNER_H
#define BITSCANNER_H

#include <cstdint>

/*
    https://stackoverflow.com/questions/31374628/fast-way-of-finding-most-and-least-significant-bit-set-in-a-64-bit-integer
    https://en.wikipedia.org/wiki/De_Bruijn_sequence
    https://www.chessprogramming.org/De_Bruijn_Sequence_Generator

    special case for argument == 0, need chcek if equal zero before */
class BitScanner {
const static uint64_t Magic = 0x37E84A99DAE458F;
constexpr static int MagicTable[] =
            {
                    0, 1, 17, 2, 18, 50, 3, 57,
                    47, 19, 22, 51, 29, 4, 33, 58,
                    15, 48, 20, 27, 25, 23, 52, 41,
                    54, 30, 38, 5, 43, 34, 59, 8,
                    63, 16, 49, 56, 46, 21, 28, 32,
                    14, 26, 24, 40, 53, 37, 42, 7,
                    62, 55, 45, 31, 13, 39, 36, 6,
                    61, 44, 12, 35, 60, 11, 10, 9,
            };
public:
static int BitScanForward(uint64_t b)
    {
        return MagicTable[((uint64_t) ((long) b & -(long) b)*Magic) >> 58];
    }

static int BitScanReverse(uint64_t b)
    {
        b |= b >> 1;
        b |= b >> 2;
        b |= b >> 4;
        b |= b >> 8;
        b |= b >> 16;
        b |= b >> 32;
        b = b & ~(b >> 1);
        return MagicTable[b*Magic >> 58];
    }
};

constexpr int BitScanner::MagicTable[];

#endif //BITSCANNER_H
