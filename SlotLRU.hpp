//
// Created by Andrzej Borucki on 2022-09-04
//

#ifndef CACHE_SLOTLRU_HPP
#define CACHE_SLOTLRU_HPP

#include "SlotList.hpp"
#include <utility>
#include <unordered_map>

namespace cache {
    template<typename ptr_t, typename key_t, typename value_t>
    class SlotLRU {
    public:
        typedef typename std::pair<key_t, value_t> key_value_pair_t;
    private:
        SlotList<ptr_t, key_value_pair_t> *_cache_items_list;
        std::unordered_map<key_t, ptr_t> _cache_items_map;
        size_t _max_size;
    public:
        explicit SlotLRU(size_t max_size) :
                _max_size(max_size) {
            _cache_items_list = new SlotList<ptr_t, key_value_pair_t>(max_size+1);
        }

        void put(const key_t& key, const value_t& value) {
            auto mapit = _cache_items_map.find(key);
            _cache_items_list->push_front(key_value_pair_t(key, value));
            if (mapit != _cache_items_map.end()) {
                ptr_t link = mapit->second;
                _cache_items_list->erase(link);
                _cache_items_map.erase(mapit);
            }
            _cache_items_map[key] = _cache_items_list->frontLink;

            if (_cache_items_map.size() > _max_size) {
                auto last = _cache_items_list->tailLink;
                _cache_items_list->dec(last);
                key_t key = _cache_items_list->data(last).first;
                auto iter = _cache_items_map.find(key);
                if (iter == _cache_items_map.end())
                    printf("dd");
                _cache_items_map.erase(key);
                _cache_items_list->pop_back();
            }
        }

        const value_t get(const key_t& key) {
            auto mapit = _cache_items_map.find(key);
            if (mapit == _cache_items_map.end()) {
                throw std::range_error("There is no such key in cache");
            } else {
                ptr_t link = mapit->second;
                _cache_items_list->moveToFront(link);
                return _cache_items_list->data(link).second;
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

#endif //CACHE_SLOTLRU_HPP
