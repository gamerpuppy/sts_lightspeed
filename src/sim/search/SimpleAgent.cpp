//
// Created by keega on 9/27/2021.
//

#include <sim/search/SimpleAgent.h>
#include <game/Game.h>
#include "sim/PrintHelpers.h"

#include <map>
#include <array>
#include <bitset>
#include <thread>
#include <mutex>

using namespace sts;

static bool haveInitMaps = false;
static int cardPriorityMap[372] {};
static int cardPlayMap[372] {};
static int bossRelicPriorityMap[200] {};
static std::bitset<372> isAoeCard;
static std::bitset<372> isDefensiveCard;
static std::map<CardId, int> *maxCopies;

static constexpr int mapWeights[3][6] = {
        {100,1000,100,10,1,0},
        {10,1000,10,100,1,0},
        {100,1000,100,1,10,0},
};


void initMaps();

bool shouldSkip(CardId id) {
    return cardPriorityMap[static_cast<int>(id)] > cardPriorityMap[static_cast<int>(CardId::ANGER)];
}

int getHighHpMonster(const BattleContext &bc) {
    int highHp = -1;
    int highIdx = -1;
    for (int i = 0; i < bc.monsters.monsterCount; ++i) {
        if (bc.monsters.arr[i].isTargetable() && bc.monsters.arr[i].curHp > highHp) {
            highHp = bc.monsters.arr[i].curHp;
            highIdx = i;
        }
    }
    return highIdx;
}

int getLowHpMonster(const BattleContext &bc) {
    int lowHp = 10000;
    int lowIdx = -1;
    for (int i = 0; i < bc.monsters.monsterCount; ++i) {
        if (bc.monsters.arr[i].isTargetable() && bc.monsters.arr[i].curHp < lowHp) {
            lowHp = bc.monsters.arr[i].curHp;
            lowIdx = i;
        }
    }
    return lowIdx;
}

int getBestCardToPlay(const BattleContext &bc, fixed_list<int,10> handIdxs) {
    int bestPriority = 10000;
    int bestHandIdx;
    for (int i = 0; i < handIdxs.size(); ++i) {
        const auto c = bc.cards.hand[handIdxs[i]];
        const int priority = 2 * cardPlayMap[static_cast<int>(c.getId())] + (c.isUpgraded() ? -1 : 0);
        if (priority < bestPriority) {
            bestPriority = priority;
            bestHandIdx = handIdxs[i];
        }
    }
    return bestHandIdx;
}

void sortCardOptions(const GameContext &gc, fixed_list<int,96> &sortedCardIdxs) {
    sortedCardIdxs.clear();
    for (int i = 0; i < gc.info.toSelectCards.size(); ++i) {
        sortedCardIdxs.push_back(i);
    }

    if (gc.info.selectScreenType == sts::CardSelectScreenType::UPGRADE ||
        gc.info.selectScreenType == sts::CardSelectScreenType::DUPLICATE)
    {
        std::sort(sortedCardIdxs.begin(), sortedCardIdxs.end(), [&](int a, int b) {
            auto ca = gc.info.toSelectCards[a].card;
            auto cb = gc.info.toSelectCards[b].card;
            auto pa = 2 * cardPriorityMap[static_cast<int>(ca.id)] + (ca.isUpgraded() ? -1 : 0);
            auto pb = 2 * cardPriorityMap[static_cast<int>(cb.id)] + (cb.isUpgraded() ? -1 : 0);
            return pa < pb;
        });

    } else {
        std::sort(sortedCardIdxs.begin(), sortedCardIdxs.end(), [&](int a, int b) {
            auto ca = gc.info.toSelectCards[a].card;
            auto cb = gc.info.toSelectCards[b].card;
            auto pa = 2 * cardPriorityMap[static_cast<int>(ca.id)] + (ca.isUpgraded() ? -1 : 0);
            auto pb = 2 * cardPriorityMap[static_cast<int>(cb.id)] + (cb.isUpgraded() ? -1 : 0);
            if (gc.info.selectScreenType == sts::CardSelectScreenType::TRANSFORM ||
                gc.info.selectScreenType == sts::CardSelectScreenType::TRANSFORM_UPGRADE) {
                if (ca.getType() == sts::CardType::CURSE) {
                    pa -= 1000;
                }
                if (cb.getType() == sts::CardType::CURSE) {
                    pb -= 1000;
                }
            }
            return  pa > pb;
        });
    }
}

//void sortCardOptions(const GameContext &gc, fixed_list<int,96> &sortedCardIdxs) {
//    sortedCardIdxs.clear();
//    for (int i = 0; i < gc.info.toSelectCards.size(); ++i) {
//        sortedCardIdxs.push_back(i);
//    }
//
//    if (gc.info.selectScreenType == sts::CardSelectScreenType::UPGRADE ||
//        gc.info.selectScreenType == sts::CardSelectScreenType::DUPLICATE)
//    {
//        std::sort(sortedCardIdxs.begin(), sortedCardIdxs.end(), [&](int a, int b) {
//            auto ca = gc.info.toSelectCards[a].card;
//            auto cb = gc.info.toSelectCards[b].card;
//            return cardPriorityMap[static_cast<int>(ca.id)] < cardPriorityMap[static_cast<int>(cb.id)];
//        });
//
//    } else {
//        std::sort(sortedCardIdxs.begin(), sortedCardIdxs.end(), [&](int a, int b) {
//            auto ca = gc.info.toSelectCards[a].card;
//            auto cb = gc.info.toSelectCards[b].card;
//            auto pa = cardPriorityMap[static_cast<int>(ca.id)];
//            auto pb = cardPriorityMap[static_cast<int>(cb.id)];
//            if (gc.info.selectScreenType == sts::CardSelectScreenType::TRANSFORM ||
//                gc.info.selectScreenType == sts::CardSelectScreenType::TRANSFORM_UPGRADE) {
//                if (ca.getType() == sts::CardType::CURSE) {
//                    pa -= 1000;
//                }
//                if (cb.getType() == sts::CardType::CURSE) {
//                    pb -= 1000;
//                }
//            }
//            return  pa > pb;
//        });
//    }
//}

