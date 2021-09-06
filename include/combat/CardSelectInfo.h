//
// Created by gamerpuppy on 8/24/2021.
//

#ifndef STS_LIGHTSPEED_CARDSELECTINFO_H
#define STS_LIGHTSPEED_CARDSELECTINFO_H

#include <array>

#include "constants/Cards.h"

namespace sts {

    enum class CardSelectTask {
        ARMAMENTS,
        DISCOVERY,
        DUAL_WIELD,
        ELIXIR_POTION,
        EXHAUST_ONE,
        EXHUME,
        FORETHOUGHT,
        GAMBLE,
        HEADBUTT,
        HOLOGRAM,
        LIQUID_MEMORIES_POTION,
        MEDITATE,
        NIGHTMARE,
        RECYCLE,
        SECRET_TECHNIQUE,
        SECRET_WEAPON,
        SEEK,
        SETUP,
        WARCRY,
    };

    struct CardSelectInfo {
        std::array<CardId, 3> cards;
        bool canPickZero = false;
        bool canPickAnyNumber = false;
        int pickCount = 0;

        int data0 = 0;
        int data1 = 0;

        CardSelectTask cardSelectTask;

        std::array<CardId, 3>& discovery_Cards() { return cards; }
        int& discovery_CopyCount() { return data0; }
        int& dualWield_CopyCount() { return data0; }
    };

}

#endif //STS_LIGHTSPEED_CARDSELECTINFO_H
