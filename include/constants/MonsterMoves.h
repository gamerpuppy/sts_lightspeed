//
// Created by gamerpuppy on 8/22/2021.
//

#ifndef MONSTERSPECIFIC_CPP_MONSTERMOVES_H
#define MONSTERSPECIFIC_CPP_MONSTERMOVES_H

#include <cstdint>

namespace sts {

    enum class MonsterMoveId : std::uint16_t {
        INVALID=0,
        GENERIC_ESCAPE_MOVE,
        ACID_SLIME_L_CORROSIVE_SPIT,
        ACID_SLIME_L_LICK,
        ACID_SLIME_L_TACKLE,
        ACID_SLIME_L_SPLIT,
        ACID_SLIME_M_CORROSIVE_SPIT,
        ACID_SLIME_M_LICK,
        ACID_SLIME_M_TACKLE,
        ACID_SLIME_S_LICK,
        ACID_SLIME_S_TACKLE,
        AWAKENED_ONE_SLASH,
        AWAKENED_ONE_SOUL_STRIKE,
        AWAKENED_ONE_REBIRTH,
        AWAKENED_ONE_DARK_ECHO,
        AWAKENED_ONE_SLUDGE,
        AWAKENED_ONE_TACKLE,
        BEAR_BEAR_HUG,
        BEAR_LUNGE,
        BEAR_MAUL,
        BLUE_SLAVER_STAB,
        BLUE_SLAVER_RAKE,
        BOOK_OF_STABBING_MULTI_STAB,
        BOOK_OF_STABBING_SINGLE_STAB,
        BRONZE_AUTOMATON_BOOST,
        BRONZE_AUTOMATON_FLAIL,
        BRONZE_AUTOMATON_HYPER_BEAM,
        BRONZE_AUTOMATON_SPAWN_ORBS,
        BRONZE_AUTOMATON_STUNNED,
        BRONZE_ORB_BEAM,
        BRONZE_ORB_STASIS,
        BRONZE_ORB_SUPPORT_BEAM,
        BYRD_CAW,
        BYRD_FLY,
        BYRD_HEADBUTT,
        BYRD_PECK,
        BYRD_STUNNED,
        BYRD_SWOOP,
        CENTURION_SLASH,
        CENTURION_FURY,
        CENTURION_DEFEND,
        CHOSEN_POKE,
        CHOSEN_ZAP,
        CHOSEN_DEBILITATE,
        CHOSEN_DRAIN,
        CHOSEN_HEX,
        CORRUPT_HEART_DEBILITATE,
        CORRUPT_HEART_BLOOD_SHOTS,
        CORRUPT_HEART_ECHO,
        CORRUPT_HEART_BUFF,
        CULTIST_INCANTATION,
        CULTIST_DARK_STRIKE,
        DAGGER_STAB,
        DAGGER_EXPLODE,
        DARKLING_NIP,
        DARKLING_CHOMP,
        DARKLING_HARDEN,
        DARKLING_REINCARNATE,
        DARKLING_REGROW,
        DECA_SQUARE_OF_PROTECTION,
        DECA_BEAM,
        DONU_CIRCLE_OF_POWER,
        DONU_BEAM,
        EXPLODER_SLAM,
        EXPLODER_EXPLODE,
        FAT_GREMLIN_SMASH,
        FUNGI_BEAST_BITE,
        FUNGI_BEAST_GROW,
        GIANT_HEAD_COUNT,
        GIANT_HEAD_GLARE,
        GIANT_HEAD_IT_IS_TIME,
        GREEN_LOUSE_BITE,
        GREEN_LOUSE_SPIT_WEB,
        GREMLIN_LEADER_ENCOURAGE,
        GREMLIN_LEADER_RALLY,
        GREMLIN_LEADER_STAB,
        GREMLIN_NOB_BELLOW,
        GREMLIN_NOB_RUSH,
        GREMLIN_NOB_SKULL_BASH,
        GREMLIN_WIZARD_CHARGING,
        GREMLIN_WIZARD_ULTIMATE_BLAST,
        HEXAGHOST_ACTIVATE,
        HEXAGHOST_DIVIDER,
        HEXAGHOST_INFERNO,
        HEXAGHOST_SEAR,
        HEXAGHOST_TACKLE,
        HEXAGHOST_INFLAME,
        JAW_WORM_CHOMP,
        JAW_WORM_THRASH,
        JAW_WORM_BELLOW,
        LAGAVULIN_ATTACK,
        LAGAVULIN_SIPHON_SOUL,
        LAGAVULIN_SLEEP,
        LOOTER_MUG,
        LOOTER_LUNGE,
        LOOTER_SMOKE_BOMB,
        LOOTER_ESCAPE,
        MAD_GREMLIN_SCRATCH,
        MUGGER_MUG,
        MUGGER_LUNGE,
        MUGGER_SMOKE_BOMB,
        MUGGER_ESCAPE,
        MYSTIC_HEAL,
        MYSTIC_BUFF,
        MYSTIC_ATTACK_DEBUFF,
        NEMESIS_DEBUFF,
        NEMESIS_ATTACK,
        NEMESIS_SCYTHE,
        ORB_WALKER_LASER,
        ORB_WALKER_CLAW,
        POINTY_ATTACK,
        RED_LOUSE_BITE,
        RED_LOUSE_GROW,
        RED_SLAVER_STAB,
        RED_SLAVER_SCRAPE,
        RED_SLAVER_ENTANGLE,
        REPTOMANCER_SUMMON,
        REPTOMANCER_SNAKE_STRIKE,
        REPTOMANCER_BIG_BITE,
        REPULSOR_BASH,
        REPULSOR_REPULSE,
        ROMEO_MOCK,
        ROMEO_AGONIZING_SLASH,
        ROMEO_CROSS_SLASH,
        SENTRY_BEAM,
        SENTRY_BOLT,
        SHELLED_PARASITE_DOUBLE_STRIKE,
        SHELLED_PARASITE_FELL,
        SHELLED_PARASITE_STUNNED,
        SHELLED_PARASITE_SUCK,
        SHIELD_GREMLIN_PROTECT,
        SHIELD_GREMLIN_SHIELD_BASH,
        SLIME_BOSS_GOOP_SPRAY,
        SLIME_BOSS_PREPARING,
        SLIME_BOSS_SLAM,
        SLIME_BOSS_SPLIT,
        SNAKE_PLANT_CHOMP,
        SNAKE_PLANT_ENFEEBLING_SPORES,
        SNEAKY_GREMLIN_PUNCTURE,
        SNECKO_PERPLEXING_GLARE,
        SNECKO_TAIL_WHIP,
        SNECKO_BITE,
        SPHERIC_GUARDIAN_SLAM,
        SPHERIC_GUARDIAN_ACTIVATE,
        SPHERIC_GUARDIAN_HARDEN,
        SPHERIC_GUARDIAN_ATTACK_DEBUFF,
        SPIKER_CUT,
        SPIKER_SPIKE,
        SPIKE_SLIME_L_FLAME_TACKLE,
        SPIKE_SLIME_L_LICK,
        SPIKE_SLIME_L_SPLIT,
        SPIKE_SLIME_M_FLAME_TACKLE,
        SPIKE_SLIME_M_LICK,
        SPIKE_SLIME_S_TACKLE,
        SPIRE_GROWTH_QUICK_TACKLE,
        SPIRE_GROWTH_SMASH,
        SPIRE_GROWTH_CONSTRICT,
        SPIRE_SHIELD_BASH,
        SPIRE_SHIELD_FORTIFY,
        SPIRE_SHIELD_SMASH,
        SPIRE_SPEAR_BURN_STRIKE,
        SPIRE_SPEAR_PIERCER,
        SPIRE_SPEAR_SKEWER,
        TASKMASTER_SCOURING_WHIP,
        TORCH_HEAD_TACKLE,
        THE_CHAMP_DEFENSIVE_STANCE,
        THE_CHAMP_FACE_SLAP,
        THE_CHAMP_TAUNT,
        THE_CHAMP_HEAVY_SLASH,
        THE_CHAMP_GLOAT,
        THE_CHAMP_EXECUTE,
        THE_CHAMP_ANGER,
        THE_COLLECTOR_BUFF,
        THE_COLLECTOR_FIREBALL,
        THE_COLLECTOR_MEGA_DEBUFF,
        THE_COLLECTOR_SPAWN,
        THE_GUARDIAN_CHARGING_UP,
        THE_GUARDIAN_FIERCE_BASH,
        THE_GUARDIAN_VENT_STEAM,
        THE_GUARDIAN_WHIRLWIND,
        THE_GUARDIAN_DEFENSIVE_MODE,
        THE_GUARDIAN_ROLL_ATTACK,
        THE_GUARDIAN_TWIN_SLAM,
        THE_MAW_ROAR,
        THE_MAW_DROOL,
        THE_MAW_SLAM,
        THE_MAW_NOM,
        TIME_EATER_REVERBERATE,
        TIME_EATER_HEAD_SLAM,
        TIME_EATER_RIPPLE,
        TIME_EATER_HASTE,
        TRANSIENT_ATTACK,
        WRITHING_MASS_IMPLANT,
        WRITHING_MASS_FLAIL,
        WRITHING_MASS_WITHER,
        WRITHING_MASS_MULTI_STRIKE,
        WRITHING_MASS_STRONG_STRIKE,
    };

