//
// Created by gamerpuppy on 7/20/2021.
//

#include "sim/BattleSimulator.h"
#include "game/GameContext.h"
#include "combat/BattleContext.h"
#include "sim/ConsoleSimulator.h"
#include "sim/PrintHelpers.h"

#include <regex>
#include <sstream>

using namespace sts;

BattleSimulator::BattleSimulator() {
    bc = new BattleContext();
}

BattleSimulator::~BattleSimulator() {
    delete bc;
}

bool BattleSimulator::isInitialized() const {
    return initialized;
}

bool BattleSimulator::isBattleComplete() const {
    return bc->outcome != Outcome::UNDECIDED;
}

void BattleSimulator::initBattle(const GameContext &gc) {
    initialized = true;
    *bc = BattleContext(); // todo this hacky and slow
    bc->init(gc);
}

void BattleSimulator::exitBattle(GameContext &gc) {
    bc->exitBattle(gc);
}

void BattleSimulator::handleInputLine(const std::string &line, std::ostream &os, SimulatorContext &c) {

#ifdef sts_asserts
    assert(initialized);
#endif

    if (line.empty()
        || (line.size() >= 2 && line.substr(0,2) == "//")
        || line[0] == '#') {
        c.tookAction = false;
        return;
    }

    if (c.printInput) {
        os << "<< " << line << "\n\n";
    }

    if (line.length() >= 4 && (line.substr(0, 4) == "quit" || line.substr(0, 4) == "stop")) {
        c.quitCommandGiven = true;
        return;
    }

    if (line.length() >= 6 && line.substr(0, 6) == "print ") {
        if (c.printLogActions) {
            doPrintCommand(os, line.substr(6));
        }

    } else if (line.length() >= 4 && line.substr(0, 4) == "set ") {
        doSetCommand(line.substr(4));

    } else if (line.length() >= 7 && line.substr(0, 7) == "assert ") {
        if (!c.skipTests) {
            const bool result = doAssertCommand(line.substr(7));
            if (!result) {
                c.failedTest = true;
            }
        }

    } else {
        c.tookAction = true;
        takeAction(line);
    }
}

void BattleSimulator::printActions(std::ostream &os) const {
    std::cout << "\n" << *bc << "\n" << "\n";

    switch (bc->inputState) {
        case InputState::PLAYER_NORMAL:
            printPotionActions(os);
            printNormalActions(os);
            break;

        case InputState::CARD_SELECT:
            printCardSelectActions(os);
            break;

        default:
            os << "input state not handled\n";
            break;
    }

    os.flush();
}

void BattleSimulator::takeAction(const std::string &action) {
    switch (bc->inputState) {
        case InputState::PLAYER_NORMAL:
            if (action.length() >= 6 && action.substr(0,6) == "drink " ||
                action.length() >= 8 && action.substr(0,8) == "discard ") {

                takePotionAction(action);
            } else {
                takeNormalAction(action);
            }
            break;

        case InputState::CARD_SELECT:
            takeCardSelectAction(action);
            break;


//        case InputState::CHOOSE_DISCOVERY_CARD:
//            takeDiscoveryAction(action);
//            break;
//
//        case InputState::CHOOSE_DISCARD_TO_HAND_CARDS:
//            takeDiscardToHandAction(action);
//            break;
//
//        case InputState::CHOOSE_CARDS_IN_HAND:
//            takeChooseCardsInHandAction(action);
//            break;
//
//        case InputState::CHOOSE_HEADBUTT_CARD: {
//            int discardIdx = std::stoi(action);
//            bc->chooseHeadbuttCard(discardIdx);
//            break;
//        }
//
//        case InputState::CHOOSE_WARCRY_CARD: {
//            int handIdx = std::stoi(action);
//            bc->chooseWarcryCard(handIdx);
//            break;
//        }

        default:
            break;
    }
    bc->inputState = InputState::EXECUTING_ACTIONS;
    bc->executeActions();
}


void BattleSimulator::doPrintCommand(std::ostream &os, const std::string &cmd) {
    if (cmd == "rng") {
        printBattleContextRng(os, *bc);
    }
}

void BattleSimulator::doSetCommand(const std::string &cmd) {

}

