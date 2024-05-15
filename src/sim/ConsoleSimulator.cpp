//
// Created by gamerpuppy on 6/25/2021.
//

#include <cassert>
#include <cmath>
#include <sstream>

#include "game/Game.h"

#include "sim/ConsoleSimulator.h"
#include "sim/PrintHelpers.h"
#include "sim/BattleSimulator.h"

using namespace sts;

CharacterClass getCharacterClassFromString(const std::string &s) {
    if (s[0] == 'i' || s[0] == 'I') {
        return CharacterClass::IRONCLAD;

    } else if (s[0] == 's' || s[0] == 'S') {
        return CharacterClass::SILENT;

    } else if (s[0] == 'd' || s[0] == 'D') {
        return CharacterClass::DEFECT;

    } else if (s[0] == 'w' || s[0] == 'W') {
        return CharacterClass::WATCHER;

    } else {
        return CharacterClass::INVALID;
    }
}

void ConsoleSimulator::setupGame(std::uint64_t seed, CharacterClass c, int ascension) {
    delete gc;
    gc = new GameContext(c, seed, ascension);
}

void ConsoleSimulator::setupGameFromSaveFile(const SaveFile &save) {
    delete gc;
    gc = new GameContext();
    gc->initFromSave(save);
    if (gc->screenState == ScreenState::BATTLE) {
        battleSim.initBattle(*gc);
    }
//    std::cout << *gc << '\n';
}


void ConsoleSimulator::reset() {
    delete gc;
}

void ConsoleSimulator::play(std::istream &is, std::ostream &os, SimulatorContext &c) {
    if (gc == nullptr) {
        ++c.line;
        std::string seedStr;
        std::string character;
        int ascensionLevel;

        is >> seedStr;
        is >> character;
        is >> ascensionLevel;

        if (c.printInput) {
            std::cout << seedStr << " " << character << " " << ascensionLevel << "\n";
        }
        setupGame(SeedHelper::getLong(seedStr), getCharacterClassFromString(character), ascensionLevel);
    }

    bool firstLine = true;

    while (!c.quitCommandGiven && !(c.failedTest && c.quitOnTestFailed) && gc->outcome == GameOutcome::UNDECIDED && !is.eof()) {
        if (c.printPrompts && (c.printFirstLine || !firstLine)) {
            printActions(os);
        }
        firstLine = false;

        getInputLoop(is, os, c);
    }
}

void ConsoleSimulator::getInputLoop(std::istream &is, std::ostream &os, SimulatorContext &c) {
    std::string line;
    c.tookAction = false;
    while (!c.quitCommandGiven && !(c.failedTest && c.quitOnTestFailed) && !c.tookAction && std::getline(is, line)) {
        ++c.line;
        c.tookAction = false;
        handleInputLine(line, os, c);
    }
    if (c.failedTest && c.quitOnTestFailed) {
        os << "FAILED TEST line(" << c.line << "): \"" << line << "\"" << std::endl;
    }
}

void ConsoleSimulator::handleInputLine(const std::string &line, std::ostream &os, SimulatorContext &c) {

    if (gc->screenState == ScreenState::BATTLE) {
        battleSim.handleInputLine(line, os, c);
        if (c.quitCommandGiven) {
            return;
        }
        if (battleSim.isBattleComplete()) {
            battleSim.exitBattle(*gc);
            battleSim.initialized = false;
            if (gc->screenState == ScreenState::BATTLE) {
                battleSim.initBattle(*gc);
            }
        }
        return;
    }

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

    } else if (line.length() >= 6 && line.substr(0, 6) == "print ") {
        if (c.printLogActions) {
            c.tookAction = true;
            doPrintCommand(os, line.substr(6));
        }

    } else if (line.length() >= 4 && line.substr(0, 4) == "set ") {
        c.tookAction = true;
        doSetCommand(line.substr(4));

    } else {
        c.tookAction = true;
        takeAction(line);
    }
}

void ConsoleSimulator::doPrintCommand(std::ostream &os, const std::string &cmd) {
    std::istringstream ss(cmd);
    std::string command;
    ss >> command;
    if (command == "all") {
        os << *gc;

    } else if (command == "relics") {
        os << gc->relics << '\n';

    } else if (command == "deck" || command == "cards") {
        os << gc->deck << '\n';

    } else if (command == "rng") {
        printRngInfo(os, *gc); os << '\n';

    } else if (command == "potion" || command == "potions") {
        printPotionInfo(os, *gc); os << '\n';

    } else if (command == "gold") {
        os << gc->gold << "g\n";

    } else if (command == "hp" || command == "curHp" || command == "maxHp") {
        os << gc->curHp << "/" << gc->maxHp << "hp\n";
    }
    os.flush();
}

