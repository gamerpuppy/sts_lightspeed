//
// Created by gamerpuppy on 8/28/2021.
//



#include "sim/RandomAgent.h"
#include "sim/StateHandler.h"
#include "game/GameContext.h"
#include "combat/BattleContext.h"
#include "game/Game.h"
#include "sim/PrintHelpers.h"

#include <iostream>

using namespace sts;

RandomAgent::RandomAgent(const Rng &rng) : rng(rng) {}

void RandomAgent::playout(GameContext &gc) {
    playout(gc, [](auto &gc) { return gc.outcome != GameOutcome::UNDECIDED; });
}

void RandomAgent::playout(GameContext &gc, const GameContextPredicate &predicate) {
    RandomStateHandler handler;
    while (gc.outcome == GameOutcome::UNDECIDED && (!predicate || !predicate(gc))) {


        handler.setupState(gc);
        const auto stateSize = handler.getStateSize(gc);
        std::uniform_int_distribution<int> distr(0, stateSize-1);

        const int randomChoice = distr(rng);
//        std::cout << "seed: " << gc.seed
//            << " floor: " << gc.floorNum
//            << " curRoom: " << roomStrings[static_cast<int>(gc.curRoom)]
//            << " curEvent: " << eventGameNames[static_cast<int>(gc.curEvent)]
//            << " stateSize: " << stateSize
//            << " choice: " << randomChoice << std::endl;

        handler.chooseOption(gc, randomChoice);
    }

//    std::cout << "random playout finished\n";
}


void RandomAgent::playoutWithBattles(GameContext &gc) {
    RandomStateHandler handler;
    BattleContext bc;
    const auto seedStr = std::string(SeedHelper::getString(gc.seed));

    while (gc.outcome == GameOutcome::UNDECIDED && gc.act == 1) {
        if (gc.screenState == ScreenState::BATTLE) {
            bc = {};
            bc.init(gc);
            bc.executeActions();

            playoutBattle(bc);
            bc.exitBattle(gc);

        } else {
            ++choiceCount;
            handler.setupState(gc);
            const auto stateSize = handler.getStateSize(gc);
            std::uniform_int_distribution<int> distr(0, stateSize-1);
            const int randomChoice = distr(rng);

            if (print) {
                std::cout << choiceCount
                          << " seed: (" << gc.seed << ":" << seedStr << ")"
                          << " floor: " << gc.floorNum
                          << " curRoom: " << roomStrings[static_cast<int>(gc.curRoom)]
                          << " curEvent: " << eventGameNames[static_cast<int>(gc.curEvent)]
                          << " stateSize: " << stateSize
                          << " choice: " << randomChoice
                          << std::endl;
                if (choiceCount >= printStartingAt) {
                    std::cout << gc << std::endl;
                }
            }
            handler.chooseOption(gc, randomChoice);
        }
    }

    if (print) {
        if (gc.outcome == GameOutcome::PLAYER_LOSS) {
            std::cout << "random playout finished with a loss" << std::endl;
        } else {
            std::cout << "random playout finished with a victory! hp: " << gc.curHp << std::endl;
        }
    }
}

void RandomAgent::playoutBattle(BattleContext &bc) {
    RandomBattleStateHandler handler;
    while (bc.outcome == Outcome::UNDECIDED) {
        ++choiceCount;

        const auto stateSize = handler.setupState(bc);
        std::uniform_int_distribution<int> distr(0, stateSize-1);
        const int randomChoice = distr(rng);

        if (bc.monsters.monstersAlive < 0) {
            std::cerr << bc.seed << std::endl;
            assert(false);
        }

        if (print) {
            std::cout << choiceCount
                      << " choice: " << randomChoice
                      << " turn: " << bc.turn
                      << " energy: " << bc.player.energy
                      << " cardsPlayedThisTurn: " << bc.player.cardsPlayedThisTurn
                      << " state: " << (bc.inputState == InputState::PLAYER_NORMAL ? "normal" : " probably card select")
                      << std::endl;

            if (choiceCount >= printStartingAt) {
                std::cout << bc << '\n';
            }
        }
        handler.chooseOption(bc, randomChoice);
    }
}