bool BattleSimulator::doAssertCommand(const std::string &cmd) {
    std::istringstream iss(cmd);
    std::string variable;
    iss >> variable;

    if (variable == "player.curHp") {
        int value;
        iss >> value;
        return value == bc->player.curHp;
    }

    if (variable == "player.maxHp") {
        int value;
        iss >> value;
        return value == bc->player.maxHp;
    }

    if (variable == "hand.size") {
        int value;
        iss >> value;
        return value == bc->cards.cardsInHand;
    }

    if (variable == "monsters.aliveCount") {
        int value;
        iss >> value;
        return value == bc->monsters.monstersAlive;
    }

    return false; // unrecognized asserts default to failing
}

void BattleSimulator::printPotionActions(std::ostream &os) const {
    for (int i = 0; i < bc->potionCapacity; ++i) {
        const auto p = bc->potions[i];
        if (p == Potion::EMPTY_POTION_SLOT ||
            p == Potion::FAIRY_POTION) {
            continue;
        }

        os << "drink/discard " << i << " (" << getPotionName(p) << ")";
        if (potionRequiresTarget(p)) {
            for (int mIdx = 0; mIdx < bc->monsters.monsterCount; ++mIdx) {
                if (bc->monsters.arr[mIdx].isTargetable()) {
                    os << ", " << mIdx;
                }
            }
        }
        os << '\n';
    }
}

void BattleSimulator::takePotionAction(const std::string &action) {
    std::istringstream ss(action);

    std::string command;
    int potionIdx;

    ss >> command;
    ss >> potionIdx;

    if (command == "drink") {
        // drink potion command

        const auto p = bc->potions[potionIdx];
        if(potionRequiresTarget(p)) {
            int target = 0;
            ss >> target;
            bc->drinkPotion(potionIdx, target);

        } else {
            bc->drinkPotion(potionIdx);
        }

    } else if (command == "discard") {
        bc->discardPotion(potionIdx);

    }
}

void BattleSimulator::printNormalActions(std::ostream &os) const {

    if (!bc->isCardPlayAllowed()) { // for velvet choker, normality
        os << "end: end turn\n";
        return;
    }

    const int targetableCount = bc->monsters.getTargetableCount();

    for (int i = 0; i < bc->cards.cardsInHand; ++i) {
        const auto card = bc->cards.hand[i];
        if (!card.canUse(*bc)) {
            continue;
        }

        os << i << ": " << card.getName();
        if (card.requiresTarget() && targetableCount > 1) {
            os << ", ";
            for (int mIdx = 0; mIdx < bc->monsters.monsterCount; ++mIdx) {
                if (bc->monsters.arr[mIdx].isTargetable()) {
                    os << " " << mIdx;
                }
            }
        }
        os << "\n";
    }

    os << "end: end turn\n";
}

void BattleSimulator::takeNormalAction(const std::string &action) {

    if (std::regex_match(action, std::regex("^end.*"))) {
        bc->endTurn();
        return;
    }

    std::istringstream ss(action);
    if (std::regex_match(action, std::regex("^\\d+.*"))) {
        // play card command
        int cardIdx;
        ss >> cardIdx;

        const auto &card = bc->cards.hand[cardIdx];

        int targetIdx = -1;
        if (card.requiresTarget()) {
            const auto targetCount = bc->monsters.getTargetableCount();
            if (targetCount == 1) {
                for (int i = 0; i < 5; ++i) {
                    if (bc->monsters.arr[i].isTargetable()) {
                        targetIdx = i;
                        break;
                    }
                }
            } else if (targetCount > 1) {
                ss >> targetIdx;
            }
        } else {
            targetIdx = 0;
        }

        assert(targetIdx != -1);
        bc->addToBotCard( CardQueueItem(card, targetIdx, bc->player.energy) );


    } else {
        assert(false);
    }
}

typedef std::function <bool (const CardInstance &c)> CIPredicate;

template <typename ForwardIt>
void printCardOptionsHelper(std::ostream &os, const ForwardIt begin, const ForwardIt end, const CIPredicate &p= nullptr) {
    for (int i = 0; begin+i != end; ++i) {
        const auto &c = begin[i];
        if (!p || (p(c))) {
            os << i << ": ";
            printCardInstanceSimple(os, c) << '\n';
        }
    }
}

