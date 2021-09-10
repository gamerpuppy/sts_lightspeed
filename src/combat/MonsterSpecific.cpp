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
            setRandomHp(hpRng, ascension >= 7); // first call is discarded by game
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
        case MonsterId::BRONZE_ORB:
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
        case MonsterId::REPTOMANCER:
        case MonsterId::SENTRY:
        case MonsterId::SPIRE_SHIELD:
        case MonsterId::SPIRE_SPEAR:
        case MonsterId::TASKMASTER:
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
    const bool asc7 = bc.ascension >= 7;
    const bool asc9 = bc.ascension >= 9;
    const bool asc17 = bc.ascension >= 17;
    const bool asc18 = bc.ascension >= 18;
    const bool asc19 = bc.ascension >= 19;

    const int hallwayDiffIdx = getTriIdx(bc.ascension, 2, 17);

    switch (id) {
        case MonsterId::DARKLING:       // game adds regrow power
            break;

        case MonsterId::MAD_GREMLIN:
            buff<MS::ANGRY>(asc17 ? 2 : 1);
            break;

        case MonsterId::EXPLODER:       // game adds explosive power
        case MonsterId::GIANT_HEAD:     // game adds slow power
        case MonsterId::REPTOMANCER:    // game adds MinionPower to all daggers
        case MonsterId::GREMLIN_LEADER: // game adds MinionPower to all gremlins
        case MonsterId::TRANSIENT:      // game adds ShiftingPower
            break;

        case MonsterId::BOOK_OF_STABBING:    // game adds PainfulStabsPower
            buff<MS::PAINFUL_STABS>();
            ++miscInfo; // stab count
            break;

        case MonsterId::FUNGI_BEAST:    // game adds SporeCloudPower
            buff<MS::SPORE_CLOUD>(2); // the value here isn't used. it is always 2
            break;

        case MonsterId::AWAKENED_ONE:
            // game adds curiosity power
            buff<MS::REGEN>(asc19 ? 15 : 10);
            break;

        case MonsterId::DONU:
        case MonsterId::DECA:
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
            // game adds reactive power
            buff<MS::MALLEABLE>(3);
            break;
        }

        case MonsterId::BRONZE_AUTOMATON: {
            buff<MS::ARTIFACT>(3);
            break;
        }

        case MonsterId::TIME_EATER: {
            uniquePower0 = 0; // TimeWarpPower
            break;
        }

        case MonsterId::BYRD: {
            buff<MS::FLIGHT>(asc17 ? 4 : 3);
            break;
        }

        case MonsterId::LOOTER:
        case MonsterId::MUGGER: {
//            buff<MS::THIEVERY>(asc17 ? 20 : 15);
            break;
        }

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

        case MonsterId::CORRUPT_HEART: {
            uniquePower0 = asc19 ? 200 : 300; // InvinciblePower
            uniquePower1 = asc19 ? 2 : 1; // BeatOfDeathPower
            break;
        }

        case MonsterId::SPIRE_SHIELD: {
            // game adds surrounded power
            buff<MS::ARTIFACT>(asc18 ? 2 : 1);
            break;
        }

        case MonsterId::SPIRE_SPEAR: {
            buff<MS::ARTIFACT>(asc18 ? 2 : 1);
            break;
        }

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
    const bool asc17 = bc.ascension >= 17;
    const bool asc18 = bc.ascension >= 18;
    const bool asc19 = bc.ascension >= 19;
    const int hallwayIdx = getTriIdx(asc, 2, 17);
    const int eliteDiffIdx = getTriIdx(bc.ascension, 3, 18);
    const int bossDiffIdx = getTriIdx(bc.ascension, 4, 19);

    switch (moveHistory[0]) {

        case MMID::ACID_SLIME_S_LICK:
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(1));
            setMove(MMID::ACID_SLIME_S_TACKLE);
            break;

        case MMID::ACID_SLIME_S_TACKLE:
            attackPlayerHelper(bc, asc2 ? 4 : 3);
            setMove(MMID::ACID_SLIME_S_LICK);
            break;

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

        // ************ Book of Stabbing ************

        case MMID::BOOK_OF_STABBING_MULTI_STAB:
            attackPlayerHelper(bc, asc3 ? 7 : 6, miscInfo);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::BOOK_OF_STABBING_SINGLE_STAB:
            attackPlayerHelper(bc, asc3 ? 24 : 21);
            bc.addToBot(Actions::RollMove(idx));
            break;

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
            setMove(MonsterMoveId::BYRD_FLY);
            break;

        case MMID::BYRD_PECK:
            attackPlayerHelper(bc, 1, asc2 ? 6 : 5);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MMID::BYRD_STUNNED:
            bc.noOpRollMove();
            setMove(MonsterMoveId::BYRD_HEADBUTT);
            break;

        case MMID::BYRD_SWOOP:
            attackPlayerHelper(bc, asc2 ? 14 : 12);
            bc.addToBot( Actions::RollMove(idx) );
            break;

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

        case MMID::ACID_SLIME_M_CORROSIVE_SPIT:
            attackPlayerHelper(bc, asc2 ? 8 : 7);
            bc.addToBot(Actions::MakeTempCardInDiscard(CardId::SLIMED));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::ACID_SLIME_M_LICK:
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(1, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MMID::ACID_SLIME_M_TACKLE:
            attackPlayerHelper(bc, asc2 ? 12 : 10);
            bc.addToBot(Actions::RollMove(idx));
            break;

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
            stealGoldFromPlayer(bc, asc17 ? 20 : 15);
            attackPlayerHelper(bc, asc2 ? 14 : 12);
            setMove(MMID::LOOTER_SMOKE_BOMB);
            break;
        }

        case MMID::LOOTER_MUG: { // 1
            if (bc.getMonsterTurnNumber() == 1) {
                bc.aiRng.randomBoolean(0.6f); // for a dialog message in game
            }
            stealGoldFromPlayer(bc, asc17 ? 20 : 15);
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
            stealGoldFromPlayer(bc, asc17 ? 20 : 15);
            attackPlayerHelper(bc, asc2 ? 18 : 16);
            setMove(MMID::MUGGER_SMOKE_BOMB);
            break;
        }

        case MMID::MUGGER_MUG: { // 1
            bc.aiRng.random(2); // for a dialog message in game
            if (bc.getMonsterTurnNumber() == 2) {
                bc.aiRng.randomBoolean(0.6f); // for a dialog message in game
            }
            stealGoldFromPlayer(bc, asc17 ? 20 : 15);
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
            attackPlayerHelper(bc, asc2 ? 8 : 7);
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

        case MMID::INVALID:
#ifdef sts_asserts
            assert(false);
#endif
        case MMID::GENERIC_ESCAPE_MOVE:
        default:
            break;
    }
}

