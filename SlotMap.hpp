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
#include "BaseHashMap.hpp"

template<typename K, typename V>
struct SlotT: BaseSlot<K,V> {
    slot_t next;
};

template<typename K, typename V>
class SlotMap : public BaseHashMap<K,V> {
protected:
    void skipEmpties(typename BaseHashMap<K,V>::iterator *it) override {
        slot_t nSlot = it->nSlot;
        while (nSlot <= BaseHashMap<K,V>::capacity_ && !slotUsed(nSlot))
            nSlot++;
        it->nSlot = nSlot;
        if (it->nSlot<=BaseHashMap<K,V>::capacity_)
            it->slot = &slots[it->nSlot];
        else
            it->slot = nullptr;
    }
    void increaseIt(typename BaseHashMap<K,V>::iterator *it) override {
        it->nSlot++;
        skipEmpties(it);
    }
private:
    using Slot = SlotT<K, V>;
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
        return (slot_t)(hash % BaseHashMap<K,V>::capacity_) + 1;
    }

    bool slotUsed(slot_t nSlot) {
        return !bits->isSlotFree(nSlot) && ebits->isSlotFree(nSlot);
    }
public:
    SlotMap(slot_t capacity, slot_t counter): BaseHashMap<K, V>(capacity, counter) {
        bits = new SlotBits<slot_t>(capacity);
        ebits = new SlotBits<slot_t>(capacity);
        slots = new Slot[capacity + 1];
    }

    ~SlotMap() {
        delete []slots;
        delete ebits;
        delete bits;
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
        for (int nStart=1; nStart<=BaseHashMap<K,V>::capacity_; nStart++)
            printSlotList(nStart);
    }

    void printSlotList(slot_t nStart) {
        std::vector<slot_t> prev(BaseHashMap<K,V>::capacity_ + 1);
        for (int n=1; n<=BaseHashMap<K,V>::capacity_; n++) {
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
        slot_t nSlot = startSlot(key);
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
        BaseHashMap<K,V>::counter--;
        return true;
    }

    void printStat() {
        slot_t maxlen = 0;
        for (slot_t i=0; i < BaseHashMap<K,V>::capacity_; i++)
            maxlen = std::max(maxlen, stat_slotlen(i));
        std::vector<slot_t> hist(maxlen+1);
        for (slot_t i=0; i < BaseHashMap<K,V>::capacity_; i++)
            hist[stat_slotlen(i)]++;
        slot_t sum = 0, count = 0;
        for (slot_t i=0; i<hist.size(); i++) {
            printf("%d: %f\n", i, double(hist[i]) / BaseHashMap<K,V>::capacity_);
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

#endif //CACHE_SLOTMAP_HPP