void ConsoleSimulator::doSetCommand(const std::string &cmd) {
    std::istringstream ss(cmd);
    std::string command;
    ss >> command;

    if (command == "curHp" || command == "hp") {
        int hp;
        ss >> hp;
        gc->curHp = hp;
    } else if (command == "maxHp") {
        int hp;
        ss >> hp;
        gc->maxHp = hp;
    } else if (command == "gold") {
        int gold;
        ss >> gold;
        gc->gold = gold;
    }
}

void ConsoleSimulator::printActions(std::ostream &os) const {
    switch (gc->screenState) {

        case ScreenState::BATTLE:
            battleSim.printActions(os);
            return;

        case ScreenState::BOSS_RELIC_REWARDS:
            printBossRelicRewardsActions(os);
            break;

        case ScreenState::EVENT_SCREEN: {
            printEventActions(os);
            break;
        }

        case ScreenState::REWARDS: {
            printRewardsScreenActions(os);
            break;
        }

        case ScreenState::REST_ROOM:
            printRestRoomActions(os);
            break;

        case ScreenState::SHOP_ROOM:
            printShopRoomActions(os);
            break;

        case ScreenState::TREASURE_ROOM: {
            printTreasureRoomActions(os);
            break;
        }

        case ScreenState::CARD_SELECT: {
            printCardSelectScreenActions(os);
            break;
        }

        case ScreenState::MAP_SCREEN: {
            printMapScreenActions(os);
            break;
        }

        case ScreenState::INVALID:
        default:
            os << "Invalid action state" << '\n';
    }

    os.flush();
}

void ConsoleSimulator::takeAction(const std::string &action) {

    if (action.length() >= 14 && action.substr(0, 12) == "drink potion") {
        int idx = action[13]-'0';
        gc->drinkPotionAtIdx(idx);
        return;

    } else if (action.length() >= 16 && action.substr(0,14) == "discard potion") {
        int idx = action[15]-'0';
        gc->discardPotionAtIdx(idx);
        return;
    }

    switch (gc->screenState) {

        case ScreenState::BOSS_RELIC_REWARDS:
            takeBossRelicRewardsAction(action);
            break;

        case ScreenState::EVENT_SCREEN:
            takeEventAction(action);
            break;

        case ScreenState::CARD_SELECT:
            takeCardSelectScreenAction(action);
            break;

        case ScreenState::SHOP_ROOM:
            takeShopRoomAction(action);
            break;

        case ScreenState::REST_ROOM:
            takeRestRoomAction(action);
            break;

        case ScreenState::TREASURE_ROOM:
            takeTreasureRoomAction(action);
            break;

        case ScreenState::REWARDS:
            takeRewardScreenAction(action);
            break;

        case ScreenState::MAP_SCREEN:
            takeMapScreenAction(action);
            break;

        case ScreenState::INVALID:
        default:
            assert(false);
    }

    if (gc->screenState == ScreenState::BATTLE && !battleSim.isInitialized()) {
        battleSim.initBattle(*gc);
    }

}

void ConsoleSimulator::printBossRelicRewardsActions(std::ostream &os) const {
    os << "Choose a Boss Relic Reward\n";
    for (int i = 0; i < 3; ++i) {
        os << i << ": Take " << getRelicName(gc->info.bossRelics[i]) << '\n';
    }
    os << "3: Skip Reward\n";
}

void ConsoleSimulator::takeBossRelicRewardsAction(const std::string &action) {
    int idx = std::stoi(action);
    gc->chooseBossRelic(idx);
}

void ConsoleSimulator::printRestRoomActions(std::ostream &os) const {
    os << "Rest Room: Select Action.\n";
    bool hasOption = false;

    if (!gc->relics.has(RelicId::COFFEE_DRIPPER)) {
        hasOption = true;
        os << "0: [Rest]\n";
    }

    if (!gc->relics.has(RelicId::FUSION_HAMMER) && gc->deck.getUpgradeableCount() > 0) {
        hasOption = true;
        os << "1: [Smith]\n";
    }

    if (!gc->hasKey(Key::RUBY_KEY)) {
        hasOption = true;
        os << "2: [Recall]\n";
    }

    if (gc->relics.has(RelicId::GIRYA) && gc->relics.getRelicValue(RelicId::GIRYA) != 3) {
        hasOption = true;
        os << "3: [Lift]\n";
    }

    if (gc->relics.has(RelicId::PEACE_PIPE)) { // assume we have card to remove
        hasOption = true;
        os << "4: [Toke]\n";
    }

    if (gc->relics.has(RelicId::SHOVEL)) { // assume we have card to remove
        hasOption = true;
        os << "5: [Dig]\n";
    }

    if (!hasOption) {
        os << "6: [Skip]\n";
    }

}

void ConsoleSimulator::takeRestRoomAction(const std::string &action) {
    int idx = std::stoi(action);
    gc->chooseCampfireOption(idx);
}

