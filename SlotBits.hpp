//
// Created by Andrzej Borucki on 2022-09-04
//

#ifndef CACHE_SLOTBITS_H
#define CACHE_SLOTBITS_H

#include <cstdint>
#include <cassert>
#include <limits>
#include <cstring>
#include <utility>
#include <stdexcept>

template<typename slot_t>
class SlotBits {
    typedef uint64_t bitType;
    const static slot_t bitsInWord = sizeof(bitType) * 8;
    bitType *bitmask;
    slot_t  numBitWords;
    slot_t capacity;
public:
    slot_t availCount;
    static slot_t limit() {
        return std::numeric_limits<slot_t>::max()-bitsInWord;
    }
    explicit SlotBits(slot_t capacity):capacity(capacity){
        if (capacity<=0)
            throw std::range_error("capacity must be positive");
        if (capacity>limit())
            throw std::range_error("capacity exceeds limit "+ std::to_string(limit()));
        numBitWords = (capacity + bitsInWord) / bitsInWord; //0 is reserved as null
        bitmask = new bitType[numBitWords];
        for (slot_t i=0; i < numBitWords; i++)
            bitmask[i] = ~bitType(0);
        setAsOccupied(0);
        for (slot_t i= capacity + 1; i < numBitWords * bitsInWord; i++)
            setAsOccupied(i);
        availCount = capacity;
    }
    ~SlotBits(){
        delete bitmask;
    }

    static std::pair<slot_t,slot_t> slotToPos(slot_t n) {
        assert(n>=0 && n<=limit());
        std::pair<slot_t,slot_t> p;
        p.first = n / bitsInWord;
        p.second = n % bitsInWord;
        assert(p.first>=0 && p.second>=0 && p.second<bitsInWord);
        p.second++;//range 1..64
        return p;
    }

    static slot_t posToSlot(slot_t first, slot_t second) {
        if (second==0)
            return 0;
        assert(first>=0 && second>0 && second<=bitsInWord);
        second--;
        slot_t n = first*bitsInWord + second;
        assert(n>=0 && n<=limit());
        return n;
    }

    static slot_t posToSlot(std::pair<slot_t,slot_t>& p) {
        return posToSlot(p.first, p.second);
    }
    //slow, for test only
    void test_setOccupiedRange(slot_t from, slot_t to) {
        for (slot_t n=from; n<=to; n++)
            setAsOccupied(n);
    }

    //for test only
    void test_setAsAvailable(slot_t n) {
        if (isSlotFree(n)) return;
        auto p = slotToPos(n);
        debug_setBit(bitmask[p.first], p.second);
        assert(isSlotFree(n));
        availCount++;
    }

    slot_t findSlotFrom(slot_t current) {
        slot_t from = current+1;
        auto p = slotToPos(from);
        slot_t bit_in_word = ffs_from(bitmask[p.first], p.second);
        slot_t res = posToSlot(p.first, bit_in_word);
        if (res>0)
            return res;
        for (slot_t i=p.first+1; i < numBitWords; i++)
            if (bitmask[i])
                return posToSlot(i, ffs_first(bitmask[i]));
        return 0;
    }

    slot_t findSlotTo(slot_t n) {
        auto p = slotToPos(n);
        for (slot_t i=0; i<p.first; i++)
            if (bitmask[i])
                return posToSlot(i, ffs_first(bitmask[i]));
        return posToSlot(p.first, ffs_to(bitmask[p.first], p.second));
    }

    slot_t findNextSlot(slot_t current) {
        slot_t res = findSlotFrom(current);
        assert(res==0 || res>current);
        if (res>0) return res;
        return findSlotTo(current);
    }

    void setAsOccupied(slot_t n) {
        if (!isSlotFree(n)) return;
        auto p = slotToPos(n);
        clearBit(bitmask[p.first], p.second);
        assert(!isSlotFree(n));
        availCount--;
    }

    void setAsErased(slot_t n){
        setAsOccupied(n);
    }

    bool isSlotFree(slot_t n) {
        auto p = slotToPos(n);
        return bitIsSet(bitmask[p.first], p.second);
    }

    bool isSlotOccupied(slot_t n) {
        return !isSlotFree(n);
    }
private:
    static slot_t ffs_first(bitType x) {
        return ffsll(x);
    }

    static slot_t ffs_from(bitType x, slot_t from) {
        assert(from>0 && from <= bitsInWord);
        bitType mask = (~bitType(0)) << (from-1);
        return ffsll(x & mask);
    }

    static slot_t ffs_to(bitType x, slot_t to) {
        assert(to > 0 && to <= bitsInWord);
        bitType mask = ((bitType)1<< (to-1)) - 1;
        return ffsll(x & mask);
    }

    static void clearBit(bitType& word, slot_t pos) {
        assert(pos > 0 && pos <= bitsInWord);
        bitType mask = ~((bitType)1 << (pos-1));
        word &= mask;
    }

    //for test only
    static void debug_setBit(bitType& word, slot_t pos) {
        assert(pos > 0 && pos <= bitsInWord);
        bitType mask = (bitType)1 << (pos-1);
        word |= mask;
    }

    static bool bitIsSet(bitType word, slot_t pos) {
        assert(pos > 0 && pos <= bitsInWord);
        bitType mask = (bitType)1 << (pos-1);
        return (word & mask)!=0;
    }

};

#endif //CACHE_SLOTBITS_H
