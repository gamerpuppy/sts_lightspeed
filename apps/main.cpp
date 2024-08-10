#include <iostream>

#include "data_structure/fixed_list.h"
#include "constants/CardPools.h"
#include "game/Game.h"
#include "game/Map.h"
#include "game/Neow.h"
#include "combat/BattleContext.h"
#include "sim/ConsoleSimulator.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
using namespace sts;

int main() {
    while (!std::cin.eof()) {
        std::cout << "usage: seed character(I/S/D/W) ascensionLevel" << std::endl;

        SimulatorContext simCtx;
        ConsoleSimulator simulator;
        simulator.play(std::cin, std::cout, simCtx);
    }

    return 0;
}

#pragma clang diagnostic pop


