//
// Created by keega on 9/19/2021.
//

#ifndef STS_LIGHTSPEED_SCUMSEARCHAGENT2_H
#define STS_LIGHTSPEED_SCUMSEARCHAGENT2_H

#include "game/GameContext.h"

#include <memory>
#include <random>

namespace sts::search {

    struct ScumSearchAgent2 {
        struct Settings {
            int simulationCountBase = 50000;
            double bossSimulationMultiplier = 3;
            bool pauseOnCardReward = false;
            bool printLogs = false;
            std::default_random_engine rng;
        };

        Settings settings;
        int stepCount = 0;

        // public interface
        ScumSearchAgent2(Settings settings);
        void playout(GameContext &gc);

        // private methods
        void playoutBattle(BattleContext &bc);

        void stepOutOfCombatPolicy(GameContext &gc);
        void cardSelectPolicy(GameContext &gc);
        void stepEventPolicy(GameContext &gc);
        void stepRandom(GameContext &gc);
        void stepRewardsPolicy(GameContext &gc);
    };

}


#endif //STS_LIGHTSPEED_SCUMSEARCHAGENT2_H
