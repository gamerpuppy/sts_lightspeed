#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <thread>

#include "combat/BattleContext.h"
#include "convert/BattleConverter.h"
#include "sim/ConsoleSimulator.h"
#include "sim/search/BattleScumSearcher2.h"

#include <nlohmann/json.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
using namespace sts;
using namespace std::chrono;

void search2(search::BattleScumSearcher2 &searcher, int simulations) {
    searcher.search(simulations, 1000);
}

int main() {
    std::string jsonString;
    std::getline(std::cin, jsonString);

    nlohmann::json json = nlohmann::json::parse(jsonString);
    BattleConverter converter;
    int monsterIdxMap[5];
    for (int i = 0; i < 5; ++i) {
        monsterIdxMap[i] = -1;
    }
    BattleContext bc = converter.convertFromJson(json, monsterIdxMap);

    milliseconds ms1 = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    // std::cout << bc << std::endl;
    int thread_count = 12;
    search::BattleScumSearcher2 *searchers[thread_count];
    std::thread *threads[thread_count];
    for (int i = 0; i < thread_count; ++i) {
        int rngMod = i;
        bc.cardRandomRng.setCounter(rngMod * 1000);
        bc.aiRng.setCounter(rngMod * 1000);
        bc.shuffleRng.setCounter(rngMod * 1000);
        bc.miscRng.setCounter(rngMod * 1000);
        bc.potionRng.setCounter(rngMod * 1000);
        searchers[i] = new search::BattleScumSearcher2(bc);
        threads[i] = new std::thread(search2, std::ref(*searchers[i]), 100000);
    }
    for (int i = 0; i < thread_count; ++i) {
        threads[i]->join();
    }
    milliseconds ms2 = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    // std::cout << "took " << (ms2 - ms1).count() << "ms\n";

    // std::cout << searcher.bestActionValue << '\n';
    // std::cout << searcher.root.simulationCount << '\n';
    // searcher.bestActionSequence[0].printDesc(std::cout, bc);
    // std::cout << '\n' << bc << '\n';
    // searcher.printSearchTree(std::cout, 1);

    auto best = searchers[0]->root.edges[0].action;
    int mostVisits = 0;
    std::ofstream outfile;
    outfile.open("test.log", std::ios_base::app);
    outfile << "====" << std::endl;
    outfile << bc << std::endl;
    for (int j = 0; j < searchers[0]->root.edges.size(); ++j) {
        double evaluationSum = 0;
        int simulationCount = 0;
        for (int i = 0; i < thread_count; ++i) {
            evaluationSum += searchers[i]->root.edges[j].node.evaluationSum;
            simulationCount += searchers[i]->root.edges[j].node.simulationCount;
        }
        outfile << simulationCount << " visits / " << std::fixed << std::setprecision(3) << (evaluationSum / simulationCount) << " value for ";
        searchers[0]->root.edges[j].action.printDesc(outfile, bc);
        outfile << std::endl;
        if (simulationCount > mostVisits) {
            mostVisits = simulationCount;
            best = searchers[0]->root.edges[j].action;
        }
    }
    nlohmann::json monsterIdxMapJson = nlohmann::json::array();
    for (int i = 0; i < 5; ++i) {
        monsterIdxMapJson[i] = monsterIdxMap[i];
    }

    std::cout << static_cast<int>(best.getActionType()) << " " << best.getSourceIdx() << " " << best.getTargetIdx() << " " << monsterIdxMapJson.dump() << std::endl;
    return 0;
}

#pragma clang diagnostic pop