//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_MONSTERENCOUNTERS_H
#define STS_LIGHTSPEED_MONSTERENCOUNTERS_H

#include <cstdint>

namespace sts {

    enum class MonsterEncounter : std::uint8_t {
        INVALID=0,
        // Act 1 Weak
        CULTIST,
        JAW_WORM,
        TWO_LOUSE,
        SMALL_SLIMES,
        BLUE_SLAVER,
        GREMLIN_GANG,
        LOOTER,
        LARGE_SLIME,
        LOTS_OF_SLIMES,
        EXORDIUM_THUGS,
        EXORDIUM_WILDLIFE,
        RED_SLAVER,
        THREE_LOUSE,
        TWO_FUNGI_BEASTS,
        GREMLIN_NOB,
        LAGAVULIN,
        THREE_SENTRIES,
        SLIME_BOSS,
        THE_GUARDIAN,
        HEXAGHOST,

        // Act 2
        SPHERIC_GUARDIAN,
        CHOSEN,
        SHELL_PARASITE,
        THREE_BYRDS,
        TWO_THIEVES,
        CHOSEN_AND_BYRDS,
        SENTRY_AND_SPHERE,
        SNAKE_PLANT,
        SNECKO,
        CENTURION_AND_HEALER,
        CULTIST_AND_CHOSEN,
        THREE_CULTIST,
        SHELLED_PARASITE_AND_FUNGI,
        GREMLIN_LEADER,
        SLAVERS,
        BOOK_OF_STABBING,
        AUTOMATON,
        COLLECTOR,
        CHAMP,

        // Act 3
        THREE_DARKLINGS,
        ORB_WALKER,
        THREE_SHAPES,
        SPIRE_GROWTH,
        TRANSIENT,
        FOUR_SHAPES,
        MAW,
        SPHERE_AND_TWO_SHAPES,
        JAW_WORM_HORDE,
        WRITHING_MASS,
        GIANT_HEAD,
        NEMESIS,
        REPTOMANCER,
        AWAKENED_ONE,
        TIME_EATER,
        DONU_AND_DECA,

        // Act 4
        SHIELD_AND_SPEAR,
        THE_HEART,

        // Events
        LAGAVULIN_EVENT,
        COLOSSEUM_EVENT_SLAVERS,
        COLOSSEUM_EVENT_NOBS,
        MASKED_BANDITS_EVENT,
        MUSHROOMS_EVENT,
        MYSTERIOUS_SPHERE_EVENT,
    };

    static constexpr const char * const monsterEncounterStrings[] = {
            "INVALID",
            "Cultist",
            "Jaw Worm",
            "Two Louse",
            "Small Slimes",
            "Blue Slaver",
            "Gremlin Gang",
            "Looter",
            "Large Slime",
            "Lots Of Slimes",
            "Exordium Thugs",
            "Exordium Wildlife",
            "Red Slaver",
            "Three Louse",
            "Two Fungi Beasts",
            "Gremlin Nob",
            "Lagavulin",
            "Three Sentries",
            "Slime Boss",
            "The Guardian",
            "Hexaghost",
            "Spheric Guardian",
            "Chosen",
            "Shell Parasite",
            "Three Byrds",
            "Two Thieves",
            "Chosen And Byrds",
            "Sentry And Sphere",
            "Snake Plant",
            "Snecko",
            "Centurion And Healer",
            "Cultist And Chosen",
            "Three Cultist",
            "Shelled Parasite And Fungi",
            "Gremlin Leader",
            "Slavers",
            "Book Of Stabbing",
            "Automaton",
            "Collector",
            "Champ",
            "Three Darklings",
            "Orb Walker",
            "Three Shapes",
            "Spire Growth",
            "Transient",
            "Four Shapes",
            "Maw",
            "Sphere And Two Shapes",
            "Jaw Worm Horde",
            "Writhing Mass",
            "Giant Head",
            "Nemesis",
            "Reptomancer",
            "Awakened One",
            "Time Eater",
            "Donu And Deca",
            "Shield And Spear",
            "The Heart",
            "LAGAVULIN_EVENT",
            "COLOSSEUM_EVENT_SLAVERS",
            "COLOSSEUM_EVENT_NOBS",
            "MASKED_BANDITS_EVENT",
            "MUSHROOMS_EVENT",
            "MYSTERIOUS_SPHERE_EVENT",
    };

    static constexpr const char *monsterEncounterEnumNames[] = {"INVALID","CULTIST","JAW_WORM","TWO_LOUSE","SMALL_SLIMES","BLUE_SLAVER","GREMLIN_GANG","LOOTER","LARGE_SLIME","LOTS_OF_SLIMES","EXORDIUM_THUGS","EXORDIUM_WILDLIFE","RED_SLAVER","THREE_LOUSE","TWO_FUNGI_BEASTS","GREMLIN_NOB","LAGAVULIN","THREE_SENTRIES","SLIME_BOSS","THE_GUARDIAN","HEXAGHOST","SPHERIC_GUARDIAN","CHOSEN","SHELL_PARASITE","THREE_BYRDS","TWO_THIEVES","CHOSEN_AND_BYRDS","SENTRY_AND_SPHERE","SNAKE_PLANT","SNECKO","CENTURION_AND_HEALER","CULTIST_AND_CHOSEN","THREE_CULTIST","SHELLED_PARASITE_AND_FUNGI","GREMLIN_LEADER","SLAVERS","BOOK_OF_STABBING","AUTOMATON","COLLECTOR","CHAMP","THREE_DARKLINGS","ORB_WALKER","THREE_SHAPES","SPIRE_GROWTH","TRANSIENT","FOUR_SHAPES","MAW","SPHERE_AND_TWO_SHAPES","JAW_WORM_HORDE","WRITHING_MASS","GIANT_HEAD","NEMESIS","REPTOMANCER","AWAKENED_ONE","TIME_EATER","DONU_AND_DECA","SHIELD_AND_SPEAR","THE_HEART",};
    typedef MonsterEncounter ME;

}

#endif //STS_LIGHTSPEED_MONSTERENCOUNTERS_H
