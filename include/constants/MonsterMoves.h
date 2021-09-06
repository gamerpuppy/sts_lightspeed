//
// Created by gamerpuppy on 8/22/2021.
//

#ifndef MONSTERSPECIFIC_CPP_MONSTERMOVES_H
#define MONSTERSPECIFIC_CPP_MONSTERMOVES_H

namespace sts {

    enum class MonsterMoveId {
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
        BRONZE_AUTOMATON_SPAWN_ORBS,
        BRONZE_AUTOMATON_BOOST,
        BRONZE_AUTOMATON_FLAIL,
        BRONZE_AUTOMATON_HYPER_BEAM,
        BRONZE_ORB_STASIS,
        BRONZE_ORB_BEAM,
        BRONZE_ORB_SUPPORT_BEAM,
        BYRD_CAW,
        BYRD_PECK,
        BYRD_SWOOP,
        BYRD_FLY,
        BYRD_HEADBUTT,
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
        SHELLED_PARASITE_SUCK,
        SHELLED_PARASITE_FELL,
        SHIELD_GREMLIN_PROTECT,
        SHIELD_GREMLIN_SHIELD_BASH,
        SLIME_BOSS_GOOP_SPRAY,
        SLIME_BOSS_PREPARING,
        SLIME_BOSS_SLAM,
        SLIME_BOSS_SPLIT,
        SNAKE_PLANT_CHOMP,
        SNAKE_PLANT_ENFEEBLING_SPORES,
        SNEAKY_GREMLIN_PUNCTURE,
        SNECKO_PERPLEXING_GAZE,
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
            "BRONZE_AUTOMATON_SPAWN_ORBS",
            "BRONZE_AUTOMATON_BOOST",
            "BRONZE_AUTOMATON_FLAIL",
            "BRONZE_AUTOMATON_HYPER_BEAM",
            "BRONZE_ORB_STASIS",
            "BRONZE_ORB_BEAM",
            "BRONZE_ORB_SUPPORT_BEAM",
            "BYRD_CAW",
            "BYRD_PECK",
            "BYRD_SWOOP",
            "BYRD_FLY",
            "BYRD_HEADBUTT",
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
            "SHELLED_PARASITE_SUCK",
            "SHELLED_PARASITE_FELL",
            "SHIELD_GREMLIN_PROTECT",
            "SHIELD_GREMLIN_SHIELD_BASH",
            "SLIME_BOSS_GOOP_SPRAY",
            "SLIME_BOSS_PREPARING",
            "SLIME_BOSS_SLAM",
            "SLIME_BOSS_SPLIT",
            "SNAKE_PLANT_CHOMP",
            "SNAKE_PLANT_ENFEEBLING_SPORES",
            "SNEAKY_GREMLIN_PUNCTURE",
            "SNECKO_PERPLEXING_GAZE",
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

