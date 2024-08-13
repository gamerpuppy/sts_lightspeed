//
// Created by keega on 9/19/2021.
//

#include "sim/search/ScumSearchAgent2.h"

#include <sim/search/ExpertKnowledge.h>
#include <game/Game.h>
#include "sim/PrintHelpers.h"
#include "sim/search/BattleScumSearcher2.h"

using namespace sts;

void search::ScumSearchAgent2::takeAction(GameContext &gc, search::GameAction a) {
    if (printActions) {
        gameActionHistory.emplace_back(a.bits);
        std::cout << std::hex << a.bits << std::endl;
    }
    a.execute(gc);
}

void search::ScumSearchAgent2::takeAction(BattleContext &bc, search::Action a) {
    if (printActions) {
        gameActionHistory.emplace_back(a.bits);
        std::cout << std::hex << a.bits << std::endl;
    }
//    a.printDesc(std::cout, bc);
    a.execute(bc);
}

void search::ScumSearchAgent2::playout(GameContext &gc) {
    paused = false;
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

static void printHelper(const BattleContext &bc, const search::Action &a) {
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
        searcher.search(simulationCount, 1000);

        if (searcher.outcomePlayerHp > bestOutcomePlayerHp)
        {
            bestActions = std::vector(
                    searcher.bestActionSequence.rbegin(),
                    searcher.bestActionSequence.rend());
            bestOutcomePlayerHp = searcher.outcomePlayerHp;
        }

        simulationCountTotal += searcher.root.simulationCount;

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

        takeAction(bc, a);
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

        takeAction(bc, maxEdge->action);
        curNode = &maxEdge->node;
    }
}

void search::ScumSearchAgent2::stepRandom(GameContext &gc) {
    std::vector<search::GameAction> possibleActions(search::GameAction::getAllActionsInState(gc));
    std::uniform_int_distribution<int> distr(0, static_cast<int>(possibleActions.size())-1);
    const int randomChoice = distr(rng);
    auto a = possibleActions[randomChoice];
    takeAction(gc, a);
}

void search::ScumSearchAgent2::stepOutOfCombatPolicy(GameContext &gc) {
    ++stepCount;

    switch (gc.screenState) {
        case ScreenState::EVENT_SCREEN:
            stepEventPolicy(gc);
            break;

        case ScreenState::REWARDS:
            stepRewardsPolicy(gc);
            break;

        case ScreenState::TREASURE_ROOM: {
            bool takeChest = true;
            if (gc.relics.has(RelicId::CURSED_KEY)) {
                takeChest = gc.info.chestSize == ChestSize::LARGE;
            }
            takeAction(gc, takeChest);
            break;
        }

        case ScreenState::SHOP_ROOM: {
            bool purchased = false;
            for (int i = 0; i < 3; ++i) {
                if (gc.info.shop.relicPrice(i) != -1 &&  gc.gold >= gc.info.shop.relicPrice(i)) {
                    takeAction(gc, GameAction(GameAction::RewardsActionType::RELIC, i));
                    purchased = true;
                    break;
                }
            }
            if (!purchased) {
                stepRandom(gc);
                return;
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
            takeAction(gc, bestIdx);
            break;
        }

        case ScreenState::REST_ROOM: {
            if (gc.curHp > 50 && gc.deck.getUpgradeableCount() > 0 && !gc.hasRelic(RelicId::FUSION_HAMMER)) {
                takeAction(gc, 1);
            } else if (gc.curHp < 15 && !gc.relics.has(RelicId::COFFEE_DRIPPER)){
                takeAction(gc, 0);
            } else {
                stepRandom(gc);
                return;
            }
            break;
        }

        case ScreenState::BATTLE:
        case ScreenState::INVALID:
            assert(false);
            break;

        case ScreenState::MAP_SCREEN:
        default:
            stepRandom(gc);
            return;
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
    takeAction(gc, selectOrder.front().first);
}

void search::ScumSearchAgent2::stepRewardsPolicy(GameContext &gc) {
    auto &r = gc.info.rewardsContainer;
    if (r.goldRewardCount > 0) {
        takeAction(gc, GameAction(GameAction::RewardsActionType::GOLD, 0));

    } else if (r.relicCount > 0) {
        takeAction(gc, GameAction(GameAction::RewardsActionType::RELIC, 0));

    } else if (r.potionCount > 0) {
        takeAction(gc, GameAction(GameAction::RewardsActionType::POTION, 0));

    } else if (r.cardRewardCount == 0) {
        takeAction(gc, GameAction(GameAction::RewardsActionType::SKIP));

    } else {
        if (pauseOnCardReward) {
            paused = true;
            return;
        }
        weightedCardRewardPolicy(gc);
    }
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

        if (skipCard) {
            takeAction(gc, GameAction(GameAction::RewardsActionType::CARD, rIdx, 5));

        } else {
            takeAction(gc, GameAction(GameAction::RewardsActionType::CARD, rIdx, weights[selection].first));
        }
    }
}

void search::ScumSearchAgent2::stepEventPolicy(GameContext &gc) {
    switch (gc.curEvent) {

        case Event::NEOW:
            if (gc.info.neowRewards[1].d == Neow::Drawback::CURSE || gc.info.neowRewards[2].d == Neow::Drawback::CURSE) {
                takeAction(gc, 0);
            } else {
                stepRandom(gc);
            }
            break;

        case Event::NOTE_FOR_YOURSELF:
        case Event::THE_DIVINE_FOUNTAIN:
            takeAction(gc, 0);
            break;

        case Event::BIG_FISH:
            takeAction(gc, 1);
            break;

        case Event::GOLDEN_IDOL: {
            if (gc.hasRelic(RelicId::GOLDEN_IDOL)) {
                takeAction(gc, 4);
            } else {
                takeAction(gc, 0);
            }
            break;
        }


        case Event::GHOSTS:
        case Event::MASKED_BANDITS:
            takeAction(gc, 0);
            break;

        case Event::CURSED_TOME:
            if (gc.info.eventData == 0) {
                takeAction(gc, 0);
            } else {
                takeAction(gc, gc.info.eventData+1);
            }
            break;

        case Event::KNOWING_SKULL:
            takeAction(gc, 3);
            break;

        default:
            stepRandom(gc);
            return;
    }
}

