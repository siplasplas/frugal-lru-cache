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

template<typename K, typename V>
struct SlotM {
    std::pair<K,V> pair;
    SlotM* next;
};


template<typename K, typename V>
class HashMap {
public:
    uint32_t counter;
private:
    uint32_t capacity_;
    uint32_t size_ = 0;
    friend class iterator;
    using Slot = SlotM<K, V>;
    using PSlot = Slot*;
    PSlot *slots;
public:
    HashMap(uint32_t capacity, uint32_t counter): capacity_(capacity), counter(counter) {
        slots = new PSlot[capacity + 1];
        memset(slots,0, sizeof(PSlot)*(capacity + 1));
    }
    ~HashMap(){
        clear();
    };
    uint32_t startSlot(const K key) {
        auto hash = murmur3_32(&key,sizeof(key));
        return (uint32_t)(hash % capacity_) + 1;
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
        counter--;
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
        for (uint32_t i=1; i<=capacity_; i++)
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
            counter++;
            return;
        }
        auto previous = slot;
        slot = slot->next;
        while(slot) {
            if (slot->pair.first==key) {
                previous->next = slot->next;
                delete slot;
                size_--;
                counter++;
                return;
            }
            previous = slot;
            slot = slot->next;
        }
    }
};

#endif //CACHE_HASHMAP_H