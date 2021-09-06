//
// Created by gamerpuppy on 8/24/2021.
//

#include "sim/BattleCommon.h"
#include "combat/BattleContext.h"

using namespace sts;

void sts::printOptionsCardsInDiscard(std::ostream &os, const BattleContext &bc) {
    for (int i = 0; i < bc.cards.discardPile.size(); ++i) {
        const auto &c = bc.cards.discardPile[i];
        os << i << ": " << c.getName() << (c.upgraded ? "+" : "") << '\n';
    }
}

void sts::printOptionsCardsInHand(std::ostream &os, const BattleContext &bc) {
    for (int i = 0; i < bc.cards.cardsInHand; ++i) {
        const auto &c = bc.cards.hand[i];
        os << i << ": " << c.getName() << (c.upgraded ? "+" : "") << '\n';
    }
}