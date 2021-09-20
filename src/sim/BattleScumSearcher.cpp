//
// Created by gamerpuppy on 9/6/2021.
//


#include "sim/BattleScumSearcher.h"
#include "sim/search/BattleScumSearcher2.h"
#include "sim/PrintHelpers.h"
#include "sim/search/ExpertKnowledge.h"

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

            playoutBattle(bc, printLogs);
            bc.exitBattle(gc);

        } else {
            if (printLogs) {
                std::cout << gc << std::endl;
            }
            pickGoodEventOutcome(gc);
        }
    }

    if (printLogs) {
        if (gc.outcome == GameOutcome::PLAYER_LOSS) {
            std::cout << "random playoutBattle finished with a loss" << std::endl;
        } else {
            std::cout << "random playoutBattle finished with a victory! hp: " << gc.curHp << std::endl;
        }
    }
}

void ScumSearcherAgent::playoutBattle(BattleContext &bc, bool print) {

    while (bc.outcome == Outcome::UNDECIDED) {
        ++choiceCount;

        search::BattleScumSearcher2 searcher(bc);

        const auto simulationCount = (isBossEncounter(bc.encounter) ? bossSearchMultiplier : 1)  * searchDepth;
        searcher.search(simulationCount);

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

void pushBackRandomEnemies(fixed_list<MonsterEncounter, 10> &encounters, std::default_random_engine &rng, int act, const int type, int count) {
    if (act < 1 || act > 3) {
        return;
    }

    --act;

    for (int i = 0 ; i < count; ++i) {
        MonsterEncounter randomEncounter = sts::MonsterEncounter::INVALID;
        switch (type) {
            case 0: {
                std::uniform_int_distribution<int> distr(0, MonsterEncounterPool::weakCount[act]-1);
                randomEncounter = MonsterEncounterPool::weakEnemies[act][distr(rng)];
                break;
            }

            case 1: {
                std::uniform_int_distribution<int> distr(0, MonsterEncounterPool::strongCount[act]-1);
                randomEncounter = MonsterEncounterPool::strongEnemies[act][distr(rng)];
                break;
            }

            case 2: {
                if (count == 3) {
                    randomEncounter = MonsterEncounterPool::elites[act][i];
                } else {
                    std::uniform_int_distribution<int> distr(0, 2);
                    randomEncounter = MonsterEncounterPool::elites[act][distr(rng)];
                }
                break;
            }
            default:
#ifdef sts_asserts
                assert(false);
#endif
                break;
        }

        if (std::find(encounters.begin(), encounters.end(), randomEncounter) == encounters.end()) {
            encounters.push_back(randomEncounter);

        } else {
            --i;

        }
    }
}

fixed_list<MonsterEncounter, 10> ScumSearcherAgent::getEncountersToTest(const GameContext &gc) {
    fixed_list<MonsterEncounter, 10> encounters;

    if (gc.act == 4) {
        encounters.push_back(MonsterEncounter::SHIELD_AND_SPEAR);
        return encounters;
    }

    if (gc.curRoom == Room::BOSS) {
        // upcoming fights are in next act
        if (gc.act == 3) {
            encounters.push_back(MonsterEncounter::SHIELD_AND_SPEAR);
            encounters.push_back(MonsterEncounter::THE_HEART);
            return encounters;
        }
        pushBackRandomEnemies(encounters, rng, gc.act+1, 0, 1);
        pushBackRandomEnemies(encounters, rng, gc.act+1, 1, 2);
        pushBackRandomEnemies(encounters, rng, gc.act+1, 2, 2);

    } else {
        if (gc.curMapNodeY < 3) {
            pushBackRandomEnemies(encounters, rng, gc.act, 0, 1);
            pushBackRandomEnemies(encounters, rng, gc.act, 1, 2);
            pushBackRandomEnemies(encounters, rng, gc.act, 2, 3);
            encounters.push_back(gc.boss);
            encounters.push_back(gc.boss);
        } else {
            pushBackRandomEnemies(encounters, rng, gc.act, 1, 1);
            pushBackRandomEnemies(encounters, rng, gc.act, 2, 3);
            encounters.push_back(gc.boss);
            encounters.push_back(gc.boss);
        }
    }

    return encounters;
}


double ScumSearcherAgent::evaluateFights(const GameContext &gc, const fixed_list<MonsterEncounter, 10> &encounters) {


    if (encounters.empty()) {
        return 0;
    }

    GameContext gcCopy(gc);
    double cumulativeScore = 0;
    for (auto encounter : encounters) {
        for (int i = 0; i < 2; ++i) {
            gcCopy.seed = rintl(rng());

            BattleContext bc;
            bc.init(gcCopy, encounter);

            playoutBattle(bc);
            double score = search::BattleScumSearcher2::evaluateEndState(bc);
            cumulativeScore += score;
            if (printLogs) {
                std::cout << monsterEncounterStrings[static_cast<int>(encounter)] << " " << score << " ";
            }
        }
    }

    double avgScore = cumulativeScore / encounters.size();
    return avgScore;
}

void ScumSearcherAgent::chooseRewardAction(GameContext &gc) {
    auto &r = gc.info.rewardsContainer;
    if (r.goldRewardCount > 0) {
        gc.gainGold(r.gold[0]);
        r.removeGoldReward(0);
        return;

    } else if (r.relicCount > 0) {
        const bool newBlueKeyValue = r.sapphireKey && (0  < r.relicCount - 1);
        gc.obtainRelic(r.relics[0]);
        r.removeRelicReward(0);
        r.sapphireKey = newBlueKeyValue;
        return;

    } else if (r.potionCount > 0) {
        gc.obtainPotion(r.potions[0]);
        r.removePotionReward(0);
        return;
    }

    if (r.cardRewardCount == 0) {
        gc.regainControl(); // skip
        return;
    }

    chooseRandom(gc);
    return;

//    const auto testFights = getEncountersToTest(gc);
//
//    // there is at least one card reward
//    double baseEvaluation;
//    {
//        baseEvaluation = evaluateFights(gc, testFights);
//        if (printLogs) {
//            std::cout << "base eval: " << baseEvaluation << std::endl;
//        }
//    }
//
//    for (int rIdx = r.cardRewardCount-1; rIdx >= 0; --rIdx) {
//        double best = -100000;
//        int bestCardIdx = -1;
//
//        if (printLogs) {
//            std::cout << "evaluating card reward " << rIdx << std::endl;
//        }
//        for (int cIdx = 0; cIdx < r.cardRewards[rIdx].size(); ++cIdx) {
//            GameContext gcCopy(gc);
//            gcCopy.deck.obtain(gcCopy, r.cardRewards[rIdx][cIdx]);
//
//            double eval = evaluateFights(gcCopy, testFights);
//            if (printLogs) {
//                std::cout << "card:" << r.cardRewards[rIdx][cIdx] << " eval: " << eval << std::endl;
//            }
//
//            if (eval > best) {
//                best = eval;
//                bestCardIdx = cIdx;
//            }
//        }
//
//        bool betterThanSkip = best > baseEvaluation;
//        if (betterThanSkip  && bestCardIdx != -1) {
//            gc.deck.obtain(gc, r.cardRewards[rIdx][bestCardIdx]);
//        }
//        gc.info.rewardsContainer.removeCardReward(rIdx);
//    }
}

void ScumSearcherAgent::pickGoodEventOutcome(GameContext &gc) {
    switch (gc.screenState) {
        case ScreenState::EVENT_SCREEN:
            switch (gc.curEvent) {

                case Event::NEOW:
                    gc.chooseEventOption(3);
//                    if (gc.info.neowRewards[1].d == Neow::Drawback::CURSE || gc.info.neowRewards[2].d == Neow::Drawback::CURSE) {
//                        gc.chooseEventOption(0);
//                    } else {
//                        chooseRandom(gc);
//                    }
                    break;

                case Event::NOTE_FOR_YOURSELF:
                case Event::FOUNTAIN_OF_CLEANSING:
                case Event::OMINOUS_FORGE:
                    gc.chooseEventOption(0);
                    break;

                case Event::BIG_FISH:
                    gc.chooseEventOption(1);
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

                case Event::GHOSTS: {
                    gc.chooseEventOption(0);
                    break;
                }

                case Event::MASKED_BANDITS: {
                    gc.chooseEventOption(0);
                    break;
                }

                case Event::KNOWING_SKULL: {
                    gc.chooseEventOption(0);
                    if (gc.potionCount != gc.potionCapacity) {
                        gc.chooseEventOption(2);
                    }
                    gc.chooseEventOption(3);
                    break;
                }

                case Event::CURSED_TOME: {
                    gc.chooseEventOption(0);
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

            } else {
                chooseRewardAction(gc);
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



        case ScreenState::CARD_SELECT: {
            fixed_list<std::pair<int,int>, Deck::MAX_SIZE> selectOrder;

            for (int i = 0; i < gc.info.toSelectCards.size(); ++i) {
                const auto &c = gc.info.toSelectCards[i].card;

                auto playOrder = search::Expert::getPlayOrdering(c.getId());

                switch (gc.info.selectScreenType) {
                    case CardSelectScreenType::TRANSFORM:
                    case CardSelectScreenType::TRANSFORM_UPGRADE:
                        if (c.getType() == CardType::CURSE) {
                            selectOrder.push_back( {i, playOrder} );
                        } else {
                            selectOrder.push_back( {i, -playOrder} );
                        }
                        break;

                    case CardSelectScreenType::BONFIRE_SPIRITS:
                    case CardSelectScreenType::REMOVE:
                        selectOrder.push_back( {i, -playOrder} );
                        break;

                    case CardSelectScreenType::UPGRADE:
                    case CardSelectScreenType::DUPLICATE:
                    case CardSelectScreenType::OBTAIN:
                        selectOrder.push_back( {i, playOrder} );
                        break;

                    case CardSelectScreenType::BOTTLE:
                        selectOrder.push_back( {i, -playOrder} );
                        break;


                    case CardSelectScreenType::INVALID:
                    default:
                        selectOrder.push_back({i, 0});
                        break;
                }
            }
            std::sort(selectOrder.begin(), selectOrder.end(), [](auto a, auto b) { return a.second < b.second; });
            gc.chooseSelectCardScreenOption(selectOrder.front().first);
            break;
        }

        case ScreenState::BOSS_RELIC_REWARDS: {
            int best = 10000;
            int bestIdx = 0;

            for (int i = 0; i < 3; ++i) {
                int value = search::Expert::getBossRelicOrdering(gc.info.bossRelics[i]);
                if (value < best) {
                    best = value;
                    bestIdx = i;
                }
            }

            gc.chooseBossRelic(bestIdx);
            break;
        }

        case ScreenState::REST_ROOM: {
            if (gc.curHp > 50 && gc.deck.upgradeableCount > 0 && !gc.hasRelic(RelicId::FUSION_HAMMER)) {
                gc.chooseCampfireOption(1);
            } else {
                chooseRandom(gc);
            }
        }


        case ScreenState::MAP_SCREEN:
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


