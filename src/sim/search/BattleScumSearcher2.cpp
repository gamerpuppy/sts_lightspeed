//
// Created by keega on 9/18/2021.
//

#include "sim/search/BattleScumSearcher2.h"
#include "sim/search/ExpertKnowledge.h"

#include <utility>
#include <string>
#include <memory>

using namespace sts;

std::int64_t simulationIdx = 0; // for debugging

namespace sts::search {
    thread_local search::BattleScumSearcher2 *g_debug_scum_search;
}



search::BattleScumSearcher2::BattleScumSearcher2(const BattleContext &bc, search::EvalFnc _evalFnc)
    : rootState(new BattleContext(bc)), evalFnc(std::move(_evalFnc)), randGen(bc.seed+bc.floorNum) {
}

void search::BattleScumSearcher2::search(int64_t simulations, long maxTimeMillis) {
    g_debug_scum_search = this;
    startTime = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );

    if (isTerminalState(*rootState)) {
        auto evaluation = evaluateEndState(*rootState, *rootState);
        outcomePlayerHp = rootState->player.curHp;
        bestActionSequence = {};

        root.evaluationSum = evaluation;
        root.simulationCount = 1;
    }

    for (std::int64_t simCount = 0; simCount < simulations; ++simCount) {
        if (simCount % 100 == 0) {
            std::chrono::milliseconds curTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()
            );

            // early termination if the time limit is reached
            if ((curTime - startTime).count() >= maxTimeMillis) {
                return;
            }
        }
        step();
    }
}

void search::BattleScumSearcher2::step() {
    searchStack = {&root};
    actionStack.clear();
    BattleContext curState;
    curState = *rootState;

    while (true) {
        auto &curNode = *searchStack.back();

        if (isTerminalState(curState)) {
            updateFromPlayout(searchStack, actionStack, curState);
            return;
        }

        const bool isLeaf = curNode.edges.empty();
        if (isLeaf) {

            ++simulationIdx;
            enumerateActionsForNode(curNode, curState, false);
            const auto selectIdx = selectFirstActionForLeafNode(curNode);
            auto &edgeTaken = curNode.edges[selectIdx];

//            edgeTaken.action.printDesc(std::cout, curState) << std::endl;
            edgeTaken.action.execute(curState);

            actionStack.push_back(edgeTaken.action);
            searchStack.push_back(&edgeTaken.node);

            playoutRandom(curState, actionStack);
            updateFromPlayout(searchStack, actionStack, curState);
            return;

        } else {
            const auto selectIdx = selectBestEdgeToSearch(curNode);
            auto &edgeTaken = curNode.edges[selectIdx];

//            edgeTaken.action.printDesc(std::cout, curState) << std::endl;
            edgeTaken.action.execute(curState);

            actionStack.push_back(edgeTaken.action);
            searchStack.push_back(&edgeTaken.node);
        }
    }
}

void search::BattleScumSearcher2::updateFromPlayout(const std::vector<Node *> &stack, const std::vector<Action> &actionStack, const BattleContext &endState) {
    const auto evaluation = evaluateEndState(*rootState, endState);

    if (evaluation > bestActionValue) {
        bestActionSequence = actionStack;
        bestActionValue = evaluation;
        outcomePlayerHp = endState.player.curHp;
    }

    if (evaluation < minActionValue) {
        minActionValue = evaluation;
    }

    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        auto &node = *(*it);
        ++node.simulationCount;
        node.evaluationSum += evaluation;
    }
}

bool search::BattleScumSearcher2::isTerminalState(const BattleContext &bc) const { // maybe can optimize by making this evaluate directly if score cannot possibly be higher than best
    return bc.outcome != Outcome::UNDECIDED;
}

double search::BattleScumSearcher2::evaluateEdge(const search::BattleScumSearcher2::Node &parent, int edgeIdx) {

    const auto &edge = parent.edges[edgeIdx];

    // unexplored nodes must be assigned a sufficiently large value
    // that they are explored at a priority over any other node
    if (edge.node.simulationCount == 0) {
        return unexploredNodeValueParameter;
    }

    double qualityValue = edge.node.evaluationSum / (edge.node.simulationCount+1);


    double explorationValue = explorationParameter *
            std::sqrt(std::log(parent.simulationCount+1) / (edge.node.simulationCount+1));

    return qualityValue + explorationValue;
}