int search::SimpleAgent::getIncomingDamage(const BattleContext &bc) const {
    int incomingDamage = 0;
    for (int i = 0; i < bc.monsters.monsterCount; ++i) {
        const auto &m = bc.monsters.arr[i];
        if (m.isDeadOrEscaped() || m.isHalfDead()) {
            continue;
        }

        DamageInfo dInfo;
        if (bc.player.hasRelic<R::RUNIC_DOME>()) {
            dInfo = {5*curGameContext->act, 1};

        } else {
            dInfo = m.getMoveBaseDamage(bc);
            dInfo.damage = m.calculateDamageToPlayer(bc, dInfo.damage);
        }

        incomingDamage += dInfo.damage * dInfo.attackCount;
    }
    return incomingDamage;
}

struct Path {
    fixed_list<int,16> route;
    int weight= 0;
};

// weights length of 6 corresponding to room enum (shop, rest, event, elite, monster, treasure)
fixed_list<int,16> search::SimpleAgent::getBestMapPathForWeights(const Map &m, const int *weights) {
    Path paths1[7];
    Path paths2[7];

    for (int x = 0; x < 7; ++x) {
        const auto &node = m.getNode(x, 0);
        paths1[x] = Path();
        paths2[x] = Path();

        if (node.edgeCount > 0) {
            paths1[x].route.push_back(x);
            paths1[x].weight = weights[static_cast<int>(node.room)];
        }
    }

    Path *lastPaths = paths1;
    Path *nextPaths = paths2;

    for (int y = 0; y < 14; ++y) {

        for (int x = 0; x < 7; ++x) {
            const auto &node = m.getNode(x, y);
            if (node.edgeCount <= 0) {
                continue;
            }
            const auto &curPath = lastPaths[x];

            for (int i = 0; i < node.edgeCount; ++i) {
                const auto edge = node.edges[i];
                const int roomWeight = weights[static_cast<int>(m.getNode(edge, y+1).room)];
                auto &path = nextPaths[edge];

                if (path.route.size() < curPath.route.size() + 1 || path.weight < curPath.weight + roomWeight) {
                    // path is better
                    path = curPath;
                    path.weight += roomWeight;
                    path.route.push_back(edge);
                }
            }
        }

        std::swap(lastPaths, nextPaths);
    }

    int bestPathX;
    int bestPathWeight = 0;

    for (int x = 0; x < 7; ++x) {
        const auto &path = paths1[x];
        if (path.weight > bestPathWeight) {
            bestPathWeight = path.weight;
            bestPathX = x;
        }
    }

    paths1[bestPathX].route.push_back(0);
    return paths1[bestPathX].route;
}

static void printHelper(const BattleContext &bc, const search::Action &a) {
    std::cout << bc << std::endl;
    a.printDesc(std::cout, bc) << " ";
    std::cout
            << " turn: " << bc.turn
            << " energy: " << bc.player.energy
            << " cardsPlayedThisTurn: " << bc.player.cardsPlayedThisTurn
            << " state: " << (bc.inputState == InputState::PLAYER_NORMAL ? "normal" : " probably card select")
            << std::endl;
}

search::SimpleAgent::SimpleAgent() {
    initMaps();
}

void search::SimpleAgent::takeAction(GameContext &gc, search::GameAction a) {
    actionHistory.emplace_back(a.bits);
    if (print) {
        std::cout << gc << '\n';
    }
    a.execute(gc);
}

void search::SimpleAgent::takeAction(BattleContext &bc, search::Action a) {
    actionHistory.emplace_back(a.bits);
    if (print) {
        printHelper(bc, a);
    }
    a.execute(bc);
}

void search::SimpleAgent::playout(GameContext &gc) {
    curGameContext = &gc;

    BattleContext bc;
    const auto seedStr = std::string(SeedHelper::getString(gc.seed));

    while (gc.outcome == GameOutcome::UNDECIDED) {
        if (gc.screenState == ScreenState::BATTLE) {
            bc = BattleContext();
            bc.init(gc);

            playoutBattle(bc);
            bc.exitBattle(gc);
            continue;
        }

        stepOutOfCombat(gc);
    }
}

// returns whether all potions have been tried
bool search::SimpleAgent::playPotion(BattleContext &bc) {
    bool usedAll = true;
    int i = 0;
    for (; i < bc.potionCapacity; ++i) {
        auto p = bc.potions[i];

        // TODO: implement smoke bombs
        bool canDrink = !(p == sts::Potion::FAIRY_POTION || p == sts::Potion::SMOKE_BOMB || p == sts::Potion::EMPTY_POTION_SLOT);

        if (canDrink) {
            int target = 0;
            if (potionRequiresTarget(bc.potions[i]) ) {
                if (bc.monsters.getTargetableCount() <= 0) {
                    continue;
                }
                target = getHighHpMonster(bc);

            } else {

            }
            takeAction(bc, search::Action(search::ActionType::POTION, i, target));
            break;
        }
    }
    return i == bc.potionCapacity;
}

