//
// Created by Andrzej Borucki on 2022-09-13
//
#ifndef CACHE_BASEHASHMAP_H
#define CACHE_BASEHASHMAP_H

#include <vector>
#include "BaseMap.h"

using slot_t = uint32_t;

template<typename K, typename V>
class BaseHashMap : public BaseMap<K,V> {
protected:
    friend class iterator;
    slot_t capacity_;
public:
    slot_t counter;
    BaseHashMap(slot_t capacity, slot_t counter): capacity_(capacity), counter(counter) {
    }
    slot_t capacity() {
        return capacity_;
    }
    class iterator : public std::iterator<std::forward_iterator_tag, std::pair<K,V>> {
        BaseHashMap *owner;
    public:
        slot_t nSlot;
        BaseSlot<K,V> *slot;
        iterator(BaseHashMap *owner, slot_t nSlot) : owner(owner), nSlot(nSlot) {
            owner->skipEmpties(this);
        }
        std::pair<K,V> operator*() {   return slot->pair; }
        std::pair<K,V>* operator-> () { return &slot->pair; }
        iterator& operator++() {
            owner->increaseIt(this);
            return *this;
        }
    };
    friend bool operator==(const iterator& _x, const iterator& _y) {
        return _x.slot == _y.slot ;
    }

    friend bool operator!=(const iterator& _x, const iterator& _y) {
        return _x.slot != _y.slot;
    }

    iterator begin() {
        auto it = iterator(this, 1);
        return it;
    }

    iterator end() {
        auto it = iterator(this, capacity_+1);
        return it;
    }

protected:
    virtual void skipEmpties(iterator *it) = 0;
    virtual void increaseIt(iterator *it) = 0;
};


#endif //CACHE_BASEHASHMAP_H
