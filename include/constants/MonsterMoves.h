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
