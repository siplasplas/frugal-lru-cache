//
// Created by Andrzej Borucki on 2022-09-13
//
// Extended cache

#ifndef CACHE_PLANARCACHE_H
#define CACHE_PLANARCACHE_H

#include <cstdint>
#include "ArMap.hpp"

template<typename K, typename V>
class PlanarCache {
    int nPlanes;
    uint32_t capacity;
    bool extended;
    std::vector<BaseHashMap<K,V>*> planes;
public:
    PlanarCache(int nPlanes, uint32_t capacity, bool extended):
                    nPlanes(nPlanes), capacity(capacity), extended(extended) {
        planes.template emplace_back(createMap());
    }

    BaseHashMap<K,V>* createMap(){
        if (extended)
            return new ArMap<K,V>;
        else
            return new SlotMap<K,V>(capacity+capacity/10, 0);
    }

    ~PlanarCache() {
        for (auto *map: planes)
            delete map;
    }

    void put(K key, V value) {
        auto mainMap = planes[0];
        assert(mainMap->size()<=capacity);
        if (mainMap->size()==capacity) {
            assert(planes.size()<=nPlanes);
            if (planes.size()==nPlanes) {
                auto map = planes.back();
                delete map;
                planes.pop_back();
            }
            planes.insert(planes.begin(), createMap());
            assert(planes.size()<=nPlanes);
            mainMap = planes[0];
        }
        mainMap->put(key,value);
    }

    BaseSlot<K,V>* find(const K key) {
        for (auto *map: planes) {
            auto slot = map->find(key);
            if (slot)
                return slot;
        }
        return nullptr;
    }
};


#endif //CACHE_PLANARCACHE_H
