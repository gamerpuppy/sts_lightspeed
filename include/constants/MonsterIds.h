//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_MONSTERIDS_H
#define STS_LIGHTSPEED_MONSTERIDS_H

#include <cstdint>
#include <algorithm>
#include <array>
#include <cassert>
#include <string>

namespace sts {

    enum class MonsterId : std::uint8_t {
        INVALID = 0,
        ACID_SLIME_L,
        ACID_SLIME_M,
        ACID_SLIME_S,
        AWAKENED_ONE,
        BEAR,
        BLUE_SLAVER,
        BOOK_OF_STABBING,
        BRONZE_AUTOMATON,
        BRONZE_ORB,
        BYRD,
        CENTURION,
        CHOSEN,
        CORRUPT_HEART,
        CULTIST,
        DAGGER,
        DARKLING,
        DECA,
        DONU,
        EXPLODER,
        FAT_GREMLIN,
        FUNGI_BEAST,
        GIANT_HEAD,
        GREEN_LOUSE,
        GREMLIN_LEADER,
        GREMLIN_NOB,
        GREMLIN_WIZARD,
        HEXAGHOST,
        JAW_WORM,
        LAGAVULIN,
        LOOTER,
        MAD_GREMLIN,
        MUGGER,
        MYSTIC,
        NEMESIS,
        ORB_WALKER,
        POINTY,
        RED_LOUSE,
        RED_SLAVER,
        REPTOMANCER,
        REPULSOR,
        ROMEO,
        SENTRY,
        SHELLED_PARASITE,
        SHIELD_GREMLIN,
        SLIME_BOSS,
        SNAKE_PLANT,
        SNEAKY_GREMLIN,
        SNECKO,
        SPHERIC_GUARDIAN,
        SPIKER,
        SPIKE_SLIME_L,
        SPIKE_SLIME_M,
        SPIKE_SLIME_S,
        SPIRE_GROWTH,
        SPIRE_SHIELD,
        SPIRE_SPEAR,
        TASKMASTER,
        THE_CHAMP,
        THE_COLLECTOR,
        THE_GUARDIAN,
        THE_MAW,
        TIME_EATER,
        TORCH_HEAD,
        TRANSIENT,
        WRITHING_MASS,
    };

    static constexpr const char* const monsterIdIds[] = {
        "INVALID",
        "AcidSlime_L",
        "AcidSlime_M",
        "AcidSlime_S",
        "AwakenedOne",
        "BanditBear",
        "SlaverBlue",
        "BookOfStabbing",
        "BronzeAutomaton",
        "BronzeOrb",
        "Byrd",
        "Centurion",
        "Chosen",
        "CorruptHeart",
        "Cultist",
        "Dagger",
        "Darkling",
        "Deca",
        "Donu",
        "Exploder",
        "GremlinFat",
        "FungiBeast",
        "GiantHead",
        "FuzzyLouseDefensive",
        "GremlinLeader",
        "GremlinNob",
        "GremlinWizard",
        "Hexaghost",
        "JawWorm",
        "Lagavulin",
        "Looter",
        "GremlinWarrior",
        "Mugger",
        "Healer",
        "Nemesis",
        "Orb Walker",
        "BanditChild",
        "FuzzyLouseNormal",
        "SlaverRed",
        "Reptomancer",
        "Repulsor",
        "BanditLeader",
        "Sentry",
        "Shelled Parasite",
        "GremlinTsundere",
        "SlimeBoss",
        "SnakePlant",
        "GremlinThief",
        "Snecko",
        "SphericGuardian",
        "Spiker",
        "SpikeSlime_L",
        "SpikeSlime_M",
        "SpikeSlime_S",
        "Serpent",
        "SpireShield",
        "SpireSpear",
        "SlaverBoss",
        "Champ",
        "TheCollector",
        "TheGuardian",
        "Maw",
        "TimeEater",
        "TorchHead",
        "Transient",
        "WrithingMass",
    };

    static constexpr const char* const  monsterIdEnumNames[] = {
            "INVALID",
            "ACID_SLIME_L",
            "ACID_SLIME_M",
            "ACID_SLIME_S",
            "AWAKENED_ONE",
            "BEAR",
            "BLUE_SLAVER",
            "BOOK_OF_STABBING",
            "BRONZE_AUTOMATON",
            "BRONZE_ORB",
            "BYRD",
            "CENTURION",
            "CHOSEN",
            "CORRUPT_HEART",
            "CULTIST",
            "DAGGER",
            "DARKLING",
            "DECA",
            "DONU",
            "EXPLODER",
            "FAT_GREMLIN",
            "FUNGI_BEAST",
            "GIANT_HEAD",
            "GREEN_LOUSE",
            "GREMLIN_LEADER",
            "GREMLIN_NOB",
            "GREMLIN_WIZARD",
            "HEXAGHOST",
            "JAW_WORM",
            "LAGAVULIN",
            "LOOTER",
            "MAD_GREMLIN",
            "MUGGER",
            "MYSTIC",
            "NEMESIS",
            "ORB_WALKER",
            "POINTY",
            "RED_LOUSE",
            "RED_SLAVER",
            "REPTOMANCER",
            "REPULSOR",
            "ROMEO",
            "SENTRY",
            "SHELLED_PARASITE",
            "SHIELD_GREMLIN",
            "SLIME_BOSS",
            "SNAKE_PLANT",
            "SNEAKY_GREMLIN",
            "SNECKO",
            "SPHERIC_GUARDIAN",
            "SPIKER",
            "SPIKE_SLIME_L",
            "SPIKE_SLIME_M",
            "SPIKE_SLIME_S",
            "SPIRE_GROWTH",
            "SPIRE_SHIELD",
            "SPIRE_SPEAR",
            "TASKMASTER",
            "THE_CHAMP",
            "THE_COLLECTOR",
            "THE_GUARDIAN",
            "THE_MAW",
            "TIME_EATER",
            "TORCH_HEAD",
            "TRANSIENT",
            "WRITHING_MASS",
    };

