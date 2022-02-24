//
// Created by kmillard on 2/23/2022.
//

#include <sts/game/deck.hpp>

#include <catch2/catch_all.hpp>

using namespace sts;


Deck getDefaultDeck() {
    Deck deck;
    for (int i = 0; i < 5; ++i) {
        deck.obtainSimple(CardId::STRIKE_RED);
    }
    for (int i = 0; i < 4; ++i) {
        deck.obtainSimple(CardId::DEFEND_RED);
    }
    deck.obtainSimple(CardId::BASH);
    return deck;
}

TEST_CASE("Setup Default Deck") {
    const auto deck = getDefaultDeck();

    CHECK(deck.size() == 10);
    CHECK(deck.hasCurse() == false);
    CHECK(deck.hasCardForWingStatue() == false);
    CHECK(deck.hasBottledCard() == false);
    CHECK(deck.cardIsBottled(0) == false);

    CHECK(deck.getUpgradeableCount() == 10);
    CHECK(deck.getUpgradeableCardIdxs().size() == 10);

    CHECK(deck.getTransformableCount(false) == 10);
    CHECK(deck.getTransformableCount(true) == 10);
    CHECK(deck.getIdxsMatching([](auto c) { return c.getType() == CardType::SKILL; }).size() == 4);
    CHECK(deck.getCountMatching([](auto c) { return c.getType() == CardType::ATTACK; }) == 6);
}

TEST_CASE("Deck upgrade cards test") {
    auto deck = getDefaultDeck();
    deck.upgrade(0);

    REQUIRE(deck.getUpgradeableCount() == 9);
    REQUIRE(deck.getUpgradeableCardIdxs().size() == 9);

    deck.upgradeStrikesAndDefends();

    REQUIRE(deck.getUpgradeableCount() == 1);
    REQUIRE(deck.getUpgradeableCardIdxs().size() == 1);
}

TEST_CASE("Deck bottle card test") {
    auto deck = getDefaultDeck();
    deck.bottleCard(0, sts::CardType::ATTACK);

    CHECK(deck.hasBottledCard() == true);
    CHECK(deck.cardIsBottled(0) == true);
}
