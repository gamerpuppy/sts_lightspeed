//
// Created by gamerpuppy on 7/7/2021.
//

#ifndef STS_LIGHTSPEED_REWARDS_H
#define STS_LIGHTSPEED_REWARDS_H

#include <string>

#include <sts/data_structure/fixed_list.hpp>
#include <sts/constants/card_ids.hpp>
#include <sts/constants/potions.hpp>
#include <sts/constants/relic_ids.hpp>
#include <sts/game/card.hpp>

namespace sts {

    class GameContext;

    struct Rewards {
        int goldRewardCount = 0;
        std::array<int,2> gold {-1, -1};

        int cardRewardCount = 0;
        std::array<CardReward, 5> cardRewards;

        int relicCount = 0;
        std::array<RelicId, 3> relics {RelicId::INVALID, RelicId::INVALID, RelicId::INVALID};

        int potionCount = 0;
        std::array<Potion,5> potions {Potion::INVALID, Potion::INVALID, Potion::INVALID, Potion::INVALID, Potion::INVALID};

        bool emeraldKey = false;
        bool sapphireKey = false;

        void addGold(int goldAmt);
        void addRelic(RelicId relic);
        void addPotion(Potion potion);
        void addCardReward(CardReward reward);

        void removeGoldReward(int idx);
        void removeCardReward(int removeIdx);
        void removeRelicReward(int removeIdx);
        void removePotionReward(int idx);
        void clear();

        int getTotalCount() const;

        [[nodiscard]] std::string toString() const;

        Rewards() = default;
        Rewards(Potion* p, int count);
        Rewards(RelicId relic);
        Rewards(CardReward cardReward);
    };


}

#endif //STS_LIGHTSPEED_REWARDS_H