int search::BattleScumSearcher2::selectBestEdgeToSearch(const search::BattleScumSearcher2::Node &cur) {
    if (cur.edges.size() == 1) {
        return 0;
    }

    auto bestEdge = 0;
    auto bestEdgeValue = evaluateEdge(cur, bestEdge);

    for (int i = 1; i < cur.edges.size(); ++i) {
        const auto value = evaluateEdge(cur, i);
        if (value > bestEdgeValue) {
            bestEdge = i;
            bestEdgeValue = value;
        }
    }
    return bestEdge;
}

int search::BattleScumSearcher2::selectFirstActionForLeafNode(const search::BattleScumSearcher2::Node &leafNode) {
    auto dist = std::uniform_int_distribution<int>(0, static_cast<int>(leafNode.edges.size())-1);
    return dist(randGen);
}

void search::BattleScumSearcher2::playoutRandom(BattleContext &state, std::vector<Action> &actionStack) {
    Node tempNode; // temp
    while (!isTerminalState(state)) {
        ++simulationIdx;

        // attempt fast selection of the random action
        Action action;
        switch (state.inputState) {
            case InputState::PLAYER_NORMAL:
                if (state.isCardPlayAllowed() && state.cards.cardsInHand > 0) {
                    auto dist = std::uniform_int_distribution<int>(0, static_cast<int>(state.cards.cardsInHand)-1);
                    const int cardIdx = dist(randGen);
                    const auto &c = state.cards.hand[cardIdx];
                    if (c.canUseOnAnyTarget(state)) {
                        if (c.requiresTarget() && state.monsters.monsterCount > 0) {
                            auto dist2 = std::uniform_int_distribution<int>(0, static_cast<int>(state.monsters.monsterCount)-1);
                            const int monsterIdx = dist(randGen);
                            const auto &m = state.monsters.arr[monsterIdx];
                            if (m.isTargetable()) {
                                action = Action(ActionType::CARD, cardIdx, monsterIdx);
                            }
                        } else {
                            action = Action(ActionType::CARD, cardIdx);
                        }
                    }
                }
                break;
                // skip potions because I don't want to write the code for it (this is technically expert knowledge suggesting potions shouldn't be used during rollouts - whatever)
                // skip end turn because heuristically it's a terrible choice (this is expert knowledge being added to the search)
            case InputState::CARD_SELECT:
                break;
                // skip card select because that code is complicated and it'd be hard to do correctly
                // in a fast manner - so we just let the normal selection do it
        };

        // if fast selection was successful use it
        if (action.isValidAction(state)) {
            actionStack.push_back(action);
            action.execute(state);

            tempNode.edges.clear();
        // otherwise compute all actions and pick one
        } else {
            enumerateActionsForNode(tempNode, state, true);
            if (tempNode.edges.empty()) {
                std::cerr << state.seed << " " << simulationIdx << std::endl;
                std::cerr << state.monsters.arr[0].getName() << " " << state.floorNum << " " << monsterEncouterNames[static_cast<int>(state.encounter)] << std::endl;
                assert(false);
            }

            auto dist = std::uniform_int_distribution<int>(0, static_cast<int>(tempNode.edges.size())-1);
            const int selectedIdx = dist(randGen);

            const auto action = tempNode.edges[selectedIdx].action;
    //        action.printDesc(std::cout, state) << std::endl;
            actionStack.push_back(action);
            action.execute(state);

            tempNode.edges.clear();
        }
    }
}

void search::BattleScumSearcher2::enumerateActionsForNode(search::BattleScumSearcher2::Node &node,
                                                               const BattleContext &bc, const bool forRandom) {
    switch (bc.inputState) {
        case InputState::PLAYER_NORMAL:
            enumerateCardActions(node, bc);
            enumeratePotionActions(node, bc);

            // skip end turn for random rollouts because it's such a terrible choice that it
            // makes it incredibly hard for the tree search to find good choices
            // this does end up incorporating a bit of expert knowledge but expert knowledge
            // is typically considered reasonable to add to monte carlo tree search
            if (!forRandom || node.edges.size() == 0) {
                node.edges.push_back({Action(ActionType::END_TURN)});
            }
            break;

        case InputState::CARD_SELECT:
            enumerateCardSelectActions(node, bc);
            break;

        default:
#ifdef sts_asserts
            std::cerr << "enumerateActionsForNode: invalid input state: " << static_cast<int>(bc.inputState) << std::endl;
            assert(false);
#endif
            break;
    }

#ifdef sts_print_debug
    std::cout << "{ (" << node.edges.size() << ") ";
    for (int i = 0; i < node.edges.size(); ++i) {
        node.edges[i].action.printDesc(std::cout, bc) << ", ";
    }
    std::cout << " }" << std::endl;
#endif
}

