//
// Created by keega on 2/22/2022.
//

#include <sts/game/game_context.hpp>

#include <catch2/catch_all.hpp>

#include <sts/game/misc.hpp>

using namespace sts;

TEST_CASE("GameContext init 1") {
    GameContext g(sts::CharacterClass::DEFECT, SeedHelper::getLong("LEWD"), 0);

    CHECK(g.curHp == 75);
    CHECK(g.maxHp == 75);
    CHECK(g.gold == 99);

    CHECK(g.potionCount == 0);
    CHECK(g.potionCapacity == 3);

    CHECK(g.rareRelicPool.front() == RelicId::STONE_CALENDAR);
    CHECK(g.bossRelicPool.front() == RelicId::INSERTER);

    CHECK(g.blueKey == false);
    CHECK(g.redKey == false);
    CHECK(g.greenKey == false);

    CHECK(g.getMonsterForRoomCreation() == MonsterEncounter::TWO_LOUSE);
}


TEST_CASE("GameContext simple manipulation") {
    GameContext g(sts::CharacterClass::DEFECT, SeedHelper::getLong("LEWD"), 0);

    {
        CHECK(g.curHp == 75);
        CHECK(g.maxHp == 75);

        g.damagePlayer(5);

        CHECK(g.curHp == 70);
        CHECK(g.maxHp == 75);

        g.playerLoseHp(5);

        CHECK(g.curHp == 65);
        CHECK(g.maxHp == 75);

        g.loseMaxHp(9);

        CHECK(g.curHp == 65);
        CHECK(g.maxHp == 66);

        g.loseMaxHp(11);

        CHECK(g.curHp == 55);
        CHECK(g.maxHp == 55);
    }


    {
        CHECK(g.gold == 99);

        g.obtainGold(10);

        CHECK(g.gold == 109);
    }

    {
        CHECK(g.potionCount == 0);
        CHECK(g.potionCapacity == 3);

        g.obtainPotion(Potion::WEAK_POTION);

        CHECK(g.potionCount == 1);
        CHECK(g.potionCapacity == 3);
    }

    {
        CHECK(g.blueKey == false);
        CHECK(g.redKey == false);
        CHECK(g.greenKey == false);

        g.obtainKey(Key::EMERALD_KEY);

        CHECK(g.blueKey == false);
        CHECK(g.redKey == false);
        CHECK(g.greenKey == true);
    }

    {
        CHECK(g.blueKey == false);
        CHECK(g.redKey == false);
        CHECK(g.greenKey == true);

        g.obtainKey(Key::SAPPHIRE_KEY);

        CHECK(g.blueKey == true);
        CHECK(g.redKey == false);
        CHECK(g.greenKey == true);
    }

    {
        CHECK(g.blueKey == true);
        CHECK(g.redKey == false);
        CHECK(g.greenKey == true);

        g.obtainKey(Key::RUBY_KEY);

        CHECK(g.blueKey == true);
        CHECK(g.redKey == true);
        CHECK(g.greenKey == true);
    }




}