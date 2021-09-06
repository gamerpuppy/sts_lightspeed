//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_ACTIONQUEUE_H
#define STS_LIGHTSPEED_ACTIONQUEUE_H

#include <bitset>
#include <functional>
#include <cassert>


namespace sts {

    class BattleContext;
    typedef std::function<void (BattleContext &)> ActionFunction;

    struct Action {
        ActionFunction actFunc;
        bool clearOnCombatVictory = true;

        Action() = default;
        Action(ActionFunction a) : actFunc(std::move(a)) {}
        Action(ActionFunction a, bool b) : actFunc(std::move(a)), clearOnCombatVictory(b) {}
    };


    // Simple deque
    template<int capacity>
    struct ActionQueue {
        friend BattleContext;
        int size = 0;
        int back = 0;
        int front = 0;
        std::array<ActionFunction, capacity> arr;
        std::bitset<capacity> bits; // for the shouldClear field

        void clear();
        void pushFront(Action a);
        void pushBack(Action a);
        bool isEmpty();
        ActionFunction popFront();

        int getCapacity();
//        ActionFunction popBack();
//        void clearOnCombatVictory();
    };

    template<int capacity>
    void ActionQueue<capacity>::clear() {
        size = 0;
        back = 0;
        front = 0;
    }

    template <int capacity>
    void ActionQueue<capacity>::pushFront(Action a) {
        assert(size != arr.size());
        --front;
        ++size;
        if (front < 0) {
            front = capacity-1;
        }
        bits.set(front, a.clearOnCombatVictory);
        arr.at(front) = std::move(a.actFunc);
    }

    template<int capacity>
    void ActionQueue<capacity>::pushBack(Action a) {
        assert(size != capacity);
        if (back >= capacity) {
            back = 0;
        }
        bits.set(back, a.clearOnCombatVictory);
        arr.at(back) = std::move(a.actFunc);
        ++back;
        ++size;
    }

    template<int capacity>
    bool ActionQueue<capacity>::isEmpty() {
        return size == 0;
    }

    template<int capacity>
    ActionFunction ActionQueue<capacity>::popFront() {
        assert(size > 0);
        ActionFunction &a = arr.at(front);
        ++front;
        --size;
        if (front >= capacity) {
            front = 0;
        }
        return a;
    }

    template<int capacity>
    int ActionQueue<capacity>::getCapacity() {
        return capacity;
    }


    // this was bugged, some ub somewhere
//    template<int capacity>
//    ActionFunction ActionQueue<capacity>::popBack() {
//        assert(size > 0);
//        --back;
//        --size;
//        if (back < 0) {
//            back = arr.size() - 1;
//        }
//        ActionFunction &a = arr.at(back);
//        return a;
//    }

//    template<int capacity>
//    void ActionQueue<capacity>::clearOnCombatVictory() {
//
//        int tempSize = 0;
//        std::array<ActionFunction, capacity> tempArr {nullptr};
//        std::bitset<capacity> tempBits;
//
//
////        while (size > 0) {
////            popFront()
////        }
//        int cur = front;
//        for (int i = 0; i < size; ++i) {
//            if (cur >= capacity) {
//                cur = 0;
//            }
//            const auto shouldClearAction = bits[cur];
//            if (!shouldClearAction) {
////                tempBits.set(tempSize, shouldClearAction);
//                tempArr.at(tempSize) = arr.at(tempSize);
//                ++tempSize;
//            }
//        }
//
//        front = 0;
//        back = 0;
//        size = tempSize;
//
//        bits.reset();
//        arr = tempArr;
//
//        for (int i = 0; i < tempSize; ++i) {
//            arr[i] = std::move(tempArr[i]);
//        }
//    }


}


#endif //STS_LIGHTSPEED_ACTIONQUEUE_H
