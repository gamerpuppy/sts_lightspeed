//
// Created by gamerpuppy on 8/28/2021.
//

#ifndef MONSTERSPECIFIC_CPP_RANDOMAGENT_H
#define MONSTERSPECIFIC_CPP_RANDOMAGENT_H

#include <random>
#include <functional>

#include <sts/constants/character_classes.hpp>

namespace sts {

    class GameContext;
    class BattleContext;

    typedef std::default_random_engine Rng;
    typedef std::function <bool(const GameContext &gc)> GameContextPredicate;

    struct RandomAgent {
        Rng rng;
        int choiceCount = 0;

        bool print = true;
        int printStartingAt = 0x7FFFFFFF;

        RandomAgent(const Rng &rng);

        void playout(GameContext &gc);
        void playout(GameContext &gc, const GameContextPredicate &p);

        void playoutWithBattles(GameContext &gc);
        void playoutBattle(BattleContext &bc);

    };

}

#endif //MONSTERSPECIFIC_CPP_RANDOMAGENT_H
