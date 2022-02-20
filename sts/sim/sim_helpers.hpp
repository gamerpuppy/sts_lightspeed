//
// Created by gamerpuppy on 9/14/2021.
//

#ifndef STS_LIGHTSPEED_SIMHELPERS_H
#define STS_LIGHTSPEED_SIMHELPERS_H

#include <string>
#include <unordered_map>

#include <sts/constants/monster_ids.hpp>
#include <sts/constants/monster_status_effects.hpp>
#include <sts/constants/monster_moves.hpp>
#include <sts/constants/player_status_effects.hpp>
#include <sts/constants/card_ids.hpp>

namespace sts {

    class SimHelpers {
    private:
        // for enum names to enum values
        std::unordered_map<std::string,MonsterId> monsterIdStringMap;
        std::unordered_map<std::string,MonsterStatus> monsterStatusStringMap;
        std::unordered_map<std::string,MonsterMoveId> monsterMoveStringMap;
        std::unordered_map<std::string,PlayerStatus> playerStatusStringMap;
        std::unordered_map<std::string,CardId> cardIdStringMap;

        SimHelpers();

        static inline SimHelpers *theInstance = nullptr;

    public:
        static MonsterId getMonsterIdForString(const std::string &s);
        static MonsterStatus getMonsterStatusForString(const std::string &s);
        static MonsterMoveId getMonsterMoveForString(const std::string &s);
        static PlayerStatus getPlayerStatusForString(const std::string &s);
        static CardId getCardIdForString(const std::string &s);
    };

}


#endif //STS_LIGHTSPEED_SIMHELPERS_H
