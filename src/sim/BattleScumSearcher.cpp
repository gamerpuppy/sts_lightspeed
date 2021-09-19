//
// Created by gamerpuppy on 9/6/2021.
//


#include "sim/BattleScumSearcher.h"
#include "sim/search/BattleScumSearcher2.h"

#include "game/Game.h"
#include "game/GameContext.h"

using namespace sts;
//
//SearchInfo::SearchInfo(const BattleContext &bc) : bc(bc) {
//    if (bc.outcome != Outcome::UNDECIDED) {
//        stateSize = 0;
//    } else {
//        stateSize = handler.setupState(bc);
//    }
//}

void BattleScumSearcher::search(const BattleContext &bc) {
    randEngine.seed(bc.seed+bc.turn+bc.ascension+bc.loopCount+bc.player.curHp);

    bestInfos.clear();
    std::vector<SearchInfo> searchStack;
    searchStack.resize(maxMoveDepth);

    assert(!searchStack.empty());

    searchStack[0].bc = bc;
    if (searchStack[0].bc.outcome == Outcome::UNDECIDED) {
        searchStack[0].stateSize = searchStack[0].handler.setupState(searchStack[0].bc);
    } else {
        searchStack[0].stateSize = 0;
    }

    int curDepth = 0;
    while (true) {
        if (curDepth < 0) {
            return;
        }

        assert(curDepth < maxMoveDepth && curDepth >= 0);
        auto &cur = searchStack[curDepth];

        const auto optionsSize = cur.stateSize < stateSizeSampleThreshold ? cur.stateSize : stateSampleCount;

        if (cur.optionIdx + 1 < optionsSize && curDepth+1 < maxMoveDepth) {
            ++cur.optionIdx;

        } else {
            // handle leaf node
            if (cur.bc.outcome == Outcome::UNDECIDED && cur.bc.inputState == InputState::PLAYER_NORMAL && cur.stateSize > 0) {
                cur.handler.chooseOption(cur.bc, 0); // thingy that ends turn, improves results a bit.
            }


            const bool nodeValid = cur.bc.outcome != Outcome::UNDECIDED || cur.bc.turn >= bc.turn+minTurnLookahead;
            if (nodeValid || bestInfos.empty() || bestInfos.back().bc.turn <= cur.bc.turn) {
                cur.value = evaluateState(cur.bc);
                ++nodesEvaluated;

                // no more actions for this state.
                if (bestInfos.empty() || cur.value > bestInfos.back().value) {

                    bestInfos.clear();
                    for (int i = 0; i <= curDepth; ++i) {
                        assert(searchStack.size() > i);
                        bestInfos.push_back(searchStack.at(i));
                    }
                }
            }

            --curDepth;
            continue;
        }

        auto &next = searchStack[curDepth+1];
        next = cur;


        if (cur.stateSize < stateSizeSampleThreshold) {
            next.handler.chooseOption(next.bc, next.optionIdx);
        } else {
            // todo maybe break up the space into parts and sample from those for a better distribution
            std::uniform_int_distribution<int> distr(0,cur.stateSize-1);
            const auto sampledChoice = distr(randEngine);
            next.handler.chooseOption(next.bc, sampledChoice);
        }

        next.optionIdx = -1;
        next.stateSize = 0;
        if (next.bc.outcome == Outcome::UNDECIDED) {
            next.stateSize = next.handler.setupState(next.bc);
        }

        ++curDepth;
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

    double normalizedCurHp = (double)bc.player.curHp/bc.player.maxHp;
    double weightedHp = 0.5 * normalizedCurHp + 0.5;
    double hpScore = weightedHp * weightedHp;

    double score = (double)(hpScore * 500)
            + (-combinedHp * 2)
            + (-bc.monsters.monstersAlive * 5);

    return {score, bc.player.curHp, combinedHp};
}

BattleScumSearcher::BattleScumSearcher(int maxMoveDepth, int minTurnLookahead) : maxMoveDepth(maxMoveDepth),
                                                                                 minTurnLookahead(minTurnLookahead) {}

ScumSearcherAgent::ScumSearcherAgent(const std::default_random_engine &_rng, int _searchDepth)
    : rng(_rng), searchDepth(_searchDepth) {}

void ScumSearcherAgent::playout(GameContext &gc) {
    RandomStateHandler handler;
    BattleContext bc;
    const auto seedStr = std::string(SeedHelper::getString(gc.seed));

    while (gc.outcome == GameOutcome::UNDECIDED) {
        if (gc.screenState == ScreenState::BATTLE) {
            bc = BattleContext();
            bc.init(gc);

            playoutBattle(bc);
            bc.exitBattle(gc);

        } else {
            pickGoodEventOutcome(gc);
        }
    }

    if (print) {
        if (gc.outcome == GameOutcome::PLAYER_LOSS) {
            std::cout << "random playoutBattle finished with a loss" << std::endl;
        } else {
            std::cout << "random playoutBattle finished with a victory! hp: " << gc.curHp << std::endl;
        }
    }
}

void ScumSearcherAgent::playoutBattle(BattleContext &bc) {

    while (bc.outcome == Outcome::UNDECIDED) {
        ++choiceCount;

        search::BattleScumSearcher2 searcher(bc);
        searcher.search(searchDepth);

        sts::search::Action bestAction;
        if (searcher.bestActionSequence.empty()) {
            std::int64_t maxSimulations = -1;
            for (const auto &edge : searcher.root.edges) {
                if (edge.node.simulationCount > maxSimulations) {
                    maxSimulations = edge.node.simulationCount;
                    bestAction = edge.action;
                }
            }

            if (print) {
                std::cout << choiceCount << " ";
                bestAction.printDesc(std::cout, bc) << " ";
                if (choiceCount == 151) {
                    std::cout << bestAction.getSourceIdx() << " " << bestAction.getTargetIdx()
                    << " " << static_cast<int>(bc.inputState)
                    << " " << static_cast<int>(bc.cardSelectInfo.cardSelectTask) << std::endl;
                }
                std::cout
                          << " turn: " << bc.turn
                          << " energy: " << bc.player.energy
                          << " cardsPlayedThisTurn: " << bc.player.cardsPlayedThisTurn
                          << " state: " << (bc.inputState == InputState::PLAYER_NORMAL ? "normal" : " probably card select")
                          << std::endl;

                std::cout << bc << std::endl;
            }

            bestAction.execute(bc);

        } else {
//            searcher.bestActionSequence.front().execute(bc);
//
            for (auto x : searcher.bestActionSequence) {
                x.execute(bc);
            }
        }



//        bestAction.execute(bc);
//        nodesEvaluated += searchDepth;
//        bestAction.handler.chooseOption(bc, bestAction.optionIdx);
    }

}

void ScumSearcherAgent::chooseRandom(GameContext &gc) {
    ++choiceCount;
    RandomStateHandler handler;
    handler.setupState(gc);
    const auto stateSize = handler.getStateSize(gc);
    std::uniform_int_distribution<int> distr(0, stateSize - 1);
    const int randomChoice = distr(rng);
    handler.chooseOption(gc, randomChoice);
}

void ScumSearcherAgent::pickGoodEventOutcome(GameContext &gc) {
    switch (gc.screenState) {
        case ScreenState::EVENT_SCREEN:
            switch (gc.curEvent) {

                case Event::NEOW:
//                    gc.chooseEventOption(3);
                    if (gc.info.neowRewards[1].d == Neow::Drawback::CURSE || gc.info.neowRewards[2].d == Neow::Drawback::CURSE) {
                        gc.chooseEventOption(0);
                    } else {
                        chooseRandom(gc);
                    }
                    break;

                case Event::NOTE_FOR_YOURSELF:
                case Event::FOUNTAIN_OF_CLEANSING:
                case Event::OMINOUS_FORGE:
                case Event::BIG_FISH:
                    gc.chooseEventOption(0);
                    break;

                case Event::WE_MEET_AGAIN: {
                    if (gc.gold >= 50) {
                        gc.chooseEventOption(1);
                    } else if (gc.potionCount > 0) {
                        gc.chooseEventOption(0);
                    } else {
                        chooseRandom(gc);
                    }
                    break;
                }

                case Event::GOLDEN_IDOL: {
                    if (gc.hasRelic(RelicId::GOLDEN_IDOL)) {
                        gc.chooseEventOption(2);
                    } else {
                        gc.chooseEventOption(1);
                    }
                    break;
                }

//                case Event::MUSHROOMS:
//                case Event::SHINING_LIGHT:
//                    gc.chooseEventOption(0);
//                    break;
//                case Event::DEAD_ADVENTURER:
//                case Event::LIARS_GAME:
//                    gc.chooseEventOption(1);
//                    break;



                default:
                    chooseRandom(gc);
                    break;
            }
            break;

        case ScreenState::REWARDS: {
            auto &r = gc.info.rewardsContainer;
            if (r.goldRewardCount > 0) {
                gc.gainGold(r.gold[0]);
                r.removeGoldReward(0);

            } else if (r.relicCount > 0) {
                const bool newBlueKeyValue = r.sapphireKey && (0  < r.relicCount - 1);
                gc.obtainRelic(r.relics[0]);
                r.removeRelicReward(0);
                r.sapphireKey = newBlueKeyValue;

            } else if (r.cardRewardCount > 0) {
                if (r.cardRewards[0][0].getRarity() == CardRarity::RARE) {
                    gc.deck.obtain(gc, r.cardRewards[0][0]);
                    gc.info.rewardsContainer.removeCardReward(0);
                } else {
                    chooseRandom(gc);
                }
            } else {
                chooseRandom(gc);
            }
            break;
        }

        case ScreenState::TREASURE_ROOM: {
            gc.chooseTreasureRoomOption(!gc.relics.has(RelicId::CURSED_KEY));
            break;
        }

        case ScreenState::BATTLE:
        case ScreenState::INVALID:
            assert(false);
            break;

        case ScreenState::SHOP_ROOM: {
            bool purchased = false;
            for (int i = 0; i < 3; ++i) {
                if (gc.info.shop.potionPrice(i) != -1 &&  gc.gold >= gc.info.shop.potionPrice(i)) {
                    gc.info.shop.buyPotion(gc, i);
                    purchased = true;
                    break;
                }
            }
            if (!purchased) {
                chooseRandom(gc);
            }
            break;
        }


        case ScreenState::BOSS_RELIC_REWARDS:
        case ScreenState::CARD_SELECT:
        case ScreenState::MAP_SCREEN:
        case ScreenState::REST_ROOM:
        default:
            chooseRandom(gc);
            break;
    }

}


namespace sts {

    std::ostream &operator<<(std::ostream &os, const SearchInfo &s) {
        os << "bestOption:" << s.optionIdx <<  " " << s.value;
        return os;
    }

    bool StateValue::operator<(const StateValue &rhs) const {
        return score < rhs.score;
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

    std::ostream &operator<<(std::ostream &os, const StateValue &value) {
        os << "score: " << value.score
            << " playerHp: " << value.playerHp
            << " totalEnemyHp: " << value.combinedHp;
        return os;
    }
}


