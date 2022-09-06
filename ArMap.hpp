//Auto-resizable map
//
// Created by Andrzej Borucki on 2022-09-06
//

#ifndef ARMAP_HPP
#define ARMAP_HPP

#include "SlotMap.hpp"
#include "FrugalResizer.hpp"

template<typename slot_t, typename K, typename V>
class ArMapT {
    const static slot_t MinSize = 16;
    SlotMapT<slot_t, K, V> *slotMap;
    FrugalResizer *rl;

    void resize() {
        slot_t newCapacity = rl->newCapacity(capacity(), slotMap->erasedCount());
        auto new_slotMap = new SlotMapT<slot_t, K, V>(newCapacity, rl->initCounter(newCapacity));
        for (auto p: *slotMap) {
            new_slotMap->put(p);
        }
        delete slotMap;
        slotMap = new_slotMap;
    }

public:
    using Slot = SlotT<slot_t, K, V>;
    ArMapT() {
        rl = new FrugalResizer(MinSize);
        slotMap = new SlotMapT<slot_t, K, V>(MinSize, rl->initCounter(MinSize));
    }
    ~ArMapT() {
        delete slotMap;
        delete rl;
    }

    slot_t capacity() {
        return slotMap->capacity();
    }
    slot_t size() {
        return slotMap->size();
    }

    Slot *find(const K key) {
        return slotMap->find(key);
    }

    void put(K key, V value)     {
        if (!slotMap->put(key, value))
            throw std::overflow_error("SlotMap overflows");
        if (!slotMap->counter)
            resize();
    }

    void erase(const K key) {
        slotMap->erase(key);
    }
};

template<typename K, typename V> using ArMap = ArMapT<uint32_t, K,V>;
template<typename K, typename V> using ArMap16 = ArMapT<uint16_t, K,V>;

#endif //ARMAP_HPP