    static const MonsterId getMonsterIdFromId(std::string id) {
        auto it = std::find(std::begin(monsterIdIds), std::end(monsterIdIds), id);
        if (it == std::end(monsterIdIds)) return MonsterId::INVALID;
        auto idx = it - std::begin(monsterIdIds);
        return static_cast<MonsterId>(idx);
    }

    constexpr int monsterHpRange[][2][2] = {
            {{0,0},{0,0}}, // INVALID = 0
            {{65,69},{68,72}}, // ACID_SLIME_L
            {{28,32},{29,34}}, // ACID_SLIME_M
            {{8,12},{9,13}}, // ACID_SLIME_S
            {{300,300},{300,320}}, // AWAKENED_ONE
            {{38,52},{40,44}}, // BEAR
            {{46,50},{48,52}}, // BLUE_SLAVER
            {{160,164},{168,172}}, // BOOK_OF_STABBING
            {{300,300},{320,320}}, // BRONZE_AUTOMATON
            {{52,58},{54,60}}, // BRONZE_ORB
            {{25,31},{26,33}}, // BYRD
            {{76,80},{76,83}}, // CENTURION
            {{95,99},{98,103}}, // CHOSEN
            {{750,750},{800,800}}, // CORRUPT_HEART
            {{48,54},{50,56}}, // CULTIST
            {{20,25},{20,25}}, // DAGGER
            {{48,56},{50,59}}, // DARKLING
            {{250,250},{265,265}}, // DECA
            {{250,250},{265,265}}, // DONU
            {{30,30},{30,35}}, // EXPLODER
            {{13,17},{14,18}}, // FAT_GREMLIN
            {{22,28},{24,28}}, // FUNGI_BEAST
            {{500,500},{520,520}}, // GIANT_HEAD
            {{11,17},{12,18}}, // GREEN_LOUSE
            {{140,148},{145,155}}, // GREMLIN_LEADER
            {{82,86},{85,90}}, // GREMLIN_NOB
            {{21,25},{22,26}}, // GREMLIN_WIZARD
            {{250,250},{264,264}}, // HEXAGHOST
            {{40,44},{42,46}}, // JAW_WORM
            {{109,111},{112,115}}, // LAGAVULIN
            {{44,48},{46,50}}, // LOOTER
            {{20,24},{21,25}}, // MAD_GREMLIN
            {{48,52},{50,54}}, // MUGGER
            {{48,56},{50,58}}, // MYSTIC
            {{185,185},{200,200}}, // NEMESIS
            {{90,96},{92,102}}, // ORB_WALKER
            {{30,30},{34,34}}, // POINTY
            {{10,15},{11,16}}, // RED_LOUSE
            {{46,50},{48,52}}, // RED_SLAVER
            {{180,190},{190,200}}, // REPTOMANCER
            {{29,35},{31,38}}, // REPULSOR
            {{35,39},{37,41}}, // ROMEO
            {{38,42},{39,45}}, // SENTRY
            {{68,72},{70,75}}, // SHELLED_PARASITE
            {{12,15},{13,17}}, // SHIELD_GREMLIN
            {{140,140},{150,150}}, // SLIME_BOSS
            {{75,79},{78,82}}, // SNAKE_PLANT
            {{10,14},{11,15}}, // SNEAKY_GREMLIN
            {{114,120},{120,125}}, // SNECKO
            {{20,20},{20,20}}, // SPHERIC_GUARDIAN
            {{42,56},{44,60}}, // SPIKER
            {{64,70},{67,73}}, // SPIKE_SLIME_L
            {{28,32},{29,34}}, // SPIKE_SLIME_M
            {{10,14},{11,15}}, // SPIKE_SLIME_S
            {{170,170},{190,190}}, // SPIRE_GROWTH
            {{110,110},{125,125}}, // SPIRE_SHIELD
            {{160,160},{180,180}}, // SPIRE_SPEAR
            {{54,60},{57,64}}, // TASKMASTER
            {{420,420},{440,440}}, // THE_CHAMP
            {{282,282},{300,300}}, // THE_COLLECTOR
            {{240,240},{250,250}}, // THE_GUARDIAN
            {{300,300},{300,300}}, // THE_MAW
            {{456,456},{480,480}}, // TIME_EATER
            {{38,40},{40,45}}, // TORCH_HEAD
            {{999,999},{999,999}}, // TRANSIENT
            {{160,160},{175,175}}, // WRITHING_MASS
    };

}


#endif //STS_LIGHTSPEED_MONSTERIDS_H
