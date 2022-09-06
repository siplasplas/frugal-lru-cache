//
// Created by Andrzej Borucki on 2022-08-31
//

#ifndef CACHE_SLOTMAP_HPP
#define CACHE_SLOTMAP_HPP

#include <vector>
#include <iostream>
#include "murmur.h"
#include "SlotBits.hpp"
#include "FrugalResizer.hpp"

template<typename slot_t, typename K, typename V>
struct SlotT {
    std::pair<K,V> pair;
    slot_t next;
};

template<typename slot_t, typename K, typename V>
class SlotMapT {
public:
    using Slot = SlotT<slot_t, K, V>;
    slot_t counter;
    slot_t capacity_;
private:
    friend class iterator;
    Slot *slots;
    SlotBits<slot_t> *bits;
    SlotBits<slot_t> *ebits;

    slot_t stat_slotlen(slot_t nSlot) {
        if (bits->isSlotFree(nSlot)) return 0;
        slot_t slotlen = 0;
        while (nSlot) {
            Slot &slot = slots[nSlot];
            slotlen++;
            nSlot = slot.next;
        }
        return slotlen;
    }

    slot_t findNFrom(const K key, slot_t nSlot) {
        while (nSlot) {
            Slot &slot = slots[nSlot];
            if (slot.pair.first==key) return nSlot;
            nSlot = slot.next;
        }
        return 0;
    }

    Slot* findFromSlot(slot_t nFoundSlot) {
        if (!nFoundSlot)
            return nullptr;
        Slot &slot = slots[nFoundSlot];
        return &slot;
    }

    Slot* findFrom(const K key, slot_t nSlot){
        return findFromSlot(findNFrom(key, nSlot));
    }

    slot_t findN(const K key) {
        return findNFrom(key, startSlot(key));
    }

    slot_t startSlot(const K key) {
        auto hash = murmur3_32(&key,sizeof(key));
        return (slot_t)(hash % capacity_) + 1;
    }

    bool slotAvail(slot_t nSlot) {
        return !bits->isSlotFree(nSlot) && ebits->isSlotFree(nSlot);
    }

public:
    class iterator : public std::iterator<std::forward_iterator_tag, std::pair<K,V>> {
        SlotMapT *map;
        void skipEmpties() {
            while (nSlot<=map->capacity() && !map->slotAvail(nSlot))
                nSlot++;
        }
    public:
        slot_t nSlot;
        iterator(SlotMapT *map, slot_t nSlot) : map(map), nSlot(nSlot) {
            skipEmpties();
        }
        std::pair<K,V> operator*() {   return map->slots[nSlot].pair; }
        std::pair<K,V>* operator-> () { return &map->slots[nSlot].pair; }
        iterator& operator++() {
            nSlot++;
            skipEmpties();
            return *this;
        }
    };
    friend bool operator==(const iterator& _x, const iterator& _y) {
        return _x.nSlot == _y.nSlot ;
    }

    friend bool operator!=(const iterator& _x, const iterator& _y) {
        return _x.nSlot != _y.nSlot;
    }

public:
    explicit SlotMapT(slot_t capacity): capacity_(capacity) {
        bits = new SlotBits<slot_t>(capacity);
        ebits = new SlotBits<slot_t>(capacity);
        slots = new Slot[capacity + 1];
        counter = (slot_t)(FrugalResizer::initCounter(capacity));
    }

    ~SlotMapT() {
        delete []slots;
        delete ebits;
        delete bits;
    }

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, capacity_+1);
    }


    slot_t capacity() {
        return capacity_;
    }

    slot_t size() {
        return ebits->availCount-bits->availCount;
    }

    Slot* find(const K key) {
        slot_t slot = findNFrom(key, startSlot(key));
        if (bits->isSlotFree(slot)) return nullptr;
        if (ebits->isSlotOccupied(slot)) return nullptr;
        return findFromSlot(slot);
    }

    void erase(const K key) {
        auto nSlot = findN(key);
        if (nSlot)
            ebits->setAsErased(nSlot);
    }

    void printSlotListFrom(slot_t n) {
        while (n) {
            K key = slots[n].key;
            if (startSlot(key)==n)
                printf("-%d(*%d)", n,key);
            else
                printf("-%d(%d)", n,key);
            n = slots[n].next;
        }
    }

    void printSlotLists() {
        for (int nStart=1; nStart<=capacity; nStart++)
            printSlotList(nStart);
    }

    void printSlotList(slot_t nStart) {
        std::vector<slot_t> prev(capacity + 1);
        for (int n=1; n<=capacity; n++) {
            prev[slots[n].next] = n;
        }
        printSlotListFrom(nStart);
        slot_t n = nStart;
        slot_t n1 = 0;
        while (n) {
            n1 = n;
            n = prev[n];
        }
        printf(" whole list: ");
        printSlotListFrom(n1);
        printf("\n");
    }

    bool put(std::pair<K,V> pair) {
        return put(pair.first, pair.second);
    }

    bool put(K key, V value) {
        auto hash = murmur3_32(&key,sizeof(key));
        slot_t nSlot = (slot_t)(hash % capacity_) + 1; //0 is reserved as nullptr
        slot_t nSlot2 = 0;
        if (bits->isSlotOccupied(nSlot)) {
            Slot *slot = findFrom(key, nSlot);
            if (slot) { //replace value
                slot->pair.second = value;
                ebits->setAsAvailable(nSlot);
                return true;
            }
            nSlot2 = bits->findNextSlot(nSlot);
            if (nSlot2<0)
                return false;
            slots[nSlot2] = slots[nSlot];
            bits->setAsOccupied(nSlot2);
        } else
            bits->setAsOccupied(nSlot);
        Slot &slot = slots[nSlot];
        slot.pair.first = key;
        slot.pair.second = value;
        slot.next = nSlot2;
        counter--;
        return true;
    }

    void printStat() {
        slot_t maxlen = 0;
        for (slot_t i=0; i < capacity; i++)
            maxlen = std::max(maxlen, stat_slotlen(i));
        std::vector<slot_t> hist(maxlen+1);
        for (slot_t i=0; i < capacity; i++)
            hist[stat_slotlen(i)]++;
        slot_t sum = 0, count = 0;
        for (slot_t i=0; i<hist.size(); i++) {
            printf("%d: %f\n", i, double(hist[i]) / capacity);
            if (i>0) {
                sum += i * hist[i];
                count+=hist[i];
            }
        }
        printf("average %f\n",double(sum)/count);
    }

    slot_t erasedCount() {
        return ebits->erasedCount();
    }
};

template<typename K, typename V> using SlotMap = SlotMapT<uint32_t, K,V>;
template<typename K, typename V> using SlotMap16 = SlotMapT<uint16_t, K,V>;

#endif //CACHE_SLOTMAP_HPP
