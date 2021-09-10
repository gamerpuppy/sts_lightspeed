//
// Created by gamerpuppy on 9/6/2021.
//


#include "sim/BattleScumSearcher.h"

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

void BattleScumSearcher::search(const BattleContext &bc, const int depth) {
    bestInfos.clear();
    std::vector<SearchInfo> searchStack;
    searchStack.resize(depth);

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

        assert(curDepth < depth && curDepth >= 0);
        auto &cur = searchStack[curDepth];


        if (cur.optionIdx + 1 < cur.stateSize && curDepth+1 < depth) {
            ++cur.optionIdx;

        } else {
            cur.value = evaluateState(cur.bc);

            // no more actions for this state.
            if (bestInfos.empty() || cur.value > bestInfos.back().value) {

                bestInfos.clear();
                for (int i = 0; i <= curDepth; ++i) {
                    assert(searchStack.size() > i);
                    bestInfos.push_back(searchStack.at(i));
                }
            }

            --curDepth;
            continue;
        }


        auto &next = searchStack[curDepth+1];
        next = cur;
        next.handler.chooseOption(next.bc, next.optionIdx);


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

//    if (bc.monsters.arr[0].id == MonsterId::GREMLIN_NOB) {
//        return {(double)(-combinedHp) + (bc.player.curHp)};
//    }

    double normalizedCurHp = (double)bc.player.curHp/bc.player.maxHp;
    double weightedHp = 0.5 * normalizedCurHp + 0.5;
    double hpScore = weightedHp * weightedHp;

    double score = (double)(hpScore * 500)
            + (-combinedHp * 2)
            + (-bc.monsters.monstersAlive * 5);

    return {score};
}


void ScumSearcherAgent::playout(GameContext &gc) {
    RandomStateHandler handler;
    BattleContext bc;
    const auto seedStr = std::string(SeedHelper::getString(gc.seed));

    while (gc.outcome == GameOutcome::UNDECIDED && gc.act == 1) {
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
    BattleScumSearcher searcher;

    while (bc.outcome == Outcome::UNDECIDED) {
        ++choiceCount;

        searcher.search(bc, searchDepth);

        const auto &bestInfo = searcher.bestInfos.at(0);

//        if (print) {
//            std::cout << choiceCount
//                      << " choice: " << bestInfo.optionIdx
//                      << " turn: " << bc.turn
//                      << " energy: " << bc.player.energy
//                      << " cardsPlayedThisTurn: " << bc.player.cardsPlayedThisTurn
//                      << " state: " << (bc.inputState == InputState::PLAYER_NORMAL ? "normal" : " probably card select")
//                      << std::endl;
//
//            std::cout << bc << '\n';
//        }


        bestInfo.handler.chooseOption(bc, bestInfo.optionIdx);

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
                    if (gc.info.neowRewards[1].d == Neow::Drawback::CURSE || gc.info.neowRewards[2].d == Neow::Drawback::CURSE) {
                        gc.chooseEventOption(0);
                    } else {
                        chooseRandom(gc);
                    }
                    break;

                case Event::NOTE_FOR_YOURSELF:
                case Event::FOUNTAIN_OF_CLEANSING:
                case Event::ACCURSED_BLACKSMITH:
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
        return os;
    }
}


