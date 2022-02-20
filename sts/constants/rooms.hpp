//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_ROOMS_H
#define STS_LIGHTSPEED_ROOMS_H

#include <cstdint>

namespace sts {

    enum class Room : uint8_t {
        SHOP = 0,
        REST,
        EVENT,
        ELITE,
        MONSTER,
        TREASURE,
        BOSS,
        BOSS_TREASURE,
        NONE,
        INVALID,
    };

    static constexpr const char* roomStrings[] = {
            "SHOP",
            "REST",
            "EVENT",
            "ELITE",
            "MONSTER",
            "TREASURE",
            "BOSS",
            "BOSS_TREASURE",
            "NONE",
            "INVALID"
    };

    constexpr char getRoomSymbol(Room room) {
        switch(room) {
            case Room::NONE: return 'N';
            case Room::EVENT: return '?';
            case Room::MONSTER: return 'M';
            case Room::ELITE: return 'E';
            case Room::REST: return 'R';
            case Room::SHOP: return '$';
            case Room::TREASURE: return 'T';
            case Room::BOSS: return 'B';
            default: return 'I';
        }
    }

}



#endif //STS_LIGHTSPEED_ROOMS_H