void search::SimpleAgent::playoutBattle(BattleContext &bc) {
    bool usedPotions = !isBossEncounter(bc.encounter);
    while (bc.outcome == Outcome::UNDECIDED) {
        if (bc.inputState == InputState::CARD_SELECT) {
            stepBattleCardSelect(bc);

        } else if (bc.inputState == InputState::PLAYER_NORMAL) {
            if (usedPotions) {
                stepBattleCardPlay(bc);

            } else {
                usedPotions = playPotion(bc);
            }
        } else {
            assert(false);
        }
    }
}

void search::SimpleAgent::stepBattleCardPlay(BattleContext &bc) {
    if (!bc.isCardPlayAllowed() || bc.player.cardsPlayedThisTurn > 1000) {
        takeAction(bc, Action(ActionType::END_TURN));
        return;
    }

    fixed_list<int,10> playableCardsIdxs;
    for (int i = 0; i < bc.cards.cardsInHand; ++i) {
        const auto &c = bc.cards.hand[i];
        if (c.canUseOnAnyTarget(bc)) {
            playableCardsIdxs.push_back(i);
        }
    }

    if (playableCardsIdxs.empty()) {
        takeAction(bc, Action(ActionType::END_TURN));
        return;
    }

    fixed_list<int,10> zeroCost;
    fixed_list<int,10> zeroCostAttacks;
    fixed_list<int,10> zeroCostNonAttacks;
    fixed_list<int,10> nonZeroCostCards;
    fixed_list<int,10> aoeCards;

    for (auto handIdx : playableCardsIdxs) {
        const auto &c = bc.cards.hand[handIdx];
        if (isAoeCard.test(static_cast<int>(c.getId()))) {
            aoeCards.push_back(handIdx);
        }
        if (c.cost == 0 || c.costForTurn == 0) {
            zeroCost.push_back(handIdx);
            if (c.getType() == CardType::ATTACK) {
                zeroCostAttacks.push_back(handIdx);
            } else {
                zeroCostNonAttacks.push_back(handIdx);
            }
        } else {
            nonZeroCostCards.push_back(handIdx);
        }
    }

    const int incomingDamage = getIncomingDamage(bc);
    if (bc.player.block > (incomingDamage - curGameContext->act - 4)) {
        fixed_list<int,10> offensiveCards;
        for (auto handIdx : nonZeroCostCards) {
            const auto &c = bc.cards.hand[handIdx];
            const bool isDefensive = isDefensiveCard.test(static_cast<int>(c.getId()));
            if (!isDefensive) {
                offensiveCards.push_back(handIdx);
            }
        }

        if (offensiveCards.empty()) {
            for (int i = nonZeroCostCards.size()-1; i >= 0; --i) {
                const auto &c = bc.cards.hand[nonZeroCostCards[i]];
                if (c.doesExhaust()) {
                    nonZeroCostCards.remove(i);
                }
            }
        } else {
            nonZeroCostCards = offensiveCards;
        }
    }

    int bestCardIdx = playableCardsIdxs.front();
    if (!zeroCostNonAttacks.empty()) {
        bestCardIdx = getBestCardToPlay(bc, zeroCostNonAttacks);

    } else if (!nonZeroCostCards.empty()) {
        bestCardIdx = getBestCardToPlay(bc, nonZeroCostCards);
        if (!aoeCards.empty() && bc.monsters.monstersAlive > 1 && bc.cards.hand[bestCardIdx].getType() == CardType::ATTACK) {
            bestCardIdx = getBestCardToPlay(bc, aoeCards);
        }

    } else if (!zeroCostAttacks.empty()) {
        bestCardIdx = getBestCardToPlay(bc, zeroCostAttacks);

    } else {
        takeAction(bc, Action(ActionType::END_TURN));
        return;
    }

    const auto &c = bc.cards.hand[bestCardIdx];
    if (!c.requiresTarget()) {
        takeAction(bc, Action(ActionType::CARD, bestCardIdx));
        return;
    }

    int targetIdx;
    if (c.getType() == CardType::ATTACK) {
         targetIdx = getLowHpMonster(bc);
    } else {
        targetIdx = getHighHpMonster(bc);
    }
    takeAction(bc, Action(ActionType::CARD, bestCardIdx, targetIdx));
}

template <typename ForwardIt>
void setupCardOptionsHelper(std::vector<std::pair<search::Action,CardInstance>> &actions, const ForwardIt begin, const ForwardIt end, const std::function<bool(const CardInstance &)> &p= nullptr) {
    for (int i = 0; begin+i != end; ++i) {
        const auto &c = begin[i];
        if (!p || (p(c))) {
            actions.emplace_back(std::make_pair(search::Action(search::ActionType::SINGLE_CARD_SELECT, i), c));
        }
    }
}





