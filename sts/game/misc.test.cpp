//
// Created by keega on 2/22/2022.
//

#include <sts/game/misc.hpp>

#include <catch2/catch_all.hpp>

using namespace sts;

TEST_CASE("getRandomPotion Test 1") {

    auto seed = SeedHelper::getLong("TK");
    Random potionRng(seed);

    REQUIRE(getRandomPotion(potionRng, CharacterClass::SILENT) == Potion::WEAK_POTION);
    REQUIRE(getRandomPotion(potionRng, CharacterClass::SILENT) == Potion::COLORLESS_POTION);
    REQUIRE(getRandomPotion(potionRng, CharacterClass::SILENT) == Potion::WEAK_POTION);
}