void ConsoleSimulator::printShopRoomActions(std::ostream &os) const {
    os << "Shop Room: Select Item to Purchase.\n";
    const auto s = gc->info.shop;

    for (int i = 0; i < 7; ++i) {
        auto price = s.cardPrice(i);
        if (price != -1 && gc->gold >= price) {
            os << "card " << i << ":  " << s.cards[i] << " " << price << "g\n";
        }
    }

    for (int i = 0; i < 3; ++i) {
        auto price = s.relicPrice(i);
        if (price != -1 && gc->gold >= price) {
            os << "relic " << i << ":  " << getRelicName(s.relics[i]) << " " << price << "g\n";
        }
    }

    for (int i = 0; i < 3; ++i) {
        auto price = s.potionPrice(i);
        if (price != -1 && gc->gold >= price) {
            os << "potion " << i << ": " << getPotionName(s.potions[i]) << " " << price << "g\n";
        }
    }

    if (s.removeCost != -1 && gc->gold >= s.removeCost) {
        os << "remove: Remove Card " << s.removeCost<< "g\n";
    }

    os << "proceed:\n";
}

void ConsoleSimulator::takeShopRoomAction(const std::string &action) {
    std::istringstream ss(action);
    std::string actionType;
    ss >> actionType;

    auto &shop = gc->info.shop;
    if (actionType == "proceed") {
        gc->screenState = ScreenState::MAP_SCREEN;
        return;
    } else if (actionType == "remove") {
        shop.buyCardRemove(*gc);
        return;
    }

    int purchaseIdx;
    ss >> purchaseIdx;

    if (actionType[0] == 'c') {
        shop.buyCard(*gc, purchaseIdx);

    } else if (actionType[0] == 'r') {
        shop.buyRelic(*gc, purchaseIdx);

    } else if (actionType[0] == 'p') {
        shop.buyPotion(*gc, purchaseIdx);
    }
}

void ConsoleSimulator::printTreasureRoomActions(std::ostream &os) const {
    os << "Treasure Room: Select Action.\n";
    os << "0: Open " << chestSizeNames[static_cast<int>(gc->info.chestSize)] << " chest.\n";
    os << "1: Skip Reward\n";
}

void ConsoleSimulator::takeTreasureRoomAction(const std::string &action) {
    const int idx = std::stoi(action);
    gc->chooseTreasureRoomOption(idx == 0);
}

void ConsoleSimulator::printMapScreenActions(std::ostream &os) const {
    os << "Map Screen: Select Next Map Node.\n";
    os << gc->map->toString(true) << '\n';

    if (gc->curMapNodeY == 14) {
        os << "0 : Advance to Boss" << '\n';
    } else if (gc->curMapNodeY == -1) {
        for (const auto firstRowNode : gc->map->nodes[0]) {
            if (firstRowNode.edgeCount > 0) {
                os << firstRowNode.x << ": " << roomStrings[static_cast<int>(firstRowNode.room)] << '\n';
            }
        }
    } else {
        os << "CurX: " << gc->curMapNodeX << " CurY: " << gc->curMapNodeY << '\n';

        auto node = gc->map->getNode(gc->curMapNodeX, gc->curMapNodeY);
        for (int i = 0; i < node.edgeCount; ++i) {
            const auto nextNodeX = node.edges[i];
            const auto &nextNode = gc->map->getNode(nextNodeX, node.y + 1);
            os << nextNode.x << ": " << roomStrings[static_cast<int>(nextNode.room)] << '\n';
        }
    }
}

void ConsoleSimulator::takeMapScreenAction(const std::string &action) {
    int x = std::stoi(action);
    gc->transitionToMapNode(x);
}

void ConsoleSimulator::printRewardsScreenActions(std::ostream &os) const {
    os << "Rewards: Select Reward to Take.\n";
    const auto &r = gc->info.rewardsContainer;

    for (int i = 0; i < r.goldRewardCount; ++i) {
        os << "gold " << i << ": take " << r.gold[i] << "g\n";
    }

    for (int i = 0; i < r.cardRewardCount; ++i) {
        for (int x = 0; x < r.cardRewards[i].size(); ++x) {
            os << "card " << i << " " << x << ": take " << r.cardRewards[i][x] << "\n";
        }
        if (gc->hasRelic(RelicId::SINGING_BOWL)) {
            os << "maxhp " << i << ": +2 max hp; lose card choice\n";
        }
    }

    for (int i = 0; i < r.relicCount; ++i) {
        os << "relic "<< i <<": " << getRelicName(r.relics[i]);
        if (r.sapphireKey && (i == r.relicCount-1)) {
            os << "; lose sapphire key\n";
        } else {
            os << '\n';
        }
    }

    for (int i = 0; i < r.potionCount; ++i) {
        os << "potion " << i << ": " << getPotionName(r.potions[i]) << '\n';
    }

    if (r.emeraldKey) {
        os << "emeraldKey: obtain emerald key\n";
    }

    if (r.sapphireKey) {
        os << "sapphireKey: obtain sapphire key";
        if (r.relicCount > 0) {
            auto loseRelic = r.relics[r.relicCount - 1];
            os << "; lose " << getRelicName(loseRelic);
        }
        os << '\n';

    }

    if (r.getTotalCount() > 0) {
        os << "skip: skip rewards\n";
    } else {
        os << "proceed:\n";
    }

}

