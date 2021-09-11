//
// Created by gamerpuppy on 7/10/2021.
//

#include "sim/PrintHelpers.h"

#include "game/GameContext.h"
#include "game/Game.h"
#include "combat/BattleContext.h"

namespace sts {

    std::ostream& operator<<(std::ostream& os,  const RelicContainer &rc) {
        os << '{';
        for (auto x : rc.relics) {
            os << relicNames[static_cast<int>(x.id)] << ":" << x.data << ',';
        }
        os << '}';
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const Card &card) {
        os << cardNames[static_cast<int>(card.id)];
        if (card.getId() == CardId::RITUAL_DAGGER) {
            os << "=" << card.misc;
        }
        if (card.upgraded) {
            if (card.getId() == CardId::SEARING_BLOW) {
                os << "+" << card.getUpgraded();
            } else {
                os << "+";
            }
        }
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const Deck &deck) {
        return os << "(" << deck.size() << "): " << deck.cards;
    }

    void printEventListState(std::ostream &os, const GameContext &gc) {
        os << "{";
        os << "\n\t\tevents:{";
        for (auto x : gc.eventList) {
            os << eventGameNames[(int)x] << ",";
        }
        os << "}";


        os << ",\n\t\toneTimeEvents:{";
        for (auto x : gc.specialOneTimeEventList) {
            os << eventGameNames[(int)x] << ",";
        }
        os << "}";

        os << ",\n\t\tshrines:{";
        for (auto x : gc.shrineList) {
            os << eventGameNames[(int)x] << ",";
        }
        os << "}";


        os << "\n\t}";
    }

    void printPlayerInfo(std::ostream &os, const GameContext &gc) {
        os << "{";
        os << "curHp:" << gc.curHp
            << ",maxHp:" << gc.maxHp
            << ",gold:" << gc.gold
            << ",keyBits:" << gc.keyBits
            << ",cc:" << getCharacterClassName(gc.cc)
            << ",potionCapacity:" << gc.potionCapacity;
        os << "}\n";
    }

    void printRngInfo(std::ostream &os, const GameContext &gc, const std::string &separator) {
        os << separator << "neowRng: " << gc.neowRng.counter;
        os << separator << "treasureRng: " << gc.treasureRng.counter;
        os << separator << "eventRng: " << gc.eventRng.counter;
        os << separator << "relicRng: " << gc.relicRng.counter;
        os << separator << "potionRng: " << gc.potionRng.counter;
        os << separator << "cardRng: " << gc.cardRng.counter;
        os << separator << "cardRandomRng: " << gc.cardRandomRng.counter;
        os << separator << "merchantRng: " << gc.merchantRng.counter;
        os << separator << "monsterRng: " << gc.monsterRng.counter;
        os << separator << "shuffleRng: " << gc.shuffleRng.counter;
        os << separator << "miscRng: " << gc.miscRng.counter;
        os << separator << "mathUtilRng: " << gc.shuffleRng.counter;
        os << '\n';
    }

    void printPotionInfo(std::ostream &os, const GameContext &gc) {
        os << "potions: {";
        for (int i = 0; i < gc.potionCapacity; ++i) {
            os << getPotionName(gc.potions[i]) << ",";
        }
        os  << "}\n";
    }


    void printMonsterLists(std::ostream &os, const GameContext &gc) {
        os << "\tmonsterList: offset(" << gc.monsterListOffset << ") {";
        for (auto m : gc.monsterList) {
            os << monsterEncounterStrings[static_cast<int>(m)] << ", ";
        }
        os << "\n";


        os << "\teliteMonsterList: offset(" << gc.eliteMonsterListOffset << ") {";
        for (auto m : gc.eliteMonsterList) {
            os << monsterEncounterStrings[static_cast<int>(m)] << ", ";
        }
        os << "\n";
    }


    std::ostream& operator<<(std::ostream &os, const sts::GameContext &gc) {
        os << "GameContext: {\n";
        os << "\tseed: \"" << SeedHelper::getString(gc.seed) << "\" seedAsLong: " << gc.seed << '\n';
        os << "\t" << "floorNum: " << gc.floorNum
            << " act: " << gc.act
            << " g_searchAscension: " << gc.ascension
            << " curRoom: " << roomStrings[static_cast<int>(gc.curRoom)]
            << " curEvent: " << eventIdStrings[static_cast<int>(gc.curEvent)]
            << " boss: " << monsterEncounterStrings[static_cast<int>(gc.boss)]
            << " boss2: " << monsterEncounterStrings[static_cast<int>(gc.secondBoss)]
            << '\n';


        os << "\t";
        printPlayerInfo(os, gc);

        os << '\t';
        printRngInfo(os, gc, " ");

        os << '\t';
        printPotionInfo(os, gc);

        printMonsterLists(os, gc);

        os << "\t" << gc.deck << "\n";
        os << "\t" << gc.relics << "\n";

        os << "\t"; printEventListState(os, gc); os << "\n";
        os << "}\n";
        return os;
    }

    void printBattleContextRng(std::ostream &os, const BattleContext &bc) {
        os << "aiRng: " << bc.aiRng.counter << "\n";
        os << "cardRandomRng: " << bc.cardRandomRng.counter << "\n";
        os << "miscRng: " << bc.miscRng.counter << "\n";
        os << "monsterHpRng: " << bc.monsterHpRng.counter << "\n";
        os << "potionRng: " << bc.potionRng.counter << "\n";
        os << "shuffleRng: " << bc.shuffleRng.counter << "\n";
    }

    std::ostream& printCardInstanceSimple(std::ostream &os, const CardInstance &card) {
        os << cardNames[static_cast<int>(card.id)];
        // todo genetic algorithm
        if (card.getId() == CardId::RITUAL_DAGGER) {
            os << "=" << card.specialData;
        }
        if (card.upgraded) {
            if (card.id == CardId::SEARING_BLOW) {
                os << "+" << card.getUpgradeCount();
            } else {
                os << "+";
            }
        }
        return os;
    }

}





