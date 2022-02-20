//
// Created by gamerpuppy on 6/27/2021.
//

#ifndef STS_LIGHTSPEED_CARD_H
#define STS_LIGHTSPEED_CARD_H


#include <sts/constants/card_ids.hpp>
#include <sts/data_structure/fixed_list.hpp>

namespace sts {

    struct Card {
        CardId id = CardId::INVALID;
        std::int16_t misc = 0;
        bool upgraded = false;

        Card() = default;
        Card(CardId id) : id(id) {}
        Card(CardId id, int upgraded) : id(id), upgraded(upgraded) {}

        [[nodiscard]] CardId getId() const;
        [[nodiscard]] int getUpgraded() const;
        [[nodiscard]] bool isUpgraded() const;

        [[nodiscard]] bool isInnate() const;
        [[nodiscard]] const char* getName() const;
        [[nodiscard]] CardType getType() const;
        [[nodiscard]] CardRarity getRarity() const;
        [[nodiscard]] int getBaseDamage() const;
        [[nodiscard]] bool canUpgrade() const;
        [[nodiscard]] bool canTransform() const;

        [[nodiscard]] bool isStrikeCard() const;
        [[nodiscard]] bool isStarterStrikeOrDefend() const;
        [[nodiscard]] bool isStarterStrike() const;


        bool operator==(const Card &rhs) const;
        bool operator!=(const Card &rhs) const;

        void upgrade();
    };

    typedef fixed_list<Card,4> CardReward;

}

#endif //STS_LIGHTSPEED_CARD_H
