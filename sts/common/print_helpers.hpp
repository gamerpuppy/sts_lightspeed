//
// Created by gamerpuppy on 7/2/2021.
//

#ifndef STS_LIGHTSPEED_PRINTHELPERS_H
#define STS_LIGHTSPEED_PRINTHELPERS_H

#include <sts/constants/card_ids.hpp>
#include <sts/constants/relic_ids.hpp>

#include <sts/game/card.hpp>
#include <sts/data_structure/fixed_list.hpp>
#include <sts/game/relic_container.hpp>

#include <string>
#include <iostream>
#include <sstream>

namespace sts {

    class CardInstance;
    class BattleContext;
    class GameContext;
    class Deck;

    template<int size>
    static std::ostream& operator<<(std::ostream& os,  const fixed_list<Card, size> &cards) {
        os << '{';
        for (auto x : cards) {
            os << x << ',';
        }
        os << '}';
        return os;
    }

    std::ostream& operator<<(std::ostream& os,  const RelicContainer &rc);

    std::ostream& operator<<(std::ostream &os, const Card &card);

    std::ostream& operator<<(std::ostream& os,  const Deck &deck);

    void printEventListState(std::ostream &os, const GameContext &gc);
    void printPlayerInfo(std::ostream &os, const GameContext &gc);
    void printRngInfo(std::ostream &os, const GameContext &gc, const std::string &separator="");
    void printPotionInfo(std::ostream &os, const GameContext &gc);
    void printMonsterLists(std::ostream &os, const GameContext &gc);

    std::ostream& operator<<(std::ostream& os,  const GameContext &gc);

    void printBattleContextRng(std::ostream &os, const sts::BattleContext &bc);

    std::ostream& printCardInstanceSimple(std::ostream &os, const CardInstance &card);
    void printOutcome(std::ostream &os, const GameContext &gc);

}





#endif //STS_LIGHTSPEED_PRINTHELPERS_H