    typedef MonsterMoveId MMID;

    static constexpr const char* const  monsterMoveStrings[] = {
            "INVALID",
            "GENERIC_ESCAPE_MOVE",
            "ACID_SLIME_L_CORROSIVE_SPIT",
            "ACID_SLIME_L_LICK",
            "ACID_SLIME_L_TACKLE",
            "ACID_SLIME_L_SPLIT",
            "ACID_SLIME_M_CORROSIVE_SPIT",
            "ACID_SLIME_M_LICK",
            "ACID_SLIME_M_TACKLE",
            "ACID_SLIME_S_LICK",
            "ACID_SLIME_S_TACKLE",
            "AWAKENED_ONE_SLASH",
            "AWAKENED_ONE_SOUL_STRIKE",
            "AWAKENED_ONE_REBIRTH",
            "AWAKENED_ONE_DARK_ECHO",
            "AWAKENED_ONE_SLUDGE",
            "AWAKENED_ONE_TACKLE",
            "BEAR_BEAR_HUG",
            "BEAR_LUNGE",
            "BEAR_MAUL",
            "BLUE_SLAVER_STAB",
            "BLUE_SLAVER_RAKE",
            "BOOK_OF_STABBING_MULTI_STAB",
            "BOOK_OF_STABBING_SINGLE_STAB",
            "BRONZE_AUTOMATON_BOOST",
            "BRONZE_AUTOMATON_FLAIL",
            "BRONZE_AUTOMATON_HYPER_BEAM",
            "BRONZE_AUTOMATON_SPAWN_ORBS",
            "BRONZE_AUTOMATON_STUNNED",
            "BRONZE_ORB_BEAM",
            "BRONZE_ORB_STASIS",
            "BRONZE_ORB_SUPPORT_BEAM",
            "BYRD_CAW",
            "BYRD_FLY",
            "BYRD_HEADBUTT",
            "BYRD_PECK",
            "BYRD_STUNNED",
            "BYRD_SWOOP",
            "CENTURION_SLASH",
            "CENTURION_FURY",
            "CENTURION_DEFEND",
            "CHOSEN_POKE",
            "CHOSEN_ZAP",
            "CHOSEN_DEBILITATE",
            "CHOSEN_DRAIN",
            "CHOSEN_HEX",
            "CORRUPT_HEART_DEBILITATE",
            "CORRUPT_HEART_BLOOD_SHOTS",
            "CORRUPT_HEART_ECHO",
            "CORRUPT_HEART_BUFF",
            "CULTIST_INCANTATION",
            "CULTIST_DARK_STRIKE",
            "DAGGER_STAB",
            "DAGGER_EXPLODE",
            "DARKLING_NIP",
            "DARKLING_CHOMP",
            "DARKLING_HARDEN",
            "DARKLING_REINCARNATE",
            "DARKLING_REGROW",
            "DECA_SQUARE_OF_PROTECTION",
            "DECA_BEAM",
            "DONU_CIRCLE_OF_POWER",
            "DONU_BEAM",
            "EXPLODER_SLAM",
            "EXPLODER_EXPLODE",
            "FAT_GREMLIN_SMASH",
            "FUNGI_BEAST_BITE",
            "FUNGI_BEAST_GROW",
            "GIANT_HEAD_COUNT",
            "GIANT_HEAD_GLARE",
            "GIANT_HEAD_IT_IS_TIME",
            "GREEN_LOUSE_BITE",
            "GREEN_LOUSE_SPIT_WEB",
            "GREMLIN_LEADER_ENCOURAGE",
            "GREMLIN_LEADER_RALLY",
            "GREMLIN_LEADER_STAB",
            "GREMLIN_NOB_BELLOW",
            "GREMLIN_NOB_RUSH",
            "GREMLIN_NOB_SKULL_BASH",
            "GREMLIN_WIZARD_CHARGING",
            "GREMLIN_WIZARD_ULTIMATE_BLAST",
            "HEXAGHOST_ACTIVATE",
            "HEXAGHOST_DIVIDER",
            "HEXAGHOST_INFERNO",
            "HEXAGHOST_SEAR",
            "HEXAGHOST_TACKLE",
            "HEXAGHOST_INFLAME",
            "JAW_WORM_CHOMP",
            "JAW_WORM_THRASH",
            "JAW_WORM_BELLOW",
            "LAGAVULIN_ATTACK",
            "LAGAVULIN_SIPHON_SOUL",
            "LAGAVULIN_SLEEP",
            "LOOTER_MUG",
            "LOOTER_LUNGE",
            "LOOTER_SMOKE_BOMB",
            "LOOTER_ESCAPE",
            "MAD_GREMLIN_SCRATCH",
            "MUGGER_MUG",
            "MUGGER_LUNGE",
            "MUGGER_SMOKE_BOMB",
            "MUGGER_ESCAPE",
            "MYSTIC_HEAL",
            "MYSTIC_BUFF",
            "MYSTIC_ATTACK_DEBUFF",
            "NEMESIS_DEBUFF",
            "NEMESIS_ATTACK",
            "NEMESIS_SCYTHE",
            "ORB_WALKER_LASER",
            "ORB_WALKER_CLAW",
            "POINTY_ATTACK",
            "RED_LOUSE_BITE",
            "RED_LOUSE_GROW",
            "RED_SLAVER_STAB",
            "RED_SLAVER_SCRAPE",
            "RED_SLAVER_ENTANGLE",
            "REPTOMANCER_SUMMON",
            "REPTOMANCER_SNAKE_STRIKE",
            "REPTOMANCER_BIG_BITE",
            "REPULSOR_BASH",
            "REPULSOR_REPULSE",
            "ROMEO_MOCK",
            "ROMEO_AGONIZING_SLASH",
            "ROMEO_CROSS_SLASH",
            "SENTRY_BEAM",
            "SENTRY_BOLT",
            "SHELLED_PARASITE_DOUBLE_STRIKE",
            "SHELLED_PARASITE_FELL",
            "SHELLED_PARASITE_STUNNED",
            "SHELLED_PARASITE_SUCK",
            "SHIELD_GREMLIN_PROTECT",
            "SHIELD_GREMLIN_SHIELD_BASH",
            "SLIME_BOSS_GOOP_SPRAY",
            "SLIME_BOSS_PREPARING",
            "SLIME_BOSS_SLAM",
            "SLIME_BOSS_SPLIT",
            "SNAKE_PLANT_CHOMP",
            "SNAKE_PLANT_ENFEEBLING_SPORES",
            "SNEAKY_GREMLIN_PUNCTURE",
            "SNECKO_PERPLEXING_GLARE",
            "SNECKO_TAIL_WHIP",
            "SNECKO_BITE",
            "SPHERIC_GUARDIAN_SLAM",
            "SPHERIC_GUARDIAN_ACTIVATE",
            "SPHERIC_GUARDIAN_HARDEN",
            "SPHERIC_GUARDIAN_ATTACK_DEBUFF",
            "SPIKER_CUT",
            "SPIKER_SPIKE",
            "SPIKE_SLIME_L_FLAME_TACKLE",
            "SPIKE_SLIME_L_LICK",
            "SPIKE_SLIME_L_SPLIT",
            "SPIKE_SLIME_M_FLAME_TACKLE",
            "SPIKE_SLIME_M_LICK",
            "SPIKE_SLIME_S_TACKLE",
            "SPIRE_GROWTH_QUICK_TACKLE",
            "SPIRE_GROWTH_SMASH",
            "SPIRE_GROWTH_CONSTRICT",
            "SPIRE_SHIELD_BASH",
            "SPIRE_SHIELD_FORTIFY",
            "SPIRE_SHIELD_SMASH",
            "SPIRE_SPEAR_BURN_STRIKE",
            "SPIRE_SPEAR_PIERCER",
            "SPIRE_SPEAR_SKEWER",
            "TASKMASTER_SCOURING_WHIP",
            "TORCH_HEAD_TACKLE",
            "THE_CHAMP_DEFENSIVE_STANCE",
            "THE_CHAMP_FACE_SLAP",
            "THE_CHAMP_TAUNT",
            "THE_CHAMP_HEAVY_SLASH",
            "THE_CHAMP_GLOAT",
            "THE_CHAMP_EXECUTE",
            "THE_CHAMP_ANGER",
            "THE_COLLECTOR_BUFF",
            "THE_COLLECTOR_FIREBALL",
            "THE_COLLECTOR_MEGA_DEBUFF",
            "THE_COLLECTOR_SPAWN",
            "THE_GUARDIAN_CHARGING_UP",
            "THE_GUARDIAN_FIERCE_BASH",
            "THE_GUARDIAN_VENT_STEAM",
            "THE_GUARDIAN_WHIRLWIND",
            "THE_GUARDIAN_DEFENSIVE_MODE",
            "THE_GUARDIAN_ROLL_ATTACK",
            "THE_GUARDIAN_TWIN_SLAM",
            "THE_MAW_ROAR",
            "THE_MAW_DROOL",
            "THE_MAW_SLAM",
            "THE_MAW_NOM",
            "TIME_EATER_REVERBERATE",
            "TIME_EATER_HEAD_SLAM",
            "TIME_EATER_RIPPLE",
            "TIME_EATER_HASTE",
            "TRANSIENT_ATTACK",
            "WRITHING_MASS_IMPLANT",
            "WRITHING_MASS_FLAIL",
            "WRITHING_MASS_WITHER",
            "WRITHING_MASS_MULTI_STRIKE",
            "WRITHING_MASS_STRONG_STRIKE",
    };