void search::SimpleAgent::stepBattleCardSelect(BattleContext &bc) {

    std::vector<std::pair<search::Action,CardInstance>> actions;
    switch (bc.cardSelectInfo.cardSelectTask) {
        case CardSelectTask::ARMAMENTS:
            setupCardOptionsHelper( actions, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand,
                                    [] (const CardInstance &c) { return c.canUpgrade(); });
            break;

        case CardSelectTask::CODEX:
            for (int i = 0; i < 3; ++i) { // i -> 3 action means skip
                actions.emplace_back(search::Action(search::ActionType::SINGLE_CARD_SELECT, i), bc.cardSelectInfo.codexCards()[i]);
            }
            break;

        case CardSelectTask::DISCOVERY:
            for (int i = 0; i < 3; ++i) {
                actions.emplace_back(search::Action(search::ActionType::SINGLE_CARD_SELECT, i), bc.cardSelectInfo.codexCards()[i]);
            }
            break;

        case CardSelectTask::DUAL_WIELD:
            setupCardOptionsHelper( actions, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand,
                                    [] (const CardInstance &c) {
                                        return c.getType() == CardType::POWER || c.getType() == CardType::ATTACK;
                                    });
            break;

        case CardSelectTask::EXHUME:
            setupCardOptionsHelper(actions, bc.cards.exhaustPile.begin(), bc.cards.exhaustPile.end(),
                                   [](const auto &c) { return c.getId() != CardId::EXHUME; });
            break;

        case CardSelectTask::EXHAUST_ONE:
            setupCardOptionsHelper(actions, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand);
            break;

        case CardSelectTask::FORETHOUGHT:
        case CardSelectTask::WARCRY:
            setupCardOptionsHelper(actions, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand);
            break;

        case CardSelectTask::HEADBUTT:
        case CardSelectTask::LIQUID_MEMORIES_POTION:
            setupCardOptionsHelper(actions, bc.cards.discardPile.begin(), bc.cards.discardPile.end());
            break;

        case CardSelectTask::SECRET_TECHNIQUE:
            setupCardOptionsHelper(actions, bc.cards.drawPile.begin(), bc.cards.drawPile.end(),
                                   [] (const CardInstance &c) {
                                       return c.getType() == CardType::SKILL;
                                   });
            break;

        case CardSelectTask::SECRET_WEAPON:
            setupCardOptionsHelper(actions, bc.cards.drawPile.begin(), bc.cards.drawPile.end(),
                                   [] (const CardInstance &c) {
                                       return c.getType() == CardType::ATTACK;
                                   });
            break;

        case CardSelectTask::EXHAUST_MANY:
        case CardSelectTask::GAMBLE: // just select none
            takeAction(bc, search::Action(search::ActionType::MULTI_CARD_SELECT, 0));
            return;

        default:
#ifdef sts_asserts
            assert(false);
#endif
            break;
    }

    std::sort(actions.begin(), actions.end(), [](std::pair<search::Action,CardInstance> a, std::pair<search::Action,CardInstance> b) {
        auto pa = 2 * cardPriorityMap[static_cast<int>(a.second.id)] + (a.second.isUpgraded() ? -1 : 0);
        auto pb = 2 * cardPriorityMap[static_cast<int>(b.second.id)] + (b.second.isUpgraded() ? -1 : 0);
        return pa < pb;
    });

    switch (bc.cardSelectInfo.cardSelectTask) {
        case CardSelectTask::ARMAMENTS:
        case CardSelectTask::DUAL_WIELD:
        case CardSelectTask::CODEX:
        case CardSelectTask::DISCOVERY:
        case CardSelectTask::EXHUME:
        case CardSelectTask::FORETHOUGHT:
        case CardSelectTask::HEADBUTT:
        case CardSelectTask::HOLOGRAM:
        case CardSelectTask::LIQUID_MEMORIES_POTION:
        case CardSelectTask::NIGHTMARE:
        case CardSelectTask::MEDITATE:
        case CardSelectTask::SECRET_TECHNIQUE:
        case CardSelectTask::SECRET_WEAPON:
        case CardSelectTask::SETUP:
        case CardSelectTask::SEEK:
        case CardSelectTask::WARCRY:
            takeAction(bc, actions.front().first);
            break;

        case CardSelectTask::EXHAUST_ONE:
        case CardSelectTask::RECYCLE:
            takeAction(bc, actions.back().first);
            break;

        case CardSelectTask::EXHAUST_MANY:
        case CardSelectTask::INVALID:
        case CardSelectTask::GAMBLE:
        default:
#ifdef sts_asserts
            assert(false);
#endif
            break;
    }
}

void search::SimpleAgent::stepOutOfCombat(GameContext &gc) {
    switch (gc.screenState) {

        case ScreenState::EVENT_SCREEN:
            stepEventScreen(gc);
            break;

        case ScreenState::REWARDS:
            stepRewardsScreen(gc);
            break;

        case ScreenState::BOSS_RELIC_REWARDS: {
            int bestIdx;
            int bestPriority = 1000;
            for (int i = 0; i < 3; ++i) {
                const auto priority = bossRelicPriorityMap[static_cast<int>(gc.info.bossRelics[i])];
                if (priority < bestPriority) {
                    bestPriority = priority;
                    bestIdx = i;
                }
            }
            takeAction(gc, {bestIdx});
            break;
        }

        case ScreenState::CARD_SELECT: {
            fixed_list<int,96> sortedCardIdxs;
            sortCardOptions(gc, sortedCardIdxs);
            takeAction(gc, sortedCardIdxs[0]);
            break;
        }

        case ScreenState::MAP_SCREEN: {
            if (gc.act == 4) {
                takeAction(gc, 3);
            }

            if (gc.curMapNodeY < 0) {
                mapPath = getBestMapPathForWeights(*gc.map, mapWeights[gc.act-1]);
            }

            takeAction(gc, mapPath[gc.curMapNodeY+1]);
            break;
        }

        case ScreenState::TREASURE_ROOM:
            takeAction(gc, 0);
            break;

        case ScreenState::REST_ROOM:
            stepRestScreen(gc);
            break;

        case ScreenState::SHOP_ROOM:
            stepShopScreen(gc);
            break;

        case ScreenState::BATTLE:
        case ScreenState::INVALID:
        default:
#ifdef sts_asserts
            assert(false);
#endif
            break;
    }
}

