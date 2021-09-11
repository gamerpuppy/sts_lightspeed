//
// Created by keega on 9/9/2021.
//

#include "game/SaveFile.h"
#include "constants/Cards.h"

#include <vector>
#include <algorithm>

using namespace sts;

struct CardIdSortOrder {
    CardId cardId;
    std::string sid;
};

void printCardIdOrder() {

    std::vector<CardIdSortOrder> vec;

    for (int i = static_cast<int>(CardId::INVALID); i <= static_cast<int>(CardId::ZAP); ++i) {
        auto id = static_cast<CardId>(i);
        std::string sid(getCardStringId(id));
        vec.push_back({id, sid});
    }

    std::stable_sort(vec.begin(), vec.end(), [](auto c0, auto c1) { return c0.sid < c1.sid; } );

    int sortedValues[371];
    for (int i = 0; i < vec.size(); ++i) {
        sortedValues[static_cast<int>(vec[i].cardId)] = i;
    }

    std::cout << "static constexpr int cardIdSortedIdx[] = {";
    for (auto x : sortedValues) {
        std::cout << x << ",";
    }
    std::cout << "};\n";
}



int main(int argc, const char *argv[]) {
    printCardIdOrder();
//    std::cout << sts::Base64::encode(argv[1]);

    return 0;
}