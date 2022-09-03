//
// Created by Andrzej Borucki on 2022-09-03
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-sizeof-expression"

#ifndef CACHE_HASHMAP_HPP
#define CACHE_HASHMAP_HPP

#include "murmur.h"
#include <cstddef>

namespace cache {
    template<typename K, typename V>
    class HashMap {
        //uint capacity_;
        typedef std::pair<K,V> map_pair_t;
        struct Link {
            Link *next_ = nullptr;
            map_pair_t pair_;
            Link(map_pair_t pair, Link *next):pair_(pair), next_(next) {}
        };
        typedef Link* PLink;
        struct IteratorExchange {
            uint capacity_;
            PLink* arena;
            explicit IteratorExchange(uint capacity) {
                capacity_ = capacity;
                arena = new PLink[capacity];
                memset(arena, 0, capacity*sizeof(PLink));
            }

            ~IteratorExchange() {
                delete []arena;
            };

            uint getSlot(K key)
            {
                auto hash = murmur3_32(&key,sizeof(key));
                uint nSlot = hash % capacity_;
                return nSlot;
            }

            PLink findKey(uint nSlot, K key) {
                PLink link = arena[nSlot];
                while (link) {
                    if (link->pair_.first == key)
                        return link;
                    link = link->next_;
                }
                return nullptr;
            }

            PLink findPreKey(uint nSlot, K key) {
                PLink prelink = PLink(1);
                PLink link = arena[nSlot];
                while (link) {
                    if (link->pair_.first == key)
                        return prelink;
                    prelink = link;
                    link = link->next_;
                }
                return nullptr;
            }
        };
        uint size_ = 0;
        IteratorExchange* ix_;

        void changeValue(PLink link, K key){
            link->pair_.second = key;
        }
    public:
        class iterator : public std::iterator<std::forward_iterator_tag, map_pair_t> {
            friend class HashMap;
            Link *link_ = nullptr;
            uint slot_ = 0;
            IteratorExchange* ix_;
            void increaseIterator() {
                link_ = link_->next_;
                if (link_) return;
                do {
                    slot_++;
                    assert(slot_ <= ix_->capacity_ && link_ == nullptr);
                    if (slot_ == ix_->capacity_) return;
                    link_ = ix_->arena[slot_];
                } while(!link_);
            }
        public:
            iterator(uint slot, Link* link, IteratorExchange* ix_): slot_(slot), link_(link), ix_(ix_) {}
            map_pair_t operator*() {
                return link_->pair_;
            }
            iterator& operator++()
            {
                increaseIterator();
                return *this;
            }
            const iterator operator++(int)
            {
                iterator temp = *this;
                temp->increaseIterator();
                return temp;
            }

            friend bool operator==(const iterator& __x, const iterator& __y) {
                return __x.slot_ == __y.slot_ && __x.link_ == __y.link_;
            }

            friend bool operator!=(const iterator& __x, const iterator& __y) {
                return __x.slot_ != __y.slot_ || __x.link_ != __y.link_;
            }
        };
        explicit HashMap(uint capacity) {
            ix_ = new IteratorExchange(capacity);
        }

        ~HashMap() {
            delete ix_;
        }

        uint size() {
            return size_;
        }

        void put(const map_pair_t &pair) {
            K key = pair.first;
            uint nSlot = ix_->getSlot(key);
            PLink pLink = ix_->findKey(nSlot, key);
            if (pLink)
                changeValue(pLink, pair.second);
            else {
                pLink = new Link(pair, ix_->arena[nSlot]);
                ix_->arena[nSlot] = pLink;
                size_++;
            }
        }

        void put(K key, V value) {
            put(std::make_pair(key, value));
        }

        iterator find(K key) {
            uint nSlot = ix_->getSlot(key);
            PLink pLink = ix_->findKey(nSlot, key);
            if (pLink)
                return iterator(nSlot, pLink, ix_);
            else
                return iterator(ix_->capacity_, nullptr, ix_);
        }

        void erase(K key) {
            uint nSlot = ix_->getSlot(key);
            PLink preLink = ix_->findPreKey(nSlot, key);
            if (!preLink)
                return;
            PLink link;
            if (preLink==PLink(1)) {
                link = ix_->arena[nSlot];
                ix_->arena[nSlot] = link->next_;
            } else {
                link = preLink->next_;
                preLink->next_ = link->next_;
            }
            delete link;
        }

        iterator begin() {
            for (int nSlot=0; nSlot<ix_->capacity_; nSlot++) {
                auto pLink = ix_->arena[nSlot];
                if (pLink)
                    return iterator(nSlot, pLink, ix_);
            }
            return iterator(ix_->capacity_, nullptr, ix_);
        }

        iterator end() {
            return iterator(ix_->capacity_, nullptr, ix_);
        }
    };
}


#endif //CACHE_HASHMAP_HPP

#pragma clang diagnostic pop