//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_MONSTERENCOUNTERS_H
#define STS_LIGHTSPEED_MONSTERENCOUNTERS_H

#include <cstdint>

namespace sts {

    enum class MonsterEncounter : std::int8_t {
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

    static constexpr const char * const monsterEncouterNames[] = {
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

    static constexpr const char *monsterEncounterEnumNames[] = {"INVALID","CULTIST","JAW_WORM","TWO_LOUSE","SMALL_SLIMES","BLUE_SLAVER","GREMLIN_GANG","LOOTER","LARGE_SLIME","LOTS_OF_SLIMES","EXORDIUM_THUGS","EXORDIUM_WILDLIFE","RED_SLAVER","THREE_LOUSE","TWO_FUNGI_BEASTS","GREMLIN_NOB","LAGAVULIN","THREE_SENTRIES","SLIME_BOSS","THE_GUARDIAN","HEXAGHOST","SPHERIC_GUARDIAN","CHOSEN","SHELL_PARASITE","THREE_BYRDS","TWO_THIEVES","CHOSEN_AND_BYRDS","SENTRY_AND_SPHERE","SNAKE_PLANT","SNECKO","CENTURION_AND_HEALER","CULTIST_AND_CHOSEN","THREE_CULTIST","SHELLED_PARASITE_AND_FUNGI","GREMLIN_LEADER","SLAVERS","BOOK_OF_STABBING","AUTOMATON","COLLECTOR","CHAMP","THREE_DARKLINGS","ORB_WALKER","THREE_SHAPES","SPIRE_GROWTH","TRANSIENT","FOUR_SHAPES","MAW","SPHERE_AND_TWO_SHAPES","JAW_WORM_HORDE","WRITHING_MASS","GIANT_HEAD","NEMESIS","REPTOMANCER","AWAKENED_ONE","TIME_EATER","DONU_AND_DECA","SHIELD_AND_SPEAR","THE_HEART","LAGAVULIN_EVENT","COLOSSEUM_EVENT_SLAVERS","COLOSSEUM_EVENT_NOBS","MASKED_BANDITS_EVENT","MUSHROOMS_EVENT","MYSTERIOUS_SPHERE_EVENT",};
    typedef MonsterEncounter ME;

    namespace MonsterEncounterPool {
        static constexpr MonsterEncounter weakEnemies[3][5] = {
                { ME::CULTIST, ME::JAW_WORM, ME::TWO_LOUSE, ME::SMALL_SLIMES },
                { ME::SPHERIC_GUARDIAN, ME::CHOSEN, ME::SHELL_PARASITE, ME::THREE_BYRDS, ME::TWO_THIEVES },
                { ME::THREE_DARKLINGS, ME::ORB_WALKER, ME::THREE_SHAPES },
        };
        static constexpr float weakWeights[3][5] = {
                { 1.0f/4, 1.0f/4, 1.0f/4, 1.0f/4 },
                { 1.0f/5, 1.0f/5, 1.0f/5, 1.0f/5, 1.0f/5 },
                { 1.0f/3, 1.0f/3, 1.0f/3 }
        };
        constexpr int weakCount[3] {4,5,3};

        static constexpr MonsterEncounter strongEnemies[3][10] = {
                { ME::GREMLIN_GANG, ME::LOTS_OF_SLIMES, ME::RED_SLAVER, ME::EXORDIUM_THUGS, ME::EXORDIUM_WILDLIFE, ME::BLUE_SLAVER, ME::LOOTER, ME::LARGE_SLIME, ME::THREE_LOUSE, ME::TWO_FUNGI_BEASTS },
                { ME::CHOSEN_AND_BYRDS, ME::SENTRY_AND_SPHERE, ME::CULTIST_AND_CHOSEN, ME::THREE_CULTIST, ME::SHELLED_PARASITE_AND_FUNGI, ME::SNECKO, ME::SNAKE_PLANT, ME::CENTURION_AND_HEALER },
                { ME::SPIRE_GROWTH, ME::TRANSIENT, ME::FOUR_SHAPES, ME::MAW, ME::SPHERE_AND_TWO_SHAPES, ME::JAW_WORM_HORDE, ME::THREE_DARKLINGS, ME::WRITHING_MASS }
        };
        static constexpr float strongWeights[3][10] = {
                { 1.0f/16, 1.0f/16, 1.0f/16, 1.5f/16, 1.5f/16, 2.0f/16, 2.0f/16, 2.0f/16, 2.0f/16, 2.0f/16 },
                { 2.0f/29, 2.0f/29, 3.0f/29, 3.0f/29, 3.0f/29, 4.0f/29, 6.0f/29, 6.0f/29 },
                { 1.0f/8, 1.0f/8, 1.0f/8, 1.0f/8, 1.0f/8, 1.0f/8, 1.0f/8, 1.0f/8,},
        };
        static constexpr int strongCount[3] {10, 8, 8};

        static const MonsterEncounter elites[3][3] = {
                { ME::GREMLIN_NOB, ME::LAGAVULIN, ME::THREE_SENTRIES },
                { ME::GREMLIN_LEADER, ME::SLAVERS, ME::BOOK_OF_STABBING },
                { ME::GIANT_HEAD, ME::NEMESIS, ME::REPTOMANCER },
        };
    }

    static constexpr bool isBossEncounter(MonsterEncounter e) {
        return e == sts::MonsterEncounter::SLIME_BOSS ||
               e == sts::MonsterEncounter::HEXAGHOST ||
               e == sts::MonsterEncounter::THE_GUARDIAN ||
               e == sts::MonsterEncounter::CHAMP ||
               e == sts::MonsterEncounter::COLLECTOR ||
               e == sts::MonsterEncounter::AUTOMATON ||
               e == sts::MonsterEncounter::DONU_AND_DECA ||
               e == sts::MonsterEncounter::TIME_EATER ||
               e == sts::MonsterEncounter::AWAKENED_ONE ||
               e == sts::MonsterEncounter::THE_HEART;
    }




}

#endif //STS_LIGHTSPEED_MONSTERENCOUNTERS_H