void search::BattleScumSearcher2::enumerateCardActions(search::BattleScumSearcher2::Node &node,
                                                            const BattleContext &bc) {
    if (!bc.isCardPlayAllowed()) {
        return;
    }

    fixed_list<std::pair<int,int>, 10> playableHandIdxs;
    for (int handIdx = 0; handIdx < bc.cards.cardsInHand; ++handIdx) {
        const auto &c = bc.cards.hand[handIdx];
        if (!c.canUseOnAnyTarget(bc)) {
            continue;
        }

        bool isUniqueAction = true;

        // this doesn't make any sense. duplicate cards can appear in more locations than just next to each other
        // this only handles like literally just dual wield on the turn it is played
        if (handIdx > 0) {
            const auto &lastCard = bc.cards.hand[handIdx-1];

            bool isEqualToLastCard = c.id == lastCard.id &&
                    c.getUpgradeCount() == lastCard.getUpgradeCount() &&
                    // both should be less than deck size c.uniqueId < bc.cards.deck
                    c.costForTurn == lastCard.costForTurn &&
                    c.cost == lastCard.cost &&
                    c.freeToPlayOnce == lastCard.freeToPlayOnce &&
                    c.specialData == lastCard.specialData;

            if (isEqualToLastCard) {
                isUniqueAction = false;
            }
        }

        if (isUniqueAction) {
            // this is being called a *lot* maybe swap it for a lookup table instead of a switch statement
            playableHandIdxs.push_back( {handIdx, search::Expert::getPlayOrdering(c.getId())} );
        }
    }

    std::sort(playableHandIdxs.begin(), playableHandIdxs.end(), [](auto a, auto b) { return a.second < b.second; });

    for (auto pair : playableHandIdxs) {
        const auto handIdx = pair.first;
        const auto &c = bc.cards.hand[handIdx];

        if (c.requiresTarget()) {
            for (int tIdx = bc.monsters.monsterCount-1; tIdx >= 0; --tIdx) {
                if (!bc.monsters.arr[tIdx].isTargetable()) {
                    continue;
                }
                node.edges.push_back({Action(ActionType::CARD, handIdx, tIdx)});
            }
        } else {
            node.edges.push_back({Action(ActionType::CARD, handIdx)});
        }
    }

}

void search::BattleScumSearcher2::enumeratePotionActions(search::BattleScumSearcher2::Node &node,
                                                              const BattleContext &bc) {

    const auto hasValidTarget = bc.monsters.getTargetableCount() > 0;

    int foundPotions = 0;
    for (int pIdx = 0; pIdx < bc.potionCapacity; ++pIdx) {

        const auto p = bc.potions[pIdx];
        if (p == Potion::EMPTY_POTION_SLOT) {
            continue;
        }
        ++foundPotions;

        // fairy potions cannot be used directly
        // TODO: smoke bombs are also not implemented lol
        if (p == Potion::FAIRY_POTION || p == Potion::SMOKE_BOMB) {
            continue;
        }

        // if the potion requires a valid target and there are none, it cannot be used
        if (potionRequiresTarget(p) && !hasValidTarget) {
            continue;
        }

        // otherwise enumerate all valid ways to use the potion
        if (!potionRequiresTarget(p)) {
            // non-targeted potions have one use action
            node.edges.push_back({Action(ActionType::POTION, pIdx)});
        } else {
            // targeted potions have one use action per valid monster target
            for (int tIdx = 0; tIdx < bc.monsters.monsterCount; ++tIdx) {
                if (bc.monsters.arr[tIdx].isTargetable()) {
                    node.edges.push_back({Action(ActionType::POTION, pIdx, tIdx)});
                }
            }
        }
    }
}

template <typename ForwardIt>
void setupCardOptionsHelper(search::BattleScumSearcher2::Node &node, const ForwardIt begin, const ForwardIt end, const std::function<bool(const CardInstance &)> &p= nullptr) {
    for (int i = 0; begin+i != end; ++i) {
        const auto &c = begin[i];
        if (!p || (p(c))) {
            node.edges.push_back(
                    {search::Action(search::ActionType::SINGLE_CARD_SELECT, i)}
                );
        }
    }
}