void ConsoleSimulator::takeRewardScreenAction(const std::string &action) {
    std::istringstream ss(action);

    std::string takeType;
    int takeIdx;

    ss >> takeType;
    ss >> takeIdx;

    auto &r = gc->info.rewardsContainer;

    if (takeType == "gold") {
        gc->obtainGold(r.gold[takeIdx]);
        r.removeGoldReward(takeIdx);

    } else if (takeType == "card") {
        int takeCardIdx;
        ss >> takeCardIdx;

        gc->deck.obtain(*gc, r.cardRewards[takeIdx][takeCardIdx]);
        r.removeCardReward(takeIdx);
        
    } else if (takeType == "maxhp") {
        // singing bowl
        gc->playerIncreaseMaxHp(2);
        r.removeCardReward(takeIdx);
    } else if (takeType == "relic") {
        gc->obtainRelic(r.relics[takeIdx]);
        if (r.sapphireKey && takeIdx == r.relicCount-1) {
            r.sapphireKey = false;
        }
        r.removeRelicReward(takeIdx);

    } else if (takeType == "emeraldKey") {
        gc->obtainKey(Key::EMERALD_KEY);
        r.emeraldKey = false;

    } else if (takeType == "sapphireKey") {
        if (r.relicCount > 0) {
            r.removeRelicReward(r.relicCount-1);
        }
        gc->obtainKey(Key::SAPPHIRE_KEY);
        r.sapphireKey = false;

    } else if (takeType == "potion") {
        gc->obtainPotion(r.potions[takeIdx]);
        r.removePotionReward(takeIdx);

    } else if (takeType == "skip" || takeType == "proceed") {
        gc->regainControl();
    }
}

void ConsoleSimulator::printCardSelectScreenActions(std::ostream &os) const {
    os << "Select Card to ";
    switch (gc->info.selectScreenType) {

        case CardSelectScreenType::BOTTLE:
            os << "Bottle.";
            break;

        case CardSelectScreenType::TRANSFORM_UPGRADE:
            os << "Transform and Upgrade.";
            break;

        case CardSelectScreenType::TRANSFORM:
            os << "Transform.";
            break;

        case CardSelectScreenType::UPGRADE:
            os << "Upgrade.";
            break;

        case CardSelectScreenType::REMOVE:
            os << "Remove.";
            break;

        case CardSelectScreenType::OBTAIN:
            os << "Obtain.";
            break;

        case CardSelectScreenType::BONFIRE_SPIRITS:
            os << "Offer to the spirits.";
            break;

        default:
            assert(false);
            break;
    }
    os << " (" << gc->info.haveSelectedCards.size() << "/" << gc->info.toSelectCount << ")\n";

    for (int i = 0; i < gc->info.toSelectCards.size(); ++i) {
        os << i << ": " << gc->info.toSelectCards[i].card << "\n";
    }
}

void ConsoleSimulator::takeCardSelectScreenAction(const std::string &action) {
    int idx = std::stoi(action);
    gc->chooseSelectCardScreenOption(idx);
}

