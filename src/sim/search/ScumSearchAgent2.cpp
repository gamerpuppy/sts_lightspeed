//
// Created by keega on 9/19/2021.
//

#include "sim/search/ScumSearchAgent2.h"

#include <sim/search/ExpertKnowledge.h>
#include <sim/StateHandler.h>
#include <game/Game.h>
#include "sim/PrintHelpers.h"

#include "sim/search/BattleScumSearcher2.h"
#include "combat/BattleContext.h"

using namespace sts;

void search::ScumSearchAgent2::playout(GameContext &gc) {
    paused = false;
    RandomStateHandler handler;
    BattleContext bc;
    const auto seedStr = std::string(SeedHelper::getString(gc.seed));

    while (gc.outcome == GameOutcome::UNDECIDED && !paused) {
        if (gc.screenState == ScreenState::BATTLE) {
            bc = BattleContext();
            bc.init(gc);

            playoutBattle(bc);
            bc.exitBattle(gc);
            continue;
        }

        if (printLogs) {
            std::cout << gc << std::endl;
        }
        stepOutOfCombatPolicy(gc);
    }
}

void printHelper(const BattleContext &bc, const search::Action &a) {
    a.printDesc(std::cout, bc) << " ";
    std::cout
            << " turn: " << bc.turn
            << " energy: " << bc.player.energy
            << " cardsPlayedThisTurn: " << bc.player.cardsPlayedThisTurn
            << " state: " << (bc.inputState == InputState::PLAYER_NORMAL ? "normal" : " probably card select")
            << std::endl;
    std::cout << bc << std::endl;
}


void search::ScumSearchAgent2::playoutBattle(BattleContext &bc) {

    std::vector<search::Action> bestActions;
    int bestOutcomePlayerHp = -1;

    while (bc.outcome == Outcome::UNDECIDED) {
        const std::int64_t simulationCount = isBossEncounter(bc.encounter) ?
                                              (bossSimulationMultiplier * simulationCountBase) : simulationCountBase;

        search::BattleScumSearcher2 searcher(bc);
        searcher.search(simulationCount);

        if (searcher.outcomePlayerHp > bestOutcomePlayerHp)
        {
            bestActions = std::vector(
                    searcher.bestActionSequence.rbegin(),
                    searcher.bestActionSequence.rend());
            bestOutcomePlayerHp = searcher.outcomePlayerHp;
        }

        if (bestOutcomePlayerHp > 0) {
            stepThroughSolution(bc, bestActions);
        } else {
            stepThroughSearchTree(bc, searcher);
        }
    }
}

void search::ScumSearchAgent2::stepThroughSolution(BattleContext &bc, std::vector<search::Action> &actions) {
    for (int i = 0; i < stepsWithSolution; ++i) {
        if (actions.empty()) {
            break;
        }

        auto &a = actions.back();
        if (printLogs) {
            printHelper(bc, a);
        }
        a.execute(bc);

        actions.pop_back();
    }
}

void search::ScumSearchAgent2::stepThroughSearchTree(BattleContext &bc, const search::BattleScumSearcher2 &s) {
    const search::BattleScumSearcher2::Node *curNode = &s.root;
    for (int actionCount = 0; actionCount < stepsNoSolution; ++actionCount) {
        if (bc.outcome != Outcome::UNDECIDED) {
            break;
        }

        std::int64_t maxSimulations = -1;
        const sts::search::BattleScumSearcher2::Edge *maxEdge = nullptr;

        for (const auto &edge : curNode->edges) {
            if (edge.node.simulationCount > maxSimulations) {
                maxSimulations = edge.node.simulationCount;
                maxEdge = &edge;
            }
        }

        if (maxEdge == nullptr) {
            break;
        }

        if (printLogs) {
            printHelper(bc, maxEdge->action);
        }

        maxEdge->action.execute(bc);
        curNode = &maxEdge->node;
    }
}

void search::ScumSearchAgent2::stepRandom(GameContext &gc) {
    RandomStateHandler handler;
    handler.setupState(gc);
    const auto stateSize = handler.getStateSize(gc);
    std::uniform_int_distribution<int> distr(0, stateSize - 1);
    const int randomChoice = distr(rng);
    handler.chooseOption(gc, randomChoice);
}

void search::ScumSearchAgent2::stepOutOfCombatPolicy(GameContext &gc) {
    ++stepCount;
    switch (gc.screenState) {
        case ScreenState::EVENT_SCREEN:
            stepEventPolicy(gc);
            break;

        case ScreenState::REWARDS: {
            stepRewardsPolicy(gc);
            break;
        }

        case ScreenState::TREASURE_ROOM: {
            bool takeChest = true;
            if (gc.relics.has(RelicId::CURSED_KEY)) {
                takeChest = gc.info.chestSize == ChestSize::LARGE;
            }
            gc.chooseTreasureRoomOption(takeChest);
            break;
        }

        case ScreenState::BATTLE:
        case ScreenState::INVALID:
            assert(false);
            break;

        case ScreenState::SHOP_ROOM: {
            bool purchased = false;
            for (int i = 0; i < 3; ++i) {
                if (gc.info.shop.relicPrice(i) != -1 &&  gc.gold >= gc.info.shop.relicPrice(i)) {
                    gc.info.shop.buyRelic(gc, i);
                    purchased = true;
                    break;
                }
            }
            if (!purchased) {
                stepRandom(gc);
            }
            break;
        }

        case ScreenState::CARD_SELECT: {
            cardSelectPolicy(gc);
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
            } else if (gc.curHp < 10 && !gc.relics.has(RelicId::COFFEE_DRIPPER)){
                gc.chooseCampfireOption(0);
            } else {
                stepRandom(gc);
            }
            break;
        }

        case ScreenState::MAP_SCREEN:
        default:
            stepRandom(gc);
            break;
    }
}

