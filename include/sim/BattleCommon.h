//
// Created by gamerpuppy on 8/24/2021.
//

#ifndef MONSTERSPECIFIC_CPP_BATTLECOMMON_H
#define MONSTERSPECIFIC_CPP_BATTLECOMMON_H

#include <iostream>

namespace sts {

    class BattleContext;

    void printOptionsCardsInDiscard(std::ostream &os, const BattleContext &bc);
    void printOptionsCardsInHand(std::ostream &os, const BattleContext &bc);

}


#endif //MONSTERSPECIFIC_CPP_BATTLECOMMON_H
