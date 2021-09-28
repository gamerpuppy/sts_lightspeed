//
// Created by keega on 9/27/2021.
//

#ifndef STS_LIGHTSPEED_SIMPLEAGENT_H
#define STS_LIGHTSPEED_SIMPLEAGENT_H

#include "game/GameContext.h"
#include "sim/search/Action.h"
#include "sim/search/GameAction.h"

namespace sts::search {


    struct SimpleAgent {
        std::vector<int> actionHistory;

        void playout(GameContext &gc);

        void takeAction(GameContext &gc, GameAction a);
        void takeAction(BattleContext &bc, Action a);
        void playoutBattle(BattleContext &bc);



    };


}

#endif //STS_LIGHTSPEED_SIMPLEAGENT_H
