//
// Created by gamerpuppy on 7/9/2021.
//

#ifndef STS_LIGHTSPEED_MISC_H
#define STS_LIGHTSPEED_MISC_H

#include <cstdint>

namespace sts {

    enum class Key : std::uint8_t {
        EMERALD_KEY=0,
        RUBY_KEY,
        SAPPHIRE_KEY,
        INVALID
    };

    enum class ChestSize : std::uint8_t {
        SMALL=0,
        MEDIUM,
        LARGE,
        INVALID
    };

    enum HpType : std::uint8_t {
        CEIL,
        FLOOR,
        ROUND,
    };

    static constexpr int SMALL_CHEST_CHANCE = 50;
    static constexpr int MEDIUM_CHEST_CHANCE = 33;
    static constexpr int LARGE_CHEST_CHANCE = 17;

    static constexpr int chestRelicTierChances[3][2] = {
            {75,25},
            {35,50},
            {0,75},
    };

    static constexpr int chestGoldChances[3] = {50,35,50};
    static constexpr int chestGoldAmounts[3] = {25,50,75};

    static constexpr const char* chestSizeEnumNames[] = {"SMALL","MEDIUM","LARGE","INVALID"};
    static constexpr const char* chestSizeNames[] = {"Small","Medium","Large","INVALID"};

}

#endif //STS_LIGHTSPEED_MISC_H
