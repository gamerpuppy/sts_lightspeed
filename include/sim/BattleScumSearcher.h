//
// Created by gamerpuppy on 9/6/2021.
//

#ifndef STS_LIGHTSPEED_BATTLESCUMSEARCHER_H
#define STS_LIGHTSPEED_BATTLESCUMSEARCHER_H

#include "combat/BattleContext.h"
#include "sim/StateHandler.h"

#include <iostream>

namespace sts {


    struct StateValue {
        double score = 0;

        bool operator<(const StateValue &rhs) const;
        bool operator>(const StateValue &rhs) const;
        bool operator<=(const StateValue &rhs) const;
        bool operator>=(const StateValue &rhs) const;

        friend std::ostream &operator<<(std::ostream &os, const StateValue &value);
    };

    struct SearchInfo {
        BattleContext bc;
        int optionIdx = -1;
        int stateSize = 0;
        RandomBattleStateHandler handler;
        StateValue value; // equal to the cur value or the best state resulting from itself
    };

    struct BattleScumSearcher {
        std::vector<SearchInfo> bestInfos;

        void search(const BattleContext &bc, int depth);
        // ************

        static StateValue evaluateState(const BattleContext &bc);
    };

    struct ScumSearcherAgent {
        std::default_random_engine rng;
        int choiceCount = 0;
        bool print = false;
        const int searchDepth = 6;

        ScumSearcherAgent(const std::default_random_engine &rng) : rng(rng) {}


        void playout(GameContext &gc);
        void playoutBattle(BattleContext &gc);

        void chooseRandom(GameContext &gc);
        void pickGoodEventOutcome(GameContext &gc);

    };


    std::ostream& operator<<(std::ostream &os, const SearchInfo &s);
}

#endif //STS_LIGHTSPEED_BATTLESCUMSEARCHER_H
