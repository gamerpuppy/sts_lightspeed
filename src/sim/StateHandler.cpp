//
// Created by gamerpuppy on 8/28/2021.
//

#include "sim/StateHandler.h"

#include "game/GameContext.h"
#include "combat/BattleContext.h"


using namespace sts;

GameContextOperator regainControlOperator() {
    return [] (GameContext &gc) {
        gc.regainControl();
    };
}

GameContextOperator restOperator(int idx) {
    return [=](GameContext &gc) {
        gc.chooseCampfireOption(idx);
    };
}

GameContextOperator mapOperator(int mapNodeX) {
    return [=](GameContext &gc) {
        gc.transitionToMapNode(mapNodeX);
    };
}

void RandomStateHandler::setupState(const GameContext &gc) {
    switch (gc.screenState) {

        case ScreenState::EVENT_SCREEN:
            setupEventOptions(gc);
            break;

        case ScreenState::REWARDS:
            setupRewardsOptions(gc);
            break;

        case ScreenState::MAP_SCREEN:
            setupMapOptions(gc);
            break;

        case ScreenState::REST_ROOM:
            setupRestOptions(gc);
            break;

        case ScreenState::SHOP_ROOM:
            setupShopOptions(gc);
            break;

        case ScreenState::TREASURE_ROOM: {
            optionFunctions.clear();
            optionFunctions.push_back(regainControlOperator());
            optionFunctions.push_back([](GameContext &gc) { gc.openTreasureRoomChest(); });
            break;
        }

        case ScreenState::BOSS_RELIC_REWARDS:
        case ScreenState::CARD_SELECT:
            break;

        case ScreenState::BATTLE:
        case ScreenState::INVALID:
        default:
            break;
    }
}

int RandomStateHandler::getStateSize(const GameContext &gc) const {
    switch (gc.screenState) {
        case ScreenState::CARD_SELECT:
            return gc.info.toSelectCards.size();

        case ScreenState::BOSS_RELIC_REWARDS:
            return 3;

        case ScreenState::TREASURE_ROOM:
            return 2;

        case ScreenState::EVENT_SCREEN:
            if (gc.curEvent == Event::MATCH_AND_KEEP) {
                return (matchAndKeepCardMap.size()-1) * (matchAndKeepCardMap.size()-1);
            } else {
                return optionFunctions.size();
            }

        case ScreenState::MAP_SCREEN:
        case ScreenState::REST_ROOM:
        case ScreenState::REWARDS:
        case ScreenState::SHOP_ROOM:
            return optionFunctions.size();

        case ScreenState::INVALID:
        case ScreenState::BATTLE:
        default:
            return 0;
    }
}

void RandomStateHandler::chooseOption(GameContext &gc, int option) const {
    switch (gc.screenState) {
        case ScreenState::INVALID:
            break;

        case ScreenState::BOSS_RELIC_REWARDS:
            gc.chooseBossRelic(option);
            break;

        case ScreenState::CARD_SELECT:
            gc.chooseSelectCardScreenOption(option);
            break;

        case ScreenState::TREASURE_ROOM:
            gc.chooseTreasureRoomOption(option == 0);
            break;

        case ScreenState::EVENT_SCREEN:
            if (gc.curEvent == Event::MATCH_AND_KEEP) {
                const auto &m = matchAndKeepCardMap;
                const int idx1 = option / m.size();
                int idx2 = option % m.size();
                if (idx2 == idx1) {
                    idx2 = idx1+1;
                }
                gc.chooseMatchAndKeepCards(m[idx1], m[idx2]);

            } else {
                optionFunctions[option](gc);
            }
            break;

        case ScreenState::REWARDS:
        case ScreenState::MAP_SCREEN:
        case ScreenState::REST_ROOM:
        case ScreenState::SHOP_ROOM:
            optionFunctions[option](gc);
            break;

        case ScreenState::BATTLE:
            break;
    }
}

