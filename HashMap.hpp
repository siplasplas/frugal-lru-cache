//
// Created by Andrzej Borucki on 2022-09-13
//

#ifndef CACHE_HASHMAP_H
#define CACHE_HASHMAP_H

#include <cstdint>
#include <utility>
#include <cstring>
#include <cassert>
#include "murmur.h"
#include "BaseHashMap.hpp"

template<typename K, typename V>
struct SlotM : BaseSlot<K,V> {
    SlotM* next;
};


template<typename K, typename V>
class HashMap: public BaseHashMap<K,V> {
protected:
    uint32_t size_ = 0;
    using Slot = SlotM<K, V>;
    using PSlot = Slot*;
    PSlot *slots;
protected:
    void skipEmpties(typename BaseHashMap<K,V>::iterator *it) override {
        slot_t nSlot = it->nSlot+1;
        while (nSlot <= BaseHashMap<K,V>::capacity_ && !(slots[nSlot]))
            nSlot++;
        it->nSlot = nSlot;
        if (it->nSlot <= BaseHashMap<K,V>::capacity_)
            it->slot = slots[it->nSlot];
        else
            it->slot = nullptr;
    }
    void increaseIt(typename BaseHashMap<K,V>::iterator *it) override {
        it->slot = PSlot(it->slot)->next;
        if (!it->slot)
            skipEmpties(it);
    }
public:
    HashMap(uint32_t capacity, uint32_t counter): BaseHashMap<K, V>(capacity,counter) {
        slots = new PSlot[capacity + 1];
        memset(slots,0, sizeof(PSlot)*(capacity + 1));
    }
    ~HashMap(){
        clear();
    };
    slot_t erasedCount() {return 0;}
    uint32_t startSlot(const K key) {
        auto hash = murmur3_32(&key,sizeof(key));
        return (uint32_t)(hash % BaseHashMap<K,V>::capacity()) + 1;
    }
    bool put(std::pair<K,V> pair) {
        return put(pair.first, pair.second);
    }
    bool put(K key, V value) {
        auto slot = find(key);
        if (slot) {
            assert(slot->pair.first==key);
            slot->pair.second = value;
            return true;
        }
        uint32_t nSlot = startSlot(key);
        slot = new Slot;
        slot->pair = std::make_pair(key, value);
        slot->next = slots[nSlot];
        slots[nSlot] = slot;
        size_++;
        BaseHashMap<K,V>::counter--;
        return true;
    }
    void deleteSlot(uint32_t nSlot) {
        auto slot = slots[nSlot];
        while(slot) {
            auto next = slot->next;
            delete slot;
            slot = next;
        }
    }
    void clear() {
        for (uint32_t i=1; i<=BaseHashMap<K,V>::capacity_; i++)
            deleteSlot(i);
    }

    uint32_t size() {
        return size_;
    }

    Slot* find(const K key) {
        uint32_t nSlot = startSlot(key);
        auto slot = slots[nSlot];
        while(slot) {
            if (slot->pair.first==key)
                return slot;
            slot = slot->next;
        }
        return nullptr;
    }

    void erase(const K key) {
        uint32_t nSlot = startSlot(key);
        auto slot = slots[nSlot];
        if (slot->pair.first==key) {
            slots[nSlot] = slot->next;
            delete slot;
            size_--;
            BaseHashMap<K,V>::counter++;
            return;
        }
        auto previous = slot;
        slot = slot->next;
        while(slot) {
            if (slot->pair.first==key) {
                previous->next = slot->next;
                delete slot;
                size_--;
                BaseHashMap<K,V>::counter++;
                return;
            }
            previous = slot;
            slot = slot->next;
        }
    }
};

#endif //CACHE_HASHMAP_H