    static constexpr bool isMoveAttack(MonsterMoveId move) { // todo add intent information / base damage / multi attack info

        switch (move) {
            case MonsterMoveId::ACID_SLIME_L_LICK:
            case MonsterMoveId::ACID_SLIME_M_LICK:
            case MonsterMoveId::ACID_SLIME_S_LICK:
            case MonsterMoveId::BRONZE_ORB_STASIS:
            case MonsterMoveId::CULTIST_INCANTATION:
            case MonsterMoveId::FUNGI_BEAST_GROW:
            case MonsterMoveId::GENERIC_ESCAPE_MOVE:
            case MonsterMoveId::GREEN_LOUSE_SPIT_WEB:
            case MonsterMoveId::INVALID:
            case MonsterMoveId::JAW_WORM_BELLOW:
            case MonsterMoveId::RED_LOUSE_GROW:
            case MonsterMoveId::SPIKE_SLIME_L_LICK:
            case MonsterMoveId::SPIKE_SLIME_M_LICK:
                return false;


            case MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT:
            case MonsterMoveId::ACID_SLIME_L_TACKLE:
            case MonsterMoveId::ACID_SLIME_L_SPLIT:
            case MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT:
            case MonsterMoveId::ACID_SLIME_M_TACKLE:
            case MonsterMoveId::ACID_SLIME_S_TACKLE:
            case MonsterMoveId::AWAKENED_ONE_SLASH:
            case MonsterMoveId::AWAKENED_ONE_SOUL_STRIKE:
            case MonsterMoveId::AWAKENED_ONE_REBIRTH:
            case MonsterMoveId::AWAKENED_ONE_DARK_ECHO:
            case MonsterMoveId::AWAKENED_ONE_SLUDGE:
            case MonsterMoveId::AWAKENED_ONE_TACKLE:
            case MonsterMoveId::BEAR_BEAR_HUG:
            case MonsterMoveId::BEAR_LUNGE:
            case MonsterMoveId::BEAR_MAUL:
            case MonsterMoveId::BLUE_SLAVER_STAB:
            case MonsterMoveId::BLUE_SLAVER_RAKE:
            case MonsterMoveId::BOOK_OF_STABBING_MULTI_STAB:
            case MonsterMoveId::BOOK_OF_STABBING_SINGLE_STAB:
            case MonsterMoveId::BRONZE_AUTOMATON_SPAWN_ORBS:
            case MonsterMoveId::BRONZE_AUTOMATON_BOOST:
            case MonsterMoveId::BRONZE_AUTOMATON_FLAIL:
            case MonsterMoveId::BRONZE_AUTOMATON_HYPER_BEAM:
            case MonsterMoveId::BRONZE_ORB_BEAM:
            case MonsterMoveId::BRONZE_ORB_SUPPORT_BEAM:
            case MonsterMoveId::BYRD_CAW:
            case MonsterMoveId::BYRD_PECK:
            case MonsterMoveId::BYRD_SWOOP:
            case MonsterMoveId::BYRD_FLY:
            case MonsterMoveId::BYRD_HEADBUTT:
            case MonsterMoveId::CENTURION_SLASH:
            case MonsterMoveId::CENTURION_FURY:
            case MonsterMoveId::CENTURION_DEFEND:
            case MonsterMoveId::CHOSEN_POKE:
            case MonsterMoveId::CHOSEN_ZAP:
            case MonsterMoveId::CHOSEN_DEBILITATE:
            case MonsterMoveId::CHOSEN_DRAIN:
            case MonsterMoveId::CHOSEN_HEX:
            case MonsterMoveId::CORRUPT_HEART_DEBILITATE:
            case MonsterMoveId::CORRUPT_HEART_BLOOD_SHOTS:
            case MonsterMoveId::CORRUPT_HEART_ECHO:
            case MonsterMoveId::CORRUPT_HEART_BUFF:
            case MonsterMoveId::CULTIST_DARK_STRIKE:
            case MonsterMoveId::DAGGER_STAB:
            case MonsterMoveId::DAGGER_EXPLODE:
            case MonsterMoveId::DARKLING_NIP:
            case MonsterMoveId::DARKLING_CHOMP:
            case MonsterMoveId::DARKLING_HARDEN:
            case MonsterMoveId::DARKLING_REINCARNATE:
            case MonsterMoveId::DARKLING_REGROW:
            case MonsterMoveId::DECA_SQUARE_OF_PROTECTION:
            case MonsterMoveId::DECA_BEAM:
            case MonsterMoveId::DONU_CIRCLE_OF_POWER:
            case MonsterMoveId::DONU_BEAM:
            case MonsterMoveId::EXPLODER_SLAM:
            case MonsterMoveId::EXPLODER_EXPLODE:
            case MonsterMoveId::FAT_GREMLIN_SMASH:
            case MonsterMoveId::FUNGI_BEAST_BITE:
            case MonsterMoveId::GIANT_HEAD_COUNT:
            case MonsterMoveId::GIANT_HEAD_GLARE:
            case MonsterMoveId::GIANT_HEAD_IT_IS_TIME:
            case MonsterMoveId::GREEN_LOUSE_BITE:
            case MonsterMoveId::GREMLIN_LEADER_ENCOURAGE:
            case MonsterMoveId::GREMLIN_LEADER_RALLY:
            case MonsterMoveId::GREMLIN_LEADER_STAB:
            case MonsterMoveId::GREMLIN_NOB_BELLOW:
            case MonsterMoveId::GREMLIN_NOB_RUSH:
            case MonsterMoveId::GREMLIN_NOB_SKULL_BASH:
            case MonsterMoveId::GREMLIN_WIZARD_CHARGING:
            case MonsterMoveId::GREMLIN_WIZARD_ULTIMATE_BLAST:
            case MonsterMoveId::HEXAGHOST_ACTIVATE:
            case MonsterMoveId::HEXAGHOST_DIVIDER:
            case MonsterMoveId::HEXAGHOST_INFERNO:
            case MonsterMoveId::HEXAGHOST_SEAR:
            case MonsterMoveId::HEXAGHOST_TACKLE:
            case MonsterMoveId::HEXAGHOST_INFLAME:
            case MonsterMoveId::JAW_WORM_CHOMP:
            case MonsterMoveId::JAW_WORM_THRASH:
            case MonsterMoveId::LAGAVULIN_ATTACK:
            case MonsterMoveId::LAGAVULIN_SIPHON_SOUL:
            case MonsterMoveId::LAGAVULIN_SLEEP:
            case MonsterMoveId::LOOTER_MUG:
            case MonsterMoveId::LOOTER_LUNGE:
            case MonsterMoveId::LOOTER_SMOKE_BOMB:
            case MonsterMoveId::LOOTER_ESCAPE:
            case MonsterMoveId::MAD_GREMLIN_SCRATCH:
            case MonsterMoveId::MUGGER_MUG:
            case MonsterMoveId::MUGGER_LUNGE:
            case MonsterMoveId::MUGGER_SMOKE_BOMB:
            case MonsterMoveId::MUGGER_ESCAPE:
            case MonsterMoveId::MYSTIC_HEAL:
            case MonsterMoveId::MYSTIC_BUFF:
            case MonsterMoveId::MYSTIC_ATTACK_DEBUFF:
            case MonsterMoveId::NEMESIS_DEBUFF:
            case MonsterMoveId::NEMESIS_ATTACK:
            case MonsterMoveId::NEMESIS_SCYTHE:
            case MonsterMoveId::ORB_WALKER_LASER:
            case MonsterMoveId::ORB_WALKER_CLAW:
            case MonsterMoveId::POINTY_ATTACK:
            case MonsterMoveId::RED_LOUSE_BITE:
            case MonsterMoveId::RED_SLAVER_STAB:
            case MonsterMoveId::RED_SLAVER_SCRAPE:
            case MonsterMoveId::RED_SLAVER_ENTANGLE:
            case MonsterMoveId::REPTOMANCER_SUMMON:
            case MonsterMoveId::REPTOMANCER_SNAKE_STRIKE:
            case MonsterMoveId::REPTOMANCER_BIG_BITE:
            case MonsterMoveId::REPULSOR_BASH:
            case MonsterMoveId::REPULSOR_REPULSE:
            case MonsterMoveId::ROMEO_MOCK:
            case MonsterMoveId::ROMEO_AGONIZING_SLASH:
            case MonsterMoveId::ROMEO_CROSS_SLASH:
            case MonsterMoveId::SENTRY_BEAM:
            case MonsterMoveId::SENTRY_BOLT:
            case MonsterMoveId::SHELLED_PARASITE_DOUBLE_STRIKE:
            case MonsterMoveId::SHELLED_PARASITE_SUCK:
            case MonsterMoveId::SHELLED_PARASITE_FELL:
            case MonsterMoveId::SHIELD_GREMLIN_PROTECT:
            case MonsterMoveId::SHIELD_GREMLIN_SHIELD_BASH:
            case MonsterMoveId::SLIME_BOSS_GOOP_SPRAY:
            case MonsterMoveId::SLIME_BOSS_PREPARING:
            case MonsterMoveId::SLIME_BOSS_SLAM:
            case MonsterMoveId::SLIME_BOSS_SPLIT:
            case MonsterMoveId::SNAKE_PLANT_CHOMP:
            case MonsterMoveId::SNAKE_PLANT_ENFEEBLING_SPORES:
            case MonsterMoveId::SNEAKY_GREMLIN_PUNCTURE:
            case MonsterMoveId::SNECKO_PERPLEXING_GAZE:
            case MonsterMoveId::SNECKO_TAIL_WHIP:
            case MonsterMoveId::SNECKO_BITE:
            case MonsterMoveId::SPHERIC_GUARDIAN_SLAM:
            case MonsterMoveId::SPHERIC_GUARDIAN_ACTIVATE:
            case MonsterMoveId::SPHERIC_GUARDIAN_HARDEN:
            case MonsterMoveId::SPHERIC_GUARDIAN_ATTACK_DEBUFF:
            case MonsterMoveId::SPIKER_CUT:
            case MonsterMoveId::SPIKER_SPIKE:
            case MonsterMoveId::SPIKE_SLIME_L_FLAME_TACKLE:
            case MonsterMoveId::SPIKE_SLIME_L_SPLIT:
            case MonsterMoveId::SPIKE_SLIME_M_FLAME_TACKLE:
            case MonsterMoveId::SPIKE_SLIME_S_TACKLE:
            case MonsterMoveId::SPIRE_GROWTH_QUICK_TACKLE:
            case MonsterMoveId::SPIRE_GROWTH_SMASH:
            case MonsterMoveId::SPIRE_GROWTH_CONSTRICT:
            case MonsterMoveId::SPIRE_SHIELD_BASH:
            case MonsterMoveId::SPIRE_SHIELD_FORTIFY:
            case MonsterMoveId::SPIRE_SHIELD_SMASH:
            case MonsterMoveId::SPIRE_SPEAR_BURN_STRIKE:
            case MonsterMoveId::SPIRE_SPEAR_PIERCER:
            case MonsterMoveId::SPIRE_SPEAR_SKEWER:
            case MonsterMoveId::TASKMASTER_SCOURING_WHIP:
            case MonsterMoveId::TORCH_HEAD_TACKLE:
            case MonsterMoveId::THE_CHAMP_DEFENSIVE_STANCE:
            case MonsterMoveId::THE_CHAMP_FACE_SLAP:
            case MonsterMoveId::THE_CHAMP_TAUNT:
            case MonsterMoveId::THE_CHAMP_HEAVY_SLASH:
            case MonsterMoveId::THE_CHAMP_GLOAT:
            case MonsterMoveId::THE_CHAMP_EXECUTE:
            case MonsterMoveId::THE_CHAMP_ANGER:
            case MonsterMoveId::THE_COLLECTOR_BUFF:
            case MonsterMoveId::THE_COLLECTOR_FIREBALL:
            case MonsterMoveId::THE_COLLECTOR_MEGA_DEBUFF:
            case MonsterMoveId::THE_COLLECTOR_SPAWN:
            case MonsterMoveId::THE_GUARDIAN_CHARGING_UP:
            case MonsterMoveId::THE_GUARDIAN_FIERCE_BASH:
            case MonsterMoveId::THE_GUARDIAN_VENT_STEAM:
            case MonsterMoveId::THE_GUARDIAN_WHIRLWIND:
            case MonsterMoveId::THE_GUARDIAN_DEFENSIVE_MODE:
            case MonsterMoveId::THE_GUARDIAN_ROLL_ATTACK:
            case MonsterMoveId::THE_GUARDIAN_TWIN_SLAM:
            case MonsterMoveId::THE_MAW_ROAR:
            case MonsterMoveId::THE_MAW_DROOL:
            case MonsterMoveId::THE_MAW_SLAM:
            case MonsterMoveId::THE_MAW_NOM:
            case MonsterMoveId::TIME_EATER_REVERBERATE:
            case MonsterMoveId::TIME_EATER_HEAD_SLAM:
            case MonsterMoveId::TIME_EATER_RIPPLE:
            case MonsterMoveId::TIME_EATER_HASTE:
            case MonsterMoveId::TRANSIENT_ATTACK:
            case MonsterMoveId::WRITHING_MASS_IMPLANT:
            case MonsterMoveId::WRITHING_MASS_FLAIL:
            case MonsterMoveId::WRITHING_MASS_WITHER:
            case MonsterMoveId::WRITHING_MASS_MULTI_STRIKE:
            case MonsterMoveId::WRITHING_MASS_STRONG_STRIKE:
                return true;

            default:
                return false;
        }

    }

}



#endif //MONSTERSPECIFIC_CPP_MONSTERMOVES_H