void RandomStateHandler::setupMapOptions(const GameContext &gc) {
    optionFunctions.clear();

    if (gc.curMapNodeY == 14) {
        optionFunctions.push_back(mapOperator(0));

    } else if (gc.curMapNodeY == -1) {
        for (auto firstRowNode : gc.map->nodes[0]) {
            if (firstRowNode.edgeCount > 0) {
                optionFunctions.push_back(mapOperator(firstRowNode.x));
            }
        }

    } else {
        auto node = gc.map->getNode(gc.curMapNodeX, gc.curMapNodeY);
        for (int i = 0; i < node.edgeCount; ++i) {
            optionFunctions.push_back(mapOperator(node.edges[i]));
        }
    }

}

void RandomStateHandler::setupRestOptions(const GameContext &gc) {
    optionFunctions.clear();

    if (!gc.relics.has(RelicId::COFFEE_DRIPPER)) {
        optionFunctions.push_back(restOperator(0));
    }

    if (!gc.relics.has(RelicId::FUSION_HAMMER) && gc.deck.getUpgradeableCount() > 0) {
        optionFunctions.push_back(restOperator(1));
    }

    if (!gc.hasKey(Key::RUBY_KEY)) {
        optionFunctions.push_back(restOperator(2));
    }

    if (gc.relics.has(RelicId::GIRYA) && gc.relics.getRelicValue(RelicId::GIRYA) != 3) {
        optionFunctions.push_back(restOperator(3));
    }

    if (gc.relics.has(RelicId::PEACE_PIPE)) { // assume we have card to remove
        optionFunctions.push_back(restOperator(4));
    }

    if (gc.relics.has(RelicId::SHOVEL)) { // assume we have card to remove
        optionFunctions.push_back(restOperator(5));
    }

    if (optionFunctions.empty()) {
        optionFunctions.push_back(restOperator(6));
    }
}

void RandomStateHandler::setupRewardsOptions(const GameContext &gc) {
    optionFunctions.clear();

    const auto &r = gc.info.rewardsContainer;
    for (int i = 0; i < r.goldRewardCount; ++i) {
        optionFunctions.push_back([=](GameContext &gc) {
            gc.gainGold(r.gold[i]);
            gc.info.rewardsContainer.removeGoldReward(i);
        });
    }

    for (int i = 0; i < r.cardRewardCount; ++i) {

        for (int x = 0; x < r.cardRewards[i].size(); ++x) {

            optionFunctions.push_back([=](GameContext &gc) {
                gc.deck.obtain(gc, r.cardRewards[i][x]);
                gc.info.rewardsContainer.removeCardReward(i);
            });

        }

    }

    for (int i = 0; i < r.relicCount; ++i) {

        const bool newBlueKeyValue = r.sapphireKey && (i < r.relicCount - 1);

        optionFunctions.push_back([=](GameContext &c) {
            c.obtainRelic(c.info.rewardsContainer.relics[i]);
            c.info.rewardsContainer.removeRelicReward(i);
            c.info.rewardsContainer.sapphireKey = newBlueKeyValue;
        });
    }

    if (r.emeraldKey) {
        optionFunctions.push_back([=](GameContext &c) {
            c.obtainKey(Key::EMERALD_KEY);
            c.info.rewardsContainer.emeraldKey = false;
        });
    }

    if (r.sapphireKey) {
        optionFunctions.push_back([=](GameContext &c) {
            c.obtainKey(Key::SAPPHIRE_KEY);
            c.info.rewardsContainer.sapphireKey = false;
        });
    }

    for (int i = 0; i < r.potionCount; ++i) {
        optionFunctions.push_back([=](GameContext &c) {
            c.obtainPotion(c.info.rewardsContainer.potions[i]);
            c.info.rewardsContainer.removePotionReward(i);
        });
    }

    optionFunctions.push_back(regainControlOperator());
}