void search::SimpleAgent::stepEventScreen(GameContext &gc) {
    switch (gc.curEvent) {
        case Event::THE_SSSSSERPENT:
        case Event::GHOSTS:
        case Event::GOLDEN_IDOL:
        case Event::MASKED_BANDITS:
        case Event::THE_LIBRARY:
            takeAction(gc, 1);
            break;

        case Event::AUGMENTER:
        case Event::VAMPIRES:
            takeAction(gc, 2);
            break;

        case Event::KNOWING_SKULL:
            takeAction(gc, 3);
            break;

        case Event::NEOW:
            takeAction(gc, 0);
            break;

        default: {
            auto actions = GameAction::getAllActionsInState(gc);
            takeAction(gc, actions[0]);
        }
    }
}

void search::SimpleAgent::stepRestScreen(GameContext &gc) {
    auto vec = GameAction::getAllActionsInState(gc);
    const bool canRest = !gc.hasRelic(RelicId::COFFEE_DRIPPER);
    const bool canSmith = !gc.relics.has(RelicId::FUSION_HAMMER) && gc.deck.getUpgradeableCount() > 0;
    const bool canLift = gc.relics.has(RelicId::GIRYA) && gc.relics.getRelicValue(RelicId::GIRYA) != 3;
    const bool canDig = gc.relics.has(RelicId::SHOVEL);
    const bool canRemove = gc.relics.has(RelicId::PEACE_PIPE);
    const bool canTakeKey = !gc.hasKey(Key::RUBY_KEY);

    if (canRest &&
        (gc.curHp < gc.maxHp / 2 || (gc.act != 1 && gc.floorNum % 17 == 15 && gc.curHp < gc.maxHp * 0.9))) {
        takeAction(gc, 0);
    } else if (canSmith) {
        takeAction(gc, 1);
    } else if (canLift) {
        takeAction(gc, 3);
    } else if (canDig) {
        takeAction(gc, 5);
    } else if (canRemove){
        takeAction(gc, 4);
    } else if (canRest) {
        takeAction(gc, 0);
    } else if (canTakeKey) {
        takeAction(gc, 2);
    } else {
        takeAction(gc, 6);
    }
}

void search::SimpleAgent::stepRewardsScreen(GameContext &gc) {
    auto &r = gc.info.rewardsContainer;

    if (r.potionCount > 0 && gc.potionCount < gc.potionCapacity) {
        takeAction(gc, {GameAction::RewardsActionType::POTION, 0});
    } else if (r.relicCount > 0) {
        takeAction(gc, {GameAction::RewardsActionType::RELIC, 0});
    } else if (r.sapphireKey || r.emeraldKey) {
        takeAction(gc, {GameAction::RewardsActionType::KEY});
    } if (r.cardRewardCount > 0) {
        stepCardReward(gc);
    } else {
        takeAction(gc, {GameAction::RewardsActionType::SKIP});
    }
}

void search::SimpleAgent::stepCardReward(GameContext &gc) {
    // assume card reward count > 0 here
    const int lastRewardIdx = gc.info.rewardsContainer.cardRewardCount-1;
    auto lastCardReward = gc.info.rewardsContainer.cardRewards[lastRewardIdx];

    std::unordered_map<CardId, int> deckCounts;
    for (auto c : lastCardReward) {
        if (deckCounts.find(c.id) == deckCounts.end()) {
            deckCounts.insert({c.id, 1});
        } else {
            deckCounts[c.id] += 1;
        }
    }

    std::vector<Card> pickableCards;
    for (auto c : lastCardReward) {
        if (maxCopies->find(c.id) == maxCopies->end() || deckCounts.at(c.id) < maxCopies->at(c.id)) {
            pickableCards.emplace_back(c);
        }
    }

    if (pickableCards.empty()) {
        takeAction(gc, GameAction(GameAction::RewardsActionType::CARD, lastRewardIdx, 5) );
        return;
    }

    int bestIdx = 10000;
    Card bestCard;
    for (auto c : pickableCards) {
        int pickIdx = 2*cardPriorityMap[static_cast<int>(c.id)] + (c.isUpgraded() ? -1 : 0);
        if (pickIdx < bestIdx) {
            bestCard = c;
            bestIdx = pickIdx;
        }
    }

    for (int i = 0; i < lastCardReward.size(); ++i) {
        if (bestCard == lastCardReward[i]) {
            takeAction(gc, GameAction(GameAction::RewardsActionType::CARD, lastRewardIdx, i));
            break;
        }
    }
}

void search::SimpleAgent::stepShopScreen(GameContext &gc) {
    auto &s = gc.info.shop;

    if (s.removeCost != -1 && gc.gold >= s.removeCost) {
        takeAction(gc, {GameAction::RewardsActionType::CARD_REMOVE});
        return;
    }

    for (int i = 0; i < 7; ++i) {
        if (s.cardPrice(i) == -1 ||
            gc.gold < s.cardPrice(i) ||
            shouldSkip(s.cards[i].getId()))
        {
            continue;
        }
        takeAction(gc, GameAction(GameAction::RewardsActionType::CARD, i));
        return;
    }

    for (int i = 0; i < 3; ++i) {
        if (s.relicPrice(i) == -1 ||
            gc.gold < s.relicPrice(i))
        {
            continue;
        }
        takeAction(gc, GameAction(GameAction::RewardsActionType::RELIC, i));
        return;
    }

    takeAction(gc, GameAction(GameAction::RewardsActionType::SKIP));
}

