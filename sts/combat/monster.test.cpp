//
// Created by keega on 2/24/2022.
//

#include <sts/combat/monster.hpp>

#include <catch2/catch_all.hpp>

using namespace sts;

TEST_CASE("Monster test") {
    Monster monster;
    monster.id == MonsterId::GREEN_LOUSE;
    monster.curHp = 10;
    monster.maxHp = 10;

    CHECK(monster.firstTurn() == true);
    CHECK(monster.isAlive() == true);
    CHECK(monster.isDying() == false);

    CHECK(monster.block == 0);

    CHECK(monster.getStatus<MS::CHOKED>() == false);
    CHECK(monster.hasStatus<MS::CHOKED>() == false);

    CHECK(monster.getStatus<MS::CORPSE_EXPLOSION>() == false);
    CHECK(monster.hasStatus<MS::CHOKED>() == false);
}