void RandomStateHandler::setupShopOptions(const GameContext &gc) {
    optionFunctions.clear();

    const auto &s = gc.info.shop;

    for (int i = 0; i < 7; ++i) {
        auto price = s.cardPrice(i);
        if (price != -1 && gc.gold >= price) {
            optionFunctions.push_back([=](GameContext &gc) {
                gc.info.shop.buyCard(gc, i);
            });
        }
    }

    for (int i = 0; i < 3; ++i) {
        auto price = s.relicPrice(i);
        if (price != -1 && gc.gold >= price) {
            optionFunctions.push_back([=](GameContext &gc) {
                gc.info.shop.buyRelic(gc, i);
            });
        }
    }

    for (int i = 0; i < 3; ++i) {
        auto price = s.potionPrice(i);
        if (price != -1 && gc.gold >= price) {
            optionFunctions.push_back([=](GameContext &gc) {
                gc.info.shop.buyPotion(gc, i);
            });
        }
    }

    if (s.removeCost != -1 && gc.gold >= s.removeCost) {
        optionFunctions.push_back([=](GameContext &gc) {
            gc.info.shop.buyCardRemove(gc);
        });
    }

    optionFunctions.push_back(regainControlOperator());
}

GameContextOperator eventOption(int idx) {
    return [=] (GameContext &gc) {
        gc.chooseEventOption(idx);
    };
}

void RandomStateHandler::pushOption(int idx) {
    optionFunctions.push_back(eventOption(idx));
}

void RandomStateHandler::pushOptions(int start, int count) {
    for (int i = start; i < start+count; ++i) {
        optionFunctions.push_back(eventOption(i));
    }
}

