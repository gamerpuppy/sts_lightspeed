//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_CARD_INSTANCE_HPP
#define STS_CARD_INSTANCE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <array>

#include <sts/constants/card_ids.hpp>

namespace sts {

    class BattleContext;
    class Card;

    struct CardInstance  {
        // bytes
        // 2*2
        // 3*1
        // bitset 1 byte
        // total bytes: 8

        CardId id = CardId::INVALID;
        std::int16_t uniqueId = -1;
        std::int16_t specialData = 0;
        std::int8_t cost = 0;
        std::int8_t costForTurn = 0;

        // todo dont need all of these, also put in bitset
        bool upgraded = false;
        bool freeToPlayOnce = false;
        bool retain = false;

        CardInstance() = default;
        CardInstance(CardId id, bool upgraded=false);
        CardInstance(const Card &card);

        [[nodiscard]] CardId getId() const;
        [[nodiscard]] CardType getType() const;
        [[nodiscard]] const char *getName() const;
        [[nodiscard]] std::int16_t getUniqueId() const;
        [[nodiscard]] bool isUpgraded() const;
        [[nodiscard]] int getUpgradeCount() const;
        [[nodiscard]] bool canUpgrade() const;

        [[nodiscard]] bool isEthereal() const;
        [[nodiscard]] bool isStrikeCard() const;
        [[nodiscard]] bool doesExhaust() const;
        [[nodiscard]] bool hasSelfRetain() const;
        [[nodiscard]] bool requiresTarget() const;
        [[nodiscard]] bool isXCost() const;
        [[nodiscard]] bool isBloodCard() const;
        [[nodiscard]] bool usesSpecialData() const;

        // *****  *****
        void upgradeBaseCost(int newBaseCost); // Blood for Blood when upgraded
        void updateCost(int amount);
//        void modifyCostForCombat(int amount);

        void setCostForCombat(int cost);
        void setCostForTurn(int newCost);
        void setUniqueId(int uniqueId);
        void upgrade();
        void tookDamage(); // only call this on "Masterful Stab" and "Blood for Blood"

        std::ostream& printSimpleDesc(std::ostream &o) const;

        void triggerOnExhaust(BattleContext &bc);
        void triggerOnManualDiscard(BattleContext &bc);
        void triggerWhenDrawn(BattleContext &bc, int myHandIdx);


        [[nodiscard]] bool isFreeToPlay(const BattleContext &bc) const;

        [[nodiscard]] bool canUseOnAnyTarget(const BattleContext &bc) const; // not for use in critical path
        [[nodiscard]] bool canUse(const BattleContext &bc, int target, bool inAutoplay) const;
    };

    std::ostream& operator <<(std::ostream &os, const CardInstance &c);

    typedef CardInstance CI;


}

#endif //STS_CARD_INSTANCE_HPP
