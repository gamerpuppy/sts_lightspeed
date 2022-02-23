//
// Created by gamerpuppy on 8/24/2021.
//

#ifndef STS_LIGHTSPEED_CARDSELECTINFO_HPP
#define STS_LIGHTSPEED_CARDSELECTINFO_HPP

#include <array>

#include <sts/constants/card_ids.hpp>

namespace sts {

    enum class CardSelectTask {
        INVALID=0,
        ARMAMENTS,
        CODEX,
        DISCOVERY,
        DUAL_WIELD,
        EXHAUST_ONE,
        EXHAUST_MANY,
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

    static constexpr const char* cardSelectTaskStrings[] = {
            "INVALID",
            "ARMAMENTS",
            "CODEX",
            "DISCOVERY",
            "DUAL_WIELD",
            "EXHAUST_ONE",
            "EXHAUST_MANY",
            "EXHUME",
            "FORETHOUGHT",
            "GAMBLE",
            "HEADBUTT",
            "HOLOGRAM",
            "LIQUID_MEMORIES_POTION",
            "MEDITATE",
            "NIGHTMARE",
            "RECYCLE",
            "SECRET_TECHNIQUE",
            "SECRET_WEAPON",
            "SEEK",
            "SETUP",
            "WARCRY",
    };

    struct CardSelectInfo {
        std::array<CardId, 3> cards;
        bool canPickZero = false;
        bool canPickAnyNumber = false;
        int pickCount = 0;
        int data0 = 0;

        CardSelectTask cardSelectTask;

        std::array<CardId, 3>& discovery_Cards() { return cards; }
        int& discovery_CopyCount() { return data0; }
        int& dualWield_CopyCount() { return data0; }
        std::array<CardId, 3>& codexCards() { return cards; }
    };

}

#endif //STS_LIGHTSPEED_CARDSELECTINFO_HPP
