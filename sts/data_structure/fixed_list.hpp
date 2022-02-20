//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_FIXEDLIST_H
#define STS_LIGHTSPEED_FIXEDLIST_H

#include <array>

namespace sts {

    template<typename T, int capacity>
    class fixed_list {
    private:
        int list_size = 0;
        std::array<T,capacity> arr;

    public:
        typedef T* iterator;
        typedef const T* const_iterator;

        fixed_list() = default;
        fixed_list(const fixed_list &rhs) = default;
        fixed_list(int size) noexcept : list_size(size) {}
//        fixed_list(fixed_list &&rhs)  noexcept : list_size(rhs.list_size), optionMap(std::move(rhs.optionMap)) {}

        fixed_list(std::initializer_list<T> l) {
            for (auto x : l) {
                arr[list_size++] = x;
            }
        }

        int size() const {
            return list_size;
        }

        iterator begin() {
            return arr.begin();
        }

        const_iterator begin() const {
            return arr.begin();
        }

        iterator end() {
            return arr.begin()+list_size;
        }

        const_iterator end() const {
            return arr.begin()+list_size;
        }

        T& operator[](int idx) {
            return arr[idx];
        }

        const T& operator[](int idx) const {
            return arr[idx];
        }

        T& front() {
            return arr[0];
        }

        const T& front() const {
            return arr[0];
        }

        T& back() {
            return arr[list_size-1];
        }

        const T& back() const {
            return arr[list_size-1];
        }

        T pop_back() noexcept {
            return arr[--list_size];
        }


        void insert(int idx, T t) {
            for (int i = list_size; i > idx; --i) {
                arr[i] = arr[i-1];
            }
            arr[idx] = t;
            list_size++;
        }

        void insert(iterator it, T t) {
            for (T* i = arr.end()-1; i != it; --i) {
                *i = *(i-1);
            }
            *it = t;
            list_size++;
        }

        void push_back(T t) {
            arr[list_size++] = std::move(t);
        }

        void remove(int idx) {
            while (idx+1 < list_size) {
                arr[idx] = arr[idx+1];
                ++idx;
            }
            --list_size;
        }

        void erase(iterator it) {
            while ((it+1) != end()) {
                *it = *(it+1);
                ++it;
            }
            --list_size;
        }

        void remove_back() {
            --list_size;
        }

        [[nodiscard]] bool empty() const {
            return list_size == 0;
        }

        void clear() {
            list_size = 0;
        }

        void resize(int size) {
            list_size = size;
        }

    };




}

#endif //STS_LIGHTSPEED_FIXEDLIST_H