    static const MonsterMoveId getMonsterMoveFromId(MonsterId monsterId, int id) {
        switch(monsterId) {
            case MonsterId::ACID_SLIME_L:
                switch(id) {
                    case 1:
                        return MMID::ACID_SLIME_L_CORROSIVE_SPIT;
                    case 2:
                        return MMID::ACID_SLIME_L_TACKLE;
                    case 3:
                        return MMID::ACID_SLIME_L_SPLIT;
                    case 4:
                        return MMID::ACID_SLIME_L_LICK;
                }
                break;
            case MonsterId::ACID_SLIME_M:
                switch(id) {
                    case 1:
                        return MMID::ACID_SLIME_M_CORROSIVE_SPIT;
                    case 2:
                        return MMID::ACID_SLIME_M_TACKLE;
                    case 4:
                        return MMID::ACID_SLIME_M_LICK;
                }
                break;
            case MonsterId::ACID_SLIME_S:
                switch(id) {
                    case 1:
                        return MMID::ACID_SLIME_S_TACKLE;
                    case 2:
                        return MMID::ACID_SLIME_S_LICK;
                }
                break;
            case MonsterId::AWAKENED_ONE:
                switch(id) {
                    case 1:
                        return MMID::AWAKENED_ONE_SLASH;
                    case 2:
                        return MMID::AWAKENED_ONE_SOUL_STRIKE;
                    case 3:
                        return MMID::AWAKENED_ONE_REBIRTH;
                    case 5:
                        return MMID::AWAKENED_ONE_DARK_ECHO;
                    case 6:
                        return MMID::AWAKENED_ONE_SLUDGE;
                    case 8:
                        return MMID::AWAKENED_ONE_TACKLE;
                }
                break;
            case MonsterId::BEAR:
                switch(id) {
                    case 1:
                        return MMID::BEAR_MAUL;
                    case 2:
                        return MMID::BEAR_BEAR_HUG;
                    case 3:
                        return MMID::BEAR_LUNGE;
                }
                break;
            case MonsterId::BLUE_SLAVER:
                switch(id) {
                    case 1:
                        return MMID::BLUE_SLAVER_STAB;
                    case 4:
                        return MMID::BLUE_SLAVER_RAKE;
                }
                break;
            case MonsterId::BOOK_OF_STABBING:
                switch(id) {
                    case 1:
                        return MMID::BOOK_OF_STABBING_MULTI_STAB;
                    case 2:
                        return MMID::BOOK_OF_STABBING_SINGLE_STAB;
                }
                break;
            case MonsterId::BRONZE_AUTOMATON:
                switch(id) {
                    case 1:
                        return MMID::BRONZE_AUTOMATON_FLAIL;
                    case 2:
                        return MMID::BRONZE_AUTOMATON_HYPER_BEAM;
                    case 3:
                        return MMID::BRONZE_AUTOMATON_STUNNED;
                    case 4:
                        return MMID::BRONZE_AUTOMATON_SPAWN_ORBS;
                    case 5:
                        return MMID::BRONZE_AUTOMATON_BOOST;
                }
                break;
            case MonsterId::BRONZE_ORB:
                switch(id) {
                    case 1:
                        return MMID::BRONZE_ORB_BEAM;
                    case 2:
                        return MMID::BRONZE_ORB_SUPPORT_BEAM;
                    case 3:
                        return MMID::BRONZE_ORB_STASIS;
                }
                break;
            case MonsterId::BYRD:
                switch(id) {
                    case 1:
                        return MMID::BYRD_PECK;
                    case 2:
                        return MMID::BYRD_FLY;
                    case 3:
                        return MMID::BYRD_SWOOP;
                    case 4:
                        return MMID::BYRD_STUNNED;
                    case 5:
                        return MMID::BYRD_HEADBUTT;
                    case 6:
                        return MMID::BYRD_CAW;
                }
                break;
            case MonsterId::CENTURION:
                switch(id) {
                    case 1:
                        return MMID::CENTURION_SLASH;
                    case 2:
                        return MMID::CENTURION_DEFEND;
                    case 3:
                        return MMID::CENTURION_FURY;
                }
                break;
            case MonsterId::CHOSEN:
                switch(id) {
                    case 1:
                        return MMID::CHOSEN_ZAP;
                    case 2:
                        return MMID::CHOSEN_DRAIN;
                    case 3:
                        return MMID::CHOSEN_DEBILITATE;
                    case 4:
                        return MMID::CHOSEN_HEX;
                    case 5:
                        return MMID::CHOSEN_POKE;
                }
                break;
            case MonsterId::CORRUPT_HEART:
                switch(id) {
                    case 1:
                        return MMID::CORRUPT_HEART_BLOOD_SHOTS;
                    case 2:
                        return MMID::CORRUPT_HEART_ECHO;
                    case 3:
                        return MMID::CORRUPT_HEART_DEBILITATE;
                    case 4:
                        return MMID::CORRUPT_HEART_BUFF;
                }
                break;
            case MonsterId::CULTIST:
                switch(id) {
                    case 1:
                        return MMID::CULTIST_DARK_STRIKE;
                    case 3:
                        return MMID::CULTIST_INCANTATION;
                }
                break;
            case MonsterId::DAGGER:
                switch(id) {
                    case 1:
                        return MMID::DAGGER_STAB;
                    case 2:
                        return MMID::DAGGER_EXPLODE;
                }
                break;
            case MonsterId::DARKLING:
                switch(id) {
                    case 1:
                        return MMID::DARKLING_CHOMP;
                    case 2:
                        return MMID::DARKLING_HARDEN;
                    case 3:
                        return MMID::DARKLING_NIP;
                    case 4:
                        return MMID::DARKLING_REGROW;
                    case 5:
                        return MMID::DARKLING_REINCARNATE;
                }
                break;
            case MonsterId::DECA:
                switch(id) {
                    case 0:
                        return MMID::DECA_BEAM;
                    case 2:
                        return MMID::DECA_SQUARE_OF_PROTECTION;
                }
                break;
            case MonsterId::DONU:
                switch(id) {
                    case 0:
                        return MMID::DONU_BEAM;
                    case 2:
                        return MMID::DONU_CIRCLE_OF_POWER;
                }
                break;
            case MonsterId::EXPLODER:
                switch(id) {
                    case 1:
                        return MMID::EXPLODER_SLAM;
                    case 2:
                        return MMID::EXPLODER_EXPLODE;
                        break;
                }
                break;
            case MonsterId::FAT_GREMLIN:
                switch(id) {
                    case 2:
                        return MMID::FAT_GREMLIN_SMASH;
                }
                break;
            case MonsterId::FUNGI_BEAST:
                switch(id) {
                    case 1:
                        return MMID::FUNGI_BEAST_BITE;
                    case 2:
                        return MMID::FUNGI_BEAST_GROW;
                }
                break;
            case MonsterId::GIANT_HEAD:
                switch(id) {
                    case 1:
                        return MMID::GIANT_HEAD_GLARE;
                    case 2:
                        return MMID::GIANT_HEAD_IT_IS_TIME;
                    case 3:
                        return MMID::GIANT_HEAD_COUNT;
                }
                break;
            case MonsterId::GREEN_LOUSE:
                switch(id) {
                    case 3:
                        return MMID::GREEN_LOUSE_BITE;
                    case 4:
                        return MMID::GREEN_LOUSE_SPIT_WEB;
                }
                break;
            case MonsterId::GREMLIN_LEADER:
                switch(id) {
                    case 2:
                        return MMID::GREMLIN_LEADER_RALLY;
                    case 3:
                        return MMID::GREMLIN_LEADER_ENCOURAGE;
                    case 4:
                        return MMID::GREMLIN_LEADER_STAB;
                }
                break;
            case MonsterId::GREMLIN_NOB:
                switch(id) {
                    case 1:
                        return MMID::GREMLIN_NOB_RUSH;
                    case 2:
                        return MMID::GREMLIN_NOB_SKULL_BASH;
                    case 3:
                        return MMID::GREMLIN_NOB_BELLOW;
                }
                break;
            case MonsterId::GREMLIN_WIZARD:
                switch(id) {
                    case 1:
                        return MMID::GREMLIN_WIZARD_ULTIMATE_BLAST;
                    case 2:
                        return MMID::GREMLIN_WIZARD_CHARGING;
                }
                break;
            case MonsterId::HEXAGHOST:
                switch(id) {
                    case 1:
                        return MMID::HEXAGHOST_DIVIDER;
                    case 2:
                        return MMID::HEXAGHOST_TACKLE;
                    case 3:
                        return MMID::HEXAGHOST_INFLAME;
                    case 4:
                        return MMID::HEXAGHOST_SEAR;
                    case 5:
                        return MMID::HEXAGHOST_ACTIVATE;
                    case 6:
                        return MMID::HEXAGHOST_INFERNO;
                }
                break;
            case MonsterId::JAW_WORM:
                switch(id) {
                    case 1:
                        return MMID::JAW_WORM_CHOMP;
                    case 2:
                        return MMID::JAW_WORM_BELLOW;
                    case 3:
                        return MMID::JAW_WORM_THRASH;
                }
                break;
            case MonsterId::LAGAVULIN:
                switch(id) {
                    case 1:
                        return MMID::LAGAVULIN_SIPHON_SOUL;
                    case 3:
                        return MMID::LAGAVULIN_ATTACK;
                    // not convinced that this correctly maps the sleeping states
                    case 4:
                        return MMID::LAGAVULIN_SLEEP;
                    case 5:
                        return MMID::LAGAVULIN_SLEEP;
                    case 6:
                        return MMID::LAGAVULIN_SLEEP;
                }
                break;
            case MonsterId::LOOTER:
                switch(id) {
                    case 1:
                        return MMID::LOOTER_MUG;
                    case 2:
                        return MMID::LOOTER_SMOKE_BOMB;
                    case 3:
                        return MMID::LOOTER_ESCAPE;
                    case 4:
                        return MMID::LOOTER_LUNGE;
                }
                break;
            case MonsterId::MAD_GREMLIN:
                switch(id) {
                    case 1:
                        return MMID::MAD_GREMLIN_SCRATCH;
                }
                break;
            case MonsterId::MUGGER:
                switch(id) {
                    case 1:
                        return MMID::MUGGER_MUG;
                    case 2:
                        return MMID::MUGGER_SMOKE_BOMB;
                    case 3:
                        return MMID::MUGGER_ESCAPE;
                    case 4:
                        return MMID::MUGGER_LUNGE;
                }
                break;
            case MonsterId::MYSTIC:
                switch(id) {
                    case 1:
                        return MMID::MYSTIC_ATTACK_DEBUFF;
                    case 2:
                        return MMID::MYSTIC_HEAL;
                    case 3:
                        return MMID::MYSTIC_BUFF;
                }
                break;
            case MonsterId::NEMESIS:
                switch(id) {
                    case 2:
                        return MMID::NEMESIS_ATTACK;
                    case 3:
                        return MMID::NEMESIS_SCYTHE;
                    case 4:
                        return MMID::NEMESIS_DEBUFF;
                }
                break;
            case MonsterId::ORB_WALKER:
                switch(id) {
                    case 1:
                        return MMID::ORB_WALKER_LASER;
                    case 2:
                        return MMID::ORB_WALKER_CLAW;
                }
                break;
            case MonsterId::POINTY:
                switch(id) {
                    case 1:
                        return MMID::POINTY_ATTACK;
                }
                break;
            case MonsterId::RED_LOUSE:
                switch(id) {
                    case 3:
                        return MMID::RED_LOUSE_BITE;
                    case 4:
                        return MMID::RED_LOUSE_GROW;
                }
                break;
            case MonsterId::RED_SLAVER:
                switch(id) {
                    case 1:
                        return MMID::RED_SLAVER_STAB;
                    case 2:
                        return MMID::RED_SLAVER_ENTANGLE;
                    case 3:
                        return MMID::RED_SLAVER_SCRAPE;
                }
                break;
            case MonsterId::REPTOMANCER:
                switch(id) {
                    case 1:
                        return MMID::REPTOMANCER_SNAKE_STRIKE;
                    case 2:
                        return MMID::REPTOMANCER_SUMMON;
                    case 3:
                        return MMID::REPTOMANCER_BIG_BITE;
                }
                break;
            case MonsterId::REPULSOR:
                switch(id) {
                    case 1:
                        return MMID::REPULSOR_REPULSE;
                    case 2:
                        return MMID::REPULSOR_BASH;
                }
                break;
            case MonsterId::ROMEO:
                switch(id) {
                    case 1:
                        return MMID::ROMEO_CROSS_SLASH;
                    case 2:
                        return MMID::ROMEO_MOCK;
                    case 3:
                        return MMID::ROMEO_AGONIZING_SLASH;
                }
                break;
            case MonsterId::SENTRY:
                switch(id) {
                    case 3:
                        return MMID::SENTRY_BOLT;
                    case 4:
                        return MMID::SENTRY_BEAM;
                }
                break;
            case MonsterId::SHELLED_PARASITE:
                switch(id) {
                    case 1:
                        return MMID::SHELLED_PARASITE_FELL;
                    case 2:
                        return MMID::SHELLED_PARASITE_DOUBLE_STRIKE;
                    case 3:
                        return MMID::SHELLED_PARASITE_SUCK;
                    case 4:
                        return MMID::SHELLED_PARASITE_STUNNED;
                }
                break;
            case MonsterId::SHIELD_GREMLIN:
                switch(id) {
                    case 1:
                        return MMID::SHIELD_GREMLIN_PROTECT;
                    case 2:
                        return MMID::SHIELD_GREMLIN_SHIELD_BASH;
                }
                break;
            case MonsterId::SLIME_BOSS:
                switch(id) {
                    case 1:
                        return MMID::SLIME_BOSS_SLAM;
                    case 2:
                        return MMID::SLIME_BOSS_PREPARING;
                    case 3:
                        return MMID::SLIME_BOSS_SPLIT;
                    case 4:
                        return MMID::SLIME_BOSS_GOOP_SPRAY;
                }
                break;
            case MonsterId::SNAKE_PLANT:
                switch(id) {
                    case 1:
                        return MMID::SNAKE_PLANT_CHOMP;
                    case 2:
                        return MMID::SNAKE_PLANT_ENFEEBLING_SPORES;
                }
                break;
            case MonsterId::SNEAKY_GREMLIN:
                switch(id) {
                    case 1:
                        return MMID::SNEAKY_GREMLIN_PUNCTURE;
                }
                break;
            case MonsterId::SNECKO:
                switch(id) {
                    case 1:
                        return MMID::SNECKO_PERPLEXING_GLARE;
                    case 2:
                        return MMID::SNECKO_BITE;
                    case 3:
                        return MMID::SNECKO_TAIL_WHIP;
                }
                break;
            case MonsterId::SPHERIC_GUARDIAN:
                switch(id) {
                    case 1:
                        return MMID::SPHERIC_GUARDIAN_SLAM;
                    case 2:
                        return MMID::SPHERIC_GUARDIAN_ACTIVATE;
                    case 3:
                        return MMID::SPHERIC_GUARDIAN_HARDEN;
                    case 4:
                        return MMID::SPHERIC_GUARDIAN_ATTACK_DEBUFF;
                }
                break;
            case MonsterId::SPIKER:
                switch(id) {
                    case 1:
                        return MMID::SPIKER_CUT;
                    case 2:
                        return MMID::SPIKER_SPIKE;
                }
                break;
            case MonsterId::SPIKE_SLIME_L:
                switch(id) {
                    case 1:
                        return MMID::SPIKE_SLIME_L_FLAME_TACKLE;
                    case 3:
                        return MMID::SPIKE_SLIME_L_SPLIT;
                    case 4:
                        return MMID::SPIKE_SLIME_L_LICK;
                }
                break;
            case MonsterId::SPIKE_SLIME_M:
                switch(id) {
                    case 1:
                        return MMID::SPIKE_SLIME_M_FLAME_TACKLE;
                    case 4:
                        return MMID::SPIKE_SLIME_M_LICK;
                }
                break;
            case MonsterId::SPIKE_SLIME_S:
                switch(id) {
                    case 1:
                        return MMID::SPIKE_SLIME_S_TACKLE;
                }
                break;
            case MonsterId::SPIRE_GROWTH:
                switch(id) {
                    case 1:
                        return MMID::SPIRE_GROWTH_QUICK_TACKLE;
                    case 2:
                        return MMID::SPIRE_GROWTH_CONSTRICT;
                    case 3:
                        return MMID::SPIRE_GROWTH_SMASH;
                }
                break;
            case MonsterId::SPIRE_SHIELD:
                switch(id) {
                    case 1:
                        return MMID::SPIRE_SHIELD_BASH;
                    case 2:
                        return MMID::SPIRE_SHIELD_FORTIFY;
                    case 3:
                        return MMID::SPIRE_SHIELD_SMASH;
                }
                break;
            case MonsterId::SPIRE_SPEAR:
                switch(id) {
                    case 1:
                        return MMID::SPIRE_SPEAR_BURN_STRIKE;
                    case 2:
                        return MMID::SPIRE_SPEAR_PIERCER;
                    case 3:
                        return MMID::SPIRE_SPEAR_SKEWER;
                }
                break;
            case MonsterId::TASKMASTER:
                switch(id) {
                    case 2:
                        return MMID::TASKMASTER_SCOURING_WHIP;
                }
                break;
            case MonsterId::THE_CHAMP:
                switch(id) {
                    case 1:
                        return MMID::THE_CHAMP_HEAVY_SLASH;
                    case 2:
                        return MMID::THE_CHAMP_DEFENSIVE_STANCE;
                    case 3:
                        return MMID::THE_CHAMP_EXECUTE;
                    case 4:
                        return MMID::THE_CHAMP_FACE_SLAP;
                    case 5:
                        return MMID::THE_CHAMP_GLOAT;
                    case 6:
                        return MMID::THE_CHAMP_TAUNT;
                    case 7:
                        return MMID::THE_CHAMP_ANGER;
                }
                break;
            case MonsterId::THE_COLLECTOR:
                switch(id) {
                    case 1:
                        return MMID::THE_COLLECTOR_SPAWN;
                    case 2:
                        return MMID::THE_COLLECTOR_FIREBALL;
                    case 3:
                        return MMID::THE_COLLECTOR_BUFF;
                    case 4:
                        return MMID::THE_COLLECTOR_MEGA_DEBUFF;
                    case 5:
                        return MMID::THE_COLLECTOR_SPAWN;
                }
                break;
            case MonsterId::THE_GUARDIAN:
                switch(id) {
                    case 1:
                        return MMID::THE_GUARDIAN_DEFENSIVE_MODE;
                    case 2:
                        return MMID::THE_GUARDIAN_FIERCE_BASH;
                    case 3:
                        return MMID::THE_GUARDIAN_ROLL_ATTACK;
                    case 4:
                        return MMID::THE_GUARDIAN_TWIN_SLAM;
                    case 5:
                        return MMID::THE_GUARDIAN_WHIRLWIND;
                    case 6:
                        return MMID::THE_GUARDIAN_CHARGING_UP;
                    case 7:
                        return MMID::THE_GUARDIAN_VENT_STEAM;
                }
                break;
            case MonsterId::THE_MAW:
                switch(id) {
                    case 2:
                        return MMID::THE_MAW_ROAR;
                    case 3:
                        return MMID::THE_MAW_SLAM;
                    case 4:
                        return MMID::THE_MAW_DROOL;
                    case 5:
                        return MMID::THE_MAW_NOM;
                }
                break;
            case MonsterId::TIME_EATER:
                switch(id) {
                    case 2:
                        return MMID::TIME_EATER_REVERBERATE;
                    case 3:
                        return MMID::TIME_EATER_RIPPLE;
                    case 4:
                        return MMID::TIME_EATER_HEAD_SLAM;
                    case 5:
                        return MMID::TIME_EATER_HASTE;
                }
                break;
            case MonsterId::TORCH_HEAD:
                switch(id) {
                    case 1:
                        return MMID::TORCH_HEAD_TACKLE;
                }
                break;
            case MonsterId::TRANSIENT:
                switch(id) {
                    case 1:
                        return MMID::TRANSIENT_ATTACK;
                }
                break;
            case MonsterId::WRITHING_MASS:
                switch(id) {
                    case 0:
                        return MMID::WRITHING_MASS_STRONG_STRIKE;
                    case 1:
                        return MMID::WRITHING_MASS_MULTI_STRIKE;
                    case 2:
                        return MMID::WRITHING_MASS_FLAIL;
                    case 3:
                        return MMID::WRITHING_MASS_WITHER;
                    case 4:
                        return MMID::WRITHING_MASS_IMPLANT;
                }
                break;

        }
        return MMID::INVALID;
    }

