//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_CARDQUEUE_H
#define STS_LIGHTSPEED_CARDQUEUE_H

#include <array>
#include <cassert>

#include <sts/combat/card_instance.hpp>

namespace sts {

    struct CardQueueItem {
        CardInstance card; // todo taking a copy is is incorrect because the card needs to be read from hand I believe in some scenarios, not sure
//        int cardIdx;
        int target = 0;

        // special data
        bool isEndTurn = false;
        bool triggerOnUse = true;
        bool ignoreEnergyTotal = false;
        int energyOnUse = 0;
        bool freeToPlay = false;
        bool randomTarget = false;
        bool autoplay = false;
        int regretCardCount = 0; // maybe use to indicate triggerOnUse too?

        bool purgeOnUse = false;
        bool exhaustOnUse = false;

        CardQueueItem() = default;
        CardQueueItem(const CardInstance &card, int target, int energyOnUse) : card(card), target(target), energyOnUse(energyOnUse) {}

        static CardQueueItem endTurnItem() {
            CardQueueItem ret;
            ret.isEndTurn = true;
            return ret;
        }
    };

    struct CardQueue {
        static constexpr int capacity = 10;
        int size = 0;
        int backIdx = 0;
        int frontIdx = 0;
        std::array<CardQueueItem, capacity> arr;

        void clear();
        void pushFront(CardQueueItem item);
        void pushBack(CardQueueItem item);
        [[nodiscard]] bool isEmpty() const;
        [[nodiscard]] bool containsCardWithId(int uniqueId) const;

        CardQueueItem popFront();
        CardQueueItem popBack();
        CardQueueItem &front();


    };

}


#endif //STS_LIGHTSPEED_CARDQUEUE_H