void RandomStateHandler::setupEventOptions(const GameContext &gc) {
    optionFunctions.clear();
    const bool unfavorable = gc.ascension >= 15;

    switch (gc.curEvent) {
        case Event::NEOW:
            pushOptions(0, 4);
            break;

        case Event::OMINOUS_FORGE:
            if (gc.deck.getUpgradeableCount() > 0) {
                pushOption(0);
            } else {
                pushOptions(1, 2);
            }
            break;

        case Event::ADDICT:
            if (gc.gold > 85) {
                pushOption(0);
            } else {
                pushOptions(1, 2);
            }
            break;

        case Event::BACK_TO_BASICS:
        case Event::BEGGAR:
            pushOptions(0, 2);
            break;

        case Event::BIG_FISH:
            pushOptions(0, 3);

        case Event::COLOSSEUM:
            if (gc.info.eventData == 0) {
                pushOption(0);
            } else {
                pushOptions(0, 2);
            }
            break;


        case Event::CURSED_TOME: {
            const int phase = gc.info.eventData;
            switch (phase) {
                case 0:
                    pushOptions(0, 2);
                    break;

                case 1:
                case 2:
                case 3:
                    pushOption(0);
                    break;

                case 4:
                    pushOptions(0, 2);
                    break;

                default:
                    assert(false);
                    break;
            }
            break;
        }

        case Event::DEAD_ADVENTURER:
            pushOptions(0, 2);
            break;

        case Event::DESIGNER: { // Designer In-Spire
            const auto upgradeOne = gc.info.upgradeOne;
            const auto cleanUpIsRemoveCard = gc.info.cleanUpIsRemoveCard;

            const auto goldCost0 = unfavorable ? 50 : 40;
            if (gc.gold >= goldCost0 && gc.deck.getUpgradeableCount(1) > 0) {
                if (upgradeOne) {
                    pushOption(0);
                } else {
                    pushOption(1);
                }
            }

            const auto goldCost1 = unfavorable ? 75 : 60;
            if (gc.gold >= goldCost1) {
                if (cleanUpIsRemoveCard) {
                    if (gc.deck.getTransformableCount(1) >= 1) {
                        pushOption(2);
                    }
                } else {
                    if (gc.deck.getTransformableCount(2) >= 2) {
                        pushOption(3);
                    }
                }
            }

            const auto goldCost2= unfavorable ? 110 : 90;
            if (gc.gold >= goldCost2) {
                if (gc.deck.getTransformableCount(1) >= 1) {
                    pushOption(4);
                }
            }

            pushOption(5);
            break;
        }

        case Event::DRUG_DEALER:
            if (gc.deck.getTransformableCount(2) >= 2) {
                pushOption(0);
            } else {
                pushOptions(1, 2);
            }
            break;

        case Event::DUPLICATOR:
            pushOptions(0, 2);
            break;

        case Event::FACE_TRADER:
            pushOptions(0, 3);
            break;

        case Event::FALLING: {
            if (gc.info.skillCardDeckIdx != -1) { // Skill
                pushOption(0);
            }
            if (gc.info.powerCardDeckIdx != -1) { // Power
                pushOption(1);
            }
            if (gc.info.attackCardDeckIdx != -1) {
                pushOption(2);
            }

            if (optionFunctions.empty()) {
                pushOption(3);
            }
            break;
        }

        case Event::FORGOTTEN_ALTAR:
            if (gc.relics.has(RelicId::GOLDEN_IDOL)) {
                pushOption(0);
            }
            pushOptions(1, 2);
            break;

        case Event::FOUNTAIN_OF_CLEANSING:
            pushOptions(0, 2);
            break;

        case Event::GHOSTS:
            pushOptions(0, 2);
            break;

        case Event::GOLDEN_IDOL:
            if (!gc.relics.has(RelicId::GOLDEN_IDOL)) {
                pushOptions(0, 2);
            } else {
                pushOptions(0, 3);
            }
            break;

        case Event::GOLDEN_SHRINE:
            pushOptions(0, 3);
            break;

        case Event::GOLDEN_WING:
            pushOption(0);
            if (gc.deck.hasCardForWingStatue()) {
                pushOption(1);
            }
            pushOption(2);
            break;

        case Event::KNOWING_SKULL:
            pushOptions(0, 4);
            break;

        case Event::LAB:
            // not handled as an event
            break;

        case Event::LIARS_GAME:
            pushOptions(0, 2);
            break;

        case Event::LIVING_WALL:
            pushOptions(0, 2);
            pushOption(2);
            break;

        case Event::MASKED_BANDITS:
            pushOptions(0, 2);
            break;


        case Event::MATCH_AND_KEEP: { // handled differently from other events
            matchAndKeepCardMap.clear();
            for (int i = 0; i < gc.info.toSelectCards.size(); ++i) {
                const auto &c = gc.info.toSelectCards[i];
                if (c.deckIdx == 0) {
                    continue;
                } else {
                    matchAndKeepCardMap.push_back(i);
                }
            }
            break;
        }

        case Event::MINDBLOOM:
            pushOptions(0, 2);
            if (gc.floorNum <= 40) {
                pushOption(2);
            } else {
                pushOption(3);
            }
            break;

        case Event::MUSHROOMS:
            pushOptions(0, 2);
            break;

        case Event::MYSTERIOUS_SPHERE:
            pushOptions(0, 2);
            break;

        case Event::NEST:
            pushOptions(0, 2);
            break;

        case Event::NLOTH:
            pushOptions(0, 3);
            break;

        case Event::NOTE_FOR_YOURSELF:
            pushOptions(0, 2);
            break;

        case Event::PURIFIER:
            if (gc.deck.getTransformableCount(1) > 0) {
                pushOption(0);
            }
            pushOption(1);
            break;

        case Event::SCRAP_OOZE:
            pushOptions(0, 2);
            break;

        case Event::SECRET_PORTAL:
            pushOptions(0, 2);
            break;

        case Event::SENSORY_STONE:
            pushOptions(0, 3);
            break;

        case Event::SHINING_LIGHT:
            pushOptions(0, 2);
            break;

        case Event::THE_CLERIC: {
            pushOption(0);
            int purifyCost = unfavorable ? 75 : 50;
            if (gc.gold >= purifyCost) {
                pushOption(1);
            }
            pushOption(2);
            break;
        }

        case Event::THE_JOUST:
            pushOptions(0, 2);
            break;

        case Event::THE_LIBRARY:
            pushOptions(0, 2);
            break;

        case Event::THE_MAUSOLEUM:
            pushOptions(0, 2);
            break;

        case Event::THE_MOAI_HEAD:
            pushOption(0);
            if (gc.hasRelic(RelicId::GOLDEN_IDOL)) {
                pushOption(1);
            }
            pushOption(2);
            break;

        case Event::THE_WOMAN_IN_BLUE:
            pushOptions(0,4);
            break;

        case Event::TOMB_OF_LORD_RED_MASK:
            if (gc.hasRelic(RelicId::RED_MASK)) {
                pushOption(0);
            } else {
                pushOption(1);
            }
            pushOption(2);
            break;

        case Event::TRANSMORGRIFIER:
            if (gc.deck.getTransformableCount(1) > 0) {
                pushOption(0);
            }
            pushOption(1);
            break;

        case Event::UPGRADE_SHRINE:
            if (gc.deck.getUpgradeableCount(1) > 0) {
                pushOption(0);
            }
            pushOption(1);
            break;

        case Event::VAMPIRES:
            if (gc.relics.has(RelicId::BLOOD_VIAL)) {
                pushOption(0);
            }
            pushOptions(1,2);
            break;

        case Event::WE_MEET_AGAIN:
            if (gc.info.potionIdx != -1) {
                pushOption(0);
            }
            if (gc.info.gold != -1) {
                pushOption(1);
            }
            if (gc.info.cardIdx != -1) {
                pushOption(2);
            }
            pushOption(3);
            break;

        case Event::WHEEL_OF_CHANGE:
            pushOption(0);
            break;

        case Event::WINDING_HALLS:
            pushOptions(0,3);
            break;

        case Event::WORLD_OF_GOOP:
            pushOptions(0,2);
            break;

        case Event::INVALID:
        case Event::MONSTER:
        case Event::REST:
        case Event::SHOP:
        case Event::TREASURE:
        default:
            assert(false);
            break;
    }

}