void search::BattleScumSearcher2::enumerateCardSelectActions(search::BattleScumSearcher2::Node &node,
                                                                  const BattleContext &bc) {

    switch (bc.cardSelectInfo.cardSelectTask) {
        case CardSelectTask::ARMAMENTS:
            setupCardOptionsHelper( node, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand,
                                    [] (const CardInstance &c) { return c.canUpgrade(); });
            break;

        case CardSelectTask::CODEX:
            for (int i = 0; i < 4; ++i) { // i -> 3 action means skip
                node.edges.push_back({Action(search::ActionType::SINGLE_CARD_SELECT, i)});
            }
            break;

        case CardSelectTask::DISCOVERY:
            for (int i = 0; i < 3; ++i) {
                node.edges.push_back({Action(search::ActionType::SINGLE_CARD_SELECT, i)});
            }
            break;

        case CardSelectTask::DUAL_WIELD:
            setupCardOptionsHelper( node, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand,
                                    [] (const CardInstance &c) {
                                        return c.getType() == CardType::POWER || c.getType() == CardType::ATTACK;
                                    });
            break;

        case CardSelectTask::EXHUME:
            setupCardOptionsHelper(node, bc.cards.exhaustPile.begin(), bc.cards.exhaustPile.end(),
                                   [](const auto &c) { return c.getId() != CardId::EXHUME; });
            break;

        case CardSelectTask::EXHAUST_ONE:
            setupCardOptionsHelper(node, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand);
            break;

        case CardSelectTask::FORETHOUGHT:
        case CardSelectTask::WARCRY:
            setupCardOptionsHelper(node, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand);
            break;

        case CardSelectTask::HEADBUTT:
        case CardSelectTask::LIQUID_MEMORIES_POTION:
            setupCardOptionsHelper(node, bc.cards.discardPile.begin(), bc.cards.discardPile.end());
            break;

        case CardSelectTask::SECRET_TECHNIQUE:
            setupCardOptionsHelper(node, bc.cards.drawPile.begin(), bc.cards.drawPile.end(),
                                    [] (const CardInstance &c) {
                                        return c.getType() == CardType::SKILL;
                                    });
            break;

        case CardSelectTask::SECRET_WEAPON:
            setupCardOptionsHelper(node, bc.cards.drawPile.begin(), bc.cards.drawPile.end(),
                                    [] (const CardInstance &c) {
                                        return c.getType() == CardType::ATTACK;
                                    });
            break;

        case CardSelectTask::EXHAUST_MANY:
        case CardSelectTask::GAMBLE:
            // just dont deal with this right now
            node.edges.push_back({search::Action(search::ActionType::MULTI_CARD_SELECT, 0)});
            break;

        default:
#ifdef sts_asserts
            assert(false);
#endif
            break;
    }
}

double getMonsterHpScale(const Monster &m) {
    // the HP amounts of splitting enemies need to be inflated in order to ensure the evaluation function
    // correctly recognizes that splitting the enemies constitutes progress towards a successful resolution
    // to the fight

    // this is necessary because otherwise the evaluation function will believe that a slime boss at 80/140 HP is a better
    // state than two large slimes both at 60/60 HP since that is 120HP total vs 80HP so the hitpoints of the bigger splitting
    // enemy need to be valued at twice the value of the hitpoints of the enemy they split into - the factor of 2 representing
    // the fact that it splits into two smaller enemies with the same HP amount as the large enemy had 
    switch (m.id) {
        case MonsterId::SLIME_BOSS:
            return 4.0;
        case MonsterId::ACID_SLIME_L:
        case MonsterId::SPIKE_SLIME_L:
            return 2.0;
        default:
            return 1.0;
    }
}

double getNonMinionMonsterCurHpTotal(const BattleContext &bc) {
    int curHpTotal = 0;

    for (int i = 0; i < bc.monsters.monsterCount; ++i) {
        const auto &m = bc.monsters.arr[i];
        if (!m.hasStatus<MS::MINION>() && m.id != sts::MonsterId::INVALID) {
            curHpTotal += m.curHp * getMonsterHpScale(m);
            if (m.id == sts::MonsterId::AWAKENED_ONE && !m.miscInfo) { // is awakened one stage 1 // todo change to status
                curHpTotal += m.maxHp * getMonsterHpScale(m);
            }
        }
    }

    return curHpTotal;
}

double getNonMinionMonsterMaxHpTotal(const BattleContext &bc) {
    int maxHpTotal = 0;

    for (int i = 0; i < bc.monsters.monsterCount; ++i) {
        const auto &m = bc.monsters.arr[i];
        if (!m.hasStatus<MS::MINION>() && m.id != sts::MonsterId::INVALID) {
            maxHpTotal += m.maxHp * getMonsterHpScale(m);
            if (m.id == sts::MonsterId::AWAKENED_ONE) {
                maxHpTotal += m.maxHp * getMonsterHpScale(m);
            }
        }
    }

    return maxHpTotal;
}