void BattleSimulator::printCardSelectActions(std::ostream &os) const {
    switch (bc->cardSelectInfo.cardSelectTask) {

        case CardSelectTask::ARMAMENTS:
            os << "Armaments Action: Choose a card to upgrade in your hand.\n";
            printCardOptionsHelper(os, bc->cards.hand.begin(), bc->cards.hand.begin() + bc->cards.cardsInHand,
                [] (const CardInstance &c) { return c.canUpgrade(); }
            );
            break;

        case CardSelectTask::DISCOVERY:
            os << "Discovery Action: Choose a card to create " << bc->cardSelectInfo.discovery_CopyCount() << " copies of in your hand.\n";
            for (int i = 0; i < 3; ++i) {
                os << i << ": " << getCardName(bc->cardSelectInfo.discovery_Cards()[i]) << '\n';
            }
            break;

        case CardSelectTask::DUAL_WIELD:
            os << "Dual Wield: Choose a card to create " << bc->cardSelectInfo.discovery_CopyCount() << " copies of in your hand.\n";
            printCardOptionsHelper(os, bc->cards.hand.begin(), bc->cards.hand.begin() + bc->cards.cardsInHand,
               [] (const CardInstance &c) {
                    return c.getType() == CardType::POWER || c.getType() == CardType::ATTACK;
                }
            );
            break;

        case CardSelectTask::ELIXIR_POTION:
            // todo unsupported
            assert(false);
            os << "Elixir Potion: Choose cards to exhaust in your hand.\n";
            printCardOptionsHelper(os, bc->cards.hand.begin(), bc->cards.hand.begin() + bc->cards.cardsInHand);
            break;

        case CardSelectTask::EXHAUST_ONE:
            os << "Choose a card to exhaust in your hand.\n";
            printCardOptionsHelper(os, bc->cards.hand.begin(), bc->cards.hand.begin() + bc->cards.cardsInHand);
            break;

        case CardSelectTask::EXHUME:
            os << "Exhume: Choose a card from the exhaust pile to return to your hand.\n";
            printCardOptionsHelper(os, bc->cards.exhaustPile.begin(), bc->cards.exhaustPile.end());
            break;

        case CardSelectTask::FORETHOUGHT: {
            if (bc->cardSelectInfo.canPickAnyNumber) {
                // todo unsupported
                assert(false);
                os << "Forethought: Choose a card to put on bottom of your draw pile.\n";
            } else {
                os << "Forethought: Choose card to put on bottom of your draw pile.\n";
            }
            printCardOptionsHelper(os, bc->cards.hand.begin(), bc->cards.hand.begin() + bc->cards.cardsInHand);
            break;
        }

        case CardSelectTask::GAMBLE:
            os << "Gamble Action: Choose cards to discard from your hand and redraw that many.\n";
            break;

        case CardSelectTask::HEADBUTT:
            os << "Headbutt Action: Choose a card from your discard pile to put on top of your draw pile.\n";
            printCardOptionsHelper(os, bc->cards.discardPile.begin(), bc->cards.discardPile.end());
            break;

        case CardSelectTask::HOLOGRAM:
        case CardSelectTask::LIQUID_MEMORIES_POTION:
        case CardSelectTask::MEDITATE:
            os << "DiscardToHandAction: Choose " << bc->cardSelectInfo.pickCount << " cards to put back in your hand.\n";
            printCardOptionsHelper(os, bc->cards.discardPile.begin(), bc->cards.discardPile.end());
            break;

        case CardSelectTask::NIGHTMARE:
            break;

        case CardSelectTask::SECRET_TECHNIQUE:
            os << "Secret Technique: Choose a card to put in your hand.\n";
            printCardOptionsHelper(os, bc->cards.drawPile.begin(), bc->cards.drawPile.end(),
                [] (const CardInstance &c) {
                    return c.getType() == CardType::SKILL;
                }
            );
            break;

        case CardSelectTask::SECRET_WEAPON:
            os << "Secret Weapon: Choose a card to put in your hand.\n";
            printCardOptionsHelper(os, bc->cards.drawPile.begin(), bc->cards.drawPile.end(),
                [] (const CardInstance &c) {
                   return c.getType() == CardType::ATTACK;
                }
            );
            break;

        case CardSelectTask::SEEK:
            break;


        case CardSelectTask::SETUP:
            break;

        case CardSelectTask::WARCRY:
            os << "Choose a card from your hand to put on top of your draw pile.\n";
            printCardOptionsHelper(os, bc->cards.hand.begin(), bc->cards.hand.begin() + bc->cards.cardsInHand);
            break;

        case CardSelectTask::RECYCLE:
            break;


        default:
#ifdef sts_asserts
            assert(false);
#endif
            break;
    }

}

