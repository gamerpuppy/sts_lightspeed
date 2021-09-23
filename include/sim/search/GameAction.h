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
            CARD = 0,
            GOLD,       // 1
            KEY,        // 2
            POTION,     // 3
            RELIC,      // 4
            CARD_REMOVE, // 5
            SKIP,       // 6
        };

        std::uint32_t bits = -1;
        GameAction() = default;
        GameAction(std::uint32_t bits);
        GameAction(int idx1, int idx2=0);
        GameAction(RewardsActionType type, int idx1=0, int idx2=0);


        [[nodiscard]] bool isPotionAction() const;
        [[nodiscard]] bool isPotionDiscard() const;
        [[nodiscard]] RewardsActionType getRewardsActionType() const;

        [[nodiscard]] int getIdx1() const; // always positive
        [[nodiscard]] int getIdx2() const; // always positive
        [[nodiscard]] int getIdx3() const; // always positive


        std::ostream& printDesc(std::ostream &os, const sts::GameContext &gc) const;

        [[nodiscard]] bool isValidAction(const sts::GameContext &gc) const;

        void execute(GameContext &gc) const;
        static std::vector<GameAction> getAllActionsInState(const sts::GameContext &gc);
        static int getValidEventSelectBits(const sts::GameContext &gc);

    };





}


#endif //STS_LIGHTSPEED_GAMEACTION_H
