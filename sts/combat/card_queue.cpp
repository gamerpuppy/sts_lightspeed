//
// Created by gamerpuppy on 8/24/2021.
//

#include <sts/combat/card_queue.hpp>

using namespace sts;

bool CardQueue::isEmpty() const {
    return size == 0;
}

bool CardQueue::containsCardWithId(int uniqueId) const {
    int idx = frontIdx;
    for (int i = 0; i < size; ++i) {
        if (frontIdx >= capacity) {
            idx = 0;
        }
        if (arr[idx].card.getUniqueId() == uniqueId) {
            return true;
        }
    }
    return false;
}

void CardQueue::clear() {
    size = 0;
    backIdx = 0;
    frontIdx = 0;
}

void CardQueue::pushFront(CardQueueItem item) {
    assert(size != arr.size());
    --frontIdx;
    ++size;
    if (frontIdx < 0) {
        frontIdx = capacity - 1;
    }
    arr.at(frontIdx) = std::move(item);
}

void CardQueue::pushBack(CardQueueItem item) {
    assert(size != capacity);
    arr.at(backIdx) = std::move(item);
    ++backIdx;
    ++size;
    if (backIdx >= capacity) {
        backIdx = 0;
    }
}

CardQueueItem CardQueue::popFront() {
    assert(size > 0);
    CardQueueItem &item = arr.at(frontIdx);
    ++frontIdx;
    --size;
    if (frontIdx >= capacity) {
        frontIdx = 0;
    }
    return item;
}

CardQueueItem CardQueue::popBack() {
    assert(size > 0);
    --backIdx;
    --size;
    if (backIdx < 0) {
        backIdx = arr.size() - 1;
    }
    CardQueueItem &item = arr.at(backIdx);
    return item;
}

CardQueueItem &CardQueue::front() {
    assert(size > 0);
    return arr.at(frontIdx);
}
