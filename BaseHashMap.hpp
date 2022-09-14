//
// Created by Andrzej Borucki on 2022-09-13
//
#ifndef CACHE_BASEHASHMAP_H
#define CACHE_BASEHASHMAP_H

#include <vector>

using slot_t = uint32_t;

template<typename K, typename V>
struct BaseSlot {
    std::pair<K,V> pair;
};

template<typename K, typename V>
class BaseHashMap {
protected:
    friend class iterator;
    slot_t capacity_;
public:
    bool putPair(std::pair<K,V> pair) {
        return put(pair.first, pair.second);
    }
    virtual bool put(K key, V value) = 0;
    virtual BaseSlot<K,V> *find(const K key) = 0;

    virtual slot_t erasedCount() = 0;
    virtual slot_t size() = 0;
    slot_t counter;
    BaseHashMap(slot_t capacity, slot_t counter): capacity_(capacity), counter(counter) {
    }
    virtual ~BaseHashMap() {

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
            slot = nullptr;
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

public:
    virtual void skipEmpties(iterator *it) = 0;
    virtual void increaseIt(iterator *it) = 0;
};


#endif //CACHE_BASEHASHMAP_H
