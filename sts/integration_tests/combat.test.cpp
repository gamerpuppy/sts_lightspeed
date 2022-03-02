//
// Created by keega on 2/27/2022.
//

#include <sts/game/game_context.hpp>
#include "sts/common/save.hpp"
#include "sts/combat/battle_context.hpp"
#include <sts/sim/battle_simulator.hpp>
#include <sts/sim/console_simulator.hpp>

#include <catch2/catch_all.hpp>

using namespace sts;

const auto save1 = SaveFile::loadFromPath("saves/2-27-01.IRONCLAD.autosave", CharacterClass::IRONCLAD);

TEST_CASE("Battle init test") {
    GameContext gc(save1);
    REQUIRE(gc.screenState == ScreenState::BATTLE);
    BattleContext bc(gc);

    REQUIRE(bc.player.curHp == 75);
    REQUIRE(bc.player.maxHp == 75);
    REQUIRE(bc.potionCount == 2);
    REQUIRE(bc.potionCapacity == 2);
    REQUIRE(bc.player.gold == 150);
    REQUIRE(bc.floorNum == 4);
    REQUIRE(bc.monsters.monsterCount == 3);

    REQUIRE(bc.monsters[0].id == MonsterId::GREEN_LOUSE);
    REQUIRE(bc.monsters[0].curHp == 17);
    REQUIRE(bc.monsters[0].maxHp == 17);
    REQUIRE(bc.monsters[0].getStatus<MS::CURL_UP>() == 11);

    REQUIRE(bc.monsters[1].id == MonsterId::RED_LOUSE);
    REQUIRE(bc.monsters[1].curHp == 15);
    REQUIRE(bc.monsters[1].maxHp == 15);
    REQUIRE(bc.monsters[1].getStatus<MS::CURL_UP>() == 9);

    REQUIRE(bc.monsters[2].id == MonsterId::GREEN_LOUSE);
    REQUIRE(bc.monsters[2].curHp == 15);
    REQUIRE(bc.monsters[2].maxHp == 15);
    REQUIRE(bc.monsters[2].getStatus<MS::CURL_UP>() == 12);

    REQUIRE(bc.cards.cardsInHand == 5);
    REQUIRE(bc.player.energy == 3);
    REQUIRE(bc.player.energyPerTurn == 3);

    REQUIRE(bc.cards.hand[0].id == CardId::DUAL_WIELD);
    REQUIRE(bc.cards.hand[1].id == CardId::EVOLVE);
    REQUIRE(bc.cards.hand[2].id == CardId::BASH);
    REQUIRE(bc.cards.hand[3].id == CardId::DEFEND_RED);
    REQUIRE(bc.cards.hand[4].id == CardId::STRIKE_RED);

    REQUIRE(bc.cards.drawPile.size() == 9);
    REQUIRE(bc.cards.discardPile.empty());
}

TEST_CASE("Battle Simple Test 1") {
    ConsoleSimulator sim;
    sim.setupGameFromSaveFile(save1);
    BattleContext *bc = sim.battleSim.bc;

    auto ctx = SimulatorContext{};
    ctx.printFirstLine = false;
    ctx.printInput = false;
    ctx.printLogActions = false;
    ctx.printPrompts = false;

    sim.handleInputLine("0", std::cout, ctx);
    sim.handleInputLine("0", std::cout, ctx);

    sim.reset();


//    REQUIRE(bc->cards.hand)



}