int RandomBattleStateHandler::setupState(const BattleContext &bc, bool withDesc) {
    if (bc.inputState == InputState::PLAYER_NORMAL) {
        return setupNormalOptions(bc);

    } else if (bc.inputState == InputState::CARD_SELECT) {
        return setupCardSelectOptions(bc);

    } else {
        assert(false);
        return 0;

    }
}

void RandomBattleStateHandler::chooseOption(BattleContext &bc, int option) const {
    if (bc.inputState == InputState::PLAYER_NORMAL) {
        optionFunctions[option](bc);

    } else if (bc.inputState == InputState::CARD_SELECT) {
        if (bc.cardSelectInfo.cardSelectTask == CardSelectTask::GAMBLE ||
            bc.cardSelectInfo.cardSelectTask == CardSelectTask::EXHAUST_MANY) {
            cardSelectFunction(bc, option);
        } else {
            cardSelectFunction(bc, validCardIdxs[option]);
        }


    } else {
        assert(false);
    }

    bc.inputState = InputState::EXECUTING_ACTIONS;
    bc.executeActions();
}



//std::string RandomBattleStateHandler::getDescForOption(BattleContext &bc, int option) {
//    if (bc.inputState == InputState::PLAYER_NORMAL) {
//        optionFunctions[option](bc);
//
//    } else if (bc.inputState == InputState::CARD_SELECT) {
//        cardSelectFunction(bc, validCardIdxs[option]);
//
//    } else {
//        assert(false);
//    }
//
//}


