//
// Created by gamerpuppy on 9/27/2021.
//

#include <sts/combat/monster.hpp>

#include <sts/combat/monster.hpp>
#include <sts/combat/battle_context.hpp>

using namespace sts;

DamageInfo Monster::getMoveBaseDamage(const BattleContext &bc) const {
    const bool asc2 = bc.ascension >= 2;
    const bool asc3 = bc.ascension >= 3;
    const bool asc4 = bc.ascension >= 4;

    switch (moveHistory[0]) {

        case MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT:    return {asc2 ? 12 : 11};
        case MonsterMoveId::ACID_SLIME_L_TACKLE:            return {asc2 ? 18 : 16};
        case MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT:    return {asc2 ? 8 : 7};
        case MonsterMoveId::ACID_SLIME_M_TACKLE:            return {asc2 ? 12 : 10};
        case MonsterMoveId::ACID_SLIME_S_TACKLE:            return {asc2 ? 4 : 3};

        case MonsterMoveId::AWAKENED_ONE_SLASH:             return {20};
        case MonsterMoveId::AWAKENED_ONE_SOUL_STRIKE:       return {6, 4};
        case MonsterMoveId::AWAKENED_ONE_DARK_ECHO:         return {40};
        case MonsterMoveId::AWAKENED_ONE_SLUDGE:            return {18};
        case MonsterMoveId::AWAKENED_ONE_TACKLE:            return {10, 3};

        case MonsterMoveId::BEAR_LUNGE:                     return {asc2 ? 10 : 9};
        case MonsterMoveId::BEAR_MAUL:                      return {asc2 ? 20 : 18};

        case MonsterMoveId::BLUE_SLAVER_RAKE:               return {asc2 ? 8 : 7};
        case MonsterMoveId::BLUE_SLAVER_STAB:               return {asc2 ? 13 : 12};

        case MonsterMoveId::BOOK_OF_STABBING_MULTI_STAB:    return {asc3 ? 7 : 6, miscInfo};
        case MonsterMoveId::BOOK_OF_STABBING_SINGLE_STAB:   return {asc3 ? 24 : 21};

        case MonsterMoveId::BRONZE_AUTOMATON_FLAIL:         return {asc4 ? 8 : 7, 2};
        case MonsterMoveId::BRONZE_AUTOMATON_HYPER_BEAM:    return {asc4 ? 50 : 45};
        case MonsterMoveId::BRONZE_ORB_BEAM:                return {8};

        case MonsterMoveId::BYRD_HEADBUTT:                  return {3};
        case MonsterMoveId::BYRD_PECK:                      return {1, asc2 ? 6 : 5};
        case MonsterMoveId::BYRD_SWOOP:                     return {asc2 ? 14 : 12};


        case MonsterMoveId::CENTURION_FURY:                 return {asc2 ? 7 : 6, 3};
        case MonsterMoveId::CENTURION_SLASH:                return {asc2 ? 14 : 12};

        case MonsterMoveId::CHOSEN_POKE:                    return {asc2 ? 6 : 5, 2};
        case MonsterMoveId::CHOSEN_ZAP:                     return {asc2 ? 21 : 18};
        case MonsterMoveId::CHOSEN_DEBILITATE:              return {asc2 ? 12 : 10};

        case MonsterMoveId::CORRUPT_HEART_BLOOD_SHOTS:      return {2, asc4 ? 15 : 12};
        case MonsterMoveId::CORRUPT_HEART_ECHO:             return {asc4 ? 45 : 40};

        case MonsterMoveId::CULTIST_DARK_STRIKE:            return {6};

        case MonsterMoveId::DAGGER_STAB:                    return {9};
        case MonsterMoveId::DAGGER_EXPLODE:                 return {25};

        case MonsterMoveId::DARKLING_NIP:                   return {miscInfo + (asc2 ? 2 : 0)};
        case MonsterMoveId::DARKLING_CHOMP:                 return {asc2 ? 9 : 8};

        case MonsterMoveId::DECA_BEAM:                      return {asc4 ? 12 : 10, 2};
        case MonsterMoveId::DONU_BEAM:                      return {asc4 ? 12 : 10, 2};

        case MonsterMoveId::EXPLODER_SLAM:                  return {asc2 ? 11 : 9};
        case MonsterMoveId::EXPLODER_EXPLODE:               return {30}; // todo should this be classified as move damage?

        case MonsterMoveId::FAT_GREMLIN_SMASH:              return {asc2 ? 5 : 4};
        case MonsterMoveId::FUNGI_BEAST_BITE:               return {6};

        case MonsterMoveId::GIANT_HEAD_COUNT:               return {13};
        case MonsterMoveId::GIANT_HEAD_IT_IS_TIME: {
            const auto t = std::min(bc.getMonsterTurnNumber()-5, 6) * 5;
            const auto damage = (asc3 ? 40 : 30) + t;
            return {damage};
        }

        case MonsterMoveId::GREEN_LOUSE_BITE:               return {miscInfo};

        case MonsterMoveId::GREMLIN_LEADER_STAB:            return {6, 3};

        case MonsterMoveId::GREMLIN_NOB_RUSH:               return {asc3 ? 16 : 14};
        case MonsterMoveId::GREMLIN_NOB_SKULL_BASH:         return {asc3 ? 8 : 6};

        case MonsterMoveId::GREMLIN_WIZARD_ULTIMATE_BLAST:  return {asc2 ? 30 : 25};

        case MonsterMoveId::HEXAGHOST_DIVIDER:              return {miscInfo, 6};
        case MonsterMoveId::HEXAGHOST_INFERNO:              return {asc4 ? 3 : 2, 6};
        case MonsterMoveId::HEXAGHOST_SEAR:                 return {6};
        case MonsterMoveId::HEXAGHOST_TACKLE:               return {asc4 ? 6 : 5, 2};

        case MonsterMoveId::JAW_WORM_CHOMP:                 return {asc2 ? 12 : 11};
        case MonsterMoveId::JAW_WORM_THRASH:                return {7};

        case MonsterMoveId::LAGAVULIN_ATTACK:               return {asc3 ? 20 : 18};


        case MonsterMoveId::LOOTER_LUNGE:                   return {asc2 ? 14 : 12};
        case MonsterMoveId::LOOTER_MUG:                     return {asc2 ? 11 : 10};

        case MonsterMoveId::MAD_GREMLIN_SCRATCH:            return {asc2 ? 5 : 4};

        case MonsterMoveId::MUGGER_MUG:                     return {asc2 ? 11 : 10};
        case MonsterMoveId::MUGGER_LUNGE:                   return {asc2 ? 18 : 16};

        case MonsterMoveId::MYSTIC_ATTACK_DEBUFF:           return {asc2 ? 9 : 8};

        case MonsterMoveId::NEMESIS_ATTACK:                 return {asc3 ? 7 : 6, 3};
        case MonsterMoveId::NEMESIS_SCYTHE:                 return {45};

        case MonsterMoveId::ORB_WALKER_LASER:               return {asc2 ? 11 : 10};
        case MonsterMoveId::ORB_WALKER_CLAW:                return {asc2 ? 16 : 15};

        case MonsterMoveId::POINTY_ATTACK:                  return {asc2 ? 6 : 5, 2};

        case MonsterMoveId::RED_LOUSE_BITE:                 return {miscInfo};

        case MonsterMoveId::RED_SLAVER_SCRAPE:              return {asc2 ? 9 : 8};
        case MonsterMoveId::RED_SLAVER_STAB:                return {asc2 ? 14 : 13};

        case MonsterMoveId::REPTOMANCER_BIG_BITE:           return {asc3 ? 34 : 30};
        case MonsterMoveId::REPTOMANCER_SNAKE_STRIKE:       return {asc3 ? 16 : 13, 2};

        case MonsterMoveId::REPULSOR_BASH:                  return {asc2 ? 13 : 11};

        case MonsterMoveId::ROMEO_AGONIZING_SLASH:          return {asc2 ? 12 : 10};
        case MonsterMoveId::ROMEO_CROSS_SLASH:              return {asc2 ? 17 : 15};

        case MonsterMoveId::SENTRY_BEAM:                    return {asc3 ? 10 : 9};

        case MonsterMoveId::SHELLED_PARASITE_DOUBLE_STRIKE: return {asc2 ? 7 : 6, 2};
        case MonsterMoveId::SHELLED_PARASITE_FELL:          return {asc2 ? 21 : 18};
        case MonsterMoveId::SHELLED_PARASITE_SUCK:          return {asc2 ? 12 : 10};

        case MonsterMoveId::SHIELD_GREMLIN_SHIELD_BASH:     return {asc2 ? 8 : 6};

        case MonsterMoveId::SLIME_BOSS_SLAM:                return {asc4 ? 38 : 35};

        case MonsterMoveId::SNAKE_PLANT_CHOMP:              return {asc2 ? 8 : 7, 3};

        case MonsterMoveId::SNEAKY_GREMLIN_PUNCTURE:        return {asc2 ? 10 : 9};

        case MonsterMoveId::SNECKO_TAIL_WHIP:               return {asc2 ? 10 : 8};
        case MonsterMoveId::SNECKO_BITE:                    return {asc2 ? 18 : 15};

        case MonsterMoveId::SPHERIC_GUARDIAN_SLAM:          return {asc2 ? 11 : 10, 2};
        case MonsterMoveId::SPHERIC_GUARDIAN_HARDEN:        return {asc2 ? 11 : 10};
        case MonsterMoveId::SPHERIC_GUARDIAN_ATTACK_DEBUFF: return {asc2 ? 11 : 10};

        case MonsterMoveId::SPIKER_CUT:                     return {asc2 ? 9 : 7};

        case MonsterMoveId::SPIKE_SLIME_L_FLAME_TACKLE:     return {asc2 ? 18 : 16};
        case MonsterMoveId::SPIKE_SLIME_M_FLAME_TACKLE:     return {asc2 ? 10 : 8};
        case MonsterMoveId::SPIKE_SLIME_S_TACKLE:           return {asc2 ? 6 : 5};

        case MonsterMoveId::SPIRE_GROWTH_QUICK_TACKLE:      return {asc2 ? 18 : 16};
        case MonsterMoveId::SPIRE_GROWTH_SMASH:             return {asc2 ? 25 : 22};

        case MonsterMoveId::SPIRE_SHIELD_BASH:              return {asc3 ? 14 : 12};
        case MonsterMoveId::SPIRE_SHIELD_SMASH:             return {asc3 ? 38 : 34};

        case MonsterMoveId::SPIRE_SPEAR_BURN_STRIKE:        return {asc3 ? 6 : 5, 2};
        case MonsterMoveId::SPIRE_SPEAR_SKEWER:             return {10, asc3 ? 4 : 3};

        case MonsterMoveId::TASKMASTER_SCOURING_WHIP:       return {7};

        case MonsterMoveId::TORCH_HEAD_TACKLE:              return {7};

        case MonsterMoveId::THE_CHAMP_FACE_SLAP:            return {asc4 ? 14 : 12};
        case MonsterMoveId::THE_CHAMP_HEAVY_SLASH:          return {asc4 ? 18 : 16};
        case MonsterMoveId::THE_CHAMP_EXECUTE:              return {10, 2};

        case MonsterMoveId::THE_COLLECTOR_FIREBALL:         return {asc4 ? 21 : 18};

        case MonsterMoveId::THE_GUARDIAN_FIERCE_BASH:       return {asc4 ? 36 : 32};
        case MonsterMoveId::THE_GUARDIAN_WHIRLWIND:         return {5, 4};
        case MonsterMoveId::THE_GUARDIAN_ROLL_ATTACK:       return {asc4 ? 10 : 9};
        case MonsterMoveId::THE_GUARDIAN_TWIN_SLAM:         return {8, 2};

        case MonsterMoveId::THE_MAW_SLAM:                   return {asc2 ? 30 : 25};
        case MonsterMoveId::THE_MAW_NOM: {
            const auto t = (bc.getMonsterTurnNumber() + 1) / 2;
            return {5, t};
        }

        case MonsterMoveId::TIME_EATER_REVERBERATE:         return {asc4 ? 8 : 7, 3};
        case MonsterMoveId::TIME_EATER_HEAD_SLAM:           return {asc4 ? 32 : 26};

        case MonsterMoveId::TRANSIENT_ATTACK: {
            const auto damage = (asc2 ? 40 : 30) + 10*(bc.getMonsterTurnNumber()-1);
            return {damage};
        }

        case MonsterMoveId::WRITHING_MASS_FLAIL:            return {asc2 ? 16 : 15};
        case MonsterMoveId::WRITHING_MASS_MULTI_STRIKE:     return {asc2 ? 9 : 7, 3};
        case MonsterMoveId::WRITHING_MASS_STRONG_STRIKE:    return {asc2 ? 38 : 32};

        default:
            return {0,0};
    }
}