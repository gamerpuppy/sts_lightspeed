//
// Created by keega on 2/22/2022.
//

#include <sts/game/neow.hpp>
#include <sts/game/misc.hpp>

#include <catch2/catch_all.hpp>

using namespace sts;

TEST_CASE("Neow Options 1") {

    Random neowRng(SeedHelper::getLong("ASDF"));
    auto options = Neow::getOptions(neowRng);

    REQUIRE( options[0] == Neow::Option{Neow::Bonus::UPGRADE_CARD, Neow::Drawback::NONE} );
    REQUIRE( options[1] == Neow::Option{Neow::Bonus::THREE_ENEMY_KILL, Neow::Drawback::NONE} );
    REQUIRE( options[2] == Neow::Option{Neow::Bonus::REMOVE_TWO, Neow::Drawback::TEN_PERCENT_HP_LOSS} );
    REQUIRE( options[3] == Neow::Option{Neow::Bonus::BOSS_RELIC, Neow::Drawback::LOSE_STARTER_RELIC} );

}

TEST_CASE("Neow Options 2") {

    Random neowRng(SeedHelper::getLong("QWERTY"));
    auto options = Neow::getOptions(neowRng);

    REQUIRE( options[0] == Neow::Option{Neow::Bonus::TRANSFORM_CARD, Neow::Drawback::NONE} );
    REQUIRE( options[1] == Neow::Option{Neow::Bonus::TEN_PERCENT_HP_BONUS, Neow::Drawback::NONE} );
    REQUIRE( options[2] == Neow::Option{Neow::Bonus::REMOVE_TWO, Neow::Drawback::TEN_PERCENT_HP_LOSS} );
    REQUIRE( options[3] == Neow::Option{Neow::Bonus::BOSS_RELIC, Neow::Drawback::LOSE_STARTER_RELIC} );
}

TEST_CASE("Neow Options 3") {

    Random neowRng(SeedHelper::getLong("123ASDFGH"));
    auto options = Neow::getOptions(neowRng);

    REQUIRE( options[0] == Neow::Option{Neow::Bonus::UPGRADE_CARD, Neow::Drawback::NONE} );
    REQUIRE( options[1] == Neow::Option{Neow::Bonus::THREE_ENEMY_KILL, Neow::Drawback::NONE} );
    REQUIRE( options[2] == Neow::Option{Neow::Bonus::TRANSFORM_TWO_CARDS, Neow::Drawback::CURSE} );
    REQUIRE( options[3] == Neow::Option{Neow::Bonus::BOSS_RELIC, Neow::Drawback::LOSE_STARTER_RELIC} );
}

TEST_CASE("Neow Options 4") {

    Random neowRng(SeedHelper::getLong("ASDFASDF"));
    auto options = Neow::getOptions(neowRng);

    REQUIRE( options[0] == Neow::Option{Neow::Bonus::ONE_RANDOM_RARE_CARD, Neow::Drawback::NONE} );
    REQUIRE( options[1] == Neow::Option{Neow::Bonus::RANDOM_COMMON_RELIC, Neow::Drawback::NONE} );
    REQUIRE( options[2] == Neow::Option{Neow::Bonus::TWENTY_PERCENT_HP_BONUS, Neow::Drawback::NO_GOLD} );
    REQUIRE( options[3] == Neow::Option{Neow::Bonus::BOSS_RELIC, Neow::Drawback::LOSE_STARTER_RELIC} );
}

TEST_CASE("Neow Options With Colorless Reward") {

    Random neowRng(SeedHelper::getLong("ZXCVZXCVZX"));
    Random cardRng(SeedHelper::getLong("ZXCVZXCVZX"));

    auto options = Neow::getOptions(neowRng);
    auto cardReward = Neow::getColorlessCardReward(neowRng, cardRng, true);

    REQUIRE( options[0] == Neow::Option{Neow::Bonus::TRANSFORM_CARD, Neow::Drawback::NONE} );
    REQUIRE( options[1] == Neow::Option{Neow::Bonus::THREE_SMALL_POTIONS, Neow::Drawback::NONE} );
    REQUIRE( options[2] == Neow::Option{Neow::Bonus::RANDOM_COLORLESS_2, Neow::Drawback::NO_GOLD} );
    REQUIRE( options[3] == Neow::Option{Neow::Bonus::BOSS_RELIC, Neow::Drawback::LOSE_STARTER_RELIC} );

    REQUIRE(cardReward[0] == CardId::MASTER_OF_STRATEGY);
    REQUIRE(cardReward[1] == CardId::MAYHEM);
    REQUIRE(cardReward[2] == CardId::METAMORPHOSIS);
}

TEST_CASE("Neow Options With Card Reward") {
    auto seed = SeedHelper::getLong("3");

    Random neowRng(seed);

    auto options = Neow::getOptions(neowRng);
    auto cardReward = Neow::getCardReward(neowRng, CharacterClass::IRONCLAD);

    REQUIRE( options[0] == Neow::Option{Neow::Bonus::THREE_CARDS, Neow::Drawback::NONE} );
    REQUIRE( options[1] == Neow::Option{Neow::Bonus::HUNDRED_GOLD, Neow::Drawback::NONE} );
    REQUIRE( options[2] == Neow::Option{Neow::Bonus::TWO_FIFTY_GOLD, Neow::Drawback::PERCENT_DAMAGE} );
    REQUIRE( options[3] == Neow::Option{Neow::Bonus::BOSS_RELIC, Neow::Drawback::LOSE_STARTER_RELIC} );

    REQUIRE(cardReward[0] == CardId::SEEING_RED);
    REQUIRE(cardReward[1] == CardId::CLOTHESLINE);
    REQUIRE(cardReward[2] == CardId::BLOOD_FOR_BLOOD);
}

TEST_CASE("Neow Options With Rare Card Reward") {
    auto seed = SeedHelper::getLong("TK");

    Random neowRng(seed);

    auto options = Neow::getOptions(neowRng);
    auto cardReward = Neow::getCardReward(neowRng, CharacterClass::SILENT, true);

    REQUIRE( options[0] == Neow::Option{Neow::Bonus::ONE_RANDOM_RARE_CARD, Neow::Drawback::NONE} );
    REQUIRE( options[1] == Neow::Option{Neow::Bonus::THREE_SMALL_POTIONS, Neow::Drawback::NONE} );
    REQUIRE( options[2] == Neow::Option{Neow::Bonus::THREE_RARE_CARDS, Neow::Drawback::NO_GOLD} );
    REQUIRE( options[3] == Neow::Option{Neow::Bonus::BOSS_RELIC, Neow::Drawback::LOSE_STARTER_RELIC} );

    REQUIRE(cardReward[0] == CardId::GLASS_KNIFE);
    REQUIRE(cardReward[1] == CardId::A_THOUSAND_CUTS);
    REQUIRE(cardReward[2] == CardId::CORPSE_EXPLOSION);
}
