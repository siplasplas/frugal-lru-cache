//
// Created by Andrzej Borucki on 2022-09-03
//

#ifndef CACHE_SLOTLIST_H
#define CACHE_SLOTLIST_H

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <cassert>

namespace cache {
    template<typename ptr_t, typename data_t>
    class SlotList {
        struct Link {
            ptr_t next_;
            ptr_t previous_;
            data_t data_;
            Link() {}
        };
        ptr_t capacity_;
        ptr_t heapPos_ = 0;
        Link* arena;
    public:
        ptr_t frontLink;
        ptr_t tailLink;
        ptr_t size() { return heapPos_;}

        data_t data(ptr_t slot) {
            return arena[slot].data_;
        }

        void inc(ptr_t &x) {
            x = arena[x].next_;
        }

        void dec(ptr_t &x) {
            x = arena[x].previous_;
        }

        explicit SlotList(ptr_t capacity) : capacity_(capacity) {
            arena = new Link[capacity+1];
            memset(arena, 0, (capacity+1)*sizeof(Link));
            tailLink = 0;
            frontLink = tailLink;
            heapPos_++;
        }
        ~SlotList() {
            delete []arena;
        }

        bool empty() {
            return frontLink == tailLink;
        }

        void push_front(data_t data){
            assert(heapPos_>0 && heapPos_<=capacity_);
            Link &temp = arena[heapPos_];
            temp.data_ = data;
            temp.next_ = frontLink;
            Link &frontLinkBody = arena[frontLink];
            frontLinkBody.previous_ = heapPos_;
            frontLink = heapPos_;
            heapPos_++;
        }

        void pop_front(){
            ptr_t next = arena[frontLink].next_;
            if (!next && !arena[0].previous_)
                throw std::runtime_error("pop_front from empty list");
            arena[next].previous_ = 0;
            frontLink = next;
        }
        void push_back(data_t data) { //insert before tail
            Link &temp = arena[heapPos_];
            temp.data_ = data;
            temp.next_ = tailLink;
            ptr_t previous =  arena[tailLink].previous_;
            temp.previous_ = previous;
            Link &frontLinkBody = arena[frontLink];
            if (previous)
                arena[previous].next_ = heapPos_;
            arena[tailLink].previous_ = heapPos_;
            heapPos_++;
        }
        void pop_back(){ //remove after tail
            ptr_t temp = arena[tailLink].previous_;
            if (!temp)
                throw std::runtime_error("pop_back from empty list");
            ptr_t previous = arena[temp].previous_;
            if (previous) {
                arena[previous].next_ = tailLink;
            } else
                frontLink = tailLink;
            arena[tailLink].previous_ = previous;
        }
        void erase(ptr_t pos) {
            if (empty())
                throw std::runtime_error("erase from empty list");
            if (pos == tailLink)
                throw std::runtime_error("erase from end of list");
            ptr_t temp = pos;
            if (arena[temp].previous_)
                arena[arena[temp].previous_].next_ = arena[temp].next_;
//            assert(arena[temp].next_); //tail is always
            arena[arena[temp].next_].previous_ = arena[temp].previous_;
            if (temp == frontLink)
                frontLink = arena[temp].next_;
        }

        void insert(const ptr_t pos, data_t data) {
            ptr_t newtemp = heapPos_;
            heapPos_++;
            arena[newtemp].next_ = pos;
            arena[newtemp].data_ = data;
            ptr_t posprevious = arena[pos].previous_;
            arena[newtemp].previous_ = posprevious;
            if (posprevious)
                arena[posprevious].next_ = newtemp;
            else {
                assert(frontLink==pos);
                frontLink = newtemp;
            }
            arena[pos].previous_ = newtemp;
            //assert(arena[newtemp].next_);
        }

        void moveToFront(ptr_t pos) {
            data_t data = arena[pos].data_;
            erase(pos);
            push_front(data);
        }

    };
}

#endif //CACHE_SLOTLIST_H