    static constexpr bool isMoveAttack(MMID move) { // todo add intent information / base damage / multi attack info

        switch (move) {
            case MMID::ACID_SLIME_L_CORROSIVE_SPIT:
            case MMID::ACID_SLIME_L_TACKLE:
            case MMID::ACID_SLIME_M_CORROSIVE_SPIT:
            case MMID::ACID_SLIME_M_TACKLE:
            case MMID::ACID_SLIME_S_TACKLE:
            case MMID::AWAKENED_ONE_SLASH:
            case MMID::AWAKENED_ONE_SOUL_STRIKE:
            case MMID::AWAKENED_ONE_DARK_ECHO:
            case MMID::AWAKENED_ONE_SLUDGE:
            case MMID::AWAKENED_ONE_TACKLE:
            case MMID::BEAR_LUNGE:
            case MMID::BEAR_MAUL:
            case MMID::BLUE_SLAVER_STAB:
            case MMID::BLUE_SLAVER_RAKE:
            case MMID::BOOK_OF_STABBING_MULTI_STAB:
            case MMID::BOOK_OF_STABBING_SINGLE_STAB:
            case MMID::BRONZE_AUTOMATON_FLAIL:
            case MMID::BRONZE_AUTOMATON_HYPER_BEAM:
            case MMID::BRONZE_ORB_BEAM:
            case MMID::BYRD_PECK:
            case MMID::BYRD_SWOOP:
            case MMID::BYRD_HEADBUTT:
            case MMID::CENTURION_SLASH:
            case MMID::CENTURION_FURY:
            case MMID::CHOSEN_POKE:
            case MMID::CHOSEN_ZAP:
            case MMID::CHOSEN_DEBILITATE:
            case MMID::CORRUPT_HEART_BLOOD_SHOTS:
            case MMID::CORRUPT_HEART_ECHO:
            case MMID::CULTIST_DARK_STRIKE:
            case MMID::DAGGER_STAB:
            case MMID::DAGGER_EXPLODE:
            case MMID::DARKLING_NIP:
            case MMID::DARKLING_CHOMP:
            case MMID::DECA_BEAM:
            case MMID::DONU_BEAM:
            case MMID::EXPLODER_SLAM:
            case MMID::FAT_GREMLIN_SMASH:
            case MMID::FUNGI_BEAST_BITE:
            case MMID::GIANT_HEAD_COUNT:
            case MMID::GIANT_HEAD_IT_IS_TIME:
            case MMID::GREEN_LOUSE_BITE:
            case MMID::GREMLIN_LEADER_STAB:
            case MMID::GREMLIN_NOB_RUSH:
            case MMID::GREMLIN_NOB_SKULL_BASH:
            case MMID::GREMLIN_WIZARD_ULTIMATE_BLAST:
            case MMID::HEXAGHOST_DIVIDER:
            case MMID::HEXAGHOST_INFERNO:
            case MMID::HEXAGHOST_SEAR:
            case MMID::HEXAGHOST_TACKLE:
            case MMID::JAW_WORM_CHOMP:
            case MMID::JAW_WORM_THRASH:
            case MMID::LAGAVULIN_ATTACK:
            case MMID::LOOTER_MUG:
            case MMID::LOOTER_LUNGE:
            case MMID::MAD_GREMLIN_SCRATCH:
            case MMID::MUGGER_MUG:
            case MMID::MUGGER_LUNGE:
            case MMID::MYSTIC_ATTACK_DEBUFF:
            case MMID::NEMESIS_ATTACK:
            case MMID::NEMESIS_SCYTHE:
            case MMID::ORB_WALKER_LASER:
            case MMID::ORB_WALKER_CLAW:
            case MMID::POINTY_ATTACK:
            case MMID::RED_LOUSE_BITE:
            case MMID::RED_SLAVER_STAB:
            case MMID::RED_SLAVER_SCRAPE:
            case MMID::REPTOMANCER_SNAKE_STRIKE:
            case MMID::REPTOMANCER_BIG_BITE:
            case MMID::REPULSOR_BASH:
            case MMID::ROMEO_AGONIZING_SLASH:
            case MMID::ROMEO_CROSS_SLASH:
            case MMID::SENTRY_BEAM:
            case MMID::SHELLED_PARASITE_DOUBLE_STRIKE:
            case MMID::SHELLED_PARASITE_SUCK:
            case MMID::SHELLED_PARASITE_FELL:
            case MMID::SHIELD_GREMLIN_SHIELD_BASH:
            case MMID::SLIME_BOSS_SLAM:
            case MMID::SNAKE_PLANT_CHOMP:
            case MMID::SNEAKY_GREMLIN_PUNCTURE:
            case MMID::SNECKO_TAIL_WHIP:
            case MMID::SNECKO_BITE:
            case MMID::SPHERIC_GUARDIAN_SLAM:
            case MMID::SPHERIC_GUARDIAN_HARDEN:
            case MMID::SPHERIC_GUARDIAN_ATTACK_DEBUFF:
            case MMID::SPIKER_CUT:
            case MMID::SPIKE_SLIME_L_FLAME_TACKLE:
            case MMID::SPIKE_SLIME_M_FLAME_TACKLE:
            case MMID::SPIKE_SLIME_S_TACKLE:
            case MMID::SPIRE_GROWTH_QUICK_TACKLE:
            case MMID::SPIRE_GROWTH_SMASH:
            case MMID::SPIRE_SHIELD_BASH:
            case MMID::SPIRE_SHIELD_SMASH:
            case MMID::SPIRE_SPEAR_BURN_STRIKE:
            case MMID::SPIRE_SPEAR_SKEWER:
            case MMID::TASKMASTER_SCOURING_WHIP:
            case MMID::TORCH_HEAD_TACKLE:
            case MMID::THE_CHAMP_FACE_SLAP:
            case MMID::THE_CHAMP_HEAVY_SLASH:
            case MMID::THE_CHAMP_EXECUTE:
            case MMID::THE_COLLECTOR_FIREBALL:
            case MMID::THE_GUARDIAN_FIERCE_BASH:
            case MMID::THE_GUARDIAN_WHIRLWIND:
            case MMID::THE_GUARDIAN_ROLL_ATTACK:
            case MMID::THE_GUARDIAN_TWIN_SLAM:
            case MMID::THE_MAW_SLAM:
            case MMID::THE_MAW_NOM:
            case MMID::TIME_EATER_REVERBERATE:
            case MMID::TIME_EATER_HEAD_SLAM:
            case MMID::TRANSIENT_ATTACK:
            case MMID::WRITHING_MASS_FLAIL:
            case MMID::WRITHING_MASS_MULTI_STRIKE:
            case MMID::WRITHING_MASS_STRONG_STRIKE:
                return true;

            default:
                return false;
        }

    }

}



#endif //MONSTERSPECIFIC_CPP_MONSTERMOVES_H
