//
// Created by keega on 2/21/2022.
//

#include <sts/game/card.hpp>

#include <catch2/catch_all.hpp>

namespace sts {

    TEST_CASE("Default Searing Blow test") {
        Card card(CardId::SEARING_BLOW);


        REQUIRE(card.id == CardId::SEARING_BLOW);
        REQUIRE(card.isUpgraded() == false);
        REQUIRE(card.getUpgraded() == 0);

        REQUIRE(card.getBaseDamage() == 12);

        REQUIRE(card.canTransform() == true);
        REQUIRE(card.canUpgrade() == true);

        REQUIRE(card.isInnate() == false);
        REQUIRE(card.isStrikeCard() == false);
        REQUIRE(card.isStarterStrike() == false);
        REQUIRE(card.isStarterStrikeOrDefend() == false);
    }

    TEST_CASE("Upgraded Searing Blow test") {
        Card card(CardId::SEARING_BLOW, 1);

        card.upgrade();

        REQUIRE(card.id == CardId::SEARING_BLOW);
        REQUIRE(card.isUpgraded() == true);
        REQUIRE(card.getUpgraded() == 2);

        REQUIRE(card.getBaseDamage() == 21);

        REQUIRE(card.canTransform() == true);
        REQUIRE(card.canUpgrade() == true);

        REQUIRE(card.isInnate() == false);
        REQUIRE(card.isStrikeCard() == false);
        REQUIRE(card.isStarterStrike() == false);
        REQUIRE(card.isStarterStrikeOrDefend() == false);
    }

    TEST_CASE("Ascender's Bane test") {
        Card card(CardId::ASCENDERS_BANE);

        REQUIRE(card.id == CardId::ASCENDERS_BANE);
        REQUIRE(card.isUpgraded() == false);
        REQUIRE(card.getUpgraded() == 0);

        REQUIRE(card.getBaseDamage() == -1);

        REQUIRE(card.canTransform() == false);
        REQUIRE(card.canUpgrade() == false);

        REQUIRE(card.isInnate() == false);
        REQUIRE(card.isStrikeCard() == false);
        REQUIRE(card.isStarterStrike() == false);
        REQUIRE(card.isStarterStrikeOrDefend() == false);
    }

    TEST_CASE("Strike test") {
        Card card(CardId::STRIKE_RED);

        REQUIRE(card.isUpgraded() == false);
        REQUIRE(card.getUpgraded() == 0);

        REQUIRE(card.getBaseDamage() == 6);

        REQUIRE(card.canTransform() == true);
        REQUIRE(card.canUpgrade() == true);

        REQUIRE(card.isInnate() == false);
        REQUIRE(card.isStrikeCard() == true);
        REQUIRE(card.isStarterStrike() == true);
        REQUIRE(card.isStarterStrikeOrDefend() == true);
    }

    TEST_CASE("Meteor Strike construct upgraded test") {
        Card card(CardId::METEOR_STRIKE, 1);

        REQUIRE(card.isUpgraded() == true);

        REQUIRE(card.getUpgraded() == 1);
        REQUIRE(card.getBaseDamage() == 30);
        REQUIRE(card.canTransform() == true);
        REQUIRE(card.canUpgrade() == false);
        REQUIRE(card.isInnate() == false);
        REQUIRE(card.isStrikeCard() == true);
        REQUIRE(card.isStarterStrike() == false);
        REQUIRE(card.isStarterStrikeOrDefend() == false);
    }

    TEST_CASE("Meteor Strike upgrade test") {
        Card card(CardId::METEOR_STRIKE);
        card.upgrade();

        REQUIRE(card.isUpgraded() == true);
        REQUIRE(card.getUpgraded() == 1);
        REQUIRE(card.getBaseDamage() == 30);
        REQUIRE(card.canTransform() == true);
        REQUIRE(card.canUpgrade() == false);
        REQUIRE(card.isInnate() == false);
        REQUIRE(card.isStrikeCard() == true);
        REQUIRE(card.isStarterStrike() == false);
        REQUIRE(card.isStarterStrikeOrDefend() == false);
    }

    TEST_CASE("equality test") {
        Card card(CardId::METEOR_STRIKE);
        card.upgrade();
        Card card2(CardId::METEOR_STRIKE, 1);
        REQUIRE(card == card2);
    }

}
