//
// Created by Andrzej Borucki on 2022-08-31
//

#ifndef CACHE_SLOTMAP_HPP
#define CACHE_SLOTMAP_HPP

#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <cstdio>
#include <algorithm>
#include "murmur.h"
//todo class SlotBits
struct Slot {
    int key;
    int value;
    int next;
};

typedef uint64_t bitType;
const static int bitsInWord = sizeof(bitType) * 8;

class  SlotMap {
    uint capacity;
    int numMaskWords;
    Slot *slots;
    bitType *bitmask;

    static int ffs_first(bitType x) {
        int res = ffsll(x);
        return res-1;
    }

    static int ffs_from(bitType x, int from) {
        assert(from>=0 && from <bitsInWord);
        bitType mask = bitType(-1) << from;
        int res = ffsll(x & mask);
        return res-1;
    }

    static int ffs_to(bitType x, int to) {
        assert(to >= 0 && to < bitsInWord);
        bitType mask = ((bitType)1<< to) - 1;
        int res = ffsll(x & mask);
        return res-1;
    }

    int posToSlot(const std::pair<int,int>& p) {
        return p.first*bitsInWord + p.second;
    }

    int posToSlot(int first, int second) {
        if (second<0)
            return -1;
        return first*bitsInWord + second;
    }

    std::pair<int,int> slotToPos(int n) {
        std::pair<int,int> p;
        p.first = n / bitsInWord;
        p.second = n % bitsInWord;
        return p;
    }

    int findSlotFrom(int n) {
        auto p = slotToPos(n);
        int bit_in_word = ffs_from(bitmask[p.first], p.second);
        int res = posToSlot(p.first, bit_in_word);
        if (res>=0)
            return res;
        for (int i=p.first+1; i<numMaskWords; i++)
            if (bitmask[i])
                return posToSlot(i, ffs_first(bitmask[i]));
        return -1;
    }

    int findSlotTo(int n) {
        auto p = slotToPos(n);
        for (int i=0; i<p.first; i++)
            if (bitmask[i])
                return posToSlot(i, ffs_first(bitmask[i]));
        return posToSlot(p.first, ffs_to(bitmask[p.first], p.second));
    }

    int findNextSlot(int current) {
        auto p = slotToPos(current);
        bool lastBit = p.first==numMaskWords-1 && p.second==bitsInWord-1;
        if (!lastBit) {
            int res = findSlotFrom(current+1);
            assert(res<0 || res>current);
            if (res>=0) return res;
        }
        return findSlotTo(current);
    }

    static void clearBit(bitType& word, int pos) {
        bitType mask = ~((bitType)1 << pos);
        word &= mask;
    }

    static bool bitIsSet(bitType word, int pos) {
        bitType mask = (bitType)1 << pos;
        return (word & mask)!=0;
    }

    bool isFree(int n) {
        auto p = slotToPos(n);
        return bitIsSet(bitmask[p.first], p.second);
    }

    void setAsOccupied(int n) {
        auto p = slotToPos(n);
        clearBit(bitmask[p.first], p.second);
        assert(!isFree(n));
    }

    uint stat_slotlen(uint nSlot) {
        if (isFree(nSlot)) return 0;
        uint slotlen = 0;
        while (nSlot) {
            Slot &slot = slots[nSlot];
            slotlen++;
            nSlot = slot.next;
        }
        return slotlen;
    }
public:
    explicit SlotMap(uint capacity):capacity(capacity) {
        numMaskWords = (capacity + bitsInWord) / bitsInWord; //0 is reserved as null
        bitmask = new bitType[numMaskWords];
        slots = new Slot[capacity + 1];
        for (int i=0; i<numMaskWords; i++)
            bitmask[i] = bitType(-1);
        setAsOccupied(0);
        for (int i= capacity + 1; i < numMaskWords * bitsInWord; i++)
            setAsOccupied(i);
    }

    ~SlotMap() {
        delete []slots;
    }

    int put(int key, int  value) {
        auto hash = murmur3_32(&key,sizeof(key));
        int nSlot = (int)(hash % capacity) + 1; //0 is reserved as nullptr
        int nSlot2 = 0;
        if (!isFree(nSlot)) {
            nSlot2 = findNextSlot(nSlot);
            if (nSlot2<0)
                return -1;
            slots[nSlot2] = slots[nSlot];
            setAsOccupied(nSlot2);
        } else
            setAsOccupied(nSlot);
        Slot &slot = slots[nSlot];
        slot.key = key;
        slot.value = value;
        slot.next = nSlot2;
        return 0;
    }

    void print() {
        uint maxlen = 0;
        for (uint i=0; i < capacity; i++)
            maxlen = std::max(maxlen, stat_slotlen(i));
        std::vector<uint> hist(maxlen+1);
        for (uint i=0; i < capacity; i++)
            hist[stat_slotlen(i)]++;
        uint sum = 0, count = 0;
        for (uint i=0; i<hist.size(); i++) {
            printf("%d: %f\n", i, double(hist[i]) / capacity);
            if (i>0) {
                sum += i * hist[i];
                count+=hist[i];
            }
        }
        printf("average %f\n",double(sum)/count);
    }
};
#endif //CACHE_SLOTMAP_HPP