constexpr std::array<CardId,133> cardsPriorities = {
        CardId::APOTHEOSIS,
        CardId::GHOSTLY_ARMOR,
        CardId::PERFECTED_STRIKE,
        CardId::WHIRLWIND,
        CardId::BATTLE_TRANCE,
        CardId::DEMON_FORM,
        CardId::RAGE,
        CardId::OFFERING,
        CardId::IMPERVIOUS,
        CardId::IMMOLATE,
        CardId::LIMIT_BREAK,
        CardId::FLAME_BARRIER,
        CardId::MASTER_OF_STRATEGY,
        CardId::INFLAME,
        CardId::DISARM,
        CardId::SHRUG_IT_OFF,
        CardId::DOUBLE_TAP,
        CardId::THUNDERCLAP,
        CardId::METALLICIZE,
        CardId::POMMEL_STRIKE,
        CardId::SHOCKWAVE,
        CardId::UPPERCUT,
        CardId::JAX,
        CardId::PANIC_BUTTON,
        CardId::FLASH_OF_STEEL,
        CardId::FLEX,
        CardId::ANGER,
        CardId::SECRET_WEAPON,
        CardId::FINESSE,
        CardId::MAYHEM,
        CardId::PANACHE,
        CardId::SECRET_TECHNIQUE,
        CardId::METAMORPHOSIS,
        CardId::THINKING_AHEAD,
        CardId::MADNESS,
        CardId::DISCOVERY,
        CardId::CHRYSALIS,
        CardId::DEEP_BREATH,
        CardId::TRIP,
        CardId::ENLIGHTENMENT,
        CardId::HEAVY_BLADE,
        CardId::FEED,
        CardId::FIEND_FIRE,
        CardId::TWIN_STRIKE,
        CardId::HEADBUTT,
        CardId::SEEING_RED,
        CardId::COMBUST,
        CardId::CLASH,
        CardId::DARK_SHACKLES,
        CardId::SWORD_BOOMERANG,
        CardId::DRAMATIC_ENTRANCE,
        CardId::BLUDGEON,
        CardId::HAND_OF_GREED,
        CardId::EVOLVE,
        CardId::VIOLENCE,
        CardId::BITE,
        CardId::CARNAGE,
        CardId::CLOTHESLINE,
        CardId::BASH,
        CardId::BANDAGE_UP,
        CardId::PANACEA,
        CardId::RECKLESS_CHARGE,
        CardId::INFERNAL_BLADE,
        CardId::SPOT_WEAKNESS,
        CardId::STRIKE_RED,
        CardId::SHIV,
        CardId::HAVOC,
        CardId::RITUAL_DAGGER,
        CardId::DROPKICK,
        CardId::FEEL_NO_PAIN,
        CardId::SWIFT_STRIKE,
        CardId::CORRUPTION,
        CardId::MAGNETISM,
        CardId::BLOODLETTING,
        CardId::IRON_WAVE,
        CardId::ARMAMENTS,
        CardId::MIND_BLAST,
        CardId::ASCENDERS_BANE,
        CardId::DAZED,
        CardId::VOID,
        CardId::RAMPAGE,
        CardId::GHOSTLY_ARMOR,
        CardId::TRUE_GRIT,
        CardId::BLIND,
        CardId::GOOD_INSTINCTS,
        CardId::PUMMEL,
        CardId::HEMOKINESIS,
        CardId::EXHUME,
        CardId::REAPER,
        CardId::CLEAVE,
        CardId::WARCRY,
        CardId::PURITY,
        CardId::DUAL_WIELD,
        CardId::WILD_STRIKE,
        CardId::DEFEND_RED,
        CardId::BODY_SLAM,
        CardId::SEVER_SOUL,
        CardId::BURNING_PACT,
        CardId::BRUTALITY,
        CardId::BARRICADE,
        CardId::INTIMIDATE,
        CardId::JUGGERNAUT,
        CardId::SADISTIC_NATURE,
        CardId::DARK_EMBRACE,
        CardId::POWER_THROUGH,
        CardId::TRANSMUTATION,
        CardId::SENTINEL,
        CardId::RUPTURE,
        CardId::SLIMED,
        CardId::FIRE_BREATHING,
        CardId::SECOND_WIND,
        CardId::IMPATIENCE,
        CardId::THE_BOMB,
        CardId::JACK_OF_ALL_TRADES,
        CardId::SEARING_BLOW,
        CardId::BLOOD_FOR_BLOOD,
        CardId::BERSERK,
        CardId::ENTRENCH,
        CardId::FORETHOUGHT,
        CardId::CLUMSY,
        CardId::PARASITE,
        CardId::SHAME,
        CardId::INJURY,
        CardId::WOUND,
        CardId::WRITHE,
        CardId::DOUBT,
        CardId::BURN,
        CardId::DECAY,
        CardId::REGRET,
        CardId::NECRONOMICURSE,
        CardId::PAIN,
        CardId::NORMALITY,
        CardId::PRIDE,
};

