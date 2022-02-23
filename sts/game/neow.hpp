//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_NEOW_H
#define STS_LIGHTSPEED_NEOW_H

#include <array>
#include <sts/constants/character_classes.hpp>
#include "random.hpp"
#include <sts/game/card.hpp>

namespace sts {

    namespace Neow {

        enum class Bonus : uint16_t {
            THREE_CARDS=0,
            ONE_RANDOM_RARE_CARD,
            REMOVE_CARD,
            UPGRADE_CARD,
            TRANSFORM_CARD,
            RANDOM_COLORLESS,

            THREE_SMALL_POTIONS,
            RANDOM_COMMON_RELIC,
            TEN_PERCENT_HP_BONUS,
            THREE_ENEMY_KILL,
            HUNDRED_GOLD,

            RANDOM_COLORLESS_2,
            REMOVE_TWO,
            ONE_RARE_RELIC,
            THREE_RARE_CARDS,
            TWO_FIFTY_GOLD,
            TRANSFORM_TWO_CARDS,
            TWENTY_PERCENT_HP_BONUS,

            BOSS_RELIC,
            INVALID,
        };

        static constexpr const char* const bonusStrings[] = {

                "Choose a card to obtain.",
                "Obtain a random rare card.",
                "Remove a card.",
                "Upgrade a card.",
                "Transform a card.",
                "Choose a colorless card to obtain.",

                "Obtain three potions.",
                "Obtain a random common relic.",
                "Max Hp +10%.",
                "Obtain Neow's Lament.",
                "Obtain 100 gold.",

                "Choose a rare colorless card to obtain.",
                "Remove two cards.",
                "Obtain a random rare relic.",
                "Choose a rare card to obtain.",
                "Obtain 250 gold.",
                "Transform two cards in your cards.",
                "Max Hp +20%.",

                "Obtain a random boss relic.",
                "INVALID",
        };


        enum class Drawback : uint16_t  {
            INVALID=0,
            NONE,
            TEN_PERCENT_HP_LOSS,
            NO_GOLD,
            CURSE,
            PERCENT_DAMAGE,
            LOSE_STARTER_RELIC,
        };


        static constexpr const char* const drawbackStrings[] = {
                "INVALID",
                "",
                "Max Hp -10%.",
                "Lose all gold.",
                "Obtain a curse.",
                "Take 30% Hp damage.",
                "Lose your starter relic.",
        };


        struct Option {
            Bonus r;
            Drawback d;

            friend bool operator==(const Option &lhs, const Option &rhs);
            friend bool operator!=(const Option &lhs, const Option &rhs);
        };

        std::array<Option, 4> getOptions(Random &r);

        CardReward getCardReward(Random &rng, CharacterClass cc, bool rareOnly= false);
        CardReward getColorlessCardReward(Random &neowRng, Random& cardRng, bool rareOnly=false);

    };

    typedef std::array<Neow::Option, 4> NeowOptions;

}


#endif //STS_LIGHTSPEED_NEOW_H
