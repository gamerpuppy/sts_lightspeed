//
// Created by gamerpuppy on 9/6/2021.
//

#include "sim/BattleScumSearcher.h"


using namespace sts;

SearchInfo::SearchInfo(const BattleContext &bc) : bc(bc) {
    if (bc.outcome != Outcome::UNDECIDED) {
        stateSize = 0;
    } else {
        stateSize = handler.setupState(bc);
    }
}

void BattleScumSearcher::search(const BattleContext &bc, int depth) {
    bestInfos.clear();
    std::vector<SearchInfo> searchStack;
    searchStack.emplace_back(bc);

    while (true) {
        if (searchStack.empty()) {
            return;
        }

        auto &cur = searchStack.back();

        const auto stopSearchingFromNode = ++cur.curOptionIdx >= cur.stateSize
                                           || searchStack.size() >= depth+1;
        if (stopSearchingFromNode) {
            cur.value = evaluateState(cur.bc);

            // no more actions for this state.
            if (bestInfos.empty() || cur.value > bestInfos.back().value) {
                bestInfos = searchStack;
            }
            for (int i = 0; i < bestInfos.size(); ++i) {
                bestInfos[i].bestOptionIdx = bestInfos[i].curOptionIdx;
            }
            searchStack.pop_back();
            continue;
        }

        // curnode is not a terminal node, create a new search node based on some action
        BattleContext nextState(cur.bc);
        cur.handler.chooseOption(nextState, cur.curOptionIdx);
        searchStack.emplace_back(nextState);
    }
}

StateValue BattleScumSearcher::evaluateState(const BattleContext &bc) {
    int combinedHp = 0;
    for (int i = 0; i < bc.monsters.monsterCount; ++i) {
        const auto &m = bc.monsters.arr[i];
        if (!m.isDeadOrEscaped()) {
            combinedHp += m.curHp;
        }
    }

    return {bc.player.curHp, combinedHp};
}

namespace sts {

    std::ostream &operator<<(std::ostream &os, const SearchInfo &s) {
        os << "bestOption:" << s.bestOptionIdx <<  " " << s.value;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const StateValue &value) {
        os << "playerHp: " << value.playerHp << " enemyHp: " << value.enemyCombinedHp;
        return os;
    }

    bool StateValue::operator<(const StateValue &rhs) const {
        if (playerHp < rhs.playerHp)
            return true;
        if (rhs.playerHp < playerHp)
            return false;
        return enemyCombinedHp > rhs.enemyCombinedHp;
    }

    bool StateValue::operator>(const StateValue &rhs) const {
        return rhs < *this;
    }

    bool StateValue::operator<=(const StateValue &rhs) const {
        return !(rhs < *this);
    }

    bool StateValue::operator>=(const StateValue &rhs) const {
        return !(*this < rhs);
    }
}