double search::BattleScumSearcher2::evaluateEndState(const BattleContext &rootBc, const BattleContext &bc) {
    // gives end state values normalized to the range (-1.0, 1.0)
    if (bc.outcome == Outcome::PLAYER_VICTORY) {
        // produces winning scores in the range (0.0, 1.0)
        return bc.player.curHp / 100.0f; 
    } else {
        double curHpTotal = getNonMinionMonsterCurHpTotal(bc);
        double maxHpTotal = getNonMinionMonsterMaxHpTotal(rootBc);
        double hpRatio = 0.0;
        if (maxHpTotal != 0.0) {
            hpRatio = curHpTotal / maxHpTotal;
        }
        // produces losing scores in the range (-1.0, 0.0)
        return -hpRatio;
    }
}

struct LayerStruct {
    const search::BattleScumSearcher2::Node *node;
    BattleContext *bc;
    int edgeIdx;
};

typedef std::pair<search::BattleScumSearcher2::Edge, std::unique_ptr<const BattleContext>> EdgeInfo;

std::vector<EdgeInfo> getEdgesForLayer(const search::BattleScumSearcher2 &s, int layerNum) {
    if (layerNum <= 0) {
        return {};
    }

    std::vector<EdgeInfo> layerEdges;

    std::vector<LayerStruct> curStack { {&s.root, new BattleContext(*s.rootState), 0} };

    while (!curStack.empty()) {
        if (curStack.size() == layerNum) {
            for (const auto &edge : curStack.back().node->edges) {
                layerEdges.emplace_back(edge, new BattleContext(*curStack.back().bc));
            }
        }

       // curStack size less than layerNum
       const bool visitedAll = curStack.back().edgeIdx >= curStack.back().node->edges.size();
       if (visitedAll || curStack.size() == layerNum) {
           delete curStack.back().bc;
           curStack.pop_back();
           continue;
       }

        // visit next edge
        auto &nextIdx = curStack.back().edgeIdx;
        const auto action = curStack.back().node->edges[nextIdx].action;

        BattleContext bc(*curStack.back().bc);
        action.execute(bc);

        curStack.push_back( {&curStack.back().node->edges[nextIdx++].node, new BattleContext(bc), 0} );
    }

    return layerEdges;
}

void search::BattleScumSearcher2::printSearchTree(std::ostream &os, int levels) {
    std::vector<std::vector<EdgeInfo>> layerEdges;
    for (int depth = 1; depth <= levels; ++depth) {
        layerEdges.push_back(getEdgesForLayer(*this, depth));
    }

//    auto maxIt = std::max(layerEdges.begin(), layerEdges.end(), [](auto a, auto b) { return a->size() < b->size(); });
//    if (maxIt == layerEdges.end()) {
//        return;
//    }
//    // maxIt points to something
//    const auto maxSize = maxIt->size();
//    constexpr auto edgeWidth = 30;

    for (int depth = 0; depth < levels; ++depth) {
        for (const auto &x : layerEdges[depth]) {
            os << "(" << x.first.node.simulationCount << ")";
            x.first.action.printDesc(os, *x.second) << "\t";
        }
        std::cout << '\n';
    }

}

void search::BattleScumSearcher2::printSearchStack(std::ostream &os, bool skipLast) {
    for (int i = 0; i < actionStack.size(); ++i) {
        const auto &a = actionStack[i];
        os << std::hex << a.bits << '\n';
    }

    os.flush();

//    BattleContext curBc = *rootState;
//    os << "explorationParameter: " << explorationParameter << '\n';
//    os << "bestActionValue: " << bestActionValue << '\n';
//    os << "minActionValue: " << minActionValue << '\n';
//    os << "outcomePlayerHp: " << outcomePlayerHp << '\n';
//    os << "root node:\n";
//    os << curBc << "\n";
//
//    for (int i = 0; i < actionStack.size(); ++i) {
//        if (i < searchStack.size()) {
//            const auto &n = searchStack[i];
//            os << i << " nodeSearched: " << n->simulationCount << " { ";
//            for (const auto &edge : n->edges) {
//                os << "(" << edge.node.simulationCount << ")";
//                edge.action.printDesc(os, curBc) << " ";
//            }
//            os << "}\n";
//        }
//
//        const auto &a = actionStack[i];
//        os << i << " actionTaken: ";
//        a.printDesc(os, curBc) << '\n';
//
//        if (skipLast && (i + 1 >= actionStack.size())) {
//            break;
//        }
//
//        a.execute(curBc);
//        os << curBc << '\n';
//    }
//
//    os.flush();
}