int RandomBattleStateHandler::setupNormalOptions(const BattleContext &bc) {
    optionFunctions.clear();

    optionFunctions.push_back([](BattleContext &bc) {
        bc.endTurn();
    });

    int foundPotions = 0;
    for (int i = 0; foundPotions < bc.potionCount && i < bc.potionCapacity; ++i) {
        if (bc.potions[i] != Potion::EMPTY_POTION_SLOT) {
            ++foundPotions;
            if (bc.potions[i] == Potion::FAIRY_POTION) { // todo you
                continue;
            }
            optionFunctions.push_back([=](BattleContext &bc) {
                bc.drinkPotion(i);
            });
            optionFunctions.push_back([=](BattleContext &bc) {
                bc.discardPotion(i);
            });
        }
    }

    if (!bc.isCardPlayAllowed()) {
        return optionFunctions.size();
    }

    for (int i = 0; i < bc.cards.cardsInHand; ++i) {
        const auto card = bc.cards.hand[i];
        if (!card.canUseOnAnyTarget(bc)) {
            continue;
        }

        if (card.requiresTarget()) {
            for (int mIdx = 0; mIdx < bc.monsters.monsterCount; ++mIdx) {
                if (bc.monsters.arr[mIdx].isTargetable()) {

                    optionFunctions.push_back( [=](BattleContext &bc) {
                        bc.addToBotCard( CardQueueItem(card, mIdx, bc.player.energy) );
                    });

                }
            }

        } else {

            optionFunctions.push_back( [=](BattleContext &bc) {
                bc.addToBotCard( CardQueueItem(card, 0, bc.player.energy) );
            });

        }
    }

    return optionFunctions.size();
}

typedef std::function <bool (const CardInstance &c)> CIPredicate;
typedef fixed_list<int, CardManager::MAX_GROUP_SIZE> IndexList;

template <typename ForwardIt>
void setupCardOptionsHelper(IndexList &idxs, const ForwardIt begin, const ForwardIt end, const CIPredicate &p= nullptr) {
    for (int i = 0; begin+i != end; ++i) {
        const auto &c = begin[i];
        if (!p || (p(c))) {
            idxs.push_back(i);
        }
    }
}


fixed_list<int,10> getIdxListFor(int bitsetOfElements) {
    // low to high ( lowest bit corresponds to 0th idx)
    fixed_list<int,10> idxList;
    int idx = 0;
    while (bitsetOfElements != 0) {
        if (bitsetOfElements & 0x1) {
            idxList.push_back(idx);
        }
        ++idx;
        bitsetOfElements >>= 1;
    }
    return idxList;
}



