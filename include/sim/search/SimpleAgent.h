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
        GameContext *curGameContext; // unsafe only use in private methods during playout

        fixed_list<int,16> mapPath;

        bool print = false;

        SimpleAgent();

        [[nodiscard]] int getIncomingDamage(const BattleContext &bc) const;

        void playout(GameContext &gc);

        void takeAction(GameContext &gc, GameAction a);
        void takeAction(BattleContext &bc, Action a);
        void playoutBattle(BattleContext &bc);

        void stepBattleCardPlay(BattleContext &bc);
        void stepBattleCardSelect(BattleContext &bc);

        void stepOutOfCombat(GameContext &gc);
        void stepEventScreen(sts::GameContext &gc);
        void stepRestScreen(GameContext &gc);
        void stepRewardsScreen(GameContext &gc);
        void stepCardReward(GameContext &gc);
        void stepShopScreen(GameContext &gc);

        bool playPotion(BattleContext &bc);
        static fixed_list<int,16> getBestMapPathForWeights(const Map &m, const int *weights);
        static void runAgentsMt(int threadCount, std::uint64_t startSeed, int playoutCount, bool print);
    };

}

#endif //STS_LIGHTSPEED_SIMPLEAGENT_H
