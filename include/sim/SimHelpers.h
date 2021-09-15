//
// Created by gamerpuppy on 9/14/2021.
//

#ifndef STS_LIGHTSPEED_SIMHELPERS_H
#define STS_LIGHTSPEED_SIMHELPERS_H

#include <string>
#include <unordered_map>

#include "constants/MonsterIds.h"
#include "constants/MonsterStatusEffects.h"
#include "constants/MonsterMoves.h"
#include "constants/PlayerStatusEffects.h"
#include "constants/Cards.h"

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
