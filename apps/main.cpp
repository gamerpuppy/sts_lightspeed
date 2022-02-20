#include <iostream>

#include <sts/sim/console_simulator.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
using namespace sts;

int main() {
    while (true) {
        std::cout << "usage: seed character(I/S/D/W) ascensionLevel" << std::endl;

        SimulatorContext simCtx;
        ConsoleSimulator simulator;
        simulator.play(std::cin, std::cout, simCtx);
    }

    return 0;
}

#pragma clang diagnostic pop