constexpr std::array<CardId,133> cardPlayPriorities = {
        CardId::APOTHEOSIS,
        CardId::OFFERING,
        CardId::DEMON_FORM,
        CardId::INFLAME,
        CardId::METALLICIZE,
        CardId::DISARM,
        CardId::SHOCKWAVE,
        CardId::GHOSTLY_ARMOR,
        CardId::LIMIT_BREAK,
        CardId::DOUBLE_TAP,
        CardId::THUNDERCLAP,
        CardId::IMMOLATE,
        CardId::UPPERCUT,
        CardId::FLAME_BARRIER,
        CardId::SHRUG_IT_OFF,
        CardId::IMPERVIOUS,
        CardId::MADNESS,
        CardId::PERFECTED_STRIKE,
        CardId::BATTLE_TRANCE,
        CardId::RAGE,
        CardId::MASTER_OF_STRATEGY,
        CardId::POMMEL_STRIKE,
        CardId::JAX,
        CardId::FLASH_OF_STEEL,
        CardId::FLEX,
        CardId::ANGER,
        CardId::DEFEND_RED,
        CardId::BASH,
        CardId::WHIRLWIND,
        CardId::PANIC_BUTTON,
        CardId::SECRET_WEAPON,
        CardId::FINESSE,
        CardId::MAYHEM,
        CardId::PANACHE,
        CardId::SECRET_TECHNIQUE,
        CardId::METAMORPHOSIS,
        CardId::THINKING_AHEAD,
        CardId::DISCOVERY,
        CardId::CHRYSALIS,
        CardId::DEEP_BREATH,
        CardId::TRIP,
        CardId::ENLIGHTENMENT,
        CardId::HEAVY_BLADE,
        CardId::FEED,
        CardId::FIEND_FIRE,
        CardId::TWIN_STRIKE,
        CardId::HEADBUTT,
        CardId::SEEING_RED,
        CardId::COMBUST,
        CardId::CLASH,
        CardId::DARK_SHACKLES,
        CardId::SWORD_BOOMERANG,
        CardId::DRAMATIC_ENTRANCE,
        CardId::BLUDGEON,
        CardId::HAND_OF_GREED,
        CardId::EVOLVE,
        CardId::VIOLENCE,
        CardId::BITE,
        CardId::CARNAGE,
        CardId::CLOTHESLINE,
        CardId::BANDAGE_UP,
        CardId::PANACEA,
        CardId::RECKLESS_CHARGE,
        CardId::INFERNAL_BLADE,
        CardId::STRIKE_RED,
        CardId::SPOT_WEAKNESS,
        CardId::SHIV,
        CardId::HAVOC,
        CardId::RITUAL_DAGGER,
        CardId::DROPKICK,
        CardId::FEEL_NO_PAIN,
        CardId::SWIFT_STRIKE,
        CardId::CORRUPTION,
        CardId::MAGNETISM,
        CardId::BLOODLETTING,
        CardId::IRON_WAVE,
        CardId::ARMAMENTS,
        CardId::MIND_BLAST,
        CardId::ASCENDERS_BANE,
        CardId::DAZED,
        CardId::VOID,
        CardId::RAMPAGE,
        CardId::GHOSTLY_ARMOR,
        CardId::TRUE_GRIT,
        CardId::BLIND,
        CardId::GOOD_INSTINCTS,
        CardId::PUMMEL,
        CardId::HEMOKINESIS,
        CardId::EXHUME,
        CardId::REAPER,
        CardId::CLEAVE,
        CardId::WARCRY,
        CardId::PURITY,
        CardId::DUAL_WIELD,
        CardId::WILD_STRIKE,
        CardId::BODY_SLAM,
        CardId::SEVER_SOUL,
        CardId::BURNING_PACT,
        CardId::BRUTALITY,
        CardId::BARRICADE,
        CardId::INTIMIDATE,
        CardId::JUGGERNAUT,
        CardId::SADISTIC_NATURE,
        CardId::DARK_EMBRACE,
        CardId::POWER_THROUGH,
        CardId::TRANSMUTATION,
        CardId::SENTINEL,
        CardId::RUPTURE,
        CardId::SLIMED,
        CardId::FIRE_BREATHING,
        CardId::SECOND_WIND,
        CardId::IMPATIENCE,
        CardId::THE_BOMB,
        CardId::JACK_OF_ALL_TRADES,
        CardId::SEARING_BLOW,
        CardId::BLOOD_FOR_BLOOD,
        CardId::BERSERK,
        CardId::ENTRENCH,
        CardId::FORETHOUGHT,
        CardId::CLUMSY,
        CardId::PARASITE,
        CardId::SHAME,
        CardId::INJURY,
        CardId::WOUND,
        CardId::WRITHE,
        CardId::DOUBT,
        CardId::BURN,
        CardId::DECAY,
        CardId::REGRET,
        CardId::NECRONOMICURSE,
        CardId::PAIN,
        CardId::NORMALITY,
        CardId::PRIDE,
};

constexpr std::array<CardId,13>  defensiveCards = {
        CardId::POWER_THROUGH,
        CardId::TRUE_GRIT,
        CardId::IMPERVIOUS,
        CardId::SHRUG_IT_OFF,
        CardId::FLAME_BARRIER,
        CardId::ENTRENCH,
        CardId::DEFEND_RED,
        CardId::SENTINEL,
        CardId::SECOND_WIND,
        CardId::GHOSTLY_ARMOR,
        CardId::DARK_SHACKLES,
        CardId::PANIC_BUTTON,
        CardId::RAGE,
};

