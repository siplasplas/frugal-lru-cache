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
        ptr_t freeList = 0;
        void toFreeList(ptr_t pos) {
            arena[pos].next_ = freeList;
            freeList = pos;
        }

        //0: error
        ptr_t fromFreeList() {
            if (!freeList) return 0;
            auto old_freeList = freeList;
            freeList = arena[freeList].next_;
            return old_freeList;
        }

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
            auto newPos = fromFreeList();
            if (!newPos)
                newPos = heapPos_;
            Link &temp = arena[newPos];
            temp.data_ = data;
            temp.next_ = frontLink;
            Link &frontLinkBody = arena[frontLink];
            frontLinkBody.previous_ = newPos;
            frontLink = newPos;
            if (newPos==heapPos_) heapPos_++;
        }

        void pop_front(){
            assert(frontLink==heapPos_-1);
            ptr_t next = arena[frontLink].next_;
            if (!next && !arena[0].previous_)
                throw std::runtime_error("pop_front from empty list");
            arena[next].previous_ = 0;
            frontLink = next;
            heapPos_--;
        }
        void push_back(data_t data) { //insert before tail
            auto newPos = fromFreeList();
            if (!newPos)
                newPos = heapPos_;
            Link &temp = arena[newPos];
            temp.data_ = data;
            temp.next_ = tailLink;
            ptr_t previous =  arena[tailLink].previous_;
            temp.previous_ = previous;
            Link &frontLinkBody = arena[frontLink];
            if (previous)
                arena[previous].next_ = newPos;
            arena[tailLink].previous_ = newPos;
            if (newPos==heapPos_) heapPos_++;
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
            toFreeList(temp);
        }
        void erase(ptr_t pos) {
            if (empty())
                throw std::runtime_error("erase from empty list");
            if (pos == tailLink)
                throw std::runtime_error("erase from end of list");
            ptr_t temp = pos;
            if (arena[temp].previous_)
                arena[arena[temp].previous_].next_ = arena[temp].next_;
            arena[arena[temp].next_].previous_ = arena[temp].previous_;
            if (temp == frontLink)
                frontLink = arena[temp].next_;
            toFreeList(temp);
        }

        void insert(const ptr_t pos, data_t data) {
            auto newPos = fromFreeList();
            if (!newPos)
                newPos = heapPos_;
            ptr_t newtemp = newPos;
            if (newPos==heapPos_) heapPos_++;;
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
