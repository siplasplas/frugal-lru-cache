//Auto-resizable map
//
// Created by Andrzej Borucki on 2022-09-06
//

#ifndef ARMAP_HPP
#define ARMAP_HPP

//#include "SlotMap.hpp"
#include "FrugalResizer.hpp"
#include "SimpleResizer.hpp"
#include "SlotMap.hpp"
#include "BaseMap.h"

template<typename K, typename V>
class ArMap : public BaseHashMap<K,V> {
    BaseHashMap<K, V> *slotMap;
    IResizer *rl;

    void resize() {
        slot_t newCapacity = rl->newCapacity(capacity(), slotMap->erasedCount());
        auto new_slotMap = new SlotMap<K, V>(newCapacity, rl->initCounter(newCapacity));
        for (auto p: *slotMap) {
            new_slotMap->putPair(p);
        }
        delete slotMap;
        slotMap = new_slotMap;
    }

public:
    enum ResizerEnum {reSimple, reFrugal};
    //using Slot = SlotT<K, V>;
    ArMap(slot_t MinSize = 16, ResizerEnum resizerEnum = reFrugal)
                    : BaseHashMap<K, V>(MinSize, 0){
        if (resizerEnum == reFrugal)
            rl = new FrugalResizer(MinSize);
        else
            rl = new SimpleResizer(MinSize);
        slotMap = new SlotMap<K, V>(MinSize, rl->initCounter(MinSize));
    }
    ~ArMap() {
        delete slotMap;
        delete rl;
    }

    slot_t capacity() {
        return slotMap->capacity();
    }
    slot_t size() {
        return slotMap->size();
    }

    BaseSlot<K,V> *find(const K key) {
        return slotMap->find(key);
    }

    bool put(K key, V value) override {
        if (!slotMap->put(key, value))
            throw std::overflow_error("SlotMap overflows");
        if (!slotMap->counter)
            resize();
        return true;
    }

    void erase(const K key) {
        slotMap->erase(key);
    }

    slot_t erasedCount() override {
        return slotMap->erasedCount();
    }

    void skipEmpties(typename BaseHashMap<K,V>::iterator *it) override{
        slotMap->skipEmpties(it);
    }

    void increaseIt(typename BaseHashMap<K,V>::iterator *it) override {
        slotMap->increaseIt(it);
    }
};

#endif //ARMAP_HPP
