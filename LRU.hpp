//
// Created by Andrzej Borucki on 2022-09-03
//

#ifndef CACHE_LRU_HPP
#define CACHE_LRU_HPP

#include "List.hpp"
#include <utility>
#include <unordered_map>

namespace cache {
    template<typename key_t, typename value_t>
    class LRU {
    public:
        typedef typename std::pair<key_t, value_t> key_value_pair_t;
        typedef typename List<key_value_pair_t>::iterator list_iterator_t;
    private:
        List<key_value_pair_t> _cache_items_list;
        std::unordered_map<key_t, list_iterator_t> _cache_items_map;
        size_t _max_size;
    public:
        LRU(size_t max_size) :
                _max_size(max_size) {
        }

        void put(const key_t& key, const value_t& value) {
            auto it = _cache_items_map.find(key);
            _cache_items_list.push_front(key_value_pair_t(key, value));
            if (it != _cache_items_map.end()) {
                _cache_items_list.erase(it->second);
                _cache_items_map.erase(it);
            }
            _cache_items_map[key] = _cache_items_list.begin();

            if (_cache_items_map.size() > _max_size) {
                auto last = _cache_items_list.end();
                last--;
                _cache_items_map.erase(last->first);
                _cache_items_list.pop_back();
            }
        }

        const value_t& get(const key_t& key) {
            auto it = _cache_items_map.find(key);
            if (it == _cache_items_map.end()) {
                throw std::range_error("There is no such key in cache");
            } else {
                _cache_items_list.moveToFront(it->second);
                return it->second->second;
            }
        }

        bool exists(const key_t& key) const {
            return _cache_items_map.find(key) != _cache_items_map.end();
        }

        size_t size() const {
            return _cache_items_map.size();
        }
    };
}

#endif //CACHE_LRU_HPP