void Monster::setMoveFromRoll(BattleContext &bc, const int roll) {
    const bool asc17 = bc.ascension >= 17;
    const bool asc18 = bc.ascension >= 18;
    const bool asc19 = bc.ascension >= 19;

    switch (id) {

        case MonsterId::ACID_SLIME_S: {
            if (asc17) {
                setMove(MMID::ACID_SLIME_S_LICK);
            } else if (bc.aiRng.randomBoolean()) {
                setMove(MMID::ACID_SLIME_S_TACKLE);
            } else {
                setMove(MMID::ACID_SLIME_S_LICK);
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
                            setMove(MMID::ACID_SLIME_M_TACKLE);
                        } else {
                            setMove(MMID::ACID_SLIME_M_LICK);
                        }
                    } else {
                        setMove(MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                    }

                } else if (roll < 80) {
                    if (lastTwoMoves(MMID::ACID_SLIME_M_TACKLE)) {
                        if (bc.aiRng.randomBoolean(0.5f)) {
                            setMove(MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                        } else {
                            setMove(MMID::ACID_SLIME_M_LICK);
                        }
                    } else {
                        setMove(MMID::ACID_SLIME_M_TACKLE);
                    }

                } else if (lastMove(MMID::ACID_SLIME_M_LICK)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        setMove(MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                    } else {
                        setMove(MMID::ACID_SLIME_M_TACKLE);
                    }
                } else {
                    setMove(MMID::ACID_SLIME_M_LICK);
                }
                // END ASCENSION 17

            } else if (roll < 30) {
                if (lastTwoMoves(MMID::ACID_SLIME_M_CORROSIVE_SPIT)) {
                    if (bc.aiRng.randomBoolean()) {
                        setMove(MMID::ACID_SLIME_M_TACKLE);
                    } else {
                        setMove(MMID::ACID_SLIME_M_LICK);
                    }
                } else {
                    setMove(MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                }

            } else if (roll < 70) {
                if (lastMove(MMID::ACID_SLIME_M_TACKLE)) {
                    if (bc.aiRng.randomBoolean(0.4f)) {
                        setMove(MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                    } else {
                        setMove(MMID::ACID_SLIME_M_LICK);
                    }
                } else {
                    setMove(MMID::ACID_SLIME_M_TACKLE);
                }

            } else if (lastTwoMoves(MMID::ACID_SLIME_M_LICK)) {
                if (bc.aiRng.randomBoolean(0.4F)) {
                    setMove(MMID::ACID_SLIME_M_CORROSIVE_SPIT);
                } else {
                    setMove(MMID::ACID_SLIME_M_TACKLE);
                }
            } else {
                setMove(MMID::ACID_SLIME_M_LICK);
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
                            setMove(MMID::ACID_SLIME_L_TACKLE);
                        } else {
                            setMove(MMID::ACID_SLIME_L_LICK);
                        }
                    } else {
                        setMove(MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                    }

                } else if (roll < 70) {
                    if (lastTwoMoves(MMID::ACID_SLIME_L_TACKLE)) {
                        if (bc.aiRng.randomBoolean(0.6F)) {
                            setMove(MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                        } else {
                            setMove(MMID::ACID_SLIME_L_LICK);
                        }
                    } else {
                        setMove(MMID::ACID_SLIME_L_TACKLE);
                    }
                } else if (lastMove(MMID::ACID_SLIME_L_LICK)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        setMove(MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                    } else {
                        setMove(MMID::ACID_SLIME_L_TACKLE);
                    }
                } else {
                    setMove(MMID::ACID_SLIME_L_LICK);
                }

            } else if (roll < 30) {
                if (lastTwoMoves(MMID::ACID_SLIME_L_CORROSIVE_SPIT)) {
                    if (bc.aiRng.randomBoolean()) {
                        setMove(MMID::ACID_SLIME_L_TACKLE);
                    } else {
                        setMove(MMID::ACID_SLIME_L_LICK);
                    }
                } else {
                    setMove(MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                }
            } else if (roll < 70) {
                if (lastMove(MMID::ACID_SLIME_L_TACKLE)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        setMove(MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                    } else {
                        setMove(MMID::ACID_SLIME_L_LICK);
                    }
                } else {
                    setMove(MMID::ACID_SLIME_L_TACKLE);
                }
            } else if (lastTwoMoves(MMID::ACID_SLIME_L_LICK)) {
                if (bc.aiRng.randomBoolean(0.4F)) {
                    setMove(MMID::ACID_SLIME_L_CORROSIVE_SPIT);
                } else {
                    setMove(MMID::ACID_SLIME_L_TACKLE);
                }
            } else {
                setMove(MMID::ACID_SLIME_L_LICK);
            }
            break;
        }

        case MonsterId::BLUE_SLAVER: {
            if (roll >= 40 && !lastTwoMoves(MMID::BLUE_SLAVER_STAB)) {
                setMove(MMID::BLUE_SLAVER_STAB);

            } else if ( !lastTwoMoves(MMID::BLUE_SLAVER_RAKE) ||
                (asc17 && !lastMove(MMID::BLUE_SLAVER_RAKE)) ) {
                setMove(MMID::BLUE_SLAVER_RAKE);

            } else {
                setMove(MMID::BLUE_SLAVER_STAB);
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
//            if (!hasStatus<MS::FLIGHT>()) {
//                setMove(MonsterMoveId::BYRD_HEADBUTT);
//                break;
//            }

            if (firstTurn()) {
                if (bc.aiRng.randomBoolean(0.375f)) {
                    setMove(MonsterMoveId::BYRD_CAW);
                } else {
                    setMove(MonsterMoveId::BYRD_PECK);
                }
                break;
            }

            if (roll < 50) {
                if (lastTwoMoves(MonsterMoveId::BYRD_PECK)) {
                    if (bc.aiRng.randomBoolean(0.4f)) {
                        setMove(MonsterMoveId::BYRD_SWOOP);
                    } else {
                        setMove(MonsterMoveId::BYRD_CAW);
                    }
                } else {
                    setMove(MonsterMoveId::BYRD_PECK);
                }
                break;
            }

            if (roll < 70) {
                if (lastMove(MonsterMoveId::BYRD_SWOOP)) {
                    if (bc.aiRng.randomBoolean(0.375f)) {
                        setMove(MonsterMoveId::BYRD_CAW);
                    } else {
                        setMove(MonsterMoveId::BYRD_PECK);
                    }
                } else {
                    setMove(MonsterMoveId::BYRD_SWOOP);
                }
                break;
            }

            if (lastMove(MonsterMoveId::BYRD_CAW)) {
                if (bc.aiRng.randomBoolean(0.2857f)) {
                    setMove(MonsterMoveId::BYRD_SWOOP);
                } else {
                    setMove(MonsterMoveId::BYRD_PECK);
                }
            } else {
                setMove(MonsterMoveId::BYRD_CAW);
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
                    setMove(MMID::CHOSEN_HEX);

                } else if (!lastMove(MMID::CHOSEN_DEBILITATE) && !lastMove(MMID::CHOSEN_DRAIN)) {
                    if (roll < 50) {
                        setMove(MMID::CHOSEN_DEBILITATE);
                    } else {
                        setMove(MMID::CHOSEN_DRAIN);
                    }

                } else if (roll < 40) {
                    setMove(MMID::CHOSEN_ZAP);
                } else {
                    setMove(MMID::CHOSEN_POKE);
                }

                break;
            }

            if (firstTurn()) {
                setMove(MMID::CHOSEN_POKE);

            } else if (lastMoveBefore(MMID::INVALID)) {
                setMove(MMID::CHOSEN_HEX);

            } else if (!lastMove(MMID::CHOSEN_DEBILITATE) && !lastMove(MMID::CHOSEN_DRAIN)) {
                if (roll < 50) {
                    setMove(MMID::CHOSEN_DEBILITATE);
                } else {
                    setMove(MMID::CHOSEN_DRAIN);
                }
            } else if (roll < 40) {
                setMove(MMID::CHOSEN_ZAP);
            } else {
                setMove(MMID::CHOSEN_POKE);
            }
            break;
        }

        case MonsterId::BOOK_OF_STABBING: {
            // 1 multi stab
            // 2 single stab
            auto &stabCount = miscInfo;
            if (roll < 15) {
                if (lastMove(MMID::BOOK_OF_STABBING_SINGLE_STAB)) {
                    ++stabCount;
                    setMove(MonsterMoveId::BOOK_OF_STABBING_MULTI_STAB);
                } else {
                    setMove(MonsterMoveId::BOOK_OF_STABBING_SINGLE_STAB);
                    if (asc18) {
                        ++stabCount;
                    }
                }
            } else if (lastTwoMoves(MMID::BOOK_OF_STABBING_MULTI_STAB)) {
                setMove(MonsterMoveId::BOOK_OF_STABBING_SINGLE_STAB);
                if (asc18) {
                    ++stabCount;
                }
            } else {
                ++stabCount;
                setMove(MonsterMoveId::BOOK_OF_STABBING_MULTI_STAB);
            }
            break;
        }

        case MonsterId::CULTIST: {
            if (lastMove(MMID::INVALID)) {
                setMove(MMID::CULTIST_INCANTATION);
            } else {
                setMove(MMID::CULTIST_DARK_STRIKE);
            }
            break;
        }

        case MonsterId::FAT_GREMLIN: {
            setMove(MMID::FAT_GREMLIN_SMASH);
            break;
        }

        case MonsterId::FUNGI_BEAST: {
            // 1 FUNGI_BEAST_BITE
            // 2 FUNGI_BEAST_GROW
            if (roll < 60) {
                if (lastTwoMoves(MMID::FUNGI_BEAST_BITE)) {
                    setMove(MMID::FUNGI_BEAST_GROW);
                } else {
                    setMove(MMID::FUNGI_BEAST_BITE);
                }

            } else if (lastMove(MMID::FUNGI_BEAST_GROW)) {
                setMove(MMID::FUNGI_BEAST_BITE);

            } else {
                setMove(MMID::FUNGI_BEAST_GROW);
            }
            break;
        }

        case MonsterId::GREEN_LOUSE: {
            if (roll < 25) {
                if (lastMove(MMID::GREEN_LOUSE_SPIT_WEB) && (asc17 || lastTwoMoves(MMID::GREEN_LOUSE_SPIT_WEB))) {
                    setMove(MMID::GREEN_LOUSE_BITE);
                } else  {
                    setMove(MMID::GREEN_LOUSE_SPIT_WEB);
                }

            } else if (lastTwoMoves(MMID::GREEN_LOUSE_BITE)) {
                setMove(MMID::GREEN_LOUSE_SPIT_WEB);

            } else {
                setMove(MMID::GREEN_LOUSE_BITE);
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
                        setMove(MMID::GREMLIN_LEADER_STAB);
                    } else {
                        setMove(MMID::GREMLIN_LEADER_RALLY);
                    }
                } else if (lastMove(MMID::GREMLIN_LEADER_STAB)) {
                    setMove(MMID::GREMLIN_LEADER_RALLY);
                } else {
                    setMove(MMID::GREMLIN_LEADER_STAB);
                }

            } else if (numAliveGremlins == 1) {

                if (roll < 50) {
                    if (lastMove(MMID::GREMLIN_LEADER_RALLY)) {
                        const auto roll2 = bc.aiRng.random(50, 99);
                        if (roll2 < 80) {
                            setMove(MMID::GREMLIN_LEADER_ENCOURAGE);
                        } else {
                            setMove(MMID::GREMLIN_LEADER_STAB);
                        }
                    } else {
                        setMove(MMID::GREMLIN_LEADER_RALLY);
                    }

                } else if (roll < 80) {
                    if (lastMove(MMID::GREMLIN_LEADER_ENCOURAGE)) {
                        setMove(MMID::GREMLIN_LEADER_STAB);
                    } else {
                        setMove(MMID::GREMLIN_LEADER_ENCOURAGE);
                    }

                } else if (lastMove(MMID::GREMLIN_LEADER_STAB)) {
                    const auto roll2 = bc.aiRng.random(0, 80);
                    if (roll2 < 50) {
                        setMove(MMID::GREMLIN_LEADER_RALLY);
                    } else {
                        setMove(MMID::GREMLIN_LEADER_ENCOURAGE);
                    }

                } else {
                    setMove(MMID::GREMLIN_LEADER_STAB);
                }

            } else { // alive gremlins > 1

                if (roll < 66) {
                    if (lastMove(MMID::GREMLIN_LEADER_ENCOURAGE)) {
                        setMove(MMID::GREMLIN_LEADER_STAB);
                    } else {
                        setMove(MMID::GREMLIN_LEADER_ENCOURAGE);
                    }

                } else if (lastMove(MMID::GREMLIN_LEADER_STAB)) {
                    setMove(MonsterMoveId::GREMLIN_LEADER_ENCOURAGE);

                } else {
                    setMove(MMID::GREMLIN_LEADER_STAB);
                }

            }
            break;
        }

        case MonsterId::GREMLIN_NOB: {
            // 1 Rush
            // 2 Skull Bash
            // 3 BELLOW

            if (lastMove(MMID::INVALID)) {
                setMove(MMID::GREMLIN_NOB_BELLOW);
                break;
            }

            if (asc18) {
                if (!lastTwoMoves(MMID::GREMLIN_NOB_SKULL_BASH)) {
                    setMove(MMID::GREMLIN_NOB_RUSH);
                    break;
                }

                if (lastTwoMoves(MMID(MMID::GREMLIN_NOB_RUSH))) {
                    setMove(MMID::GREMLIN_NOB_SKULL_BASH);

                } else {
                    setMove(MMID::GREMLIN_NOB_RUSH);
                }
                break;
            }

            if (roll < 33 || lastTwoMoves(MMID::GREMLIN_NOB_RUSH)) {
                setMove(MMID::GREMLIN_NOB_SKULL_BASH);

            } else {
                setMove(MMID::GREMLIN_NOB_RUSH);
            }
            break;
        }

        case MonsterId::GREMLIN_WIZARD: {
            miscInfo = 1; // gremlin wizard charge
            setMove(MMID::GREMLIN_WIZARD_CHARGING);
            break;
        }

        case MonsterId::HEXAGHOST: {
#ifdef sts_asserts
            assert(firstTurn());
#endif
            setMove(MMID::HEXAGHOST_ACTIVATE);
            break;
        }

        case MonsterId::JAW_WORM: {
            if (firstTurn()) {
                setMove(MMID::JAW_WORM_CHOMP);
                break;
            }

            if (roll < 25) {
                if (lastMove(MMID::JAW_WORM_CHOMP)) {
                    if (bc.aiRng.randomBoolean(0.5625f)) {
                        setMove(MMID::JAW_WORM_BELLOW);
                    } else {
                        setMove(MMID::JAW_WORM_THRASH);
                    }

                } else {
                    setMove(MMID::JAW_WORM_CHOMP);
                }

            } else if (roll < 55) {
                if (lastTwoMoves(MMID::JAW_WORM_THRASH)) {
                    if (bc.aiRng.randomBoolean(0.357f)) {
                        setMove(MMID::JAW_WORM_CHOMP);
                    } else {
                        setMove(MMID::JAW_WORM_BELLOW);
                    }

                } else {
                    setMove(MMID::JAW_WORM_THRASH);
                }

            } else if (lastMove(MMID::JAW_WORM_BELLOW)) {
                if (bc.aiRng.randomBoolean(0.416f)) {
                    setMove(MMID::JAW_WORM_CHOMP);
                } else {
                    setMove(MMID::JAW_WORM_THRASH);
                }

            } else {
                setMove(MMID::JAW_WORM_BELLOW);
            }
            break;
        }

        case MonsterId::LAGAVULIN: // called first turn only
            if (hasStatus<MS::ASLEEP>()) {
                setMove(MMID::LAGAVULIN_SLEEP);
            } else {
                setMove(MMID::LAGAVULIN_SIPHON_SOUL);
            }
            break;

        case MonsterId::LOOTER: // called first turn only
            setMove(MMID::LOOTER_MUG);
            break;

        case MonsterId::MAD_GREMLIN:
            setMove(MMID::MAD_GREMLIN_SCRATCH);
            break;

        case MonsterId::MUGGER: // called first turn only
            setMove(MMID::MUGGER_MUG);
            break;

        case MonsterId::RED_LOUSE: {
            if (roll < 25) {
                if (lastMove(MMID::RED_LOUSE_GROW) && (asc17 || lastTwoMoves(MMID::RED_LOUSE_GROW))) {
                    setMove(MMID::RED_LOUSE_BITE);
                } else  {
                    setMove(MMID::RED_LOUSE_GROW);
                }

            } else if (lastTwoMoves(MMID::RED_LOUSE_BITE)) {
                setMove(MMID::RED_LOUSE_GROW);

            } else {
                setMove(MMID::RED_LOUSE_BITE);
            }
            break;
        }

        case MonsterId::SENTRY: {
            if (firstTurn()) {
                if (idx % 2 == 0) {
                    setMove(MMID::SENTRY_BOLT);
                } else {
                    setMove(MMID::SENTRY_BEAM);
                }
            }
            break;
        }


        case MonsterId::SHIELD_GREMLIN: {
            setMove(MMID::SHIELD_GREMLIN_PROTECT);
            break;
        }

        case MonsterId::SLIME_BOSS: {
            // 1 SLIME_BOSS_PREPARING
            // 2 SLIME_BOSS_SLAM
            // 3 SLIME_BOSS_SPLIT
            // 4 SLIME_BOSS_GOOP_SPRAY
            if (firstTurn()) {
                setMove(MMID::SLIME_BOSS_GOOP_SPRAY);
            }
            break;
        }

        case MonsterId::SNAKE_PLANT: {
            // 1 chomp
            // 2 enfeebling spores
            if (asc17) {
                if (roll < 65) {
                    if (lastTwoMoves(MMID::SNAKE_PLANT_CHOMP)) {
                        setMove(MMID::SNAKE_PLANT_ENFEEBLING_SPORES);
                    } else {
                        setMove(MMID::SNAKE_PLANT_CHOMP);
                    }
                } else if (!lastTwoMoves(MMID::SNAKE_PLANT_ENFEEBLING_SPORES)) {
                    setMove(MMID::SNAKE_PLANT_ENFEEBLING_SPORES);
                } else {
                    setMove(MMID::SNAKE_PLANT_CHOMP);
                }
                break;
            }

            if (roll < 65) {
                if (lastTwoMoves(MMID::SNAKE_PLANT_CHOMP)) {
                    setMove(MMID::SNAKE_PLANT_ENFEEBLING_SPORES);
                } else {
                    setMove(MMID::SNAKE_PLANT_CHOMP);
                }
            } else if (lastMove(MMID::SNAKE_PLANT_ENFEEBLING_SPORES)) {
                setMove(MMID::SNAKE_PLANT_CHOMP);
            } else {
                setMove(MMID::SNAKE_PLANT_ENFEEBLING_SPORES);
            }
            break;
        }

        case MonsterId::SNEAKY_GREMLIN:
            setMove(MMID::SNEAKY_GREMLIN_PUNCTURE);
            break;

        case MonsterId::SNECKO: {
            // 1 perplexing glare
            // 2 bite
            // 3 tail whip
            if (firstTurn()) {
                setMove(MMID::SNECKO_PERPLEXING_GLARE);

            } else if (roll < 40) {
                setMove(MMID::SNECKO_TAIL_WHIP);

            } else if (lastTwoMoves(MMID::SNECKO_BITE)) {
                setMove(MMID::SNECKO_TAIL_WHIP);

            } else {
                setMove(MMID::SNECKO_BITE);
            }
            break;
        }

        case MonsterId::SPHERIC_GUARDIAN: // called first turn only
            setMove(MMID::SPHERIC_GUARDIAN_ACTIVATE);
            break;

        case MonsterId::SPIKE_SLIME_S:
            setMove(MMID::SPIKE_SLIME_S_TACKLE);
            break;

        case MonsterId::RED_SLAVER: {
            // 1 Stab
            // 2 Entangle
            // 3 Scrape
            const bool usedEntangle = miscInfo;

            if (lastMove(MMID::INVALID)) {
                setMove(MMID::RED_SLAVER_STAB);

            } else if (roll >= 75 && !usedEntangle) {
                setMove(MMID::RED_SLAVER_ENTANGLE);

            } else if (roll >= 50 && usedEntangle && !lastTwoMoves(MMID::RED_SLAVER_STAB)) {
                setMove(MMID::RED_SLAVER_STAB);

            } else if (!lastTwoMoves(MMID::RED_SLAVER_SCRAPE) || (asc17 && !lastMove(MMID::RED_SLAVER_SCRAPE))) {
                setMove(MMID::RED_SLAVER_SCRAPE);

            } else {
                setMove(MMID::RED_SLAVER_STAB);

            }
            break;
        }


        case MonsterId::SPIKE_SLIME_L: {
            // 1 flame tackle
            // 3 split
            // 4 lick
           if (roll < 30) {
                if (lastTwoMoves(MMID::SPIKE_SLIME_L_FLAME_TACKLE)) {
                    setMove(MMID::SPIKE_SLIME_L_LICK);
                } else {
                    setMove(MMID::SPIKE_SLIME_L_FLAME_TACKLE);
                }

            } else if (lastTwoMoves(MMID::SPIKE_SLIME_L_LICK) || (asc17 && lastMove(MMID::SPIKE_SLIME_L_LICK)) ) {
                setMove(MMID::SPIKE_SLIME_L_FLAME_TACKLE);

            } else {
                setMove(MMID::SPIKE_SLIME_L_LICK);

            }
            break;
        }

        case MonsterId::SPIKE_SLIME_M: {
            // 1 SPIKE_SLIME_M_FLAME_TACKLE
            // 4 SPIKE_SLIME_M_LICK
            if (roll < 30) {
                if (lastTwoMoves(MMID::SPIKE_SLIME_M_FLAME_TACKLE)) {
                    setMove(MMID::SPIKE_SLIME_M_LICK);
                } else {
                    setMove(MMID::SPIKE_SLIME_M_FLAME_TACKLE);
                }
            } else if (lastTwoMoves(MMID::SPIKE_SLIME_M_LICK) || (asc17 && lastMove(MMID::SPIKE_SLIME_M_LICK))) {
                setMove(MMID::SPIKE_SLIME_M_FLAME_TACKLE);
            } else {
                setMove(MMID::SPIKE_SLIME_M_LICK);
            }
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
            if (miscInfo & 0x4) {
                if (!lastMove(MonsterMoveId::THE_CHAMP_EXECUTE) && !lastMoveBefore(MonsterMoveId::THE_CHAMP_EXECUTE)) {
                    setMove(MonsterMoveId::THE_CHAMP_EXECUTE);
                    break;
                }

            } else {
                if (curHp < maxHp / 2) {
                    miscInfo |= 0x4;
                    setMove(MonsterMoveId::THE_CHAMP_ANGER);
                    break;

                } else if ((bc.getMonsterTurnNumber()+1) % 4 == 0) {
                    setMove(MonsterMoveId::THE_CHAMP_TAUNT);
                    break;
                }
            }


            // check if should use defensive stance
            const auto defensiveStanceUseCount = miscInfo & 0x3;
            const auto rollThreshold = asc19 ? 30 : 15;
            if (roll <= rollThreshold
                && !lastMove(MonsterMoveId::THE_CHAMP_DEFENSIVE_STANCE)
                && defensiveStanceUseCount < 2)
            {
                ++miscInfo;
                setMove(MonsterMoveId::THE_CHAMP_DEFENSIVE_STANCE);
                break;
            }

            if (roll <= 30 && !lastMove(MonsterMoveId::THE_CHAMP_GLOAT) && !lastMove(MonsterMoveId::THE_CHAMP_DEFENSIVE_STANCE)) {
                setMove(MonsterMoveId::THE_CHAMP_GLOAT);

            } else if (roll <= 55 && !lastMove(MonsterMoveId::THE_CHAMP_FACE_SLAP)) {
                setMove(MonsterMoveId::THE_CHAMP_FACE_SLAP);

            } else if (!lastMove(MonsterMoveId::THE_CHAMP_HEAVY_SLASH)) {
                setMove(MonsterMoveId::THE_CHAMP_HEAVY_SLASH);

            } else {
                setMove(MonsterMoveId::THE_CHAMP_FACE_SLAP);
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
                setMove(MonsterMoveId::THE_COLLECTOR_SPAWN);
                break;
            }

            // always uses mega debuff turn 4
            if (bc.getMonsterTurnNumber() == 3) {
                setMove(MonsterMoveId::THE_COLLECTOR_MEGA_DEBUFF);
                break;
            }

            const auto canUseSpawn = bc.monsters.monstersAlive < 3
                    && !lastMove(MonsterMoveId::THE_COLLECTOR_SPAWN);

            if (roll <= 25 && canUseSpawn) {
                setMove(MonsterMoveId::THE_COLLECTOR_SPAWN);
                break;
            }

            if (roll <= 70 && !lastTwoMoves(MonsterMoveId::THE_COLLECTOR_FIREBALL)) {
                setMove(MonsterMoveId::THE_COLLECTOR_FIREBALL);
                break;
            }

            if (lastMove(MonsterMoveId::THE_COLLECTOR_BUFF)) {
                setMove(MonsterMoveId::THE_COLLECTOR_FIREBALL);

            } else {
                setMove(MonsterMoveId::THE_COLLECTOR_BUFF);
            }
            break;
        }

        case MonsterId::THE_GUARDIAN: {
            setMove(MMID::THE_GUARDIAN_CHARGING_UP);
            break;
        }

        // just setting in collector spawn move
//        case MonsterId::TORCH_HEAD: {
//            setMove(MonsterMoveId::TORCH_HEAD_TACKLE);
//            break;
//        }

        default:
            break;
    }

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





