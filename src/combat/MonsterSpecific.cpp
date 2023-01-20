//
// Created by gamerpuppy on 8/21/2021.
//

#include "combat/Monster.h"
#include "game/Random.h"
#include "combat/BattleContext.h"
#include "game/GameContext.h"

using namespace sts;

// returns 0, 1, 2 based on value compared to a and b
// 0 if value < a
// 1 if value < b
// else 2
static int getTriIdx(int value, int a, int b) {
    if (value >= b) {
        return 2;
    } else if (value >= a) {
        return 1;
    } else {
        return 0;
    }
}

void Monster::initHp(Random &hpRng, int ascension) {
    switch (id) {
        case MonsterId::INVALID:
            assert(false);
            break;

        case MonsterId::ORB_WALKER:
            hpRng.random(90, 96);// first call is discarded by game
            setRandomHp(hpRng, ascension >= 7);
            break;

        case MonsterId::ACID_SLIME_L:
        case MonsterId::ACID_SLIME_M:
        case MonsterId::ACID_SLIME_S:
        case MonsterId::BEAR:
        case MonsterId::BLUE_SLAVER:
        case MonsterId::BYRD:
        case MonsterId::CENTURION:
        case MonsterId::CHOSEN:
        case MonsterId::CULTIST:
        case MonsterId::DARKLING:
        case MonsterId::EXPLODER:
        case MonsterId::FAT_GREMLIN:
        case MonsterId::FUNGI_BEAST:
        case MonsterId::GREEN_LOUSE:
        case MonsterId::GREMLIN_WIZARD:
        case MonsterId::JAW_WORM:
        case MonsterId::LOOTER:
        case MonsterId::MAD_GREMLIN:
        case MonsterId::MUGGER:
        case MonsterId::MYSTIC:
        case MonsterId::POINTY:
        case MonsterId::RED_LOUSE:
        case MonsterId::RED_SLAVER:
        case MonsterId::REPULSOR:
        case MonsterId::ROMEO:
        case MonsterId::SHELLED_PARASITE:
        case MonsterId::SHIELD_GREMLIN:
        case MonsterId::SNAKE_PLANT:
        case MonsterId::SNEAKY_GREMLIN:
        case MonsterId::SNECKO:
        case MonsterId::SPIKER:
        case MonsterId::SPIKE_SLIME_L:
        case MonsterId::SPIKE_SLIME_M:
        case MonsterId::SPIKE_SLIME_S:
        case MonsterId::SPIRE_GROWTH:
        case MonsterId::WRITHING_MASS:
            setRandomHp(hpRng, ascension >= 7);
            break;

        case MonsterId::AWAKENED_ONE:
        case MonsterId::BRONZE_AUTOMATON:
        case MonsterId::CORRUPT_HEART:
        case MonsterId::DECA:
        case MonsterId::DONU:
        case MonsterId::HEXAGHOST:
        case MonsterId::SLIME_BOSS:
        case MonsterId::THE_CHAMP:
        case MonsterId::THE_COLLECTOR:
        case MonsterId::THE_GUARDIAN:
        case MonsterId::TIME_EATER:
        case MonsterId::TORCH_HEAD:
            setRandomHp(hpRng, ascension >= 9);
            break;

        case MonsterId::BOOK_OF_STABBING:
        case MonsterId::DAGGER:
        case MonsterId::GIANT_HEAD:
        case MonsterId::GREMLIN_LEADER:
        case MonsterId::GREMLIN_NOB:
        case MonsterId::LAGAVULIN:
        case MonsterId::NEMESIS:
        case MonsterId::SENTRY:
        case MonsterId::SPIRE_SHIELD:
        case MonsterId::SPIRE_SPEAR:
            setRandomHp(hpRng, ascension >= 8);
            break;

        case MonsterId::REPTOMANCER:
            hpRng.random(180, 190);
            setRandomHp(hpRng, ascension >= 8);
            break;

        case MonsterId::BRONZE_ORB:
            hpRng.random(52, 58);
            setRandomHp(hpRng, ascension >= 9);
            break;

        case MonsterId::TASKMASTER:
            hpRng.random(54, 60);
            setRandomHp(hpRng, ascension >= 8);
            break;

        case MonsterId::SPHERIC_GUARDIAN:
        case MonsterId::THE_MAW:
        case MonsterId::TRANSIENT:
            curHp = monsterHpRange[static_cast<int>(id)][0][0];
            maxHp = curHp;
            break;

        default:
            assert(false);
    }
}

void Monster::preBattleAction(BattleContext &bc) {
    const bool asc4 = bc.ascension >= 4;
    const bool asc7 = bc.ascension >= 7;
    const bool asc9 = bc.ascension >= 9;
    const bool asc17 = bc.ascension >= 17;
    const bool asc18 = bc.ascension >= 18;
    const bool asc19 = bc.ascension >= 19;

    const int hallwayDiffIdx = getTriIdx(bc.ascension, 2, 17);

    switch (id) {
        case MonsterId::CORRUPT_HEART: {
            buff<MS::BEAT_OF_DEATH>(asc19 ? 2 : 1);
            buff<MS::INVINCIBLE>(asc19 ? 200 : 300);
            break;
        }

        case MonsterId::DAGGER:
            buff<MS::MINION>();
            break;

        case MonsterId::DARKLING:       // game adds regrow power
            buff<MS::REGROW>();
            break;

        case MonsterId::MAD_GREMLIN:
            buff<MS::ANGRY>(asc17 ? 2 : 1);
            break;

        case MonsterId::EXPLODER:       // game adds explosive power
            break;

        case MonsterId::GIANT_HEAD:     // game adds slow power
            setHasStatus<MS::SLOW>(true);
            setStatus<MS::SLOW>(0);
            break;

        case MonsterId::GREMLIN_LEADER: // game adds MinionPower to all gremlins
            buff<MS::MINION_LEADER>();
            break;

        case MonsterId::TRANSIENT:      // game adds ShiftingPower
            buff<MS::SHIFTING>();
            buff<MS::FADING>(asc17 ? 6 : 5);
            break;

        case MonsterId::BOOK_OF_STABBING:    // game adds PainfulStabsPower
            buff<MS::PAINFUL_STABS>();
            ++miscInfo; // stab count
            break;

        case MonsterId::FUNGI_BEAST:    // game adds SporeCloudPower
            buff<MS::SPORE_CLOUD>(2); // the value here isn't used. it is always 2
            break;

        case MonsterId::AWAKENED_ONE:
            // buff minion leader only in stage 2
            if (asc4) {
                buff<MS::STRENGTH>(2);
            }
            buff<MS::CURIOSITY>(asc19 ? 2 : 1);
            buff<MS::REGEN>(asc19 ? 15 : 10);
            break;

        case MonsterId::DECA:
        case MonsterId::DONU:
            buff<MS::ARTIFACT>(asc19 ? 3 : 2);
            break;

        case MonsterId::ORB_WALKER:
            buff<MS::GENERIC_STRENGTH_UP>(asc17 ? 5 : 3);
            break;

        case MonsterId::SPIKER: {
            const int thorns[] {3,4,7};
            buff<MS::THORNS>(thorns[hallwayDiffIdx]);
            break;
        }

        case MonsterId::WRITHING_MASS: {
            setHasStatus<MS::REACTIVE>(true);
            setStatus<MS::REACTIVE>(0);
            buff<MS::MALLEABLE>(3);
            break;
        }

        case MonsterId::BRONZE_AUTOMATON: {
            buff<MS::MINION_LEADER>();
            buff<MS::ARTIFACT>(3);
            break;
        }

        case MonsterId::TIME_EATER: {
            buff<MS::TIME_WARP>(0);
            break;
        }

        case MonsterId::BYRD: {
            buff<MS::FLIGHT>(asc17 ? 4 : 3);
            break;
        }

        case MonsterId::LOOTER:
        case MonsterId::MUGGER:
            buff<MS::THIEVERY>(asc17 ? 20 : 15);
            break;

        case MonsterId::REPTOMANCER:
            buff<MS::MINION_LEADER>();
            break;

        case MonsterId::SHELLED_PARASITE: {
            buff<MS::PLATED_ARMOR>(14);
            addBlock(14);
            break;
        }

        case MonsterId::SNAKE_PLANT: {
            buff<MS::MALLEABLE>(3);
            break;
        }

        case MonsterId::SPHERIC_GUARDIAN: {
            // game adds barricade
            buff<MS::ARTIFACT>(3);
            buff<MS::BARRICADE>();
            addBlock(40);
            break;
        }

        case MonsterId::SPIRE_SHIELD: {
            bc.player.buff<PS::SURROUNDED>();
            buff<MS::ARTIFACT>(asc18 ? 2 : 1);
            break;
        }

        case MonsterId::SPIRE_SPEAR: {
            buff<MS::ARTIFACT>(asc18 ? 2 : 1);
            break;
        }

        case MonsterId::THE_COLLECTOR:
            buff<MS::MINION_LEADER>();
            break;


            // handle in MonsterGroup instead
//        case MonsterId::JAW_WORM: {
//            if (bc.act == 3) {
//                const int str[] {3,4,5};
//                buff<MS::STRENGTH>(str[hallwayDiffIdx]);
//            }
//            break;
//        }

        case MonsterId::LAGAVULIN:
            if (hasStatus<MS::ASLEEP>()) {
                buff<MS::METALLICIZE>(8);
                addBlock(8);
            }
            break;

        case MonsterId::GREEN_LOUSE:
        case MonsterId::RED_LOUSE: {
            int curlUpMin;
            int curlUpMax;
            if (asc17) {
                curlUpMin = 9;
                curlUpMax = 12;
            } else if (asc7) {
                curlUpMin = 4;
                curlUpMax = 8;
            } else {
                curlUpMin = 3;
                curlUpMax = 7;
            }
            buff<MS::CURL_UP>(bc.monsterHpRng.random(curlUpMin, curlUpMax));
            break;
        }

        case MonsterId::SENTRY:
            buff<MS::ARTIFACT>(1);
            break;

        case MonsterId::THE_GUARDIAN: {
            // game adds ModeShiftPower
            int d;
            if (asc19) {
                d = 40;
            } else if (asc9) {
                d = 35;
            } else {
                d = 30;
            }
            miscInfo = d;
            buff<MS::MODE_SHIFT>(d);
            break;
        }

        default:
            break;
    }
}