int RandomBattleStateHandler::setupCardSelectOptions(const BattleContext &bc) {
    validCardIdxs.clear();
    cardSelectFunction = nullptr;

    switch (bc.cardSelectInfo.cardSelectTask) {

        case CardSelectTask::ARMAMENTS:
            setupCardOptionsHelper( validCardIdxs, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand,
                                    [] (const CardInstance &c) { return c.canUpgrade(); });
            cardSelectFunction = [] (BattleContext &bc, int handIdx) {
                bc.chooseArmamentsCard(handIdx);
            };
            break;

        case CardSelectTask::CODEX:
            validCardIdxs = {0,1,2,3};
            cardSelectFunction = [] (BattleContext &bc, int choice) {
                if (choice != 3) {
                    bc.chooseCodexCard(bc.cardSelectInfo.discovery_Cards()[choice]);
                }
            };
            break;

        case CardSelectTask::DISCOVERY:
            validCardIdxs = {0,1,2};
            cardSelectFunction = [] (BattleContext &bc, int choice) {
                bc.chooseDiscoveryCard(bc.cardSelectInfo.discovery_Cards()[choice]);
            };
            break;

        case CardSelectTask::DUAL_WIELD:
            setupCardOptionsHelper( validCardIdxs, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand,
                                    [] (const CardInstance &c) {
                                        return c.getType() == CardType::POWER || c.getType() == CardType::ATTACK;
                                    });
            cardSelectFunction = [] (BattleContext &bc, int handIdx) {
                bc.chooseDualWieldCard(handIdx);
            };
            break;

        case CardSelectTask::EXHUME:
            setupCardOptionsHelper(validCardIdxs, bc.cards.exhaustPile.begin(), bc.cards.exhaustPile.end());
            cardSelectFunction = [] (BattleContext &bc, int idx) {
                bc.chooseExhumeCard(idx);
            };
            break;

        case CardSelectTask::EXHAUST_MANY:
            // return number of unique sets of cards in hand. which is 2^n
            cardSelectFunction = [] (BattleContext &bc, int bitset) {
                auto idxList = getIdxListFor(bitset);
                bc.chooseExhaustCards(idxList);
            };
            return 1 << std::min(bc.cardSelectInfo.pickCount, bc.cards.cardsInHand);

        case CardSelectTask::EXHAUST_ONE:
            setupCardOptionsHelper(validCardIdxs, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand);
            cardSelectFunction = [] (BattleContext &bc, int idx) {
                bc.chooseExhaustOneCard(idx);
            };
            break;

        case CardSelectTask::FORETHOUGHT: {
            if (bc.cardSelectInfo.canPickAnyNumber) {
                // todo unsupported
                //assert(false);
                validCardIdxs.push_back(0);
                cardSelectFunction = [] (BattleContext &bc, int handIdx) {
                    // do nothing
                    bc.inputState = InputState::PLAYER_NORMAL;
                };

            } else {
                setupCardOptionsHelper(validCardIdxs, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand);
                cardSelectFunction = [] (BattleContext &bc, int handIdx) {
                    bc.chooseForethoughtCard(handIdx);
                };
            }
            break;
        }

        case CardSelectTask::GAMBLE:
            // return number of unique sets of cards in hand. which is 2^n
            cardSelectFunction = [] (BattleContext &bc, int bitset) {
                auto idxList = getIdxListFor(bitset);
                bc.chooseGambleCards(idxList);
            };
            return 1 << bc.cards.cardsInHand;

        case CardSelectTask::HEADBUTT:
            setupCardOptionsHelper(validCardIdxs, bc.cards.discardPile.begin(), bc.cards.discardPile.end());
            cardSelectFunction = [] (BattleContext &bc, int idx) {
                bc.chooseHeadbuttCard(idx);
            };
            break;

        case CardSelectTask::LIQUID_MEMORIES_POTION:
            setupCardOptionsHelper(validCardIdxs, bc.cards.discardPile.begin(), bc.cards.discardPile.end());
            cardSelectFunction = [] (BattleContext &bc, int idx) {
                bc.chooseDiscardToHandCard(idx, true);
            };
            break;

        case CardSelectTask::SECRET_TECHNIQUE:
            setupCardOptionsHelper( validCardIdxs, bc.cards.drawPile.begin(), bc.cards.drawPile.end(),
                                    [] (const CardInstance &c) {
                                        return c.getType() == CardType::SKILL;
                                    });
            cardSelectFunction = [] (BattleContext &bc, int idx) {
                bc.chooseDrawToHandCards(&idx, 1);
            };
            break;

        case CardSelectTask::SECRET_WEAPON:
            setupCardOptionsHelper( validCardIdxs, bc.cards.drawPile.begin(), bc.cards.drawPile.end(),
                                    [] (const CardInstance &c) {
                                        return c.getType() == CardType::ATTACK;
                                    });
            cardSelectFunction = [] (BattleContext &bc, int idx) {
                bc.chooseDrawToHandCards(&idx, 1);
            };
            break;

        case CardSelectTask::WARCRY:
            setupCardOptionsHelper( validCardIdxs, bc.cards.hand.begin(), bc.cards.hand.begin() + bc.cards.cardsInHand);
            cardSelectFunction = [] (BattleContext &bc, int idx) {
                bc.chooseWarcryCard(idx);
            };
            break;


        case CardSelectTask::HOLOGRAM:
        case CardSelectTask::MEDITATE:
        case CardSelectTask::NIGHTMARE:
        case CardSelectTask::RECYCLE:
        case CardSelectTask::SEEK:
        case CardSelectTask::SETUP:
        default:
#ifdef sts_asserts
            // todo unsupported
            std::cerr << "unspported card select task: " << static_cast<int>(bc.cardSelectInfo.cardSelectTask) << std::endl;
            assert(false);
#endif
            break;
    }

    return validCardIdxs.size();
}



























