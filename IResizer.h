//
// Created by Andrzej Borucki on 2022-09-06
//

#ifndef CACHE_IRESIZER_H
#define CACHE_IRESIZER_H


#include <cstdint>

class IResizer {
public:
    virtual int64_t toBaseSize(int64_t size) = 0;
    virtual int64_t initCounter(int64_t capacity) = 0;
    virtual int64_t newCapacity(int64_t capacity, int64_t erased) = 0;
};


#endif //CACHE_IRESIZER_H
