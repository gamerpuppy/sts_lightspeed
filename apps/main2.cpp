#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

#include "combat/BattleContext.h"
#include "convert/BattleConverter.h"
#include "sim/ConsoleSimulator.h"
#include "sim/search/BattleScumSearcher2.h"

#include <nlohmann/json.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
using namespace sts;
using namespace std::chrono;

int main() {
    std::string jsonString;
    //std::ifstream infile("test.json");
    // std::getline(infile, jsonString);
    std::getline(std::cin, jsonString);

    nlohmann::json json = nlohmann::json::parse(jsonString);
    BattleConverter converter;
    BattleContext bc = converter.convertFromJson(json);

    search::BattleScumSearcher2 searcher(bc);

    milliseconds ms1 = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    searcher.search(100000);
    milliseconds ms2 = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );

    // std::cout << "took " << (ms2 - ms1).count() << "ms\n";

    // std::cout << searcher.bestActionValue << '\n';
    // std::cout << searcher.root.simulationCount << '\n';
    // searcher.bestActionSequence[0].printDesc(std::cout, bc);
    // std::cout << '\n' << bc << '\n';
    // searcher.printSearchTree(std::cout, 1);

    auto best = searcher.root.edges[0].action;
    int mostVisits = 0;
    std::ofstream outfile;
    outfile.open("test.log", std::ios_base::app);
    outfile << "====" << std::endl;
    for (auto edge : searcher.root.edges) {
        outfile << edge.node.simulationCount << " visits / " << std::fixed << std::setprecision(3) << (edge.node.evaluationSum / edge.node.simulationCount) << " value for ";
        edge.action.printDesc(outfile, bc);
        outfile << std::endl;
        if (edge.node.simulationCount > mostVisits) {
            mostVisits = edge.node.simulationCount;
            best = edge.action;
            
        }
    }
    std::cout << static_cast<int>(best.getActionType()) << " " << best.getSourceIdx() << " " << best.getTargetIdx() << std::endl;
    return 0;
}

#pragma clang diagnostic pop