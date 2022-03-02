//
// Created by keega on 2/27/2022.
//

#include "save.hpp"

#include <catch2/catch_all.hpp>

using namespace sts;

TEST_CASE("Save File Load") {
    const auto save = SaveFile::loadFromPath("saves/2-27-01.IRONCLAD.autosave", sts::CharacterClass::IRONCLAD);

    CHECK(save.seed == 7888320651155556997ULL);
    CHECK(save.gold == 150);
    CHECK(save.act_num == 1);
    CHECK(save.floor_num == 4);

    CHECK(save.has_emerald_key == false);
    CHECK(save.has_ruby_key == false);
    CHECK(save.has_sapphire_key == false);

    CHECK(save.ascension_level == 20);
    CHECK(save.max_health == 75);
    CHECK(save.current_health == 75);

    REQUIRE(save.boss_list.size() == 3);
    CHECK(save.boss_list[0] == MonsterEncounter::THE_GUARDIAN);

    REQUIRE(save.relics.size() == 2);
    REQUIRE(save.relic_counters.size() == 2);

    CHECK(save.relics[0] == RelicId::BURNING_BLOOD);
    CHECK(save.relics[1] == RelicId::NEOWS_LAMENT);
    CHECK(save.relic_counters[1] == -2);

    REQUIRE(save.cards.size() == 14);
    CHECK(save.cards[0] == CardId::ASCENDERS_BANE);

//    CHECK(save.bottledCards[0] == CardId::INVALID);
}
