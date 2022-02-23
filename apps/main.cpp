#include <iostream>

#include <sts/sim/console_simulator.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
using namespace sts;

#include <sts/game/misc.hpp>

void doSomething() {

    for (auto seed = 1000ULL; seed < 1000000000; ++seed) {

        Random neowRng(seed);
        auto options = Neow::getOptions(neowRng);

        if (options[2].r == Neow::Bonus::THREE_RARE_CARDS) {
            std::cout << SeedHelper::getString(seed) << '\n';
            break;
        }
    }
}


int main() {
    doSomething();

    while (true) {
        std::cout << "usage: seed character(I/S/D/W) ascensionLevel" << std::endl;

        SimulatorContext simCtx;
        ConsoleSimulator simulator;
        simulator.play(std::cin, std::cout, simCtx);
    }

    return 0;
}

#pragma clang diagnostic pop


