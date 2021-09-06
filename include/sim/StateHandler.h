//
// Created by gamerpuppy on 8/28/2021.
//

#ifndef STS_LIGHTSPEED_STATEHANDLER_H
#define STS_LIGHTSPEED_STATEHANDLER_H

#include "data_structure/fixed_list.h"
#include "combat/CardManager.h"

#include <cstdint>
#include <array>
#include <functional>

namespace sts {

    class GameContext;
    class BattleContext;

    typedef std::function <void (GameContext&)> GameContextOperator;

    // todo potion options
    class RandomStateHandler {
        fixed_list<GameContextOperator,32> optionFunctions;
        fixed_list<int,12> matchAndKeepCardMap;

    public:
        void setupState(const GameContext &gc);
        int getStateSize(const GameContext &gc);
        void chooseOption(GameContext &gc, int option);

    private:
        void setupMapOptions(const GameContext &gc);
        void setupRestOptions(const GameContext &gc);
        void setupRewardsOptions(const GameContext &c);
        void setupShopOptions(const GameContext &c);

        void pushOption(int idx);
        void pushOptions(int start, int count);
        void setupEventOptions(const GameContext &gc); // todo match and keep
    };

    typedef std::function <void (BattleContext&)> BattleContextOperator;

    class RandomBattleStateHandler {
        static constexpr int NORMAL_STATE_SIZE = (6 * 5) + (10 * 5) + 1; // 81
        fixed_list<BattleContextOperator, NORMAL_STATE_SIZE> optionFunctions;
        fixed_list<int, CardManager::MAX_DRAWPILE_SIZE> validCardIdxs;
        std::function <void (BattleContext&, int idx)> cardSelectFunction;

    public:
        int setupState(const BattleContext &bc); // returns state size
        void chooseOption(BattleContext &bc, int option);

    private:
        int setupNormalOptions(const BattleContext &bc);
        int setupCardSelectOptions(const BattleContext &bc);
    };

}


#endif //STS_LIGHTSPEED_STATEHANDLER_H
