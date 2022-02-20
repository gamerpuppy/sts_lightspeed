//
// Created by gamerpuppy on 6/25/2021.
//

#ifndef STS_LIGHTSPEED_CONSOLE_SIMULATOR_HPP
#define STS_LIGHTSPEED_CONSOLE_SIMULATOR_HPP

#include <cstdint>
#include <iostream>

#include <sts/constants/character_classes.hpp>
#include <sts/game/game_context.hpp>
#include <sts/sim/battle_simulator.hpp>

namespace sts {

    class BattleSimulator;

    struct SimulatorContext {
        // state
        int line = 0;
        bool quitCommandGiven = false;
        bool tookAction = false;
        bool failedTest = false;

        // settings
        bool printFirstLine = true;
        bool skipTests = false;
        bool printLogActions = true;
        bool printInput = true;
        bool printPrompts = true;
        bool quitOnTestFailed = true;
    };

    struct ConsoleSimulator {
        // settings

        // state
        GameContext *gc = nullptr;
        BattleSimulator battleSim;

        ConsoleSimulator() = default;
        void setupGame(std::uint64_t seed, CharacterClass c, int ascension);
        void setupGameFromSaveFile(const SaveFile &save);
        void reset();

        void play(std::istream &is, std::ostream &os, SimulatorContext &c);

        void getInputLoop(std::istream &is, std::ostream &os, SimulatorContext &c);
        void handleInputLine(const std::string &line, std::ostream &os, SimulatorContext &c);

        void doPrintCommand(std::ostream &os, const std::string &cmd);
        void doSetCommand(const std::string &cmd);

        void printActions(std::ostream &os) const;
        void takeAction(const std::string &action);

        void printBossRelicRewardsActions(std::ostream &os) const;
        void takeBossRelicRewardsAction(const std::string &action);

        void printRestRoomActions(std::ostream &os) const;
        void takeRestRoomAction(const std::string &action);

        void printShopRoomActions(std::ostream &os) const;
        void takeShopRoomAction(const std::string &action);

        void printTreasureRoomActions(std::ostream &os) const;
        void takeTreasureRoomAction(const std::string &action);

        void printMapScreenActions(std::ostream &os) const;
        void takeMapScreenAction(const std::string &action);

        void printRewardsScreenActions(std::ostream &os) const;
        void takeRewardScreenAction(const std::string &action);

        void printCardSelectScreenActions(std::ostream &os) const;
        void takeCardSelectScreenAction(const std::string &action);

        void printEventActions(std::ostream &) const;
        void takeEventAction(const std::string &action);
    };

}

#endif //STS_LIGHTSPEED_CONSOLE_SIMULATOR_HPP
