//
// Created by Andrzej Borucki on 2022-09-03
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-const-return-type"

#ifndef GTEST_LRU_LIST_HPP
#define GTEST_LRU_LIST_HPP

#include <cassert>
#include <stdexcept>
#include <iostream>

namespace cache {
    template<typename data_t>
    class List {
        struct Link {
            Link *next_;
            Link *previous_;
            data_t data_;
            explicit Link(data_t data, Link *next, Link *previous) :
                            next_(next), previous_(previous), data_(data) {}
        };
        Link *frontLink;
        Link *tailLink;
    public:
        class iterator {
            Link* ptr_;
            friend class List;
        public:
            iterator(): ptr_(nullptr) {}
            explicit iterator(Link* ptr): ptr_(ptr) {}
            int operator* () { return ptr_->data_; }
            iterator& operator++()
            {
                if (ptr_->next_)
                    ptr_ = ptr_->next_;
                return *this;
            }
            const iterator operator++(int)
            {
                iterator temp = *this;
                if (ptr_->next_)
                    ptr_ = ptr_->next_;
                return temp;
            }
            iterator& operator--()
            {
                if (ptr_->previous_)
                    ptr_ = ptr_->previous_;
                return *this;
            }
            const iterator operator--(int)
            {
                iterator temp = *this;
                if (ptr_->previous_)
                    ptr_ = ptr_->previous_;
                assert(ptr_);
                return temp;
            }

            iterator& operator= ( const iterator& i )
            {
                ptr_ = i.ptr_;
                return *this;
            }

            friend bool operator==(const iterator& __x, const iterator& __y) {
                return __x.ptr_ == __y.ptr_;
            }

            friend bool operator!=(const iterator& __x, const iterator& __y) {
                return __x.ptr_ != __y.ptr_;
            }
        };
        List(){
            tailLink = new Link(data_t(), nullptr, nullptr);
            frontLink = tailLink;
        }

        bool empty() {
            return frontLink == tailLink;
        }

        void push_front(data_t data){
            Link * temp = new Link(data, frontLink, nullptr);
            assert(frontLink);
            frontLink->previous_ = temp;
            frontLink = temp;
        }
        void pop_front(){
            Link *next = frontLink->next_;
            if (!next)
                throw std::runtime_error("pop_front from empty list");
            next->previous_ = nullptr;
            delete frontLink;
            frontLink = next;
        }
        void push_back(data_t data) { //insert before tail
            assert(tailLink);
            Link *previous = tailLink->previous_;
            Link *temp = new Link(data, tailLink, previous);
            if (previous)
                previous->next_ = temp;
            tailLink->previous_ = temp;
        }
        void pop_back(){ //remove after tail
            assert(tailLink);
            Link* temp = tailLink->previous_;
            if (!temp)
                throw std::runtime_error("pop_back from empty list");
            Link* previous = temp->previous_;
            if (previous) {
                previous->next_ = tailLink;
            } else
                frontLink = tailLink;
            tailLink->previous_ = previous;
            delete temp;
        }
        iterator begin() { return iterator(frontLink); }
        iterator end() { return iterator(tailLink); }
        void erase(const iterator position) {
            if (empty())
                throw std::runtime_error("erase from empty list");
            if (position.ptr_ == tailLink)
                throw std::runtime_error("erase from end of list");
            Link *temp = position.ptr_;
            if (temp->previous_)
                temp->previous_->next_ = temp->next_;
            assert(temp->next_); //tail is always
            temp->next_->previous_ = temp->previous_;
            if (temp == frontLink)
                frontLink = temp->next_;
            delete temp;
        }

        void display() {
            for (iterator it=begin(); it!=end(); it++)
                std::cout << *it << std::endl;
            std::cout << std::endl;
            iterator it=end();
            do {
                it--;
                std::cout << *it << std::endl;
            } while (it!=begin());
        }

        void insert(const iterator position, data_t data) {
            Link *postemp = position.ptr_;
            Link * newtemp = new Link(data, postemp, postemp->previous_);
            if (postemp->previous_)
                postemp->previous_->next_ = newtemp;
            else {
                assert(frontLink==postemp);
                frontLink = newtemp;
            }
            postemp->previous_ = newtemp;
            assert(newtemp->next_);
        }
    };
}

#endif //GTEST_LRU_LIST_HPP

#pragma clang diagnostic pop