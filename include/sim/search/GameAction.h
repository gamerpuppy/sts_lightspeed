//
// Created by keega on 9/22/2021.
//

#ifndef STS_LIGHTSPEED_GAMEACTION_H
#define STS_LIGHTSPEED_GAMEACTION_H

#include "sts_common.h"

#include <iostream>
#include <vector>

namespace sts {
    class GameContext;
}

namespace sts::search {

    struct GameAction {

        enum class RewardsActionType {
            CARD=0,
            GOLD,
            KEY,
            POTION,
            RELIC,
            CARD_REMOVE,
            SKIP,
        };

        std::uint32_t bits = -1;

        GameAction() = default;

        [[nodiscard]] int getSelectIdx() const;

        [[nodiscard]] bool isPotionAction() const;
        [[nodiscard]] bool isPotionDiscard() const;

        [[nodiscard]] RewardsActionType getRewardsActionType() const;

        [[nodiscard]] int getIdx1() const;
        [[nodiscard]] int getIdx2() const;
        [[nodiscard]] int getIdx3() const;



        [[nodiscard]] bool isValidAction(const sts::GameContext &gc) const;
        std::ostream& printDesc(std::ostream &os, const sts::GameContext &gc) const;

        void execute(GameContext &gc) const;

        static int getValidEventSelectBits(const sts::GameContext &gc);
        static std::vector<GameAction> getAllActionsInState(const sts::GameContext &gc);
    };





}


#endif //STS_LIGHTSPEED_GAMEACTION_H