void search::ScumSearchAgent2::cardSelectPolicy(GameContext &gc) {
    fixed_list<std::pair<int,int>, Deck::MAX_SIZE> selectOrder;

    for (int i = 0; i < gc.info.toSelectCards.size(); ++i) {
        const auto &c = gc.info.toSelectCards[i].card;

        auto playOrder = search::Expert::getPlayOrdering(c.getId());
        auto obtainWeight = search::Expert::getObtainWeight(c.getId());

        switch (gc.info.selectScreenType) {
            case CardSelectScreenType::TRANSFORM:
            case CardSelectScreenType::TRANSFORM_UPGRADE:
                if (c.getType() == CardType::CURSE) {
                    selectOrder.push_back( {i, playOrder} );
                } else {
                    selectOrder.push_back( {i, obtainWeight} );
                }
                break;

            case CardSelectScreenType::BONFIRE_SPIRITS:
            case CardSelectScreenType::REMOVE:
                selectOrder.push_back( {i, -obtainWeight} );
                break;

            case CardSelectScreenType::UPGRADE:
            case CardSelectScreenType::DUPLICATE:
            case CardSelectScreenType::OBTAIN:
            case CardSelectScreenType::BOTTLE:
                selectOrder.push_back( {i, -obtainWeight} );
                break;

            case CardSelectScreenType::INVALID:
            default:
                selectOrder.push_back({i, 0});
                break;
        }
    }
    std::sort(selectOrder.begin(), selectOrder.end(), [](auto a, auto b) { return a.second < b.second; });
    gc.chooseSelectCardScreenOption(selectOrder.front().first);
}

void search::ScumSearchAgent2::stepRewardsPolicy(GameContext &gc) {
    auto &r = gc.info.rewardsContainer;
    if (r.goldRewardCount > 0) {
        gc.obtainGold(r.gold[0]);
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

    if (pauseOnCardReward) {
        paused = true;
        return;
    }


//    stepRandom(gc);
    weightedCardRewardPolicy(gc);
}

double getAvgDeckWeight(const GameContext &gc) {
    int sum = 0;
    for (const auto &c : gc.deck.cards) {
        sum += search::Expert::getObtainWeight(c.getId(), c.isUpgraded());
    }
    return (double) sum / gc.deck.size();
}

void search::ScumSearchAgent2::weightedCardRewardPolicy(GameContext &gc) {
    auto &r = gc.info.rewardsContainer;
    for (int rIdx = r.cardRewardCount-1; rIdx >= 0; --rIdx) {

        const auto deckWeight = getAvgDeckWeight(gc);
        if (printLogs) {
            std::cout << "evaluating card reward " << rIdx << " avgDeckWeight: " << deckWeight << std::endl;
        }
        fixed_list<std::pair<int,double>,4> weights;
        double weightSum = 0;
        for (int cIdx = 0; cIdx < r.cardRewards[rIdx].size(); ++cIdx) {
            constexpr double act1AttackMultiplier = 1.4;

            const auto &c = r.cardRewards[rIdx][cIdx];
            double weight = std::pow(search::Expert::getObtainWeight(c.getId(), c.isUpgraded()), 1.2);
            if (gc.act == 1 && c.getType() == CardType::ATTACK) {
                weight *= act1AttackMultiplier;
            }

            weights.push_back({cIdx, weight});
            weightSum += weight;

            if (printLogs) {
                std::cout << "card:" << r.cardRewards[rIdx][cIdx] << " eval: " << weight << std::endl;
            }
        }

        // choose a weighted card
        int selection = 0;
        {
            std::uniform_real_distribution<double> distr(0,weightSum);
            double roll = distr(rng);
            double acc = 0;
            for (int i = 0; i < weights.size(); ++i) {
                acc += weights[i].second;
                if (roll <= acc) {
                    selection = weights[i].first;
                }
            }
        }

        bool skipCard = true;
        {
            std::uniform_real_distribution<double> distr(0,weights[selection].second + deckWeight*0.6);
            double roll = distr(rng);
            if (roll < weights[selection].second) {
                skipCard = false;
            }
        }

        if (!skipCard) {
            gc.deck.obtain(gc, r.cardRewards[rIdx][weights[selection].first]);
        }
        gc.info.rewardsContainer.removeCardReward(rIdx);
    }
}

void search::ScumSearchAgent2::stepEventPolicy(GameContext &gc) {
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
        case Event::THE_DIVINE_FOUNTAIN:
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
                stepRandom(gc);
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

        default:
            stepRandom(gc);
            break;
    }
}

