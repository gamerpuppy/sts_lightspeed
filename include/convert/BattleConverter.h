#ifndef STS_LIGHTSPEED_BATTLECONVERTER_H
#define STS_LIGHTSPEED_BATTLECONVERTER_H

#include <nlohmann/json.hpp>

namespace sts {

    class BattleContext;
    class GameContext;

    struct BattleConverter {
        BattleConverter();
        ~BattleConverter();

        BattleContext convertFromJson(const nlohmann::json &json, int *monsterIdxMap);
    };

}

#endif //STS_LIGHTSPEED_BATTLECONVERTER_H