void BattleSimulator::takeCardSelectAction(const std::string &action) {
    switch (bc->cardSelectInfo.cardSelectTask) {
        case CardSelectTask::ARMAMENTS:
            bc->chooseArmamentsCard(std::stoi(action));
            break;

        case CardSelectTask::DISCOVERY:
            bc->chooseDiscoveryCard(std::stoi(action));
            break;

        case CardSelectTask::DUAL_WIELD:
            bc->chooseDualWieldCard(std::stoi(action));
            break;

        case CardSelectTask::ELIXIR_POTION:
            break;

        case CardSelectTask::EXHUME:
            bc->chooseExhumeCard(std::stoi(action));
            break;

        case CardSelectTask::FORETHOUGHT:
            if (bc->cardSelectInfo.canPickAnyNumber) {

            } else {
                bc->chooseForethoughtCard(std::stoi(action));
            }
            break;

        case CardSelectTask::GAMBLE:
            break;

        case CardSelectTask::HEADBUTT:
            bc->chooseHeadbuttCard(std::stoi(action));
            break;

        case CardSelectTask::HOLOGRAM:
            bc->chooseDiscardToHandCard(std::stoi(action), false);
            break;

        case CardSelectTask::LIQUID_MEMORIES_POTION:
            bc->chooseDiscardToHandCard(std::stoi(action), true);
            break;

        case CardSelectTask::MEDITATE:  // todo
            break;

        case CardSelectTask::NIGHTMARE:  // todo
            break;

        case CardSelectTask::RECYCLE: // todo
            break;

        case CardSelectTask::SECRET_TECHNIQUE:
        case CardSelectTask::SECRET_WEAPON: {
            const int drawIdx = std::stoi(action);
            bc->chooseDrawToHandCards(&drawIdx, 1);
            break;
        }

        case CardSelectTask::SEEK:  // todo
            break;

        case CardSelectTask::SETUP:  // todo
            break;

        case CardSelectTask::EXHAUST_ONE:
            bc->chooseExhaustOneCard(std::stoi(action));
            break;

        case CardSelectTask::WARCRY:
            bc->chooseWarcryCard(std::stoi(action));
            break;

    }
}

//void BattleSimulator::printDiscoveryActions(std::ostream &os) const {
//    os << "DiscoveryAction: Choose a card to create " << bc->cardSelectInfo.data0 << " copies of in your hand.\n";
//    for (int i = 0; i < 3; ++i) {
//        os << i << ": " << getCardName(bc->cardSelectInfo.discovery_Cards()[i]) << "\n";
//    }
//}
//
//void BattleSimulator::takeDiscoveryAction(const std::string &action) {
//    const int idx = std::stoi(action);
//    bc->chooseDiscoveryCard(idx);
//}
//
//void BattleSimulator::printDiscardToHandActions(std::ostream &os) const {
//        << (bc->cardSelectInfo.discardToHand_ForZeroCost() ? " for zero cost.\n" : ".\n");
//
//    for (int i = 0; i < bc->cards.discardPile.size(); ++i) {
//        os << i << ": " << bc->cards.discardPile[i].getName() << "\n";
//    }
//}
//
//void BattleSimulator::takeChooseCardsInHandAction(const std::string &action) const {
//    std::istringstream iss(action);
//
//    int chooseCount;
//    if (bc->cardSelectInfo.cardSelect_MaxCount()) {
//        iss >> chooseCount;
//    } else {
//        chooseCount = bc->cardSelectInfo.cardSelect_MinCount();
//    }
//
//    int chooseIdxs[10];
//    for (int i = 0; i < chooseCount; ++i) {
//        iss >> chooseIdxs[i];
//    }
//
//    bc->chooseHandSelectCards(chooseIdxs, chooseCount);
//}
//
