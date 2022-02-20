//
// Created by keega on 9/9/2021.
//

#include "game/SaveFile.h"
#include "game/GameContext.h"
#include "game/Game.h"
#include "game/Map.h"
#include "constants/Cards.h"
#include "slaythespire.h"
#include "sim/SimHelpers.h"
#include "sim/agents/SimpleAgent.h"

#include <vector>
#include <algorithm>

using namespace sts;

struct CardIdSortOrder {
    CardId cardId;
    std::string sid;
};

void printCardIdOrder() {

    std::vector<CardIdSortOrder> vec;

    for (int i = static_cast<int>(CardId::INVALID); i <= static_cast<int>(CardId::ZAP); ++i) {
        auto id = static_cast<CardId>(i);
        std::string sid(getCardStringId(id));
        vec.push_back({id, sid});
    }

    std::stable_sort(vec.begin(), vec.end(), [](auto c0, auto c1) { return c0.sid < c1.sid; } );

    int sortedValues[371];
    for (int i = 0; i < vec.size(); ++i) {
        sortedValues[static_cast<int>(vec[i].cardId)] = i;
    }

    std::cout << "static constexpr int cardIdSortedIdx[] = {";
    for (auto x : sortedValues) {
        std::cout << x << ",";
    }
    std::cout << "};\n";
}

//
//void findShovelSeed() {
//    for (std::uint64_t seed = 0; seed < 100000; ++seed) {
//        GameContext g(seed, CharacterClass::IRONCLAD, 20);
//        if (g.info.neowRewards[2].r == sts::Neow::Bonus::ONE_RARE_RELIC) {
//            g.chooseNeowOption(g.info.neowRewards[2]);
//            if (g.hasRelic(sts::RelicId::SHOVEL)) {
//                std::cout << SeedHelper::getString(seed) << "\n";
//            }
//        }
//
//
//    }
//}

void printCardsForOrdering() {
    for (int i = static_cast<int>(CardId::INVALID); i <= static_cast<int>(CardId::ZAP); ++i) {
        auto id = static_cast<CardId>(i);
        switch (getCardColor(id)) {
            case CardColor::RED:
            case CardColor::COLORLESS:
            case CardColor::CURSE:
            case CardColor::INVALID:
                std::cout << "case CardId::" << getCardEnumName(id) << ":\n";
                break;

            case CardColor::GREEN:
            case CardColor::BLUE:
            case CardColor::PURPLE:
            default:
                break;
        }
    }
}

void printCardsForBinding() {
    for (int i = static_cast<int>(CardId::INVALID); i <= static_cast<int>(CardId::ZAP); ++i) {
        auto id = static_cast<CardId>(i);
        printf(".value(\"%s\", CardId::%s)\n", getCardEnumName(id), getCardEnumName(id));
    }
}

void printAllCardInfo(std::ostream &os) {
    const auto s = ",";

    os << "MyEnumName" << s
        << "Name" << s
        << "StringId" << s
        << "Rarity" << s
        << "Color" << s
        << "Type" << s
        << "cost" << s
        << "costUp" << s
        << "baseDamage" << s
        << "baseDamageUp" << s
        << "isEthereal" << s
        << "isEtherealUp" << s
        << "doesExhaust" << s
        << "doesExhaustUp" << s
        << "isInnate" << s
        << "isInnateUp" << s
        << "hasSelfRetain" << s
        << "hasSelfRetainUp" << s
        << "targetsEnemy" << s
        << "targetsEnemyUp" << s
        << "IsStrike" << s
        << "IsStarterStrikeOrDefend" << "\n";

    for (int i = static_cast<int>(CardId::INVALID); i <= static_cast<int>(CardId::ZAP); ++i) {
        auto id = static_cast<CardId>(i);
        os << getCardEnumName(id) << s
            << getCardName(id) << s
            << getCardStringId(id) << s
            << cardRarityStrings[static_cast<int>(getCardRarity(id))] << s
            << cardColorStrings[static_cast<int>(getCardColor(id))]  << s
            << cardTypeStrings[static_cast<int>(getCardType(id))] << s
            << getEnergyCost(id, false) << s
            << getEnergyCost(id, true) << s
            << getBaseDamage(id, false) << s
            << getBaseDamage(id, true) << s
            << isCardEthereal(id, false) << s
            << isCardEthereal(id, true) << s
            << doesCardExhaust(id, false) << s
            << doesCardExhaust(id, true) << s
            << isCardInnate(id, false) << s
            << isCardInnate(id, true) << s
            << doesCardSelfRetain(id, false) << s
            << doesCardSelfRetain(id, true) << s
            << cardTargetsEnemy(id, false) << s
            << cardTargetsEnemy(id, true) << s
            << isCardStrikeCard(id) << s
            << isStarterStrikeOrDefend(id) << '\n';
    }
}

void printEncountersForBinding() {
    for (auto i = static_cast<int8_t>(ME::INVALID); i <= static_cast<std::int8_t>(ME::MYSTERIOUS_SPHERE_EVENT); ++i) {
        std::cout << ".value(\"" << monsterEncounterEnumNames[i] << "\", "
        << "ME::" << monsterEncounterEnumNames[i] << ")\n";
    }
}

void printAllRelicForBinding() {
    for (auto i = static_cast<int>(RelicId::AKABEKO); i <= static_cast<int>(RelicId::INVALID); ++i) {
        std::cout << ".value(\"" << relicEnumNames[i] << "\", " << "RelicId::" << relicEnumNames[i] << ")\n";
    }
}


template<typename ForwardIt>
void printVec(const std::string &name, std::ostream &os, ForwardIt begin, ForwardIt end) {
    os << name << " (" << (end-begin) << "): ";
    int i = 0;
    for (auto it = begin; it != end; ++it) {
        os << "(" << i << "," << *it << "), ";
        ++i;
    }
    os << '\n';
}


int main(int argc, const char *argv[]) {
//    printAllRelicForBinding();

    Map map(sts::Map::fromSeed(1));
    std::cout << map.toString() << std::endl;


    int weights[] = {100,1000,100,10,1,0};

    auto path = search::SimpleAgent::getBestMapPathForWeights(map, weights);
    printVec("path", std::cout, path.begin(), path.end());

//    auto rep = sts::py::getNNMapRepresentation(map);
//    printVec("map_rep", std::cout, rep.begin(), rep.end());

//
//    GameContext gc(sts::CharacterClass::IRONCLAD, 10000, 0);
//
//
//    const auto maximums = NNInterface::getInstance()->getObservationMaximums();
//    printVec("maximums", std::cout, maximums.begin(), maximums.end());
//
//    const auto observation = NNInterface::getInstance()->getObservation(gc);
//    printVec("observation", std::cout, observation.begin(), observation.end());


//    printEncountersForBinding();
//    printAllCardInfo(std::cout);
//    printCardsForOrdering();
//    printCardsForBinding();

//    printCardIdOrder();
//    findShovelSeed();
//    std::cout << sts::Base64::encode(argv[1]);

    return 0;
}