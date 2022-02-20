//
// Created by gamerpuppy on 7/20/2021.
//

#ifndef STS_LIGHTSPEED_BATTLESIMULATOR_H
#define STS_LIGHTSPEED_BATTLESIMULATOR_H

#include <string>
#include <iostream>

namespace sts {

    class BattleContext;
    class GameContext;
    struct SimulatorContext;

    struct BattleSimulator {
        BattleContext *bc;
        bool initialized = false;

        BattleSimulator();
        ~BattleSimulator();

        [[nodiscard]] bool isInitialized() const;
        [[nodiscard]] bool isBattleComplete() const;
        void initBattle(const GameContext &gc);
        void exitBattle(GameContext &gc);

        void handleInputLine(const std::string &line, std::ostream &os, SimulatorContext &state);

        void printActions(std::ostream &os) const;
        void takeAction(const std::string &action);

        void doPrintCommand(std::ostream &os, const std::string &cmd);
        void doSetCommand(const std::string &cmd);
        bool doAssertCommand(const std::string &cmd);

        void printPotionActions(std::ostream  &os) const;
        void takePotionAction(const std::string &action);

        void printNormalActions(std::ostream &os) const;
        void takeNormalAction(const std::string &action);

        void printCardSelectActions(std::ostream &os) const;
        void takeCardSelectAction(const std::string &action);
//
//        void printDiscoveryActions(std::ostream &os) const;
//        void takeDiscoveryAction(const std::string &action);
//
//        void printDiscardToHandActions(std::ostream &os) const;
//        void takeDiscardToHandAction(const std::string &action);
//
//        void printChooseCardsInHandAction(std::ostream &os) const;
//        void takeChooseCardsInHandAction(const std::string &action) const;

    };


}

#endif //STS_LIGHTSPEED_BATTLESIMULATOR_H