constexpr std::array<RelicId,24> bossRelicPriorities = {
        R::SOZU,
        R::SNECKO_EYE,
        R::PHILOSOPHERS_STONE,
        R::RUNIC_DOME,
        R::CURSED_KEY,
        R::FUSION_HAMMER,
        R::VELVET_CHOKER,
        R::ECTOPLASM,
        R::MARK_OF_PAIN,
        R::BUSTED_CROWN,
        R::EMPTY_CAGE,
        R::ASTROLABE,
        R::RUNIC_PYRAMID,
        R::LIZARD_TAIL,
        R::ETERNAL_FEATHER,
        R::COFFEE_DRIPPER,
        R::BLACK_BLOOD,
        R::TINY_HOUSE,
        R::BLACK_STAR,
        R::ORRERY,
        R::RUNIC_CUBE,
        R::PANDORAS_BOX,
        R::WHITE_BEAST_STATUE,
        R::CALLING_BELL,
};


void initMaps() {
    // there are synchronization issues with this if multiple threads start at once
    if (haveInitMaps) {
        return;
    }
    haveInitMaps = true;
    for (int i = 0; i < cardPlayPriorities.size(); ++i) {
        CardId c = cardPlayPriorities[i];
        cardPlayMap[static_cast<int>(c)] = i + 1;
    }

    for (int i = 0; i < cardsPriorities.size(); ++i) {
        CardId c = cardsPriorities[i];
        cardPriorityMap[static_cast<int>(c)] = i + 1;
    }

    for (int i = 0; i < bossRelicPriorities.size(); ++i) {
        RelicId r = bossRelicPriorities[i];
        bossRelicPriorityMap[static_cast<int>(r)] = i + 1;
    }

    maxCopies = new std::map<CardId, int>({
        {CardId::OFFERING, 1},
        {CardId::IMPERVIOUS, 99},
        {CardId::APOTHEOSIS, 1},
        {CardId::GHOSTLY_ARMOR, 99},
        {CardId::PERFECTED_STRIKE, 99},
        {CardId::WHIRLWIND, 2},
        {CardId::BATTLE_TRANCE, 2},
        {CardId::DEMON_FORM, 1},
        {CardId::IMMOLATE, 1},
        {CardId::RAGE, 2},
        {CardId::LIMIT_BREAK, 3},
        {CardId::FLAME_BARRIER, 2},
        {CardId::MASTER_OF_STRATEGY, 99},
        {CardId::INFLAME, 1},
        {CardId::DISARM, 2},
        {CardId::SHRUG_IT_OFF, 3},
        {CardId::DOUBLE_TAP, 1},
        {CardId::THUNDERCLAP, 1},
        {CardId::METALLICIZE, 1},
        {CardId::POMMEL_STRIKE, 1},
        {CardId::SHOCKWAVE, 1},
        {CardId::UPPERCUT, 1},
        {CardId::JAX, 1},
        {CardId::PANIC_BUTTON, 1},
        {CardId::FLASH_OF_STEEL, 99},
        {CardId::FLEX, 1}
    });

    for (auto c : defensiveCards) {
        isDefensiveCard.set(static_cast<int>(c));
    }

    isAoeCard.set(static_cast<int>(CardId::CLEAVE));
    isAoeCard.set(static_cast<int>(CardId::IMMOLATE));
    isAoeCard.set(static_cast<int>(CardId::THUNDERCLAP));
    isAoeCard.set(static_cast<int>(CardId::WHIRLWIND));
}

struct SimpleAgentInfo {
    bool shouldPrint;
    std::uint64_t seedStart;
    std::uint64_t seedEnd;

    std::mutex m;
    std::uint64_t curSeed;
    std::int64_t winCount = 0;
    std::int64_t lossCount = 0;
    std::int64_t floorSum = 0;
};

void agentMtRunner(SimpleAgentInfo *info) {
    std::uint64_t seed;
    {
        std::scoped_lock lock(info->m);
        seed = info->curSeed++;
    }

    while(true) {
        if (seed >= info->seedEnd) {
            break;
        }

        GameContext gc(CharacterClass::IRONCLAD, seed, 0);

//        gc.obtainRelic(sts::RelicId::NEOWS_LAMENT);
//        gc.playerIncreaseMaxHp(100);

        search::SimpleAgent agent;
        agent.print = info->shouldPrint;
        agent.playout(gc);


//        printOutcome(std::cout, gc);
        {
            std::scoped_lock lock(info->m);
            info->floorSum += gc.floorNum;
            if (gc.outcome == sts::GameOutcome::PLAYER_VICTORY) {
                ++info->winCount;

            } else {
                ++info->lossCount;
            }
            seed = info->curSeed++;
        }
    }
}

void search::SimpleAgent::runAgentsMt(int threadCount, std::uint64_t startSeed, int playoutCount, bool print) {
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::unique_ptr<std::thread>> threads;

    SimpleAgentInfo info;
    info.curSeed = startSeed;
    info.seedStart = startSeed;
    info.seedEnd = startSeed + playoutCount;
    info.shouldPrint = print;


    if (threadCount == 1) { // doing this for more consistency when benchmarking
        agentMtRunner(&info);

    } else {
        for (int tid = 0; tid < threadCount; ++tid) {
            threads.emplace_back(new std::thread(agentMtRunner, &info));
        }
    }

    for (int tid = 0; tid < threadCount; ++tid) {
        if (threadCount > 1) {
            threads[tid]->join();
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(endTime-startTime).count();

    std::cout << "w/l: (" << info.winCount  << ", " << info.lossCount << ")"
              << " percentWin: " << static_cast<double>(info.winCount) / playoutCount * 100 << "%"
              << " avgFloorReached: " << static_cast<double>(info.floorSum) / playoutCount << '\n';

    std::cout << "threads: " << threadCount
              << " playoutCount: " << playoutCount
              << " elapsed: " << duration
              << std::endl;
}