void ConsoleSimulator::printEventActions(std::ostream &os) const {
    os << "Event - " << eventGameNames[static_cast<int>(gc->curEvent)] << ": Select Action.\n";

    const bool unfavorable = gc->ascension >= 15;

    switch (gc->curEvent) {
        case Event::NEOW: {
            const auto &r = gc->info.neowRewards;
            os << "0: " << Neow::bonusStrings[static_cast<int>(r[0].r)] << '\n';
            os << "1: " << Neow::bonusStrings[static_cast<int>(r[1].r)] << '\t' << Neow::drawbackStrings[static_cast<int>(r[1].d)] << '\n';
            os << "2: " << Neow::bonusStrings[static_cast<int>(r[2].r)] << '\t' << Neow::drawbackStrings[static_cast<int>(r[2].d)] << '\n';
            os << "3: " << Neow::bonusStrings[static_cast<int>(r[3].r)] << '\t' << Neow::drawbackStrings[static_cast<int>(r[3].d)] << '\n';
            break;
        }

        case Event::OMINOUS_FORGE: { // Ominous Forge
            if (gc->deck.getUpgradeableCount() > 0) {
                os << "0: [Forge] Upgrade a card.\n";
            }
            os << "1: [Rummage] Obtain Warped Tongs, Become Cursed - Pain.\n";
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::PLEADING_VAGRANT: { // Pleading Vagrant
            if (gc->gold >= 85) {
                os << "0: [Give 85 Gold] Obtain a random Relic.\n";
            }
            os << "1: [Rob] Obtain a random Relic. Become Cursed - Shame.\n";
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::ANCIENT_WRITING:  { // Ancient Writing
            os << "0: [Elegance] Remove a card from your deck.\n";
            os << "1: [Simplicity] Upgrade all Strikes and Defends.\n";
            break;
        }

        case Event::OLD_BEGGAR: { // Old Beggar
            os << "0: [Offer Gold] Lose 75 Gold. Remove a card from your cards.\n";
            os << "1: [Leave] Nothing happens.\n";
            break;
        }

        case Event::BIG_FISH: {
            os << "0: [Banana] Heal " << gc->info.hpAmount0 << " HP.\n";
            os << "1: [Donut] Max HP +5.\n";
            os << "2: [Box] Receive a Relic. Become Cursed: Regret.\n";
            break;
        }

        // goes straight to card select screen
//        case Event::BONFIRE_SPIRITS: { // Bonfire Spirits
//            os << "0: [Offer] Choose a card in your cards and offer it to the spirits.\n";
//            break;
//        }

        case Event::COLOSSEUM: { // The Colosseum
            if (gc->info.eventData == 0) {
                os << "0: [Fight]\n";
            } else {
                os << "0: [COWARDICE] Escape.\n";
                os << "1: [VICTORY] A powerful fight with many rewards.\n";
            }
            break;
        }

        case Event::CURSED_TOME: {
            const int phase = gc->info.eventData;
            switch (phase) {
                case 0:
                    os << "0: [Read]\n";
                    os << "1: [Leave]\n";
                    break;

                case 1:
                case 2:
                case 3:
                    os << phase+1 << ": [Continue] Lose " << phase << "HP.\n";
                    break;

                case 4:
                    os << "5: [Take] Obtain the Book. Lose " << (unfavorable ? 15 : 10) << " HP\n";
                    os << "6: [Stop] Lose 3 HP.\n";
                    break;

                default:
                    assert(false);
                    break;
            }

//            switch (phase) {
//                case 0:
//                    os << "0: [Read]\n";
//                    os << "1: [Leave]\n";
//                    break;
//
//                case 1:
//                case 2:
//                case 3:
//                    os << "0: [Continue] Lose " << phase << "HP.\n";
//                    break;
//                case 4:
//                    os << "0: [Take] Obtain the Book. Lose " << (unfavorable ? 15 : 10) << " HP\n";
//                    os << "1: [Stop] Lose 3 HP.\n";
//                    break;
//
//                default:
//                    assert(false);
//                    break;
//            }
            break;
        }

        case Event::DEAD_ADVENTURER: {
            // If the player successfully [Search] three times without encountering an Elite, the event will end with a short dialogue detailing the success.
            os << "0: [Search] Find Loot. " << (gc->info.phase * 25 + (unfavorable ? 35 : 25)) << "% that an Elite will return to fight you.\n";
            os << "1: [Escape] End the search and resume your journey.\n";
            break;
        }

        case Event::DESIGNER_IN_SPIRE: { // Designer In-Spire
            const auto upgradeOne = gc->info.upgradeOne;
            const auto cleanUpIsRemoveCard = gc->info.cleanUpIsRemoveCard;

            int goldCost0 = unfavorable ? 50 : 40;
            if (gc->gold >= goldCost0 && gc->deck.getUpgradeableCount() > 0) {
                if (upgradeOne) {
                    os << "0: [Adjustments] Lose " << goldCost0 << " Gold. Upgrade a card.\n";
                } else {
                    os << "1: [Adjustments] Lose " << goldCost0 << " Gold. Upgrade 2 random cards.\n";
                }
            }

            int goldCost1 = unfavorable ? 75 : 60;
            if (gc->gold >= goldCost1) {
                if (cleanUpIsRemoveCard) {
                    if (gc->deck.getTransformableCount(1) >= 1) { // todo this could be wrong,
                        os << "2: [Clean Up] Lose " << goldCost1 << " Gold. Remove a card.\n";
                    }
                } else {
                    if (gc->deck.getTransformableCount(2) >= 2) { // todo the requirement for this could be too strict
                        os << "3: [Clean Up] Lose " << goldCost1 << " Gold. Transform 2 random cards.\n";
                    }
                }
            }

            int goldCost2 = unfavorable ? 110 : 90;
            if (gc->gold >= goldCost2) {
                if (gc->deck.getTransformableCount(1) >= 1) {
                    os << "4: [Full Service] Lose "<< goldCost2 << " Gold. Remove a card, then upgrade a random card.\n";
                }
            }
            int hpCost = unfavorable ? 5 : 3;
            os << "5: [Punch] Lose "<< hpCost << " HP.\n";
            break;
        }

        case Event::AUGMENTER: { // Augmenter
            os << "0: [Test J.A.X] Obtain a J.A.X.\n";
            if (gc->deck.getTransformableCount(2) > 1) {
                os << "1: [Become Test Subject] Choose and Transform 2 cards in your deck.\n";
            }
            os << "2: [Ingest Mutagens] Obtain Mutagenic Strength.\n";
            break;
        }

        case Event::DUPLICATOR: {
            os << "0: [Pray] Duplicate a card in your deck.\n";
            os << "1: [Leave] Nothing happens.\n";
            break;
        }

        case Event::FACE_TRADER: {
            os << "0: [Touch] Lose " << gc->info.hpAmount0 << " HP. Gain " << (unfavorable ? 50 : 75) << " Gold.\n";
            os << "1: [Trade] 50%: Good Face. 50%: Bad Face.\n";
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::FALLING: {
            if (gc->info.skillCardDeckIdx != -1) { // Skill
                os << "0: [Land] Lose " << gc->deck.cards[gc->info.skillCardDeckIdx] << ".\n";
            }
            if (gc->info.powerCardDeckIdx != -1) { // Power
                os << "1: [Channel] Lose " << gc->deck.cards[gc->info.powerCardDeckIdx] << ".\n";
            }
            if (gc->info.attackCardDeckIdx != -1) {
                os << "2: [Strike] Lose " << gc->deck.cards[gc->info.attackCardDeckIdx] << ".\n";
            }

            bool haveNoOptions =
                    gc->info.skillCardDeckIdx == -1 &&
                    gc->info.powerCardDeckIdx == -1 &&
                    gc->info.powerCardDeckIdx == -1;

            if (haveNoOptions) {
                os << "3: [Splat] Lose nothing?\n";
            }

            break;
        }


        case Event::FORGOTTEN_ALTAR: {
            if (gc->relics.has(RelicId::GOLDEN_IDOL)) {
                os << "0: [Offer: Golden Idol] Obtain Bloody Idol. Lose Golden Idol.\n";
            }
            os << "1: [Sacrifice] Gain 5 Max HP. Lose " << gc->info.hpAmount0 << " HP.\n";
            os << "2: [Desecrate] Become Cursed - Decay.\n";
            break;
        }

        case Event::THE_DIVINE_FOUNTAIN: { // The Divine Fountain
            os << "0: [Drink] Remove all Curses from your deck.\n";
            os << "1: [Leave] Nothing happens.\n";
            break;
        }

        case Event::GHOSTS: { // Council of Ghosts
            os << "0: [Accept] Receive " << (unfavorable ? 3 : 5) << " Apparition. Lose " << gc->info.hpAmount0 << " Max HP.\n";
            os << "1: [Refuse] Nothing happens.\n";
            break;
        }

        case Event::GOLDEN_IDOL: {
            if (!gc->relics.has(RelicId::GOLDEN_IDOL)) {
                os << "0: [Take] Obtain Golden Idol. Trigger a trap.\n";
                os << "1: [Leave] Nothing happens.\n";
            } else {
                os << "2: [Outrun] Become Cursed - Injury\n";
                os << "3: [Smash] Take " << gc->info.hpAmount0 << "damage.\n";
                os << "4: [Hide] Lose " << gc->info.hpAmount1 << " Max HP\n";
            }
            break;
        }

        case Event::GOLDEN_SHRINE: { // Golden Shrine
            os << "0: [Pray] Gain " << (unfavorable ? 50 : 100) << " Gold.\n";
            os << "1: [Desecrate]  Gain 275 Gold. Become Cursed - Regret.\n";
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::WING_STATUE: { // Wing Statue
            os << "0: [Pray] Remove a card from your cards. Lose 7 HP.\n";
            if (gc->deck.hasCardForWingStatue()) {
                os << "1: [Destroy] Gain 50-80 Gold.\n";
            }
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::KNOWING_SKULL: {
            os << "0: [Riches?] Gain 90 Gold. Lose " << gc->info.hpAmount0 << " HP.\n";
            os << "1: [Success?] Get a Colorless Card. Lose " << gc->info.hpAmount1 << " HP.\n";
            os << "2: [A Pick Me Up?] Get a Potion. Lose " << gc->info.hpAmount2 << " HP.\n";
            os << "3: [How do I leave?] Lose 6 HP.\n";
            break;
        }

        case Event::LAB:
            // opens combat reward and should not be in event screen state here
            break;

        case Event::THE_SSSSSERPENT: {
            os << "0: [Agree] Gain " << (unfavorable ? 150 : 175) << " Gold. Become Cursed - Doubt.\n";
            os << "1: [Disagree] Nothing happens.\n";
            break;
        }

        case Event::LIVING_WALL: {
            os << "0: [Forget] Remove a card from your cards.\n";
            os << "1: [Change] Transform a card in your cards.\n";
            if (gc->deck.getUpgradeableCount() > 0) {
                os << "2: [Grow] Upgrade a card in your cards.\n";
            }
            break;
        }

        case Event::MASKED_BANDITS:
            os << "0: [Pay] Lose ALL of your Gold.\n";
            os << "1: [Fight!]\n";
            break;

        case Event::MATCH_AND_KEEP: { // Match and Keep toSelectCards will store knownCards - actualCards
            int attemptsRemaining = gc->info.eventData;
            os << "Select two: (attempts remaining " << attemptsRemaining <<")\n";

            for (int i = 0; i < gc->info.toSelectCards.size(); ++i) {
                const auto &c = gc->info.toSelectCards[i];
                if (c.deckIdx == 0) {
                    continue;
                } else if (c.deckIdx == 1) {
                    os << i << ": " << c.card.getName() << "\n";
                } else {
                    os << i << ": -Unknown- " << c.card.getName() << "\n";
                }
            }
            break;
        }

        case Event::MINDBLOOM: {
            os << "0: [I am War] Fight a Boss from Act 1. Obtain a Rare Relic.\n";
            os << "1: [I am Awake] Upgrade all Cards. You can no longer heal.\n";
            if (gc->floorNum <= 40) {
                os << "2: [I am Rich] Gain 999 Gold. Become Cursed - 2 Normality.\n";
            } else {
                os << "3: [I am Healthy] Heal to full HP. Become Cursed - Doubt.\n";
            }
            break;
        }

        case Event::HYPNOTIZING_COLORED_MUSHROOMS: { // Hypnotizing Colored Mushrooms todo check this events second phase optionIdxs
            os << "0: [Stomp] Anger the Mushrooms.\n";
            os << "1: [Eat] Heal " << gc->fractionMaxHp(.25f) << " HP. Become Cursed - Parasite.\n";
            break;
        }

        case Event::MYSTERIOUS_SPHERE:
            os << "0: [Open Sphere] Fight. Reward: Rare Relic.\n";
            os << "1: [Leave] Nothing happens.\n";
            break;

        case Event::THE_NEST: // The Nest
            os << "0: [Smash and Grab] Obtain " << (unfavorable ? 50 : 99) << " Gold.\n";
            os << "1: [Stay in Line] Obtain Ritual Dagger. Lose 6 HP.\n";
            break;

        case Event::NLOTH: {// N'loth
            // Guaranteed to have 2 or more relics
            auto r1 = gc->relics.relics[gc->info.relicIdx0].id;
            auto r2 = gc->relics.relics[gc->info.relicIdx1].id;
            os << "0: [Offer " << getRelicName(r1) << "] Lose this relic. Obtain N'loth's Gift.\n";
            os << "1: [Offer " << getRelicName(r2) << "] Lose this relic. Obtain N'loth's Gift.\n";
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::NOTE_FOR_YOURSELF: // A Note For Yourself
            os << "0: [Take and Give] Receive "<< gc->noteForYourselfCard.getName() <<". Store a card in your deck in return.\n";
            os << "1: [Ignore] Nothing happens.\n";
            break;

        case Event::PURIFIER:
            if (gc->deck.getTransformableCount(1) > 0) {
                os << "0: [Pray] Remove a card. \n";
            }
            os << "1: [Leave] Nothing happens.\n";
            break;

        case Event::SCRAP_OOZE: {
            const int phase = gc->info.eventData;
            int hpLoss = (unfavorable ? 5 : 3) + phase;
            int chance = 10*phase + 25;
            os << "0: [Reach Inside] Lose " << hpLoss << "HP. " << chance << "%: Find a Relic.\n";
            os << "1: [Leave] Nothing happens.\n";
            break;
        }

        case Event::SECRET_PORTAL:
            os << "0: [Enter the Portal] Immediately travel to the boss.\n";
            os << "1: [Leave] Nothing happens.\n";
            break;

        case Event::SENSORY_STONE: {
            os << "0: [Recall] Add 1 Colorless Card to your deck.\n";
            os << "1: [Recall] Add 2 Colorless Card to your deck. Lose 5 HP.\n";
            os << "2: [Recall] Add 3 Colorless Card to your deck. Lose 10 HP.\n";
            break;
        }

        case Event::SHINING_LIGHT:
            os << "0: [Enter] Upgrade 2 random cards. Take " << gc->info.hpAmount0 << " damage.\n";
            os << "1: [Leave] Nothing happens.\n";
            break;

        case Event::THE_CLERIC: {
            // guaranteed to have 35 gold or more
            os << "0: [Heal] Lose 35 Gold. Heal " << gc->info.hpAmount0 << " HP.\n";
            int purifyCost = unfavorable ? 75 : 50;
            if (gc->gold >= purifyCost) {
                os << "1: [Purify] Lose " << purifyCost << " Gold. Remove a card from your cards.\n";
            }
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::THE_JOUST: { // The Joust
            // Guaranteed to have >= 50 gold
            os << "0: [Murderer] Bet 50 Gold - 70%: win 100 Gold.\n";
            os << "1: [Owner] Bet 50 Gold - 30%: win 250 Gold.\n";
            break;
        }

        case Event::THE_LIBRARY: {
            os << "0: [Read] Choose 1 of 20 cards to add to your deck.\n";
            os << "1: [Sleep] Heal " << gc->info.hpAmount0 <<  "HP.\n";
            break;
        }

        case Event::THE_MAUSOLEUM: {
            os << "0: [Open Coffin] Obtain a relic. " << (unfavorable ? 100 : 50) << "% Become Cursed - Writhe.\n";
            os << "1: [Leave] Nothing happens.\n";
            break;
        }

        case Event::THE_MOAI_HEAD: {
            os << "0: [Jump Inside] Heal to full HP. Lose " << gc->info.hpAmount0 << " Max HP.\n";

            if (gc->hasRelic(RelicId::GOLDEN_IDOL)) {
                os << "1: [Offer: Golden Idol] Receive 333 Gold. Lose Golden Idol.\n";
            }
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::THE_WOMAN_IN_BLUE: { // The Woman in Blue
            os << "0: [Buy 1 Potion] 20 Gold.\n";
            os << "1: [Buy 2 Potion] 30 Gold.\n";
            os << "2: [Buy 3 Potion] 40 Gold.\n";
            if (unfavorable) {
                os << "3: [Leave] Lose " << gc->info.hpAmount0 << " HP.\n";
            } else {
                os << "3: [Leave] Nothing happens.\n";
            }
            break;
        }

        case Event::TOMB_OF_LORD_RED_MASK: {
            if (gc->hasRelic(RelicId::RED_MASK)) {
                os << "0: [Don the Red Mask] Gain 222 Gold.\n";
            } else {
                os << "1: [Offer: " << gc->gold << "] Lose all Gold. Obtain a Red Mask.\n";
            }
            os << "2: [Leave] Nothing happens.\n";
            break;
        }

        case Event::TRANSMORGRIFIER:
            if (gc->deck.getTransformableCount(1) > 0) {
                os << "0: [Pray] Transform a card. \n";
            }
            os << "1: [Leave] Nothing happens.\n";
            break;

        case Event::UPGRADE_SHRINE:
            if (gc->deck.getUpgradeableCount() > 0) {
                os << "0: [Pray] Upgrade a card. \n";
            }
            os << "1: [Leave] Nothing happens.\n";
            break;

        case Event::VAMPIRES: {
            if (gc->relics.has(RelicId::BLOOD_VIAL)) {
                os << "0: [Offer] Lose Blood Vial. Remove all Strikes. Receive 5 Bites" << ".\n";
            }
            os << "1: [Accept] Remove all Strikes. Receive 5 Bites. Lose " << gc->info.hpAmount0 << " Max Hp.\n";
            os << "2: [Refuse] Nothing happens.\n";
            break;
        }

        case Event::WE_MEET_AGAIN: {
            if (gc->info.potionIdx != -1) {
                os << "0: [Give Potion] Lose " << getPotionName(gc->potions[gc->info.potionIdx]) << ".\n";
            }
            if (gc->info.gold != -1) {
                os << "1: [Give Gold] Lose " << gc->info.gold << "g.\n";
            }
            if (gc->info.cardIdx != -1) {
                os << "2: [Give Card] Lose " << gc->deck.cards[gc->info.cardIdx]<< ".\n";
            }
            os << "3: [Attack] Nothing happens.\n";
            break;
        }

        case Event::WHEEL_OF_CHANGE: {
            os << "0: [Play] Spin the wheel and get a prize.\n";
            break;
        }

        case Event::WINDING_HALLS: {
            os << "0: [Embrace Madness] Receive 2 Madness. Lose " << gc->info.hpAmount0 << " HP.\n";
            os << "1: [Press On] Become Cursed - Writhe. Heal " << gc->info.hpAmount1 << " HP.\n";
            os << "2: [Retrace Your Steps] Lose " << gc->info.hpAmount2 << " Max HP.\n";
            break;
        }

        case Event::WORLD_OF_GOOP: {
            os << "0: [Gather Gold] Gain 75 Gold. Lose 11 HP.\n";
            os << "1: [Leave It] Lose some gold.\n";
            break;
        }

        case Event::INVALID:
        case Event::MONSTER:
        case Event::TREASURE:
        case Event::SHOP:
            assert(false);
            break;

        default:
            break;
    }
}

void ConsoleSimulator::takeEventAction(const std::string &action) {

    if (gc->curEvent == Event::MATCH_AND_KEEP) {
        int idx1 = std::stoi(action.substr(0, action.find(' ')));
        int idx2 = std::stoi(action.substr(action.find(' ')+1));
        gc->chooseMatchAndKeepCards(idx1, idx2);

    } else {
        int idx = std::stoi(action);
        gc->chooseEventOption(idx);
    }

}
