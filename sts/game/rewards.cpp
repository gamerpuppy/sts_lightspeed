//
// Created by gamerpuppy on 7/7/2021.
//

#include <sts/game/rewards.hpp>

#include <sstream>

#include <sts/game/misc.hpp>
#include <sts/game/game_context.hpp>

using namespace sts;

void Rewards::addGold(int goldAmt) {
    gold[goldRewardCount++] = goldAmt;
}

void Rewards::addRelic(RelicId relic) {
    relics[relicCount++] = relic;
}

void Rewards::addPotion(Potion potion) {
    potions[potionCount++] = potion;
}

void Rewards::addCardReward(CardReward reward) {
    cardRewards[cardRewardCount++] = reward;
}

void Rewards::removeGoldReward(int idx) {
    if (idx == 0 && goldRewardCount == 2) {
        gold[0] = gold[1];
    }
    --goldRewardCount;
}

void Rewards::removeCardReward(int removeIdx) {
    for (int i = removeIdx; i < cardRewardCount; ++i) {
        cardRewards[i] = cardRewards[i+1];
    }
    --cardRewardCount;
}

void Rewards::removeRelicReward(int removeIdx) {
    for (int i = removeIdx; i < relicCount-1; ++i) {
        relics[i] = relics[i+1];
    }
    --relicCount;
}

void Rewards::removePotionReward(int idx) {
    while (idx + 1 < potionCount) {
        potions[idx] = potions[idx+1];
        ++idx;
    }
    --potionCount;
}

void Rewards::clear() {
    goldRewardCount = 0;
    cardRewardCount = 0;
    relicCount = 0;
    potionCount = 0;
    emeraldKey = false;
    sapphireKey = false;
}

int Rewards::getTotalCount() const {
    return goldRewardCount +
        potionCount +
        cardRewardCount +
        (sapphireKey ? 1 : 0) +
        (emeraldKey ? 1 : 0);
}

void writeCardReward(std::stringstream &ss, CardReward cardReward) {
    ss << "{ ";
    for (int i = 0; i < cardReward.size(); ++i) {
        auto c = cardReward[i];
        ss << c.getName();
        if (i+1 < cardReward.size()) {
            ss << ", ";
        }
    }
    ss << " }";
}

std::string Rewards::toString() const {
    std::stringstream ss;
    ss << "Rewards { \n";
    for (int i = 0; i < cardRewardCount; ++i) {
        ss << '\t';
        writeCardReward(ss, cardRewards[i]);
        ss << '\n';
    }

    for (int i = 0; i < goldRewardCount; ++i) {
        ss << '\t';
        auto g = gold[i];
        ss << std::to_string(g) << "g" << '\n';
    }

    for (int i = 0; i < potionCount; ++i) {
        ss << '\t';
        auto p = potions[i];
        ss << potionNames[(int)p] << '\n';
    }

    for (int i = 0; i < relicCount; ++i) {
        ss << '\t';
        auto r = relics[i];
        ss << relicNames[static_cast<int>(r)] << '\n';
    }

    if (emeraldKey) {
        ss << "emerald key\n";
    }
    if (sapphireKey) {
        ss << "sapphire key\n";
    }

    ss << "}\n";

    return ss.str();
}

Rewards::Rewards(Potion *p, int count) {
    for (int i = 0; i < count; ++i) {
        addPotion(p[i]);
    }
}

Rewards::Rewards(RelicId relic) {
    addRelic(relic);
}


Rewards::Rewards(CardReward reward) {
    addCardReward(reward);
}


