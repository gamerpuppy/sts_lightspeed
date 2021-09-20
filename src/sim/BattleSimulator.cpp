//
// Created by gamerpuppy on 7/20/2021.
//

#include "sim/BattleSimulator.h"
#include "game/GameContext.h"
#include "combat/BattleContext.h"
#include "sim/ConsoleSimulator.h"
#include "sim/PrintHelpers.h"
#include "sim/SimHelpers.h"

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
        os << "<< " << line << "\n";
    }

    if (line.length() >= 4 && (line.substr(0, 4) == "quit" || line.substr(0, 4) == "stop")) {
        c.quitCommandGiven = true;
        return;
    }

    if (line.length() >= 6 && line.substr(0, 6) == "printLogs ") {
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

    for (int i = 0; i < bc->cards.cardsInHand; ++i) {
        const auto card = bc->cards.hand[i];

        if (!card.canUseOnAnyTarget(*bc)) {
            continue;
        }

        os << i << ": " << card;
        if (card.requiresTarget()) {

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

        assert(cardIdx >= 0 && cardIdx < bc->cards.cardsInHand);
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

        case CardSelectTask::CODEX:
            os << "Codex Action: Choose a card to shuffle int your draw pile.\n";
            printCardOptionsHelper(os, bc->cardSelectInfo.codexCards().begin(), bc->cardSelectInfo.codexCards().end());
            os << "3: skip\n";
            break;


        case CardSelectTask::DISCOVERY:
            os << "Discovery Action: Choose a card to create " << bc->cardSelectInfo.discovery_CopyCount() << " copies of in your hand.\n";
            printCardOptionsHelper(os, bc->cardSelectInfo.discovery_Cards().begin(), bc->cardSelectInfo.discovery_Cards().end());
            break;

        case CardSelectTask::DUAL_WIELD:
            os << "Dual Wield: Choose a card to create " << bc->cardSelectInfo.discovery_CopyCount() << " copies of in your hand.\n";
            printCardOptionsHelper(os, bc->cards.hand.begin(), bc->cards.hand.begin() + bc->cards.cardsInHand,
               [] (const CardInstance &c) {
                    return c.getType() == CardType::POWER || c.getType() == CardType::ATTACK;
                }
            );
            break;

        case CardSelectTask::EXHAUST_MANY:
            os << "Choose cards to exhaust in your hand. (use descending order separated by spaces)\n";
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
            os << "Gamble Action: Choose cards to discard from your hand and redraw that many. (will be sorted to descending order)\n";
            printCardOptionsHelper(os, bc->cards.hand.begin(), bc->cards.hand.begin() + bc->cards.cardsInHand);
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

fixed_list<int,10> getIdxListFromString(const std::string &action) {
    fixed_list<int,10> ret;
    if (action.empty() || action == "none") {
        return ret;
    }

    std::istringstream iss(action);
    int idx;
    while (true) {
        iss >> idx;
        if (iss.fail()) {
            break;
        }
        assert(idx >= 0 && idx < 10 && ret.size() != 10);

        ret.push_back(idx);
    }

    return ret;
}

void BattleSimulator::takeCardSelectAction(const std::string &action) {
    switch (bc->cardSelectInfo.cardSelectTask) {
        case CardSelectTask::ARMAMENTS:
            bc->chooseArmamentsCard(std::stoi(action));
            break;

        case CardSelectTask::CODEX: {
            const auto chooseIdx = std::stoi(action);
            assert(chooseIdx >= 0 && chooseIdx < 4);
            if (chooseIdx != 3) {
                bc->chooseCodexCard(bc->cardSelectInfo.codexCards()[chooseIdx]);
            }
            break;
        }

        case CardSelectTask::DISCOVERY:
            bc->chooseDiscoveryCard(bc->cardSelectInfo.discovery_Cards()[std::stoi(action)]);
            break;

        case CardSelectTask::DUAL_WIELD:
            bc->chooseDualWieldCard(std::stoi(action));
            break;

        case CardSelectTask::EXHAUST_MANY:
            bc->chooseExhaustCards(getIdxListFromString(action));
            break;

        case CardSelectTask::EXHAUST_ONE:
            bc->chooseExhaustOneCard(std::stoi(action));
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
            bc->chooseGambleCards(getIdxListFromString(action));
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

        case CardSelectTask::WARCRY:
            bc->chooseWarcryCard(std::stoi(action));
            break;
    }
}


bool cardAssert(std::istringstream &iss, const CardInstance &c) {
    std::string attribute;
    iss >> attribute;

    if (attribute == "id") {
        std::string str;
        iss >> str;
        auto id = SimHelpers::getCardIdForString(str);
        return c.id == id;
    }

    if (attribute == "upgraded") {
        int value;
        iss >> value;
        return c.getUpgradeCount() == value;
    }

    if (attribute == "cost") {
        int value;
        iss >> value;
        return c.cost == value;
    }

    if (attribute == "costForTurn") {
        int value;
        iss >> value;
        return c.costForTurn == value;
    }

    return false;
}


bool monsterAssert(std::istringstream &iss, const Monster &m) {
    std::string attribute;
    iss >> attribute;

    if (attribute == "block") {
        int value;
        iss >> value;
        return m.block == value;
    }

    if (attribute == "hp" || attribute == "curHp") {
        int value;
        iss >> value;
        return m.curHp == value;
    }

    if (attribute == "maxHp") {
        int value;
        iss >> value;
        return m.maxHp == value;
    }

    if (attribute == "maxHp") {
        int value;
        iss >> value;
        return m.maxHp == value;
    }

    if (attribute == "id") {
        std::string str;
        iss >> str;
        return m.id == SimHelpers::getMonsterIdForString(str);
    }

    if (attribute == "hasStatus") {
        std::string str;
        iss >> str;
        auto status = SimHelpers::getMonsterStatusForString(str);
        return m.hasStatusInternal(status);
    }

    if (attribute == "getStatus") {
        std::string str;
        int value;
        iss >> str;
        iss >> value;
        auto status = SimHelpers::getMonsterStatusForString(str);
        return m.getStatusInternal(status) == value;
    }

    if (attribute == "miscInfo" || attribute == "misc") {
        int value;
        iss >> value;
        return m.miscInfo == value;
    }

    if (attribute == "move" || attribute == "nextMove" || attribute == "lastMove") {
        int idx = (attribute == "move" || attribute == "nextMove") ? 0 : 1;
        std::string str;
        iss >> str;
        auto move = SimHelpers::getMonsterMoveForString(str);
        return m.moveHistory[idx] == move;
    }

    return false;
}

bool BattleSimulator::doAssertCommand(const std::string &cmd) {
    std::istringstream iss(cmd);
    std::string tok;
    iss >> tok;

    if (tok == "hand") {
        std::string tok2;
        iss >> tok2;

        if (tok2 == "size") {
            int value;
            iss >> value;
            return value == bc->cards.cardsInHand;
        }

        int cardIdx = std::stoi(tok2);
        assert(cardIdx >= 0 && cardIdx < 10);
        return cardAssert(iss, bc->cards.hand[cardIdx]);
    }

    if (tok == "discard" || tok == "discardPile") {
        std::string tok2;
        iss >> tok2;

        if (tok2 == "size") {
            int value;
            iss >> value;
            return value == bc->cards.discardPile.size();
        }

        int cardIdx = std::stoi(tok2);
        assert(cardIdx >= 0 && cardIdx < bc->cards.discardPile.size());
        return cardAssert(iss, bc->cards.discardPile[cardIdx]);
    }

    if (tok == "draw" || tok == "drawPile") {
        std::string tok2;
        iss >> tok2;

        if (tok2 == "size") {
            int value;
            iss >> value;
            return value == bc->cards.drawPile.size();
        }

        int cardIdx = std::stoi(tok2);
        assert(cardIdx >= 0 && cardIdx < bc->cards.drawPile.size());
        return cardAssert(iss, bc->cards.drawPile[cardIdx]);
    }

    if (tok == "exhaust" || tok == "exhaustPile") {
        std::string tok2;
        iss >> tok2;

        if (tok2 == "size") {
            int value;
            iss >> value;
            return value == bc->cards.exhaustPile.size();
        }

        int cardIdx = std::stoi(tok2);
        assert(cardIdx >= 0 && cardIdx < bc->cards.exhaustPile.size());
        return cardAssert(iss, bc->cards.exhaustPile[cardIdx]);
    }


    if (tok == "player") {
        std::string tok2;
        iss >> tok2;

        if (tok2 == "block") {
            int value;
            iss >> value;
            return value == bc->player.block;
        }

        if (tok2 == "energy") {
            int value;
            iss >> value;
            return value == bc->player.energy;
        }

        if (tok2 == "hp" || tok2 == "curHp") {
            int value;
            iss >> value;
            return value == bc->player.curHp;
        }

        if (tok2 == "maxHp") {
            int value;
            iss >> value;
            return value == bc->player.maxHp;
        }

        if (tok2 == "gold") {
            int value;
            iss >> value;
            return value == bc->player.gold;
        }

        if (tok2 == "hasStatus") {
            std::string str;
            iss >> str;
            const auto status = SimHelpers::getPlayerStatusForString(str);
            return bc->player.hasStatusRuntime(status);
        }

        if (tok2 == "getStatus") {
            std::string str;
            int value;
            iss >> str;
            iss >> value;
            const auto status = SimHelpers::getPlayerStatusForString(str);
            return value == bc->player.getStatusRuntime(status);
        }


    }

    // monster idx -> tok2
    if (tok == "monsters" || tok == "monster") {
        std::string tok2;
        iss >> tok2;

        if (tok2 == "alive") {
            int value;
            iss >> value;
            return value == bc->monsters.monstersAlive;
        }

        if (tok2 == "count" || tok2 == "size") {
            int value;
            iss >> value;
            return value == bc->monsters.monsterCount;
        }

        int mIdx = std::stoi(tok2);
        assert(mIdx >= 0 && mIdx < 5);
        const auto &m = bc->monsters.arr[mIdx];
        return monsterAssert(iss, m);
    }

    return false; // unrecognized asserts default to failing
}
