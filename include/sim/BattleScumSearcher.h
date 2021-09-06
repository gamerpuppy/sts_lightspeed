//
// Created by keega on 9/6/2021.
//

#ifndef STS_LIGHTSPEED_BATTLESCUMSEARCHER_H
#define STS_LIGHTSPEED_BATTLESCUMSEARCHER_H

#include "combat/BattleContext.h"
#include "sim/StateHandler.h"

#include <iostream>

namespace sts {


    struct StateValue {
        int playerHp = 0;
        int enemyCombinedHp = 0;

        bool operator<(const StateValue &rhs) const;
        bool operator>(const StateValue &rhs) const;
        bool operator<=(const StateValue &rhs) const;
        bool operator>=(const StateValue &rhs) const;

        friend std::ostream &operator<<(std::ostream &os, const StateValue &value);
    };

    struct SearchInfo {
        BattleContext bc;
        int curOptionIdx = -1;

        int stateSize = 0;
        RandomBattleStateHandler handler;

        StateValue value; // equal to the cur value or the best state resulting from itself
        int bestOptionIdx = -1;

        SearchInfo(const BattleContext &bc);
    };

    struct BattleScumSearcher {
        std::vector<SearchInfo> bestInfos;

        void search(const BattleContext &bc, int depth);
        // ************

        static StateValue evaluateState(const BattleContext &bc);
    };

    std::ostream& operator<<(std::ostream &os, const SearchInfo &s);
}

#endif //STS_LIGHTSPEED_BATTLESCUMSEARCHER_H