void Monster::takeTurn(BattleContext &bc) {     // todo, maybe for monsters that do not appear in the Colosseum event, adding rollMove to bot is unnecessary, because if they die before rollMove would have been triggered, it can't affect the next fight
    const int asc = bc.ascension;

    const bool asc2 = bc.ascension >= 2;
    const bool asc3 = bc.ascension >= 3;
    const bool asc4 = bc.ascension >= 4;
    const bool asc9 = bc.ascension >= 9;
    const bool asc17 = bc.ascension >= 17;
    const bool asc18 = bc.ascension >= 18;
    const bool asc19 = bc.ascension >= 19;
    const int hallwayIdx = getTriIdx(asc, 2, 17);
    const int eliteDiffIdx = getTriIdx(bc.ascension, 3, 18);
    const int bossDiffIdx = getTriIdx(bc.ascension, 4, 19);

    switch (moveHistory[0]) {

        // ************ ACID_SLIME_L ************

        case MMID::ACID_SLIME_L_CORROSIVE_SPIT:
            attackPlayerHelper(bc, asc2 ? 12 : 11);
            bc.addToBot(Actions::MakeTempCardInDiscard({CardId::SLIMED}, 2));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::ACID_SLIME_L_LICK:
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(2, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::ACID_SLIME_L_SPLIT:
            largeSlimeSplit(bc, MonsterId::ACID_SLIME_M, idx, curHp);
            break;

        case MMID::ACID_SLIME_L_TACKLE:
            attackPlayerHelper(bc, asc2 ? 18 : 16);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::ACID_SLIME_M_CORROSIVE_SPIT:
            attackPlayerHelper(bc, asc2 ? 8 : 7);
            bc.addToBot(Actions::MakeTempCardInDiscard(CardId::SLIMED));
            bc.addToBot(Actions::RollMove(idx));
            break;

        // ************ ACID_SLIME_M ************

        case MMID::ACID_SLIME_M_LICK:
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(1, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::ACID_SLIME_M_TACKLE:
            attackPlayerHelper(bc, asc2 ? 12 : 10);
            bc.addToBot(Actions::RollMove(idx));
            break;

        // ************ ACID_SLIME_S ************

        case MMID::ACID_SLIME_S_LICK:
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(1, true));
            setMove(MMID::ACID_SLIME_S_TACKLE);
            break;

        case MMID::ACID_SLIME_S_TACKLE:
            attackPlayerHelper(bc, asc2 ? 4 : 3);
            setMove(MMID::ACID_SLIME_S_LICK);
            break;

        // ************ RED MASK BOIS ************

        case MMID::BEAR_BEAR_HUG:
            bc.player.debuff<PS::DEXTERITY>(asc17 ? -4 : -2);
            setMove(MMID::BEAR_LUNGE);
            break;

        case MMID::BEAR_LUNGE:
            attackPlayerHelper(bc, asc2 ? 10 : 9);
            bc.addToBot( Actions::MonsterGainBlock(idx, 9) );
            setMove(MMID::BEAR_MAUL);
            break;

        case MMID::BEAR_MAUL:
            attackPlayerHelper(bc, asc2 ? 20 : 18);
            setMove(MMID::BEAR_LUNGE);
            break;

        case MMID::POINTY_ATTACK:
            attackPlayerHelper(bc, asc2 ? 6 : 5, 2);
            break;

        case MMID::ROMEO_AGONIZING_SLASH:
            attackPlayerHelper(bc, asc2 ? 12 : 10);
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(asc17 ? 3 : 2, true) );
            setMove(MMID::ROMEO_CROSS_SLASH);
            break;

        case MMID::ROMEO_CROSS_SLASH:
            attackPlayerHelper(bc, asc2 ? 17 : 15);
            setMove(MMID::ROMEO_AGONIZING_SLASH);
            break;

        case MMID::ROMEO_MOCK: // 2
            setMove(MMID::ROMEO_AGONIZING_SLASH);
            break;

        // ************ BLUE SLAVER ************

        case MMID::BLUE_SLAVER_RAKE:
            // 4
            attackPlayerHelper(bc, asc2 ? 8 : 7);
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(asc17 ? 2 : 1, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::BLUE_SLAVER_STAB:
            // 1
            attackPlayerHelper(bc, asc2 ? 13 : 12);
            bc.addToBot(Actions::RollMove(idx));
            break;

        // ************ BOOK OF STABBING ************

        case MMID::BOOK_OF_STABBING_MULTI_STAB:
            attackPlayerHelper(bc, asc3 ? 7 : 6, miscInfo);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::BOOK_OF_STABBING_SINGLE_STAB:
            attackPlayerHelper(bc, asc3 ? 24 : 21);
            bc.addToBot(Actions::RollMove(idx));
            break;


        // ************ BRONZE AUTOMATON ************


        case MMID::BRONZE_AUTOMATON_BOOST: {// 5
            buff<MS::STRENGTH>(asc4 ? 4 : 3);
            addBlock(asc9 ? 12 : 9);
            auto &lastBoostWasFlail = miscInfo;
            if (lastBoostWasFlail) {
                setMove(MMID::BRONZE_AUTOMATON_HYPER_BEAM);
                lastBoostWasFlail = false;
            } else {
                setMove(MMID::BRONZE_AUTOMATON_FLAIL);
                lastBoostWasFlail = true;
            }
            bc.noOpRollMove();
            break;
        }

        case MMID::BRONZE_AUTOMATON_FLAIL: // 1
            attackPlayerHelper(bc, asc4 ? 8 : 7, 2);
            setMove(MMID::BRONZE_AUTOMATON_BOOST);
            bc.noOpRollMove();
            break;

        case MMID::BRONZE_AUTOMATON_HYPER_BEAM: // 2
            attackPlayerHelper(bc, asc4 ? 50 : 45);
            if (asc19) {
                setMove(MMID::BRONZE_AUTOMATON_BOOST);
            } else {
                setMove(MMID::BRONZE_AUTOMATON_STUNNED);
            }
            bc.noOpRollMove();
            break;

        case MMID::BRONZE_AUTOMATON_SPAWN_ORBS: // 4
            spawnBronzeOrbs(bc);
            setMove(MMID::BRONZE_AUTOMATON_FLAIL);
            bc.noOpRollMove();
            break;

        case MMID::BRONZE_AUTOMATON_STUNNED: // 3
            setMove(MMID::BRONZE_AUTOMATON_FLAIL);
            bc.noOpRollMove();
            break;

        case MMID::BRONZE_ORB_BEAM:
            attackPlayerHelper(bc, 8);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::BRONZE_ORB_STASIS:
            stasisAction(bc);
            miscInfo = 1;
            rollMove(bc);
            break;

        case MMID::BRONZE_ORB_SUPPORT_BEAM:
            bc.monsters.arr[1].addBlock(12);
            rollMove(bc);
            break;


        // ************ BYRD ************

        case MMID::BYRD_CAW:
            buff<MS::STRENGTH>(1);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::BYRD_FLY:
            buff<MS::FLIGHT>(asc17 ? 4 : 3);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::BYRD_HEADBUTT:
            attackPlayerHelper(bc, 3);
            setMove(MMID::BYRD_FLY);
            break;

        case MMID::BYRD_PECK:
            attackPlayerHelper(bc, 1, asc2 ? 6 : 5);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::BYRD_STUNNED:
            bc.noOpRollMove();
            setMove(MMID::BYRD_HEADBUTT);
            break;

        case MMID::BYRD_SWOOP:
            attackPlayerHelper(bc, asc2 ? 14 : 12);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::CENTURION_DEFEND: {
            if (bc.monsters.getAliveCount() > 1) {
                auto &mystic = bc.monsters.arr[1];
                mystic.addBlock(asc17 ? 20 : 15);
            }
            rollMove(bc);
            break;
        }

        case MMID::CENTURION_FURY:
            attackPlayerHelper(bc, asc2 ? 7 : 6, 3);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::CENTURION_SLASH:
            attackPlayerHelper(bc, asc2 ? 14 : 12);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::MYSTIC_ATTACK_DEBUFF: {
            attackPlayerHelper(bc, asc2 ? 9 : 8);
            bc.addToBot( Actions::DebuffPlayer<PS::FRAIL>(2, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        case MMID::MYSTIC_BUFF: {
            const int strAmts[] {2,3,4};
            const auto strBuff = strAmts[hallwayIdx];
            if (bc.monsters.monstersAlive > 1) {
                auto &knight = bc.monsters.arr[0];
                knight.buff<MS::STRENGTH>(strBuff);
            }
            buff<MS::STRENGTH>(strBuff);
            rollMove(bc);
            break;
        }

        case MMID::MYSTIC_HEAL: {
            const auto healAmt = asc17 ? 20 : 16;
            if (bc.monsters.monstersAlive > 1) {
                auto &knight = bc.monsters.arr[0];
                knight.heal(healAmt);
            }
            heal(healAmt);
            rollMove(bc);
            break;
        }


        // ************ CHOSEN ************

        case MMID::CHOSEN_DEBILITATE: // 3
            attackPlayerHelper(bc, asc2 ? 12 : 10);
            bc.addToBot(Actions::DebuffPlayer<PS::VULNERABLE>(2, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::CHOSEN_DRAIN: // 2
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(3, true));
            buff<MS::STRENGTH>(3);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::CHOSEN_HEX: // 4
            bc.addToBot(Actions::DebuffPlayer<PS::HEX>(1));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::CHOSEN_POKE: // 5
            attackPlayerHelper(bc, asc2 ? 6 : 5, 2);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::CHOSEN_ZAP: // 1
            attackPlayerHelper(bc, asc2 ? 21 : 18);
            bc.addToBot(Actions::RollMove(idx));
            break;

            // ************ FAT GREMLIN ************

        case MMID::FAT_GREMLIN_SMASH: {
            attackPlayerHelper(bc, asc2 ? 5 : 4);
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(1, true));
            if (asc17) {
                bc.addToBot(Actions::DebuffPlayer<PS::FRAIL>(1, true));
            }

            if (doesEscapeNext()) {
                setMove(MMID::GENERIC_ESCAPE_MOVE);
            } else {
                bc.addToBot(Actions::NoOpRollMove());
            }
            break;
        }

        case MMID::MAD_GREMLIN_SCRATCH: {
            attackPlayerHelper(bc, asc2 ? 5 : 4);
            if (doesEscapeNext()) {
                setMove(MMID::GENERIC_ESCAPE_MOVE);
            } else {
                bc.addToBot(Actions::NoOpRollMove());
            }
            break;
        }

        case MMID::SNEAKY_GREMLIN_PUNCTURE: {
            attackPlayerHelper(bc, asc2 ? 10 : 9);
            if (doesEscapeNext()) {
                setMove(MMID::GENERIC_ESCAPE_MOVE);
            }
            break;
        }

        case MMID::CULTIST_DARK_STRIKE:
            attackPlayerHelper(bc, 6);
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MMID::CULTIST_INCANTATION: {
            const int ritualAmount[] = {3, 4, 5};
            buff<MS::RITUAL>(ritualAmount[hallwayIdx]);
            setMove(MMID::CULTIST_DARK_STRIKE);
            bc.noOpRollMove();
            break;
        }

            // ************ FUNGI BEAST ************

        case MMID::FUNGI_BEAST_BITE:
            attackPlayerHelper(bc, 6);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::FUNGI_BEAST_GROW: {
            const int strengthBuff[] = {3, 4, 5};
            buff<MS::STRENGTH>(strengthBuff[hallwayIdx]);
            bc.addToBot(Actions::RollMove(idx));
            break;
        }

        // ************ GREMLIN LEADER ************

        /*
         *  Gremlin Leader always lives in the 4th monster slot (idx=3)
         *  Up to 3 minions can be alive at the same time
         */

        case MMID::GREMLIN_LEADER_ENCOURAGE: { // 3
            const int strBuff[] {3,4,5};
            const auto strGain = strBuff[eliteDiffIdx];

            bc.aiRng.random(0, 2); // for in game quote
            // buff all monsters
            // not going to use action queue here, doesn't seem necessary
            for (int i = 0; i < 3; ++i) {
                auto &minion = bc.monsters.arr[i];
                if (!minion.isDying()) {
                    minion.buff<MS::STRENGTH>(strGain);
                    minion.addBlock(asc3 ? 10 : 6);
                }
            }
            buff<MS::STRENGTH>(strGain);
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        case MMID::GREMLIN_LEADER_RALLY: { // 2
            // summon two gremlins in open slots
            bc.addToBot(Actions::SummonGremlins() );
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        case MMID::GREMLIN_LEADER_STAB: { // 4
            attackPlayerHelper(bc, 6, 3);
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        // ************ GREEN LOUSE ************

        case MMID::GREEN_LOUSE_BITE:
            attackPlayerHelper(bc, miscInfo);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::GREEN_LOUSE_SPIT_WEB:
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(2)); // isSourceMonster true
            bc.addToBot(Actions::RollMove(idx));
            break;

        // ************ GREMLIN NOB ************

        case MMID::GREMLIN_NOB_BELLOW:
            buff<MS::ENRAGE>(asc18 ? 3 : 2);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::GREMLIN_NOB_RUSH:
            attackPlayerHelper(bc, asc3 ? 16 : 14);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::GREMLIN_NOB_SKULL_BASH:
            attackPlayerHelper(bc, asc3 ? 8 : 6);
            bc.addToBot(Actions::DebuffPlayer<PS::VULNERABLE>(2, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        // ************ GREMLIN WIZARD ************

        case MMID::GREMLIN_WIZARD_CHARGING: { // 2
            ++miscInfo;
            if (miscInfo == 3) {
                setMove(MMID::GREMLIN_WIZARD_ULTIMATE_BLAST);
            }
            break;
        }

        case MMID::GREMLIN_WIZARD_ULTIMATE_BLAST: { // 1
            attackPlayerHelper(bc, asc2 ? 30 : 25);
            if (!asc17) {
                miscInfo = 0; // gremlin wizard charge
                setMove(MMID::GREMLIN_WIZARD_CHARGING);
            }
            break;
        }

        // ************ HEXAGHOST ************

        case MMID::HEXAGHOST_ACTIVATE: {// 5
            miscInfo = bc.player.curHp / 12 + 1; // set divider damage
            setMove(MMID::HEXAGHOST_DIVIDER);
            bc.noOpRollMove();
            break;
        }

        case MMID::HEXAGHOST_DIVIDER: // 1
            attackPlayerHelper(bc, miscInfo, 6);
            uniquePower0 = 0;
            setMove(MMID::HEXAGHOST_SEAR);
            bc.addToBot( Actions::NoOpRollMove() );
            break;

        case MMID::HEXAGHOST_INFERNO: // 6
            attackPlayerHelper(bc, asc4 ? 3 : 2, 6);
            uniquePower0 = 0;
            setMove(MMID::HEXAGHOST_SEAR);
            bc.addToBot( Actions::NoOpRollMove() );
            break;

        case MMID::HEXAGHOST_INFLAME: // 3
            addBlock(12);
            buff<MS::STRENGTH>(asc19 ? 3 : 2);
            ++uniquePower0;
            setMove(MMID::HEXAGHOST_TACKLE);
            bc.noOpRollMove();
            break;

        case MMID::HEXAGHOST_SEAR: // 4
            attackPlayerHelper(bc, 6);
            bc.addToBot (
                    Actions::MakeTempCardInDiscard(CardInstance(CardId::BURN, bc.turn > 8), asc19 ? 2 : 1)
            );
            if (uniquePower0 == 0) {
                setMove(MMID::HEXAGHOST_TACKLE);

            } else if (uniquePower0 == 2) {
                setMove(MMID::HEXAGHOST_INFLAME);

            } else {
                setMove(MMID::HEXAGHOST_INFERNO);
            }
            ++uniquePower0;
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MMID::HEXAGHOST_TACKLE: // 2
            attackPlayerHelper(bc, asc4 ? 6 : 5, 2);
            setMove(MMID::HEXAGHOST_SEAR);
            ++uniquePower0;
            bc.addToBot(Actions::NoOpRollMove());
            break;


            // ************ JAW WORM ************

        case MMID::JAW_WORM_CHOMP:
            attackPlayerHelper(bc, asc2 ? 12 : 11);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::JAW_WORM_BELLOW: {
            const int strengthBuff[] = {3,4,5};
            buff<MS::STRENGTH>(strengthBuff[hallwayIdx]);
            bc.addToBot(Actions::MonsterGainBlock(idx, asc17 ? 9 : 6));
            bc.addToBot(Actions::RollMove(idx));
            break;
        }

        case MMID::JAW_WORM_THRASH:
            attackPlayerHelper(bc, 7);
            bc.addToBot(Actions::MonsterGainBlock(idx, 5));
            bc.addToBot(Actions::RollMove(idx));
            break;

        // ************ LAGAVULIN ************

        case MMID::LAGAVULIN_ATTACK:
            attackPlayerHelper(bc, asc3 ? 20 : 18);
            if (lastTwoMoves(MMID::LAGAVULIN_ATTACK)) {
                setMove(MMID::LAGAVULIN_SIPHON_SOUL);
            } else {
                setMove(MMID::LAGAVULIN_ATTACK);
            }
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MMID::LAGAVULIN_SIPHON_SOUL:
            Actions::DebuffPlayer<PS::DEXTERITY>(asc18 ? -2 : -1).actFunc(bc);
            Actions::DebuffPlayer<PS::STRENGTH>(asc18 ? -2 : -1).actFunc(bc);
            setMove(MMID::LAGAVULIN_ATTACK);
            bc.noOpRollMove();
            break;

        case MMID::LAGAVULIN_SLEEP:
            if (bc.turn == 2 || !hasStatus<MS::ASLEEP>()) {
                setMove(MMID::LAGAVULIN_ATTACK);
            } else {
                setMove(MMID::LAGAVULIN_SLEEP);
            }
            bc.noOpRollMove();
            break;

        // ************ LOOTER ************

        case MMID::LOOTER_ESCAPE: { // 3
#ifdef sts_asserts
      assert(curHp > 0);
#endif
            isEscapingB = true;
            bc.monsters.monstersAlive--;
            if (bc.monsters.getAliveCount() == 0) {
                bc.outcome = Outcome::PLAYER_VICTORY;
            }
            break;
        }

        case MMID::LOOTER_LUNGE: {// 4
            stealGoldFromPlayer(bc, getStatus<MS::THIEVERY>());
            attackPlayerHelper(bc, asc2 ? 14 : 12);
            setMove(MMID::LOOTER_SMOKE_BOMB);
            break;
        }

        case MMID::LOOTER_MUG: { // 1
            if (bc.getMonsterTurnNumber() == 1) {
                bc.aiRng.randomBoolean(0.6f); // for a dialog message in game
            }
            stealGoldFromPlayer(bc, getStatus<MS::THIEVERY>());
            attackPlayerHelper(bc, asc2 ? 11 : 10);
            if (bc.getMonsterTurnNumber() == 1) {
                setMove(MMID::LOOTER_MUG);

            } else {
                const auto nextMove = bc.aiRng.randomBoolean(0.5f) ?
                                      MMID::LOOTER_SMOKE_BOMB : MMID::LOOTER_LUNGE;

                setMove(nextMove);
            }
            break;
        }

        case MMID::LOOTER_SMOKE_BOMB: { // 2
            addBlock(6);
            setMove(MMID::LOOTER_ESCAPE);
            break;
        }

        // ************ MUGGER ************

        case MMID::MUGGER_ESCAPE: { // 3
#ifdef sts_asserts
            assert(curHp > 0);
#endif
            isEscapingB = true;
            bc.monsters.monstersAlive--;
            if (bc.monsters.getAliveCount() == 0) {
                bc.outcome = Outcome::PLAYER_VICTORY;
            }
            break;
        }

        case MMID::MUGGER_LUNGE: { // 4
            bc.aiRng.random(2); // for a dialog message in gam
            stealGoldFromPlayer(bc, getStatus<MS::THIEVERY>());
            attackPlayerHelper(bc, asc2 ? 18 : 16);
            setMove(MMID::MUGGER_SMOKE_BOMB);
            break;
        }

        case MMID::MUGGER_MUG: { // 1
            bc.aiRng.random(2); // for a dialog message in game
            if (bc.getMonsterTurnNumber() == 2) {
                bc.aiRng.randomBoolean(0.6f); // for a dialog message in game
            }
            stealGoldFromPlayer(bc, getStatus<MS::THIEVERY>());
            attackPlayerHelper(bc, asc2 ? 11 : 10);

            if (bc.getMonsterTurnNumber() == 2) {
                const auto nextMove = bc.aiRng.randomBoolean(0.5f) ?
                        MMID::MUGGER_SMOKE_BOMB : MMID::MUGGER_LUNGE;

                setMove(nextMove);

            } else {
                setMove(MMID::MUGGER_MUG);
            }
            break;
        }

        case MMID::MUGGER_SMOKE_BOMB: { // 2
            addBlock(asc17 ? 17 : 11);
            setMove(MMID::MUGGER_ESCAPE);
            break;
        }

        case MMID::ORB_WALKER_CLAW: // 2
            attackPlayerHelper(bc, asc2 ? 16 : 15);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::ORB_WALKER_LASER: { // 1
            attackPlayerHelper(bc, asc2 ? 11 : 10);
            bc.addToBot( Actions::ShuffleTempCardIntoDrawPile(CardId::BURN, 1) );
            bc.addToBot( Actions::MakeTempCardInDiscard({CardId::BURN}) );
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        // ************ RED LOUSE ************

        case MMID::RED_LOUSE_BITE:
            attackPlayerHelper(bc, miscInfo);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::RED_LOUSE_GROW:
            buff<MS::STRENGTH>(asc17 ? 4 : 3);
            bc.addToBot(Actions::RollMove(idx));
            break;

        // ************ RED SLAVER ************

        case MMID::RED_SLAVER_ENTANGLE:
            bc.addToBot(Actions::DebuffPlayer<PS::ENTANGLED>(1));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::RED_SLAVER_SCRAPE:
            attackPlayerHelper(bc, asc2 ? 9 : 8);
            bc.addToBot(Actions::DebuffPlayer<PS::VULNERABLE>(asc17 ? 2 : 1));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::RED_SLAVER_STAB:
            attackPlayerHelper(bc, asc2 ? 14 : 13);
            bc.addToBot(Actions::RollMove(idx));
            break;

        // ************ SENTRY ************

        case MMID::SENTRY_BEAM:
            attackPlayerHelper(bc, asc3 ? 10 : 9);
            setMove(MMID::SENTRY_BOLT);
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MMID::SENTRY_BOLT:
            bc.addToBot(Actions::MakeTempCardInDiscard({CardId::DAZED}, asc18 ? 3 : 2));
            setMove(MMID::SENTRY_BEAM);
            bc.addToBot(Actions::NoOpRollMove());
            break;


        // ************ SHELLED PARASITE ************

        case MMID::SHELLED_PARASITE_DOUBLE_STRIKE: // 2
            attackPlayerHelper(bc, asc2 ? 7 : 6, 2);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SHELLED_PARASITE_FELL: // 1
            attackPlayerHelper(bc, asc2 ? 21 : 18);
            bc.addToBot( Actions::DebuffPlayer<PS::FRAIL>(2, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SHELLED_PARASITE_STUNNED: // 4
            setMove(MMID::SHELLED_PARASITE_FELL);
            rollMove(bc);
            break;

        case MMID::SHELLED_PARASITE_SUCK: // 3
            bc.addToBot( Actions::VampireAttack(calculateDamageToPlayer(bc, asc2 ? 12 : 10)) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        // ************ SHIELD GREMLIN ************

        case MMID::SHIELD_GREMLIN_PROTECT: {
            const int blockAmounts[] = {7,8,11};
            const int blockAmount = blockAmounts[getTriIdx(asc, 7, 17)];
            bc.addToBot( Actions::GainBlockRandomEnemy(idx, blockAmount) );
            if (bc.monsters.getAliveCount() <= 1)  {
                setMove(MMID::SHIELD_GREMLIN_SHIELD_BASH);
            }
            break;
        }

        case MMID::SHIELD_GREMLIN_SHIELD_BASH:
            attackPlayerHelper(bc, asc2 ? 8 : 6);
            break;

        // ************ SLIME BOSS ************

        case MMID::SLIME_BOSS_GOOP_SPRAY:
            Actions::MakeTempCardInDiscard( {CardId::SLIMED}, asc19 ? 5 : 3).actFunc(bc);
            setMove(MMID::SLIME_BOSS_PREPARING);
            break;

        case MMID::SLIME_BOSS_PREPARING:
            setMove(MMID::SLIME_BOSS_SLAM);
            break;

        case MMID::SLIME_BOSS_SLAM:
            attackPlayerHelper(bc, asc4 ? 38 : 35);
            setMove(MMID::SLIME_BOSS_GOOP_SPRAY); // the attack is executed after, which is critical
            break;

        case MMID::SLIME_BOSS_SPLIT:
            slimeBossSplit(bc, curHp);
            break;

        // ************ SNAKE PLANT ************

        case MMID::SNAKE_PLANT_CHOMP:
            attackPlayerHelper(bc, asc2 ? 8 : 7, 3);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SNAKE_PLANT_ENFEEBLING_SPORES:
            bc.addToBot( Actions::DebuffPlayer<PS::FRAIL>(2, true) );
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(2, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;


        // ************ SNECKO ************

        case MMID::SNECKO_BITE: // 2
            attackPlayerHelper(bc, asc2 ? 18 : 15);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SNECKO_PERPLEXING_GLARE: // 1
            bc.addToBot( Actions::DebuffPlayer<PS::CONFUSED>() );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SNECKO_TAIL_WHIP: // 3
            attackPlayerHelper(bc, asc2 ? 10 : 8);
            bc.addToBot( Actions::DebuffPlayer<PS::VULNERABLE>(2, true) );
            if (asc17) {
                bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(2, true) );
            }
            bc.addToBot( Actions::RollMove(idx) );
            break;

        // ************ SPHERIC_GUARDIAN ************

        case MMID::SPHERIC_GUARDIAN_ACTIVATE: // 2
            addBlock(asc17 ? 35 : 25);
            setMove(MMID::SPHERIC_GUARDIAN_ATTACK_DEBUFF);
            bc.noOpRollMove();
            break;

        case MMID::SPHERIC_GUARDIAN_ATTACK_DEBUFF: // 4
            attackPlayerHelper(bc, asc2 ? 11 : 10);
            bc.addToBot( Actions::DebuffPlayer<PS::FRAIL>(5, true) );
            setMove(MMID::SPHERIC_GUARDIAN_SLAM);
            bc.noOpRollMove();
            break;

        case MMID::SPHERIC_GUARDIAN_HARDEN: // 3
            bc.addToBot( Actions::MonsterGainBlock(idx, 15) );
            attackPlayerHelper(bc, asc2 ? 11 : 10);
            setMove(MMID::SPHERIC_GUARDIAN_SLAM);
            bc.noOpRollMove();
            break;

        case MMID::SPHERIC_GUARDIAN_SLAM: // 1
            attackPlayerHelper(bc, asc2 ? 11 : 10, 2);
            setMove(MMID::SPHERIC_GUARDIAN_HARDEN);
            bc.noOpRollMove();
            break;

        // ************ SPIKE SLIME M ************

        case MMID::SPIKE_SLIME_M_LICK: {
            bc.addToBot(Actions::DebuffPlayer<PS::FRAIL>(1));
            bc.addToBot(Actions::RollMove(idx));
            break;
        }

        case MMID::SPIKE_SLIME_M_FLAME_TACKLE: {
            attackPlayerHelper(bc, asc2 ? 10 : 8);
            bc.addToBot(Actions::MakeTempCardInDiscard(CardInstance(CardId::SLIMED)));
            bc.addToBot(Actions::RollMove(idx));
            break;
        }

        // ************ SPIKE SLIME L ************

        case MMID::SPIKE_SLIME_L_FLAME_TACKLE:
            attackPlayerHelper(bc, asc2 ? 18 : 16);
            bc.addToBot( Actions::MakeTempCardInDiscard({CardId::SLIMED}, 2) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SPIKE_SLIME_L_LICK: // 4
            bc.addToBot( Actions::DebuffPlayer<PS::FRAIL>(asc17 ? 3 : 2, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SPIKE_SLIME_L_SPLIT: // 3
            largeSlimeSplit(bc, MonsterId::SPIKE_SLIME_M, idx, curHp);
            break;

        // ************ SPIKE SLIME S ************

        case MMID::SPIKE_SLIME_S_TACKLE: {
            attackPlayerHelper(bc, asc2 ? 6 : 5);
            bc.addToBot(Actions::NoOpRollMove());
            break;
        }

        // ************ TASKMASTER ************

        case MMID::TASKMASTER_SCOURING_WHIP: // todo buff after calculating damage so no need to add to bot?
            attackPlayerHelper(bc, 7);
            if (asc18) {
                bc.addToBot( Actions::BuffEnemy<MS::STRENGTH>(idx, 1) );
                bc.addToBot( Actions::MakeTempCardInDiscard({CardId::WOUND}, 3) );

            } else if (asc3) {
                bc.addToBot( Actions::MakeTempCardInDiscard({CardId::WOUND}, 2) );

            } else {
                bc.addToBot( Actions::MakeTempCardInDiscard({CardId::WOUND}) );
            }
            bc.noOpRollMove();
            break;

        // ************ THE CHAMP ************

        case MMID::THE_CHAMP_ANGER: {
            const int strAmts[3] {6, 9, 12};
            removeDebuffs();
            buff<MS::STRENGTH>(strAmts[bossDiffIdx]);
            rollMove(bc);
            break;
        }

        case MMID::THE_CHAMP_DEFENSIVE_STANCE: {
            const int blockAmts[3] {15, 18, 20};
            const int metallicizeAmts[3] {5, 6, 7};
            const auto buffIdx = getTriIdx(bc.ascension, 9, 19);

            const auto blockAmt = blockAmts[buffIdx];
            const auto metallicizeAmt = metallicizeAmts[buffIdx];

            addBlock(blockAmt);
            buff<MS::METALLICIZE>(metallicizeAmt);

            rollMove(bc);
            break;
        }

        case MMID::THE_CHAMP_EXECUTE: {
            attackPlayerHelper(bc, 10, 2);
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        case MMID::THE_CHAMP_FACE_SLAP: {
            attackPlayerHelper(bc, asc4 ? 14 : 12);
            bc.addToBot( Actions::DebuffPlayer<PS::FRAIL>(2, true) );
            bc.addToBot( Actions::DebuffPlayer<PS::VULNERABLE>(2, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        case MMID::THE_CHAMP_GLOAT: {
            const int strAmts[3] {3, 4, 5};
            buff<MS::STRENGTH>(strAmts[bossDiffIdx]);
            rollMove(bc);
            break;
        }

        case MMID::THE_CHAMP_HEAVY_SLASH: {
            attackPlayerHelper(bc, asc4 ? 18 : 16);
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        case MMID::THE_CHAMP_TAUNT: {
            bc.player.debuff<PS::WEAK>(2, true);
            bc.player.debuff<PS::VULNERABLE>(2, true);
            rollMove(bc);
            break;
        }

        // ************ THE COLLECTOR ************

        case MMID::THE_COLLECTOR_BUFF: { //3
            const int strAmounts[3] {3, 4, 5};
            const int blockAmounts[3] {15, 18, 23};

            for (int i = 0; i < 2; ++i) {
                auto &torchHead = bc.monsters.arr[i];
                if (!torchHead.isDying()) {
                    torchHead.buff<MS::STRENGTH>(strAmounts[bossDiffIdx]);
                }
            }
            buff<MS::STRENGTH>(strAmounts[bossDiffIdx]);
            addBlock(blockAmounts[bossDiffIdx]);

            bc.addToBot(Actions::RollMove(idx));
            break;
        }

        case MMID::THE_COLLECTOR_FIREBALL: // 2
            attackPlayerHelper(bc, asc4 ? 21 : 18);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::THE_COLLECTOR_MEGA_DEBUFF: // 4
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(3, true) );
            bc.addToBot( Actions::DebuffPlayer<PS::VULNERABLE>(3, true) );
            bc.addToBot( Actions::DebuffPlayer<PS::FRAIL>(3, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::THE_COLLECTOR_SPAWN: // 5, 1(initial spawn)
            bc.addToBot( Actions::SpawnTorchHeads() );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        // ************ THE GUARDIAN ************

        case MMID::THE_GUARDIAN_CHARGING_UP:
            addBlock(9);
            setMove(MMID::THE_GUARDIAN_FIERCE_BASH);
            break;

        case MMID::THE_GUARDIAN_DEFENSIVE_MODE:
            buff<MS::SHARP_HIDE>(asc19 ? 4 : 3);
            setMove(MMID::THE_GUARDIAN_ROLL_ATTACK);
            break;

        case MMID::THE_GUARDIAN_FIERCE_BASH:
            attackPlayerHelper(bc, asc4 ? 36 : 32);
            setMove(MMID::THE_GUARDIAN_VENT_STEAM);
            break;

        case MMID::THE_GUARDIAN_ROLL_ATTACK:
            attackPlayerHelper(bc, asc4 ? 10 : 9);
            setMove(MMID::THE_GUARDIAN_TWIN_SLAM);
            break;

        case MMID::THE_GUARDIAN_TWIN_SLAM:
            attackPlayerHelper(bc, 8, 2);
            removeStatus<MS::SHARP_HIDE>();
            miscInfo += 10;
            setMove(MMID::THE_GUARDIAN_WHIRLWIND);
            bc.addToBot( Actions::BuffEnemy<MS::MODE_SHIFT>(idx, miscInfo) );
            break;

        case MMID::THE_GUARDIAN_VENT_STEAM:
            bc.addToBot( Actions::DebuffPlayer<PS::VULNERABLE>(2, true) );
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(2, true) );
            setMove(MMID::THE_GUARDIAN_WHIRLWIND);
            break;

        case MMID::THE_GUARDIAN_WHIRLWIND:
            attackPlayerHelper(bc, 5, 4);
            setMove(MMID::THE_GUARDIAN_CHARGING_UP);
            break;


        // ************ TORCH HEAD ************

        case MMID::TORCH_HEAD_TACKLE:
            attackPlayerHelper(bc, 7);
            break;

        // ************ SHAPES ************

        case MMID::EXPLODER_EXPLODE:
            bc.addToBot( Actions::DamagePlayer(30) );
            bc.addToBot( Actions::SuicideAction(idx, true) );
            bc.noOpRollMove();
            break;

        case MMID::EXPLODER_SLAM: // 1
            attackPlayerHelper(bc, asc2 ? 11 : 9);
            if (lastTwoMoves(MMID::EXPLODER_SLAM)) {
                setMove(MMID::EXPLODER_EXPLODE);
            } else {
                setMove(MonsterMoveId::EXPLODER_SLAM);
            }
            bc.noOpRollMove();
            break;

        case MMID::REPULSOR_BASH: // 2
            attackPlayerHelper(bc, asc2 ? 13 : 11);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::REPULSOR_REPULSE: // 1
            Actions::ShuffleTempCardIntoDrawPile(CardId::DAZED, 2).actFunc(bc);
            rollMove(bc);
            break;

        case MMID::SPIKER_CUT: // 1
            attackPlayerHelper(bc, asc2 ? 9 : 7);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SPIKER_SPIKE: // 2
            ++miscInfo; // used thorns count
            buff<MS::THORNS>(2);
            rollMove(bc);
            break;


        // ************ THE MAW ************

        case MMID::THE_MAW_DROOL: // 4
            buff<MS::STRENGTH>(asc17 ? 5 : 3);
            rollMove(bc);
            break;

        case MMID::THE_MAW_NOM: { // 5
            const auto t = (bc.getMonsterTurnNumber() + 1) / 2;
            attackPlayerHelper(bc, 5, t);
            setMove(MMID::THE_MAW_DROOL);
            bc.noOpRollMove();
            break;
        }

        case MMID::THE_MAW_ROAR: { // 2
            bc.player.debuff<PS::WEAK>(asc17 ? 5 : 3, true);
            bc.player.debuff<PS::FRAIL>(asc17 ? 5 : 3, true);
            rollMove(bc);
            break;
        }

        case MMID::THE_MAW_SLAM: // 3
            attackPlayerHelper(bc, asc2 ? 30 : 25);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        // ************ DARKLING ************

        case MMID::DARKLING_CHOMP:
            attackPlayerHelper(bc, asc2 ? 9 : 8);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::DARKLING_HARDEN:
            addBlock(12);
            if (asc17) {
                buff<MS::STRENGTH>(2);
            }
            rollMove(bc);
            break;

        case MMID::DARKLING_NIP: {
            const auto damage = miscInfo + (asc2 ? 2 : 0); // todo maybe make d part of the miscInfo at prebattle
            attackPlayerHelper(bc, damage);
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        case MMID::DARKLING_REGROW:
            // do nothing
            rollMove(bc);
            break;

        case MMID::DARKLING_REINCARNATE:
            // revive with 50% hp
            // todo does it heep its buffs and debuffs?
            curHp = maxHp / 2;
            halfDead = false;
            ++bc.monsters.monstersAlive;

            buff<MS::REGROW>();
            if (bc.player.hasRelic<R::PHILOSOPHERS_STONE>()) {
                buff<MS::STRENGTH>(1);
            }

            rollMove(bc);
            break;

        // ************ SPIRE GROWTH ************

        case MMID::SPIRE_GROWTH_QUICK_TACKLE: // 1
            attackPlayerHelper(bc, asc2 ? 18 : 16);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SPIRE_GROWTH_SMASH: // 3
            attackPlayerHelper(bc, asc2 ? 25 : 22);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SPIRE_GROWTH_CONSTRICT: // 2
            bc.player.debuff<PS::CONSTRICTED>(asc17 ? 12 : 10);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        // ************ TRANSIENT ************

        case MMID::TRANSIENT_ATTACK: {

            const auto damage = (asc2 ? 40 : 30) + 10*(bc.getMonsterTurnNumber()-1);
            attackPlayerHelper(bc, damage);
            if (getStatus<MS::FADING>() == 1) {
                bc.addToBot( Actions::SuicideAction(idx, false) );
            }
            bc.noOpRollMove();
            decrementStatus<MS::FADING>();
            break;
        }

        // ************ WRITHING MASS ************

        case MMID::WRITHING_MASS_FLAIL: // 2
            attackPlayerHelper(bc, asc2 ? 16 : 15);
            bc.addToBot( Actions::MonsterGainBlock(idx, asc2 ? 18 : 16) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::WRITHING_MASS_IMPLANT: // 4
            miscInfo = true;
            if (!bc.player.hasRelic<R::OMAMORI>()) {
                if (bc.player.hasRelic<R::DARKSTONE_PERIAPT>()) {
                    bc.player.increaseMaxHp(6);
                }
            }
            rollMove(bc);
            break;

        case MMID::WRITHING_MASS_MULTI_STRIKE: // 1
            attackPlayerHelper(bc, asc2 ? 9 : 7, 3);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::WRITHING_MASS_STRONG_STRIKE: // 0
            attackPlayerHelper(bc, asc2 ? 38 : 32);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::WRITHING_MASS_WITHER: // 3
            attackPlayerHelper(bc, asc2 ? 12 : 10);
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(2, true) );
            bc.addToBot( Actions::DebuffPlayer<PS::VULNERABLE>(2, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::GIANT_HEAD_COUNT: // 3
            attackPlayerHelper(bc, 13);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::GIANT_HEAD_GLARE: // 1
            bc.player.debuff<PS::WEAK>(1, true);
            rollMove(bc);
            break;

        case MMID::GIANT_HEAD_IT_IS_TIME: { // 2
            const auto t = std::min(bc.getMonsterTurnNumber()-5, 6) * 5;
            const auto damage = (asc3 ? 40 : 30) + t;
            attackPlayerHelper(bc, damage); // todo this can be done immediately
            bc.noOpRollMove();
            break;
        }

        case MMID::NEMESIS_ATTACK:
            attackPlayerHelper(bc, asc3 ? 7 : 6, 3);
            bc.addToBot( Actions::RollMove(idx) );
            if (!hasStatus<MS::INTANGIBLE>()) {
                bc.addToBot( Actions::BuffEnemy<MS::INTANGIBLE>(idx, 2) );
            }
            break;

        case MMID::NEMESIS_DEBUFF:
            Actions::MakeTempCardInDiscard({CardId::BURN}, asc3 ? 5 : 3).actFunc(bc);
            rollMove(bc);
            if (!hasStatus<MS::INTANGIBLE>()) {
                buff<MS::INTANGIBLE>(2);
            }
            break;

        case MMID::NEMESIS_SCYTHE:
            attackPlayerHelper(bc, 45);
            bc.addToBot( Actions::RollMove(idx) );
            if (!hasStatus<MS::INTANGIBLE>()) {
                bc.addToBot( Actions::BuffEnemy<MS::INTANGIBLE>(idx, 2) );
            }
            break;

        case MMID::REPTOMANCER_BIG_BITE: // 3
            attackPlayerHelper(bc, asc3 ? 34 : 30);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::REPTOMANCER_SNAKE_STRIKE: // 1
            attackPlayerHelper(bc, asc3 ? 16 : 13, 2);
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(1, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::REPTOMANCER_SUMMON: // 2
            reptomancerSummon(bc, asc18 ? 2 : 1);
            rollMove(bc);
            break;

        case MMID::DAGGER_STAB:
            attackPlayerHelper(bc, 9);
            bc.addToBot( Actions::MakeTempCardInDiscard(CardId::WOUND) );
            setMove(MMID::DAGGER_EXPLODE);
            bc.noOpRollMove();
            break;

        case MMID::DAGGER_EXPLODE:
            attackPlayerHelper(bc, 25);
            bc.addToBot( Actions::SuicideAction(idx, true) );
            bc.noOpRollMove();
            break;

        case MMID::TIME_EATER_HASTE:
            miscInfo = true; // set have used haste true
            curHp = maxHp / 2;
            if (asc19) {
                addBlock(32);
            }
            removeDebuffs();  // also removes shackled here
            rollMove(bc);
            break;

        case MMID::TIME_EATER_HEAD_SLAM:
            attackPlayerHelper(bc, asc4 ? 32 : 26);
            bc.addToBot( Actions::DebuffPlayer<PS::DRAW_REDUCTION>(1, true) );
            if (asc19) {
                bc.addToBot( Actions::MakeTempCardInDiscard(CardId::SLIMED, 2) );
            }
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::TIME_EATER_REVERBERATE:
            attackPlayerHelper(bc, asc4 ? 8 : 7, 3);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::TIME_EATER_RIPPLE: // 3
            addBlock(20);
            bc.player.debuff<PS::WEAK>(1, true);
            bc.player.debuff<PS::VULNERABLE>(1, true);
            if (asc19) {
                bc.player.debuff<PS::FRAIL>(1, true);
            }
            rollMove(bc);
            break;

        case MMID::DONU_BEAM:
            attackPlayerHelper(bc, asc4 ? 12 : 10, 2);
            setMove(MonsterMoveId::DONU_CIRCLE_OF_POWER);
            break;

        case MMID::DONU_CIRCLE_OF_POWER:
            bc.monsters.arr[0].buff<MS::STRENGTH>(3); // shouldn't matter if deca is dead
            buff<MS::STRENGTH>(3);
            setMove(MonsterMoveId::DONU_BEAM);
            break;

        case MMID::DECA_BEAM:
            attackPlayerHelper(bc, asc4 ? 12 : 10, 2);
            bc.addToBot( Actions::MakeTempCardInDiscard(CardId::DAZED, 2) );
            setMove(MonsterMoveId::DECA_SQUARE_OF_PROTECTION);
            break;

        case MMID::DECA_SQUARE_OF_PROTECTION: {
            auto &deca = *this;
            auto &donu = bc.monsters.arr[1];
            deca.addBlock(16);
            donu.addBlock(16);
            if (asc19) {
                deca.buff<MS::PLATED_ARMOR>(3);
                donu.buff<MS::PLATED_ARMOR>(3);
            }
            setMove(MonsterMoveId::DECA_BEAM);
            break;
        }

        case MMID::AWAKENED_ONE_DARK_ECHO:
            if (halfDead) {
                std::cerr << bc << std::endl;
                assert(false);
            }
            attackPlayerHelper(bc, 40);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::AWAKENED_ONE_REBIRTH: {
            maxHp = asc9 ? 320 : 300;
            curHp = maxHp;
            halfDead = false;
            miscInfo = true;
            strength = std::max(0,strength);
            ++bc.monsters.monstersAlive;
            buff<MS::MINION_LEADER>();
            setMove(MonsterMoveId::AWAKENED_ONE_DARK_ECHO);
            bc.noOpRollMove();
            break;
        }

        case MMID::AWAKENED_ONE_SLASH:
            if (halfDead) {
                std::cerr << bc << std::endl;
                assert(false);
            }
            attackPlayerHelper(bc, 20);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::AWAKENED_ONE_SLUDGE:
            if (halfDead) {
                std::cerr << bc << std::endl;
                assert(false);
            }
            attackPlayerHelper(bc, 18);
            bc.addToBot( Actions::ShuffleTempCardIntoDrawPile(CardId::VOID) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::AWAKENED_ONE_SOUL_STRIKE:
            if (halfDead) {
                std::cerr << bc << std::endl;
                assert(false);
            }
            attackPlayerHelper(bc, 6, 4);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::AWAKENED_ONE_TACKLE:
            if (halfDead) {
                std::cerr << bc << std::endl;
                assert(false);
            }
            attackPlayerHelper(bc, 10, 3);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::SPIRE_SHIELD_BASH: // 1
            attackPlayerHelper(bc, asc3 ? 14 : 12);
            if (bc.player.orbSlots > 0) {
                bc.addToBot( Actions::SpireShieldDebuff() );
            } else {
                bc.player.debuff<PS::STRENGTH>(-1);
            }
            if (lastMoveBefore(MonsterMoveId::SPIRE_SHIELD_SMASH) || lastMoveBefore(MonsterMoveId::INVALID)) {
                setMove(MonsterMoveId::SPIRE_SHIELD_FORTIFY);
            } else {
                setMove(MonsterMoveId::SPIRE_SHIELD_SMASH);
            }
            bc.noOpRollMove();
            break;


        case MMID::SPIRE_SHIELD_FORTIFY: // 2
            addBlock(30);
            bc.monsters.arr[1].addBlock(30);
            if (lastMoveBefore(MonsterMoveId::SPIRE_SHIELD_SMASH) || lastMoveBefore(MonsterMoveId::INVALID)) {
                setMove(MonsterMoveId::SPIRE_SHIELD_BASH);
            } else {
                setMove(MonsterMoveId::SPIRE_SHIELD_SMASH);
            }
            bc.noOpRollMove();
            break;


        case MMID::SPIRE_SHIELD_SMASH: { // 3
            const auto damageOutput = calculateDamageToPlayer(bc, asc3 ? 38 : 34);
            bc.addToBot( Actions::AttackPlayer(idx, damageOutput) );
            bc.addToBot( Actions::MonsterGainBlock(idx, asc18 ? 99 : damageOutput));
            bc.addToBot( Actions::RollMove(idx) );
            break;
        }

        case MMID::SPIRE_SPEAR_BURN_STRIKE: // 1
            attackPlayerHelper(bc, asc3 ? 6 : 5, 2);
            if (asc18) {
                bc.addToBot( Actions::MakeTempCardInDrawPile(CardId::BURN, 2, false) );
            } else {
                bc.addToBot( Actions::MakeTempCardInDiscard(CardId::BURN, 2) );
            }
            if (lastMoveBefore(MonsterMoveId::SPIRE_SPEAR_SKEWER)) {
                setMove(MonsterMoveId::SPIRE_SPEAR_PIERCER);
            } else {
                setMove(MonsterMoveId::SPIRE_SPEAR_SKEWER);
            }
            bc.noOpRollMove();
            break;

        case MMID::SPIRE_SPEAR_PIERCER: // 2
            buff<MS::STRENGTH>(2);
            bc.monsters.arr[0].buff<MS::STRENGTH>(2);
            if (lastMoveBefore(MonsterMoveId::SPIRE_SPEAR_SKEWER)) {
                setMove(MonsterMoveId::SPIRE_SPEAR_BURN_STRIKE);
            } else {
                setMove(MonsterMoveId::SPIRE_SPEAR_SKEWER);
            }
            bc.noOpRollMove();
            break;

        case MMID::SPIRE_SPEAR_SKEWER: // 3
            attackPlayerHelper(bc, 10, asc3 ? 4 : 3);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::CORRUPT_HEART_BLOOD_SHOTS: // 1
            attackPlayerHelper(bc, 2, asc4 ? 15 : 12);
            if (bc.getMonsterTurnNumber() % 3 == 0) {
                setMove(MonsterMoveId::CORRUPT_HEART_BUFF);
            } else {
                setMove(MonsterMoveId::CORRUPT_HEART_ECHO);
            }
            bc.noOpRollMove();
            break;

        case MMID::CORRUPT_HEART_BUFF: { // 4
            // remove negative str and buff 2
            const auto newStr = std::max(0, getStatus<MS::STRENGTH>()) + 2;
            setStatus<MS::STRENGTH>(newStr);
            const auto buffCount = bc.getMonsterTurnNumber() / 3;
            switch (buffCount) {
                case 1:
                    buff<MS::ARTIFACT>(2);
                    break;
                case 2:
                    buff<MS::BEAT_OF_DEATH>(1);
                    break;
                case 3:
                    buff<MS::PAINFUL_STABS>();
                    break;
                case 4:
                    buff<MS::STRENGTH>(10);
                    break;
                default:
                    buff<MS::STRENGTH>(50);
                    break;
            }
            rollMove(bc);
            break;
        }

        case MMID::CORRUPT_HEART_DEBILITATE: // 3
            bc.player.debuff<PS::VULNERABLE>(2, true);
            bc.player.debuff<PS::WEAK>(2, true);
            bc.player.debuff<PS::FRAIL>(2, true);
            Actions::ShuffleTempCardIntoDrawPile(CardId::DAZED).actFunc(bc);
            Actions::ShuffleTempCardIntoDrawPile(CardId::SLIMED).actFunc(bc);
            Actions::ShuffleTempCardIntoDrawPile(CardId::WOUND).actFunc(bc);
            Actions::ShuffleTempCardIntoDrawPile(CardId::BURN).actFunc(bc);
            Actions::ShuffleTempCardIntoDrawPile(CardId::VOID).actFunc(bc);
            rollMove(bc);
            break;

        case MMID::CORRUPT_HEART_ECHO: // 2
            attackPlayerHelper(bc, asc4 ? 45 : 40);
            if (bc.getMonsterTurnNumber() % 3 == 0) {
                setMove(MonsterMoveId::CORRUPT_HEART_BUFF);
            } else {
                setMove(MonsterMoveId::CORRUPT_HEART_BLOOD_SHOTS);
            }
            bc.noOpRollMove();
            break;


        case MMID::INVALID:
#ifdef sts_asserts
        {
            std::cerr << bc.seed << " ";
            auto x = static_cast<int>(moveHistory[0]);
            if (x >= 0 && x <= static_cast<int>(MMID::WRITHING_MASS_STRONG_STRIKE)) {
                std::cerr << monsterIdEnumNames[static_cast<int>(id)] << " " << monsterMoveStrings[x] << std::endl;
            }
            assert(false);
        }
#endif
        case MMID::GENERIC_ESCAPE_MOVE:
        default:
            break;
    }
}

MMID Monster::getMoveForRoll(BattleContext &bc, int &monsterData, const int roll) const {
    const bool asc17 = bc.ascension >= 17;
    const bool asc18 = bc.ascension >= 18;
    const bool asc19 = bc.ascension >= 19;

    switch (id) {

        case MonsterId::ACID_SLIME_S: {
            if (asc17) {
                return (MMID::ACID_SLIME_S_LICK);

            } else if (bc.aiRng.randomBoolean()) {
                return (MMID::ACID_SLIME_S_TACKLE);

            } else {
                return (MMID::ACID_SLIME_S_LICK);

            }
            break;
        }

        case MonsterId::ACID_SLIME_M: {
            // corrosive spit = 1
            // tackle = 2
            // lick = 4
            if (asc17) {
                // START ASCENSION 17
                if (roll < 40) {
                    if (lastTwoMoves(MMID::ACID_SLIME_M_CORROSIVE_SPIT)) {
                        if (bc.aiRng.randomBoolean()) {
                            return (MMID::ACID_SLIME_M_TACKLE);
                        } else {
                            return (MMID::ACID_SLIME_M_LICK);
                        }
                    } else {
                        return (MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                    }

                } else if (roll < 80) {
                    if (lastTwoMoves(MMID::ACID_SLIME_M_TACKLE)) {
                        if (bc.aiRng.randomBoolean(0.5f)) {
                            return (MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                        } else {
                            return (MMID::ACID_SLIME_M_LICK);
                        }
                    } else {
                        return (MMID::ACID_SLIME_M_TACKLE);
                    }

                } else if (lastMove(MMID::ACID_SLIME_M_LICK)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        return (MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                    } else {
                        return (MMID::ACID_SLIME_M_TACKLE);
                    }
                } else {
                    return (MMID::ACID_SLIME_M_LICK);
                }
                // END ASCENSION 17

            } else if (roll < 30) {
                if (lastTwoMoves(MMID::ACID_SLIME_M_CORROSIVE_SPIT)) {
                    if (bc.aiRng.randomBoolean()) {
                        return (MMID::ACID_SLIME_M_TACKLE);
                    } else {
                        return (MMID::ACID_SLIME_M_LICK);
                    }
                } else {
                    return (MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                }

            } else if (roll < 70) {
                if (lastMove(MMID::ACID_SLIME_M_TACKLE)) {
                    if (bc.aiRng.randomBoolean(0.4f)) {
                        return (MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                    } else {
                        return (MMID::ACID_SLIME_M_LICK);
                    }
                } else {
                    return (MMID::ACID_SLIME_M_TACKLE);
                }

            } else if (lastTwoMoves(MMID::ACID_SLIME_M_LICK)) {
                if (bc.aiRng.randomBoolean(0.4F)) {
                    return (MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                } else {
                    return (MMID::ACID_SLIME_M_TACKLE);
                }
            } else {
                return (MMID::ACID_SLIME_M_LICK);
            }
            break;
        }

        case MonsterId::ACID_SLIME_L: {
            // 1 corrosive spit
            // 2 tackle
            // 3 split
            // 4 lick

            if (asc17) {
                if (roll < 40) {
                    if (lastTwoMoves(MMID::ACID_SLIME_M_CORROSIVE_SPIT)) {
                        if (bc.aiRng.randomBoolean(0.6F)) {
                            return (MMID::ACID_SLIME_L_TACKLE);
                        } else {
                            return (MMID::ACID_SLIME_L_LICK);
                        }
                    } else {
                        return (MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                    }

                } else if (roll < 70) {
                    if (lastTwoMoves(MMID::ACID_SLIME_L_TACKLE)) {
                        if (bc.aiRng.randomBoolean(0.6F)) {
                            return (MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                        } else {
                            return (MMID::ACID_SLIME_L_LICK);
                        }
                    } else {
                        return (MMID::ACID_SLIME_L_TACKLE);
                    }
                } else if (lastMove(MMID::ACID_SLIME_L_LICK)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        return (MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                    } else {
                        return (MMID::ACID_SLIME_L_TACKLE);
                    }
                } else {
                    return (MMID::ACID_SLIME_L_LICK);
                }

            } else if (roll < 30) {
                if (lastTwoMoves(MMID::ACID_SLIME_L_CORROSIVE_SPIT)) {
                    if (bc.aiRng.randomBoolean()) {
                        return (MMID::ACID_SLIME_L_TACKLE);
                    } else {
                        return (MMID::ACID_SLIME_L_LICK);
                    }
                } else {
                    return (MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                }
            } else if (roll < 70) {
                if (lastMove(MMID::ACID_SLIME_L_TACKLE)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        return (MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                    } else {
                        return (MMID::ACID_SLIME_L_LICK);
                    }
                } else {
                    return (MMID::ACID_SLIME_L_TACKLE);
                }
            } else if (lastTwoMoves(MMID::ACID_SLIME_L_LICK)) {
                if (bc.aiRng.randomBoolean(0.4F)) {
                    return (MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                } else {
                    return (MMID::ACID_SLIME_L_TACKLE);
                }
            } else {
                return (MMID::ACID_SLIME_L_LICK);
            }
            break;
        }

        case MonsterId::BLUE_SLAVER: {
            if (roll >= 40 && !lastTwoMoves(MMID::BLUE_SLAVER_STAB)) {
                return (MMID::BLUE_SLAVER_STAB);

            } else if ( !lastTwoMoves(MMID::BLUE_SLAVER_RAKE) ||
                (asc17 && !lastMove(MMID::BLUE_SLAVER_RAKE)) ) {
                return (MMID::BLUE_SLAVER_RAKE);

            } else {
                return (MMID::BLUE_SLAVER_STAB);
            }
            break;
        }

        case MonsterId::BRONZE_AUTOMATON: {
            return (MMID::BRONZE_AUTOMATON_SPAWN_ORBS);
            break;
        }

        case MonsterId::BRONZE_ORB: { // todo bug with discarded cards - blind card not showing in discard pile
            // 1 beam
            // 2 support beam
            // 3 stasis
            const auto haveUsedStasis = miscInfo;
            if (!haveUsedStasis && roll >= 25) {
                return (MMID::BRONZE_ORB_STASIS);

            } else if (roll >= 70 && !lastTwoMoves(MMID::BRONZE_ORB_SUPPORT_BEAM)) {
                return (MMID::BRONZE_ORB_SUPPORT_BEAM);

            } else if (!lastTwoMoves(MMID::BRONZE_ORB_BEAM)) {
                return (MMID::BRONZE_ORB_BEAM);

            } else {
                return (MMID::BRONZE_ORB_SUPPORT_BEAM);
            }
            break;
        }

        case MonsterId::BYRD: {
            // 1 peck
            // 2 fly
            // 3 swoop
            // 4 stunned
            // 5 headbutt
            // 6 caw

            // handled during turn
//            if (!hasStatusInternal<MS::FLIGHT>()) {
//                return (MMID::BYRD_HEADBUTT);
//                break;
//            }

            if (firstTurn()) {
                if (bc.aiRng.randomBoolean(0.375f)) {
                    return (MMID::BYRD_CAW);
                } else {
                    return (MMID::BYRD_PECK);
                }
                break;
            }

            if (roll < 50) {
                if (lastTwoMoves(MMID::BYRD_PECK)) {
                    if (bc.aiRng.randomBoolean(0.4f)) {
                        return (MMID::BYRD_SWOOP);
                    } else {
                        return (MMID::BYRD_CAW);
                    }
                } else {
                    return (MMID::BYRD_PECK);
                }
                break;
            }

            if (roll < 70) {
                if (lastMove(MMID::BYRD_SWOOP)) {
                    if (bc.aiRng.randomBoolean(0.375f)) {
                        return (MMID::BYRD_CAW);
                    } else {
                        return (MMID::BYRD_PECK);
                    }
                } else {
                    return (MMID::BYRD_SWOOP);
                }
                break;
            }

            if (lastMove(MMID::BYRD_CAW)) {
                if (bc.aiRng.randomBoolean(0.2857f)) {
                    return (MMID::BYRD_SWOOP);
                } else {
                    return (MMID::BYRD_PECK);
                }
            } else {
                return (MMID::BYRD_CAW);
            }
            break;
        }

        case MonsterId::CENTURION: {
            // 1 Slash
            // 2 Defend
            // 3 Fury

            const auto mysticAlive = bc.monsters.getAliveCount() > 1;

            if (roll >= 65
                && !lastTwoMoves(MMID::CENTURION_DEFEND)
                && !lastTwoMoves(MMID::CENTURION_FURY))
            {
                if (mysticAlive) {
                    return (MMID::CENTURION_DEFEND);
                } else {
                    return (MMID::CENTURION_FURY);
                }
                break;
            }

            if (!lastTwoMoves(MMID::CENTURION_SLASH)) {
                return (MMID::CENTURION_SLASH);
                break;
            }

            if (mysticAlive) {
                return (MMID::CENTURION_DEFEND);
            } else {
                return (MMID::CENTURION_FURY);
            }
            break;
        }

        case MonsterId::MYSTIC: {
            // 1 attack debuff
            // 2 heal
            // 3 buff

            const auto healNeedAmt = asc17 ? 21 : 16;
            const auto &knight = bc.monsters.arr[0];

            if (maxHp-curHp >= healNeedAmt || knight.isAlive() && knight.maxHp-knight.curHp >= healNeedAmt) {
                return (MMID::MYSTIC_HEAL);
                break;
            }

            if (roll >= 40 && (asc17 ? !lastMove(MMID::MYSTIC_ATTACK_DEBUFF) : !lastTwoMoves(MMID::MYSTIC_ATTACK_DEBUFF))) {
                return (MMID::MYSTIC_ATTACK_DEBUFF);
                break;
            }

            if (!lastTwoMoves(MMID::MYSTIC_BUFF)) {
                return (MMID::MYSTIC_BUFF);

            } else {
                return (MMID::MYSTIC_ATTACK_DEBUFF);
            }
            break;
        }

        case MonsterId::CHOSEN: {
            // 1 zap
            // 2 drain
            // 3 debilitate
            // 4 hex
            // 5 poke

            if (asc17) {
                if (firstTurn()) {
                    return (MMID::CHOSEN_HEX);

                } else if (!lastMove(MMID::CHOSEN_DEBILITATE) && !lastMove(MMID::CHOSEN_DRAIN)) {
                    if (roll < 50) {
                        return (MMID::CHOSEN_DEBILITATE);
                    } else {
                        return (MMID::CHOSEN_DRAIN);
                    }

                } else if (roll < 40) {
                    return (MMID::CHOSEN_ZAP);

                } else {
                    return (MMID::CHOSEN_POKE);

                }
                break;
            }

            // Ascension < 17
            if (firstTurn()) {
                return (MMID::CHOSEN_POKE);

            } else if (lastMoveBefore(MMID::INVALID)) {
                return (MMID::CHOSEN_HEX);

            } else if (!lastMove(MMID::CHOSEN_DEBILITATE) && !lastMove(MMID::CHOSEN_DRAIN)) {
                if (roll < 50) {
                    return (MMID::CHOSEN_DEBILITATE);
                } else {
                    return (MMID::CHOSEN_DRAIN);
                }
            } else if (roll < 40) {
                return (MMID::CHOSEN_ZAP);
            } else {
                return (MMID::CHOSEN_POKE);
            }
            break;
        }

        case MonsterId::BOOK_OF_STABBING: {
            // 1 multi stab
            // 2 single stab
            auto &stabCount = monsterData;
            if (roll < 15) {
                if (lastMove(MMID::BOOK_OF_STABBING_SINGLE_STAB)) {
                    ++stabCount;
                    return (MMID::BOOK_OF_STABBING_MULTI_STAB);
                } else {
                    return (MMID::BOOK_OF_STABBING_SINGLE_STAB);
                    if (asc18) {
                        ++stabCount;
                    }
                }
            } else if (lastTwoMoves(MMID::BOOK_OF_STABBING_MULTI_STAB)) {
                return (MMID::BOOK_OF_STABBING_SINGLE_STAB);
                if (asc18) {
                    ++stabCount;
                }
            } else {
                ++stabCount;
                return (MMID::BOOK_OF_STABBING_MULTI_STAB);
            }
            break;
        }

        case MonsterId::CULTIST: {
            if (lastMove(MMID::INVALID)) {
                return (MMID::CULTIST_INCANTATION);
            } else {
                return (MMID::CULTIST_DARK_STRIKE);
            }
            break;
        }

        case MonsterId::FAT_GREMLIN: {
            return (MMID::FAT_GREMLIN_SMASH);
            break;
        }

        case MonsterId::FUNGI_BEAST: {
            // 1 FUNGI_BEAST_BITE
            // 2 FUNGI_BEAST_GROW
            if (roll < 60) {
                if (lastTwoMoves(MMID::FUNGI_BEAST_BITE)) {
                    return (MMID::FUNGI_BEAST_GROW);
                } else {
                    return (MMID::FUNGI_BEAST_BITE);
                }

            } else if (lastMove(MMID::FUNGI_BEAST_GROW)) {
                return (MMID::FUNGI_BEAST_BITE);

            } else {
                return (MMID::FUNGI_BEAST_GROW);
            }
            break;
        }

        case MonsterId::GREEN_LOUSE: {
            if (roll < 25) {
                if (lastMove(MMID::GREEN_LOUSE_SPIT_WEB) && (asc17 || lastTwoMoves(MMID::GREEN_LOUSE_SPIT_WEB))) {
                    return (MMID::GREEN_LOUSE_BITE);
                } else  {
                    return (MMID::GREEN_LOUSE_SPIT_WEB);
                }

            } else if (lastTwoMoves(MMID::GREEN_LOUSE_BITE)) {
                return (MMID::GREEN_LOUSE_SPIT_WEB);

            } else {
                return (MMID::GREEN_LOUSE_BITE);
            }
            break;
        }

        case MonsterId::GREMLIN_LEADER: {
            // 2 RALLY
            // 3 ENCOURAGE
            // 4 STAB
            const auto numAliveGremlins = Monster::getAliveGremlinCount(bc);
            if (numAliveGremlins == 0) {

                if (roll < 75) {
                    if (lastMove(MMID::GREMLIN_LEADER_RALLY)) {
                        return (MMID::GREMLIN_LEADER_STAB);
                    } else {
                        return (MMID::GREMLIN_LEADER_RALLY);
                    }
                } else if (lastMove(MMID::GREMLIN_LEADER_STAB)) {
                    return (MMID::GREMLIN_LEADER_RALLY);
                } else {
                    return (MMID::GREMLIN_LEADER_STAB);
                }

            } else if (numAliveGremlins == 1) {

                if (roll < 50) {
                    if (lastMove(MMID::GREMLIN_LEADER_RALLY)) {
                        const auto roll2 = bc.aiRng.random(50, 99);
                        if (roll2 < 80) {
                            return (MMID::GREMLIN_LEADER_ENCOURAGE);
                        } else {
                            return (MMID::GREMLIN_LEADER_STAB);
                        }
                    } else {
                        return (MMID::GREMLIN_LEADER_RALLY);
                    }

                } else if (roll < 80) {
                    if (lastMove(MMID::GREMLIN_LEADER_ENCOURAGE)) {
                        return (MMID::GREMLIN_LEADER_STAB);
                    } else {
                        return (MMID::GREMLIN_LEADER_ENCOURAGE);
                    }

                } else if (lastMove(MMID::GREMLIN_LEADER_STAB)) {
                    const auto roll2 = bc.aiRng.random(0, 80);
                    if (roll2 < 50) {
                        return (MMID::GREMLIN_LEADER_RALLY);
                    } else {
                        return (MMID::GREMLIN_LEADER_ENCOURAGE);
                    }

                } else {
                    return (MMID::GREMLIN_LEADER_STAB);
                }

            } else { // alive gremlins > 1

                if (roll < 66) {
                    if (lastMove(MMID::GREMLIN_LEADER_ENCOURAGE)) {
                        return (MMID::GREMLIN_LEADER_STAB);
                    } else {
                        return (MMID::GREMLIN_LEADER_ENCOURAGE);
                    }

                } else if (lastMove(MMID::GREMLIN_LEADER_STAB)) {
                    return (MMID::GREMLIN_LEADER_ENCOURAGE);

                } else {
                    return (MMID::GREMLIN_LEADER_STAB);
                }

            }
            break;
        }

        case MonsterId::GREMLIN_NOB: {
            // 1 Rush
            // 2 Skull Bash
            // 3 BELLOW

            if (lastMove(MMID::INVALID)) {
                return (MMID::GREMLIN_NOB_BELLOW);
                break;
            }

            if (asc18) {
                if (!lastTwoMoves(MMID::GREMLIN_NOB_SKULL_BASH)) {
                    return (MMID::GREMLIN_NOB_RUSH);
                    break;
                }

                if (lastTwoMoves(MMID(MMID::GREMLIN_NOB_RUSH))) {
                    return (MMID::GREMLIN_NOB_SKULL_BASH);

                } else {
                    return (MMID::GREMLIN_NOB_RUSH);
                }
                break;
            }

            if (roll < 33 || lastTwoMoves(MMID::GREMLIN_NOB_RUSH)) {
                return (MMID::GREMLIN_NOB_SKULL_BASH);

            } else {
                return (MMID::GREMLIN_NOB_RUSH);
            }
            break;
        }

        case MonsterId::GREMLIN_WIZARD: {
            monsterData = 1; // gremlin wizard charge
            return (MMID::GREMLIN_WIZARD_CHARGING);
            break;
        }

        case MonsterId::HEXAGHOST: {
#ifdef sts_asserts
            assert(firstTurn());
#endif
            return (MMID::HEXAGHOST_ACTIVATE);
            break;
        }

        case MonsterId::JAW_WORM: {
            if (firstTurn()) {
                return (MMID::JAW_WORM_CHOMP);
                break;
            }

            if (roll < 25) {
                if (lastMove(MMID::JAW_WORM_CHOMP)) {
                    if (bc.aiRng.randomBoolean(0.5625f)) {
                        return (MMID::JAW_WORM_BELLOW);
                    } else {
                        return (MMID::JAW_WORM_THRASH);
                    }

                } else {
                    return (MMID::JAW_WORM_CHOMP);
                }

            } else if (roll < 55) {
                if (lastTwoMoves(MMID::JAW_WORM_THRASH)) {
                    if (bc.aiRng.randomBoolean(0.357f)) {
                        return (MMID::JAW_WORM_CHOMP);
                    } else {
                        return (MMID::JAW_WORM_BELLOW);
                    }

                } else {
                    return (MMID::JAW_WORM_THRASH);
                }

            } else if (lastMove(MMID::JAW_WORM_BELLOW)) {
                if (bc.aiRng.randomBoolean(0.416f)) {
                    return (MMID::JAW_WORM_CHOMP);
                } else {
                    return (MMID::JAW_WORM_THRASH);
                }

            } else {
                return (MMID::JAW_WORM_BELLOW);
            }
            break;
        }

        case MonsterId::LAGAVULIN: // called first turn only
            if (hasStatus<MS::ASLEEP>()) {
                return (MMID::LAGAVULIN_SLEEP);
            } else {
                return (MMID::LAGAVULIN_SIPHON_SOUL);
            }
            break;

        case MonsterId::LOOTER: // called first turn only
            return (MMID::LOOTER_MUG);
            break;

        case MonsterId::MAD_GREMLIN:
            return (MMID::MAD_GREMLIN_SCRATCH);
            break;

        case MonsterId::MUGGER: // called first turn only
            return (MMID::MUGGER_MUG);
            break;

        case MonsterId::NEMESIS: {
            // 2 attack
            // 3 scythe
            // 4 debuff

            if (firstTurn()) {
                if (roll < 50) {
                    return MMID::NEMESIS_ATTACK;
                } else {
                    return MMID::NEMESIS_DEBUFF;
                }
            }

            if (roll < 30) {
                if (!eitherLastTwo(MMID::NEMESIS_SCYTHE)) {
                    return MMID::NEMESIS_SCYTHE;

                } else if (bc.aiRng.randomBoolean()) {
                    if (!lastTwoMoves(MMID::NEMESIS_ATTACK)) {
                        return MMID::NEMESIS_ATTACK;
                    } else {
                        return MMID::NEMESIS_DEBUFF;
                    }

                } else if (!lastMove(MMID::NEMESIS_DEBUFF)) {
                    return MMID::NEMESIS_DEBUFF;

                } else {
                    return MMID::NEMESIS_ATTACK;
                }
            }

            if (roll < 65) {
                if (!lastTwoMoves(MMID::NEMESIS_ATTACK)) {
                    return MMID::NEMESIS_ATTACK;

                } else if (!bc.aiRng.randomBoolean() || eitherLastTwo(MMID::NEMESIS_SCYTHE)) {
                    return MMID::NEMESIS_DEBUFF;

                } else {
                    return MMID::NEMESIS_SCYTHE;
                }
            }

            if (!lastMove(MMID::NEMESIS_DEBUFF)) {
                return MMID::NEMESIS_DEBUFF;
            }

            if (bc.aiRng.randomBoolean() && !eitherLastTwo(MMID::NEMESIS_SCYTHE)) {
                return MMID::NEMESIS_SCYTHE;
            }

            return MMID::NEMESIS_ATTACK;
        }

        case MonsterId::ORB_WALKER: {
            if (roll < 40) {
                if (!lastTwoMoves(MMID::ORB_WALKER_CLAW)) {
                    return (MMID::ORB_WALKER_CLAW);
                } else {
                    return (MMID::ORB_WALKER_LASER);
                }
            } else if (!lastTwoMoves(MMID::ORB_WALKER_LASER)) {
                return (MMID::ORB_WALKER_LASER);
            } else {
                return (MMID::ORB_WALKER_CLAW);
            }
            break;
        }

        case MonsterId::RED_LOUSE: {
            if (roll < 25) {
                if (lastMove(MMID::RED_LOUSE_GROW) && (asc17 || lastTwoMoves(MMID::RED_LOUSE_GROW))) {
                    return (MMID::RED_LOUSE_BITE);
                } else  {
                    return (MMID::RED_LOUSE_GROW);
                }

            } else if (lastTwoMoves(MMID::RED_LOUSE_BITE)) {
                return (MMID::RED_LOUSE_GROW);

            } else {
                return (MMID::RED_LOUSE_BITE);
            }
            break;
        }

        case MonsterId::REPTOMANCER: {
            // 1 snake strike
            // 2 summon
            // 3 big bite
            if (firstTurn()) {
                return MMID::REPTOMANCER_SUMMON;
            }

            int myRoll = roll;
            const bool canSpawn = bc.monsters.monstersAlive < 4;

            while (true) {
                if (myRoll < 33) {
                    if (!lastMove(MMID::REPTOMANCER_SNAKE_STRIKE)) {
                        return MMID::REPTOMANCER_SNAKE_STRIKE;
                    } else {
                        myRoll = bc.aiRng.random(33, 99);
                    }
                }

                if (myRoll < 66) {
                    if (!lastTwoMoves(MMID::REPTOMANCER_SUMMON) && canSpawn) {
                        return MMID::REPTOMANCER_SUMMON;
                    } else {
                        return MMID::REPTOMANCER_SNAKE_STRIKE;
                    }
                }

                if (!lastMove(MMID::REPTOMANCER_BIG_BITE)) {
                    return MMID::REPTOMANCER_BIG_BITE;
                }

                // getMove(0-65)
                // could probably unroll this but would be big
                myRoll = bc.aiRng.random(0, 65);
            }
        }

        case MonsterId::DAGGER: {
            return MMID::DAGGER_STAB;
        }

        case MonsterId::SENTRY: {
            if (firstTurn()) {
                if (idx % 2 == 0) {
                    return (MMID::SENTRY_BOLT);
                } else {
                    return (MMID::SENTRY_BEAM);
                }
            }
            break;
        }

        case MonsterId::SHELLED_PARASITE: {
            // 1 fell
            // 2 double strike
            // 3 suck
            // 4 stunned
            if (firstTurn()) {
                if (asc17) {
                    return (MMID::SHELLED_PARASITE_FELL);
                } else {
                    if (bc.aiRng.randomBoolean()) {
                        return (MMID::SHELLED_PARASITE_DOUBLE_STRIKE);
                    } else {
                        return (MMID::SHELLED_PARASITE_SUCK);
                    }
                }
                break;
            }

            int roll2 = 100;

            if (roll < 20) {
                if (!lastMove(MMID::SHELLED_PARASITE_FELL)) {
                    return (MMID::SHELLED_PARASITE_FELL);
                    break;
                }
                roll2 = bc.aiRng.random(20,99);
            }

            if (roll < 60 || roll2 < 60) {
                if (!lastTwoMoves(MMID::SHELLED_PARASITE_DOUBLE_STRIKE)) {
                    return (MMID::SHELLED_PARASITE_DOUBLE_STRIKE);
                } else {
                    return (MMID::SHELLED_PARASITE_SUCK);
                }

            } else if (!lastTwoMoves(MMID::SHELLED_PARASITE_SUCK)) {
                return (MMID::SHELLED_PARASITE_SUCK);

            } else {
                return (MMID::SHELLED_PARASITE_DOUBLE_STRIKE);
            }
            break;
        }

        case MonsterId::SHIELD_GREMLIN: {
            return (MMID::SHIELD_GREMLIN_PROTECT);
            break;
        }

        case MonsterId::SLIME_BOSS: {
            // 1 SLIME_BOSS_PREPARING
            // 2 SLIME_BOSS_SLAM
            // 3 SLIME_BOSS_SPLIT
            // 4 SLIME_BOSS_GOOP_SPRAY
            if (firstTurn()) {
                return (MMID::SLIME_BOSS_GOOP_SPRAY);
            }
            break;
        }

        case MonsterId::SNAKE_PLANT: {
            // 1 chomp
            // 2 enfeebling spores
            if (asc17) {
                if (roll < 65) {
                    if (lastTwoMoves(MMID::SNAKE_PLANT_CHOMP)) {
                        return (MMID::SNAKE_PLANT_ENFEEBLING_SPORES);
                    } else {
                        return (MMID::SNAKE_PLANT_CHOMP);
                    }
                } else if (!lastTwoMoves(MMID::SNAKE_PLANT_ENFEEBLING_SPORES)) {
                    return (MMID::SNAKE_PLANT_ENFEEBLING_SPORES);
                } else {
                    return (MMID::SNAKE_PLANT_CHOMP);
                }
                break;
            }

            if (roll < 65) {
                if (lastTwoMoves(MMID::SNAKE_PLANT_CHOMP)) {
                    return (MMID::SNAKE_PLANT_ENFEEBLING_SPORES);
                } else {
                    return (MMID::SNAKE_PLANT_CHOMP);
                }
            } else if (lastMove(MMID::SNAKE_PLANT_ENFEEBLING_SPORES)) {
                return (MMID::SNAKE_PLANT_CHOMP);
            } else {
                return (MMID::SNAKE_PLANT_ENFEEBLING_SPORES);
            }
            break;
        }

        case MonsterId::SNEAKY_GREMLIN:
            return (MMID::SNEAKY_GREMLIN_PUNCTURE);
            break;

        case MonsterId::SNECKO: {
            // 1 perplexing glare
            // 2 bite
            // 3 tail whip
            if (firstTurn()) {
                return (MMID::SNECKO_PERPLEXING_GLARE);

            } else if (roll < 40 || lastTwoMoves(MMID::SNECKO_BITE)) {
                return (MMID::SNECKO_TAIL_WHIP);

            } else {
                return (MMID::SNECKO_BITE);
            }
            break;
        }

        case MonsterId::SPHERIC_GUARDIAN: // called first turn only
            return (MMID::SPHERIC_GUARDIAN_ACTIVATE);
            break;

        case MonsterId::SPIKE_SLIME_S:
            return (MMID::SPIKE_SLIME_S_TACKLE);
            break;

        case MonsterId::RED_SLAVER: {
            // 1 Stab
            // 2 Entangle
            // 3 Scrape
            const bool usedEntangle = miscInfo;

            if (lastMove(MMID::INVALID)) {
                return (MMID::RED_SLAVER_STAB);

            } else if (roll >= 75 && !usedEntangle) {
                return (MMID::RED_SLAVER_ENTANGLE);

            } else if (roll >= 50 && usedEntangle && !lastTwoMoves(MMID::RED_SLAVER_STAB)) {
                return (MMID::RED_SLAVER_STAB);

            } else if (!lastTwoMoves(MMID::RED_SLAVER_SCRAPE) || (asc17 && !lastMove(MMID::RED_SLAVER_SCRAPE))) {
                return (MMID::RED_SLAVER_SCRAPE);

            } else {
                return (MMID::RED_SLAVER_STAB);

            }
            break;
        }


        case MonsterId::SPIKE_SLIME_L: {
            // 1 flame tackle
            // 3 split
            // 4 lick
           if (roll < 30) {
                if (lastTwoMoves(MMID::SPIKE_SLIME_L_FLAME_TACKLE)) {
                    return (MMID::SPIKE_SLIME_L_LICK);
                } else {
                    return (MMID::SPIKE_SLIME_L_FLAME_TACKLE);
                }

            } else if (lastTwoMoves(MMID::SPIKE_SLIME_L_LICK) || (asc17 && lastMove(MMID::SPIKE_SLIME_L_LICK)) ) {
                return (MMID::SPIKE_SLIME_L_FLAME_TACKLE);

            } else {
                return (MMID::SPIKE_SLIME_L_LICK);

            }
            break;
        }

        case MonsterId::SPIKE_SLIME_M: {
            // 1 SPIKE_SLIME_M_FLAME_TACKLE
            // 4 SPIKE_SLIME_M_LICK
            if (roll < 30) {
                if (lastTwoMoves(MMID::SPIKE_SLIME_M_FLAME_TACKLE)) {
                    return (MMID::SPIKE_SLIME_M_LICK);
                } else {
                    return (MMID::SPIKE_SLIME_M_FLAME_TACKLE);
                }
            } else if (lastTwoMoves(MMID::SPIKE_SLIME_M_LICK) || (asc17 && lastMove(MMID::SPIKE_SLIME_M_LICK))) {
                return (MMID::SPIKE_SLIME_M_FLAME_TACKLE);
            } else {
                return (MMID::SPIKE_SLIME_M_LICK);
            }
            break;
        }

        case MonsterId::TASKMASTER: {
            return (MMID::TASKMASTER_SCOURING_WHIP);
            break;
        }

        case MonsterId::THE_CHAMP: {
            // 1 Heavy Slash
            // 2 Defensive Stance
            // 3 Execute
            // 4 Face Slap
            // 5 Gloat
            // 6 Taunt
            // 7 Anger

            // is in phase2
            if (monsterData & 0x4) {
                if (!lastMove(MMID::THE_CHAMP_EXECUTE) && !lastMoveBefore(MMID::THE_CHAMP_EXECUTE)) {
                    return (MMID::THE_CHAMP_EXECUTE);
                    break;
                }

            } else {
                if (curHp < maxHp / 2) {
                    monsterData |= 0x4;
                    return (MMID::THE_CHAMP_ANGER);
                    break;

                } else if ((bc.getMonsterTurnNumber()+1) % 4 == 0) {
                    return (MMID::THE_CHAMP_TAUNT);
                    break;
                }
            }


            // check if should use defensive stance
            const auto defensiveStanceUseCount = miscInfo & 0x3;
            const auto rollThreshold = asc19 ? 30 : 15;
            if (roll <= rollThreshold
                && !lastMove(MMID::THE_CHAMP_DEFENSIVE_STANCE)
                && defensiveStanceUseCount < 2)
            {
                ++monsterData;
                return (MMID::THE_CHAMP_DEFENSIVE_STANCE);
                break;
            }

            if (roll <= 30 && !lastMove(MMID::THE_CHAMP_GLOAT) && !lastMove(MMID::THE_CHAMP_DEFENSIVE_STANCE)) {
                return (MMID::THE_CHAMP_GLOAT);

            } else if (roll <= 55 && !lastMove(MMID::THE_CHAMP_FACE_SLAP)) {
                return (MMID::THE_CHAMP_FACE_SLAP);

            } else if (!lastMove(MMID::THE_CHAMP_HEAVY_SLASH)) {
                return (MMID::THE_CHAMP_HEAVY_SLASH);

            } else {
                return (MMID::THE_CHAMP_FACE_SLAP);
            }
            break;
        }

        case MonsterId::THE_COLLECTOR: {
            // 1 initial spawn
            // 2 fireball
            // 3 buff
            // 4 mega debuff
            // 5 spawn

            // first turn always spawn
            if (firstTurn()) {
                return (MMID::THE_COLLECTOR_SPAWN);
                break;
            }

            // always uses mega debuff turn 4
            if (bc.getMonsterTurnNumber() == 3) {
                return (MMID::THE_COLLECTOR_MEGA_DEBUFF);
                break;
            }

            const auto canUseSpawn = bc.monsters.monstersAlive < 3
                    && !lastMove(MMID::THE_COLLECTOR_SPAWN);

            if (roll <= 25 && canUseSpawn) {
                return (MMID::THE_COLLECTOR_SPAWN);
                break;
            }

            if (roll <= 70 && !lastTwoMoves(MMID::THE_COLLECTOR_FIREBALL)) {
                return (MMID::THE_COLLECTOR_FIREBALL);
                break;
            }

            if (lastMove(MMID::THE_COLLECTOR_BUFF)) {
                return (MMID::THE_COLLECTOR_FIREBALL);

            } else {
                return (MMID::THE_COLLECTOR_BUFF);
            }
            break;
        }

        case MonsterId::THE_GUARDIAN: {
            return (MMID::THE_GUARDIAN_CHARGING_UP);
            break;
        }

        // RED MASK BOIS

        case MonsterId::BEAR: {
            return (MMID::BEAR_BEAR_HUG);
            break;
        }

        case MonsterId::ROMEO: {
            return (MMID::ROMEO_MOCK);
            break;
        }

        case MonsterId::POINTY: {
            return (MMID::POINTY_ATTACK);
            break;
        }

        // SHAPES
        case MonsterId::EXPLODER: {
            // first turn only
            return (MMID::EXPLODER_SLAM);
        }

        case MonsterId::REPULSOR: {
            if (roll < 20 && !lastMove(MMID::REPULSOR_BASH)) {
                return (MMID::REPULSOR_BASH);
            } else {
                return (MMID::REPULSOR_REPULSE);
            }
            break;
        }

        case MonsterId::SPIKER: {
            // times used thorns > 5
            if (miscInfo > 5 || roll < 50 && !lastMove(MMID::SPIKER_CUT)) {
                return (MMID::SPIKER_CUT);
            } else {
                return (MMID::SPIKER_SPIKE);
            }
        }

        case MonsterId::THE_MAW: {
            if (firstTurn()) {
                return (MMID::THE_MAW_ROAR);

            } else if (roll < 50 && !lastMove(MMID::THE_MAW_NOM)) {
                return (MMID::THE_MAW_NOM);

            } else if (!lastMove(MMID::THE_MAW_SLAM)) {
                // dont include not last move nom condition, because it can't be, we handle in the move logic
                return (MMID::THE_MAW_SLAM);

            } else {
                return (MMID::THE_MAW_DROOL);
            }
            break;
        }

        case MonsterId::DARKLING: {
            // 1 chomp
            // 2 harden
            // 3 nip
            // 4 regrow
            // 5 reincarnate
            auto myRoll = roll;

            if (firstTurn()) {
                if (myRoll < 50) {
                    return MMID::DARKLING_HARDEN;
                } else {
                    return MMID::DARKLING_NIP;
                }
            }

            if (halfDead) {
                return MMID::DARKLING_REINCARNATE;
            }

            if (myRoll < 40) {
                if (!lastMove(MMID::DARKLING_CHOMP) && idx != 1) {
                    return MMID::DARKLING_CHOMP;
                } else {
                    myRoll = bc.aiRng.random(40, 99);
                }
            }

            if (myRoll < 70) {
                if (!lastMove(MMID::DARKLING_HARDEN)) {
                    return MMID::DARKLING_HARDEN;
                } else {
                    return MMID::DARKLING_NIP;
                }
            }

            if (!lastTwoMoves(MMID::DARKLING_NIP)) {
                return MMID::DARKLING_NIP;

            } else {
                // one of last two moves was darkling nip
                return getMoveForRoll(bc, monsterData, bc.aiRng.random(0, 99));
            }
        }

        case MonsterId::SPIRE_GROWTH: {
            const auto useConstrict = !bc.player.hasStatus<PS::CONSTRICTED>()
                                      && !lastMove(MMID::SPIRE_GROWTH_CONSTRICT)
                                      && (asc17 || roll >= 50);
            if (useConstrict) {
                return (MMID::SPIRE_GROWTH_CONSTRICT);

            } else if (roll < 50 && !lastTwoMoves(MMID::SPIRE_GROWTH_QUICK_TACKLE)) {
                return (MMID::SPIRE_GROWTH_QUICK_TACKLE);

            } else if (!lastTwoMoves(MMID::SPIRE_GROWTH_SMASH)) {
                return (MMID::SPIRE_GROWTH_SMASH);

            } else {
                return (MMID::SPIRE_GROWTH_QUICK_TACKLE);
            }
        }

        case MonsterId::TRANSIENT: {
            return (MMID::TRANSIENT_ATTACK);
        }

        case MonsterId::WRITHING_MASS: {
            // 0 strong strike
            // 1 multi strike
            // 2 flail
            // 3 wither
            // 4 implant
            if (firstTurn()) {
                if (roll < 33) {
                    return (MMID::WRITHING_MASS_MULTI_STRIKE);

                } else if (roll < 66) {
                    return (MMID::WRITHING_MASS_FLAIL);

                } else {
                    return (MMID::WRITHING_MASS_WITHER);
                }
            }

            const bool haveUsedImplant = miscInfo;
            auto myRoll = roll;
            while (true) {

                if (myRoll < 10) {
                    if (!lastMove(MMID::WRITHING_MASS_STRONG_STRIKE)) {
                        return (MMID::WRITHING_MASS_STRONG_STRIKE);
                    }
                    myRoll = bc.aiRng.random(10, 99);
                }

                if (myRoll < 20) {
                    if (!haveUsedImplant && !lastMove(MMID::WRITHING_MASS_IMPLANT)) {
                        return (MMID::WRITHING_MASS_IMPLANT);

                    } else if (bc.aiRng.randomBoolean(0.1f)) {
                        return (MMID::WRITHING_MASS_STRONG_STRIKE);
                    }
                    myRoll = bc.aiRng.random(20, 99);
                }

                if (myRoll < 40) {
                    if (!lastMove(MMID::WRITHING_MASS_WITHER)) {
                        return MMID::WRITHING_MASS_WITHER;
                    }

                    // last move was wither
                    if (bc.aiRng.randomBoolean(0.4f)) {
                        myRoll = bc.aiRng.random(0, 19);
                        if (myRoll < 10) {
                            return (MMID::WRITHING_MASS_STRONG_STRIKE);

                        } else {
                            if (!haveUsedImplant) {
                                return (MMID::WRITHING_MASS_IMPLANT);

                            } else if (bc.aiRng.randomBoolean(0.1f)) {
                                return (MMID::WRITHING_MASS_STRONG_STRIKE);

                            } else {
                                myRoll= bc.aiRng.random(20, 99);
                                continue;
                            }
                        }
                    }
                    myRoll = bc.aiRng.random(40, 99);
                }

                if (myRoll < 70) {
                    if (!lastMove(MMID::WRITHING_MASS_MULTI_STRIKE)) {
                        return MMID::WRITHING_MASS_MULTI_STRIKE;

                    } else if (bc.aiRng.randomBoolean(0.3f)) {
                        return MMID::WRITHING_MASS_FLAIL;

                    } else {
                        myRoll= bc.aiRng.random(0, 39);
                        continue;
                    }
                }

                if (!lastMove(MMID::WRITHING_MASS_FLAIL)) {
                    return MMID::WRITHING_MASS_FLAIL;

                } else {
                    return (MMID::WRITHING_MASS_WITHER);
                }
            }
        }

        case MonsterId::GIANT_HEAD: {
            // 1 glare
            // 2 it is time
            // 3 count
            if (bc.getMonsterTurnNumber() >= 4) {
                return MMID::GIANT_HEAD_IT_IS_TIME;
            }

            if (roll < 50) {
                if (!lastTwoMoves(MMID::GIANT_HEAD_GLARE)) {
                    return MMID::GIANT_HEAD_GLARE;
                } else {
                    return MMID::GIANT_HEAD_COUNT;
                }
            }

            // roll >= 50
            if (!lastTwoMoves(MMID::GIANT_HEAD_COUNT)) {
                return MMID::GIANT_HEAD_COUNT;
            } else {
                return MMID::GIANT_HEAD_GLARE;
            }
        }

        case MonsterId::TIME_EATER: {
            // 2 reverberate
            // 3 ripple
            // 4 head slam
            // 5 haste
            const bool usedHaste = miscInfo;
            const bool underHalfHp = curHp < maxHp/2;
            if (!usedHaste && underHalfHp) {
                return MonsterMoveId::TIME_EATER_HASTE;
            }

            auto myRoll = roll;
            if (myRoll < 45) {
                if (!lastTwoMoves(MonsterMoveId::TIME_EATER_REVERBERATE)) {
                    return MonsterMoveId::TIME_EATER_REVERBERATE;
                }
                myRoll = bc.aiRng.random(50,99);
            }

            if (myRoll < 80) {
                if (!lastMove(MonsterMoveId::TIME_EATER_HEAD_SLAM)) {
                    return MonsterMoveId::TIME_EATER_HEAD_SLAM;
                }
                if (bc.aiRng.randomBoolean(0.66f)) {
                    return MonsterMoveId::TIME_EATER_REVERBERATE;
                }
                return MonsterMoveId::TIME_EATER_RIPPLE;
            }

            if (lastMove(MMID::TIME_EATER_RIPPLE)) {
                myRoll = bc.aiRng.random(74);
                if (myRoll < 45) {
                    return MonsterMoveId::TIME_EATER_REVERBERATE;
                } else {
                    return MonsterMoveId::TIME_EATER_HEAD_SLAM;
                }
            }

            return MonsterMoveId::TIME_EATER_RIPPLE;
        }

        case MonsterId::DECA: {
            return MonsterMoveId::DECA_BEAM;
        }

        case MonsterId::DONU: {
            return MonsterMoveId::DONU_CIRCLE_OF_POWER;
        }

        case MonsterId::AWAKENED_ONE: {
            // 1 slash
            // 2 soul strike
            // 3 rebirth
            // 5 dark echo
            // 6 sludge
            // 8 tackle

            if (halfDead) {
                return MonsterMoveId::AWAKENED_ONE_REBIRTH;
            }

            const bool phase2 = miscInfo;
            if (!phase2) {
                if (firstTurn()) {
                    return MonsterMoveId::AWAKENED_ONE_SLASH;
                }

                if (roll < 25) {
                    if (lastMove(MonsterMoveId::AWAKENED_ONE_SOUL_STRIKE)) {
                        return MonsterMoveId::AWAKENED_ONE_SLASH;
                    } else {
                        return MonsterMoveId::AWAKENED_ONE_SOUL_STRIKE;
                    }
                }

                if (!lastTwoMoves(MonsterMoveId::AWAKENED_ONE_SLASH)) {
                    return MonsterMoveId::AWAKENED_ONE_SLASH;
                } else {
                    return MonsterMoveId::AWAKENED_ONE_SOUL_STRIKE;
                }
            }

            // phase 2

            if (roll < 50) {
                if (!lastTwoMoves(MonsterMoveId::AWAKENED_ONE_SLUDGE)) {
                    return MonsterMoveId::AWAKENED_ONE_SLUDGE;
                } else {
                    return MonsterMoveId::AWAKENED_ONE_TACKLE;
                }
            }

            if (!lastTwoMoves(MonsterMoveId::AWAKENED_ONE_TACKLE)) {
                return MonsterMoveId::AWAKENED_ONE_TACKLE;
            } else {
                return MonsterMoveId::AWAKENED_ONE_SLUDGE;
            }
        }

        case MonsterId::CORRUPT_HEART: {
            if (firstTurn()) {
                return MonsterMoveId::CORRUPT_HEART_DEBILITATE;
            }
            // only called if not going to buff
            if (bc.aiRng.randomBoolean()) {
                return MonsterMoveId::CORRUPT_HEART_BLOOD_SHOTS;
            } else {
                return MonsterMoveId::CORRUPT_HEART_ECHO;
            }
        }

        case MonsterId::SPIRE_SHIELD: {
            // 1 bash
            // 2 fortify
            // 3 smash
            if (bc.aiRng.randomBoolean()) {
                return MonsterMoveId::SPIRE_SHIELD_FORTIFY;
            } else {
                return MonsterMoveId::SPIRE_SHIELD_BASH;
            }
        }

        case MonsterId::SPIRE_SPEAR: {
            if (firstTurn()) {
                return MonsterMoveId::SPIRE_SPEAR_BURN_STRIKE;
            }
            if (bc.aiRng.randomBoolean()) {
                return MonsterMoveId::SPIRE_SPEAR_PIERCER;
            } else {
                return MonsterMoveId::SPIRE_SPEAR_BURN_STRIKE;
            }
        }

        case MonsterId::TORCH_HEAD:
            return MonsterMoveId::TORCH_HEAD_TACKLE;

        default:
            break;
    }
#ifdef sts_asserts
    std::cerr << "getMove did not return a value " << bc.seed << " " << static_cast<int>(id) << std::endl;
    assert(false);
#endif
    return MMID::INVALID;
}

void Monster::initSpawnedMonster(BattleContext &bc, const MonsterId monsterId, const int monsterIdx, const int hp) {
    idx = monsterIdx;
    id = monsterId;
    curHp = hp;
    maxHp = hp;
    rollMove(bc);
}

void Monster::stealGoldFromPlayer(BattleContext &bc, int amount) {
    const auto theftAmount = std::min(static_cast<int>(bc.player.gold), amount);
    if (theftAmount > 0) {
        miscInfo += theftAmount;
        bc.player.gold -= theftAmount;
        bc.setRequiresStolenGoldCheck(true);
    }
}

void Monster::largeSlimeSplit(BattleContext &bc, const MonsterId mediumSlimeType, const int placeIdx, const int hp) {
#ifdef sts_asserts
    assert(hp > 0);
    assert(bc.monsters.monstersAlive > 0);
#endif
    const auto idx1 = placeIdx;
    const auto idx2 = placeIdx + 1;

    bc.monsters.arr[idx1] = Monster();
    bc.monsters.arr[idx1].initSpawnedMonster(bc, mediumSlimeType, idx1, hp);

    bc.monsters.arr[idx2] = Monster();
    bc.monsters.arr[idx2].initSpawnedMonster(bc, mediumSlimeType, idx2, hp);

    if (bc.player.hasRelic<R::PHILOSOPHERS_STONE>()) {
        bc.monsters.arr[idx1].buff<MS::STRENGTH>(1);
        bc.monsters.arr[idx2].buff<MS::STRENGTH>(1);
    }

    bc.monsters.monstersAlive++;
    bc.monsters.monsterCount = std::min(bc.monsters.monsterCount+1, 4);

    bc.noOpRollMove();
    bc.monsters.extraRollMoveOnTurn.set(idx2, true);
    bc.monsterTurnIdx++;
}

void Monster::slimeBossSplit(BattleContext &bc, const int hp) {
#ifdef sts_asserts
    assert(hp > 0);
    assert(bc.monsters.monstersAlive > 0);
#endif
    const auto idx1 = 0;
    const auto idx2 = 2;

    bc.monsters.arr[idx1] = Monster();
    bc.monsters.arr[idx1].initSpawnedMonster(bc, MonsterId::SPIKE_SLIME_L, idx1, hp);

    bc.monsters.arr[idx2] = Monster();
    bc.monsters.arr[idx2].initSpawnedMonster(bc, MonsterId::ACID_SLIME_L, idx2, hp);

    if (bc.player.hasRelic<R::PHILOSOPHERS_STONE>()) {
        bc.monsters.arr[idx1].buff<MS::STRENGTH>(1);
        bc.monsters.arr[idx2].buff<MS::STRENGTH>(1);
    }

    bc.monsters.monsterCount = 3;
    bc.monsters.monstersAlive = 2;
    bc.monsterTurnIdx = 3;
}

void Monster::spawnBronzeOrbs(BattleContext &bc) {

    auto &orb1 = bc.monsters.arr[0];
    auto &orb2 = bc.monsters.arr[2];

    orb1.construct(bc, MonsterId::BRONZE_ORB, 0);
    orb2.construct(bc, MonsterId::BRONZE_ORB, 2);

    orb1.buff<MS::MINION>();
    orb2.buff<MS::MINION>();

    if (bc.player.hasRelic<R::PHILOSOPHERS_STONE>()) {
        orb1.buff<MS::STRENGTH>(1);
        orb2.buff<MS::STRENGTH>(1);
    }

    orb1.rollMove(bc);
    orb2.rollMove(bc);

    bc.monsters.monstersAlive += 2;
    ++bc.monsterTurnIdx;
}


struct StasisPair {
    int groupIdx;
    int idOrder;
};

// returns index of card to remove
// list is guaranteed to not be empty
template<typename ForwardIt>
int stasisHelper(Random &rng, ForwardIt begin, ForwardIt end) {
    int cardRarityCounts[6] {0,0,0,0,0,0};

    for (auto it = begin; it != end; ++it) {
        int rarity = static_cast<int>(getCardRarity(it->id));
        ++cardRarityCounts[rarity];
    }

    CardRarity targetRarity;
    if (cardRarityCounts[static_cast<int>(CardRarity::RARE)] != 0) {
        targetRarity = CardRarity::RARE;

    } else if (cardRarityCounts[static_cast<int>(CardRarity::UNCOMMON)] != 0) {
        targetRarity = CardRarity::UNCOMMON;

    } else if (cardRarityCounts[static_cast<int>(CardRarity::COMMON)] != 0) {
        targetRarity = CardRarity::COMMON;

    } else {
        const int groupSize = end-begin;
        return rng.random(groupSize-1);
    }

    fixed_list<StasisPair, CardManager::MAX_GROUP_SIZE> idxList;

    int i = 0;
    for (auto it = begin; it != end; ++it) {
        if (getCardRarity(it->id) == targetRarity) {
            StasisPair pair {i, cardSortedIdx[static_cast<int>(it->id)]};
            idxList.push_back(pair);
        }
        ++i;
    }

    std::stable_sort(idxList.begin(), idxList.end(), [](auto p1, auto p2) { return p1.idOrder < p2.idOrder; } );

    const auto idxListSelectIdx = rng.random(idxList.size()-1);
    return idxList[idxListSelectIdx].groupIdx;
}

void Monster::stasisAction(BattleContext &bc) {
    auto &cards = bc.cards;
    if (cards.drawPile.empty() && cards.discardPile.empty()) {
        return; // do nothing
    }

    CardInstance stasisCard;
    if (cards.drawPile.empty()) {
        const auto removeIdx = stasisHelper(
                bc.cardRandomRng, cards.discardPile.begin(), cards.discardPile.end()
                );

        stasisCard = cards.discardPile[removeIdx];
        cards.removeFromDiscard(removeIdx);

    } else {
        const auto removeIdx = stasisHelper(
                bc.cardRandomRng, cards.drawPile.begin(), cards.drawPile.end()
        );

        stasisCard = cards.drawPile[removeIdx];
        cards.removeFromDrawPileAtIdx(removeIdx);
    }

    cards.notifyRemoveFromCombat(stasisCard);
    cards.stasisCards[std::min(1, idx)] = stasisCard;

#ifdef sts_asserts
    if (stasisCard.getId() == CardId::INVALID) {
        std::cerr << bc << std::endl;
        assert(false);
    }
#endif
    buff<MS::STASIS>();
}


void Monster::returnStasisCard(BattleContext &bc) {
    auto &stasisCard = bc.cards.stasisCards[std::min(idx,1)];

#ifdef sts_asserts
    if (stasisCard.id == CardId::INVALID) {
        std::cerr << bc.seed << " " << bc.loopCount << " stasis card invalid" << idx << "\n" << bc << std::endl;
    }
    assert(stasisCard.id != CardId::INVALID);
#endif

    bc.cards.notifyAddCardToCombat(stasisCard);
    bc.moveToHandHelper(stasisCard);
    stasisCard = {CardId::INVALID};
}

void reptoSummonHelper(const BattleContext &bc, int daggerIdxs[2], int daggerCount) {
    constexpr int searchOrder[4] = {4, 1, 3, 0};

    int openSlotsFound = 0;
    for (int i = 0; i < 4; ++i) {
        const auto mIdx = searchOrder[i];
        const auto &m = bc.monsters.arr[mIdx];

        if (!m.isAlive()) {
            daggerIdxs[openSlotsFound++] = mIdx;
        }

        if (openSlotsFound == daggerCount) {
            return;
        }
    }

#ifdef sts_asserts
    assert(false); // should always return first
#endif
}

void Monster::reptomancerSummon(BattleContext &bc, int daggerCount) {
    int daggerIdxs[2];
    reptoSummonHelper(bc, daggerIdxs, daggerCount);

    for (int i = 0; i < daggerCount; ++i) {
        const auto daggerIdx = daggerIdxs[i];
        auto &dagger = bc.monsters.arr[daggerIdx];
        dagger = Monster();
        dagger.construct(bc, MonsterId::DAGGER, daggerIdx);
        ++bc.monsters.monstersAlive;
        dagger.setMove(MMID::DAGGER_STAB);
        dagger.buff<MS::MINION>();
        if (bc.player.hasRelic<R::PHILOSOPHERS_STONE>()) {
            dagger.buff<MS::STRENGTH>(1);
        }

        bc.noOpRollMove();
        bc.monsters.skipTurn.set(daggerIdx, true);
    }

//    std::cout << "repto summon: " << daggerIdxs[0] << " " << daggerIdxs[1] << '\n';
}

int Monster::getAliveGremlinCount(const BattleContext &bc) {
    int count = 0;
    for (int i = 0; i < 3; ++i) {
        const auto &m = bc.monsters.arr[i];
        if (!m.isDying()) {
            ++count;
        }
    }
    return count;
}
