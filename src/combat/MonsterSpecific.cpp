//
// Created by gamerpuppy on 8/21/2021.
//

#include "combat/Monster.h"
#include "game/Random.h"
#include "combat/BattleContext.h"
#include "game/GameContext.h"

using namespace sts;

// returns 0, 1, 2 based on value compared to a and b
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
        case MonsterId::BOOK_OF_STABBING:    // game adds PainfulStabsPower
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
            uniquePower0 = asc17 ? 4 : 3; // FlightPower
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


    switch (moveHistory[0]) {

        case MonsterMoveId::ACID_SLIME_S_LICK:
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(1) );
            setMove(MonsterMoveId::ACID_SLIME_S_TACKLE);
            break;

        case MonsterMoveId::ACID_SLIME_S_TACKLE:
            attackPlayerHelper(bc, asc2 ? 4 : 3);
            setMove(MonsterMoveId::ACID_SLIME_S_LICK);
            break;

        case MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT:
            attackPlayerHelper(bc, asc2 ? 12 : 11);
            bc.addToBot( Actions::MakeTempCardInDiscard({CardId::SLIMED}, 2) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MonsterMoveId::ACID_SLIME_L_LICK:
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(2, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MonsterMoveId::ACID_SLIME_L_SPLIT:
            largeSlimeSplit(bc, MonsterId::ACID_SLIME_M, idx, curHp);
            break;

        case MonsterMoveId::ACID_SLIME_L_TACKLE:
            attackPlayerHelper(bc, asc2 ? 18 : 16);
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MonsterMoveId::BLUE_SLAVER_RAKE:
            // 4
            attackPlayerHelper(bc, asc2 ? 8 : 7);
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(asc17 ? 2 : 1, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::BLUE_SLAVER_STAB:
            // 1
            attackPlayerHelper(bc, asc2 ? 13 : 12);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::FAT_GREMLIN_SMASH: {
            attackPlayerHelper(bc, asc2 ? 5 : 4);
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(1, true));
            if (asc17) {
                bc.addToBot(Actions::DebuffPlayer<PS::FRAIL>(1, true));
            }

            if (doesEscapeNext()) {
                setMove(MonsterMoveId::GENERIC_ESCAPE_MOVE);
            } else {
                bc.addToBot(Actions::NoOpRollMove());
            }
            break;
        }

        case MonsterMoveId::MAD_GREMLIN_SCRATCH: {
            attackPlayerHelper(bc, asc2 ? 5 : 4);
            if (doesEscapeNext()) {
                setMove(MonsterMoveId::GENERIC_ESCAPE_MOVE);
            } else {
                bc.addToBot(Actions::NoOpRollMove());
            }
            break;
        }

        case MonsterMoveId::SNEAKY_GREMLIN_PUNCTURE: {
            attackPlayerHelper(bc, asc2 ? 10 : 9);
            if (doesEscapeNext()) {
                setMove(MonsterMoveId::GENERIC_ESCAPE_MOVE);
            }
            break;
        }

        case MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT:
            attackPlayerHelper(bc, asc2 ? 8 : 7);
            bc.addToBot(Actions::MakeTempCardInDiscard(CardId::SLIMED));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::ACID_SLIME_M_LICK:
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(1, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::ACID_SLIME_M_TACKLE:
            attackPlayerHelper(bc, asc2 ? 12 : 10);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::CULTIST_DARK_STRIKE:
            attackPlayerHelper(bc, 6);
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MonsterMoveId::CULTIST_INCANTATION: {
            const int ritualAmount[] = {3,4,5};
            buff<MS::RITUAL>(ritualAmount[hallwayIdx]);
            setMove(MonsterMoveId::CULTIST_DARK_STRIKE);
            bc.noOpRollMove();
            break;
        }

        case MonsterMoveId::FUNGI_BEAST_BITE:
            attackPlayerHelper(bc, 6);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::FUNGI_BEAST_GROW: {
            const int strengthBuff[] = {3, 4, 5};
            buff<MS::STRENGTH>(strengthBuff[hallwayIdx]);
            rollMove(bc);
            break;
        }

        case MonsterMoveId::GREEN_LOUSE_BITE:
            attackPlayerHelper(bc, miscInfo);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::GREEN_LOUSE_SPIT_WEB:
            bc.addToBot(Actions::DebuffPlayer<PS::WEAK>(2)); // isSourceMonster true
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::GREMLIN_NOB_BELLOW:
            buff<MS::ENRAGE>(asc18 ? 3 : 2);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::GREMLIN_NOB_RUSH:
            attackPlayerHelper(bc, asc3 ? 16 : 14);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::GREMLIN_NOB_SKULL_BASH:
            attackPlayerHelper(bc, asc3 ? 8 : 6);
            bc.addToBot(Actions::DebuffPlayer<PS::VULNERABLE>(2, true));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::GREMLIN_WIZARD_CHARGING: { // 2
            ++miscInfo;
            if (miscInfo == 3) {
                setMove(MonsterMoveId::GREMLIN_WIZARD_ULTIMATE_BLAST);
            }
            break;
        }

        case MonsterMoveId::GREMLIN_WIZARD_ULTIMATE_BLAST: { // 1
            attackPlayerHelper(bc, asc2 ? 30 : 25);
            if (!asc17) {
                miscInfo = 0; // gremlin wizard charge
                setMove(MonsterMoveId::GREMLIN_WIZARD_CHARGING);
            }
            break;
        }

        case MonsterMoveId::HEXAGHOST_ACTIVATE: {// 5
            miscInfo = bc.player.curHp / 12 + 1; // set divider damage
            setMove(MonsterMoveId::HEXAGHOST_DIVIDER);
            bc.noOpRollMove();
            break;
        }

        case MonsterMoveId::HEXAGHOST_DIVIDER: // 1
            attackPlayerHelper(bc, miscInfo, 6);
            uniquePower0 = 0;
            setMove(MonsterMoveId::HEXAGHOST_SEAR);
            bc.addToBot( Actions::NoOpRollMove() );
            break;

        case MonsterMoveId::HEXAGHOST_INFERNO: // 6
            attackPlayerHelper(bc, asc4 ? 3 : 2, 6);
            uniquePower0 = 0;
            setMove(MonsterMoveId::HEXAGHOST_SEAR);
            bc.addToBot( Actions::NoOpRollMove() );
            break;

        case MonsterMoveId::HEXAGHOST_INFLAME: // 3
            addBlock(12);
            buff<MS::STRENGTH>(asc19 ? 3 : 2);
            ++uniquePower0;
            setMove(MonsterMoveId::HEXAGHOST_TACKLE);
            bc.noOpRollMove();
            break;

        case MonsterMoveId::HEXAGHOST_SEAR: // 4
            attackPlayerHelper(bc, 6);
            bc.addToBot (
                    Actions::MakeTempCardInDiscard(CardInstance(CardId::BURN, bc.turn > 8), asc19 ? 2 : 1)
            );
            if (uniquePower0 == 0) {
                setMove(MonsterMoveId::HEXAGHOST_TACKLE);

            } else if (uniquePower0 == 2) {
                setMove(MonsterMoveId::HEXAGHOST_INFLAME);

            } else {
                setMove(MonsterMoveId::HEXAGHOST_INFERNO);
            }
            ++uniquePower0;
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MonsterMoveId::HEXAGHOST_TACKLE: // 2
            attackPlayerHelper(bc, asc4 ? 6 : 5, 2);
            setMove(MonsterMoveId::HEXAGHOST_SEAR);
            ++uniquePower0;
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MonsterMoveId::RED_LOUSE_BITE:
            attackPlayerHelper(bc, miscInfo);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::RED_LOUSE_GROW:
            buff<MS::STRENGTH>(asc17 ? 4 : 3);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::JAW_WORM_CHOMP:
            attackPlayerHelper(bc, asc2 ? 12 : 11);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::JAW_WORM_BELLOW: {
            const int strengthBuff[] = {3,4,5};
            buff<MS::STRENGTH>(strengthBuff[hallwayIdx]);
            bc.addToBot(Actions::MonsterGainBlock(idx, asc17 ? 9 : 6));
            bc.addToBot(Actions::RollMove(idx));
            break;
        }

        case MonsterMoveId::JAW_WORM_THRASH:
            attackPlayerHelper(bc, 7);
            bc.addToBot(Actions::MonsterGainBlock(idx, 5));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::LAGAVULIN_ATTACK:
            attackPlayerHelper(bc, asc3 ? 20 : 18);
            if (lastTwoMoves(MonsterMoveId::LAGAVULIN_ATTACK)) {
                setMove(MonsterMoveId::LAGAVULIN_SIPHON_SOUL);
            } else {
                setMove(MonsterMoveId::LAGAVULIN_ATTACK);
            }
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MonsterMoveId::LAGAVULIN_SIPHON_SOUL:
            Actions::DebuffPlayer<PS::DEXTERITY>(asc18 ? -2 : -1).actFunc(bc);
            Actions::DebuffPlayer<PS::STRENGTH>(asc18 ? -2 : -1).actFunc(bc);
            setMove(MonsterMoveId::LAGAVULIN_ATTACK);
            bc.noOpRollMove();
            break;

        case MonsterMoveId::LAGAVULIN_SLEEP:
            if (bc.turn == 2 || !hasStatus<MS::ASLEEP>()) {
                setMove(MonsterMoveId::LAGAVULIN_ATTACK);
            } else {
                setMove(MonsterMoveId::LAGAVULIN_SLEEP);
            }
            bc.noOpRollMove();
            break;

        case MonsterMoveId::LOOTER_ESCAPE: {
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

        case MonsterMoveId::LOOTER_LUNGE: // 4
            stealGoldFromPlayer(bc, asc17 ? 20 : 15);
            attackPlayerHelper(bc, asc2 ? 14 : 12);
            setMove(MonsterMoveId::LOOTER_SMOKE_BOMB);
            break;

        case MonsterMoveId::LOOTER_MUG: { // 1
            if (firstTurn()) {
                bc.aiRng.randomBoolean(0.6f); // for a dialog message in game
            }
            stealGoldFromPlayer(bc, asc17 ? 20 : 15);
            attackPlayerHelper(bc, asc2 ? 11 : 10);
            if (bc.getMonsterTurnCount() < 2) {
                setMove(MonsterMoveId::LOOTER_MUG);
            } else {
                const auto nextMove = bc.aiRng.randomBoolean(0.5f) ?
                                      MonsterMoveId::LOOTER_LUNGE : MonsterMoveId::LOOTER_SMOKE_BOMB;

                setMove(nextMove);
            }
            break;
        }

        case MonsterMoveId::LOOTER_SMOKE_BOMB: // 2
            addBlock(6);
            setMove(MonsterMoveId::LOOTER_ESCAPE);
            break;

        case MonsterMoveId::RED_SLAVER_ENTANGLE:
            bc.addToBot(Actions::DebuffPlayer<PS::ENTANGLED>(1));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::RED_SLAVER_SCRAPE:
            attackPlayerHelper(bc, asc2 ? 9 : 8);
            bc.addToBot(Actions::DebuffPlayer<PS::VULNERABLE>(asc17 ? 2 : 1));
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::RED_SLAVER_STAB:
            attackPlayerHelper(bc, asc2 ? 14 : 13);
            bc.addToBot(Actions::RollMove(idx));
            break;

        case MonsterMoveId::SENTRY_BEAM:
            attackPlayerHelper(bc, asc3 ? 10 : 9);
            setMove(MonsterMoveId::SENTRY_BOLT);
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MonsterMoveId::SENTRY_BOLT:
            bc.addToBot(Actions::MakeTempCardInDiscard({CardId::DAZED}, asc18 ? 3 : 2));
            setMove(MonsterMoveId::SENTRY_BEAM);
            bc.addToBot(Actions::NoOpRollMove());
            break;

        case MonsterMoveId::SHIELD_GREMLIN_PROTECT: {
            const int blockAmounts[] = {7,8,11};
            const int blockAmount = blockAmounts[getTriIdx(asc, 7, 17)];
            bc.addToBot( Actions::GainBlockRandomEnemy(idx, blockAmount) );
            if (bc.monsters.getAliveCount() <= 1)  {
                setMove(MonsterMoveId::SHIELD_GREMLIN_SHIELD_BASH);
            }
            break;
        }

        case MonsterMoveId::SHIELD_GREMLIN_SHIELD_BASH:
            attackPlayerHelper(bc, asc2 ? 8 : 6);
            break;

        case MonsterMoveId::SLIME_BOSS_GOOP_SPRAY:
            Actions::MakeTempCardInDiscard( {CardId::SLIMED}, asc19 ? 5 : 3).actFunc(bc);
            setMove(MonsterMoveId::SLIME_BOSS_PREPARING);
            break;

        case MonsterMoveId::SLIME_BOSS_PREPARING:
            setMove(MonsterMoveId::SLIME_BOSS_SLAM);
            break;

        case MonsterMoveId::SLIME_BOSS_SLAM:
            attackPlayerHelper(bc, asc4 ? 38 : 35);
            setMove(MonsterMoveId::SLIME_BOSS_GOOP_SPRAY); // the attack is executed after, which is critical
            break;

        case MonsterMoveId::SLIME_BOSS_SPLIT:
            slimeBossSplit(bc, curHp);
            break;

        case MonsterMoveId::SPIKE_SLIME_M_FLAME_TACKLE: {
            attackPlayerHelper(bc, asc2 ? 10 : 8);
            bc.addToBot(Actions::MakeTempCardInDiscard(CardInstance(CardId::SLIMED)));
            bc.addToBot(Actions::RollMove(idx));
            break;
        }

        case MonsterMoveId::SPIKE_SLIME_L_FLAME_TACKLE:
            attackPlayerHelper(bc, asc2 ? 18 : 16);
            bc.addToBot( Actions::MakeTempCardInDiscard({CardId::SLIMED}, 2) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MonsterMoveId::SPIKE_SLIME_L_LICK: // 4
            bc.addToBot( Actions::DebuffPlayer<PS::FRAIL>(asc17 ? 3 : 2, true) );
            bc.addToBot( Actions::RollMove(idx) );
            break;

        case MonsterMoveId::SPIKE_SLIME_L_SPLIT: // 3
            largeSlimeSplit(bc, MonsterId::SPIKE_SLIME_M, idx, curHp);
            break;

        case MonsterMoveId::SPIKE_SLIME_M_LICK: {
            bc.addToBot(Actions::DebuffPlayer<PS::FRAIL>(1));
            bc.addToBot(Actions::RollMove(idx));
            break;
        }

        case MonsterMoveId::SPIKE_SLIME_S_TACKLE: {
            attackPlayerHelper(bc, asc2 ? 6 : 5);
            bc.addToBot(Actions::NoOpRollMove());
            break;
        }

        case MonsterMoveId::THE_GUARDIAN_CHARGING_UP:
            addBlock(9);
            setMove(MonsterMoveId::THE_GUARDIAN_FIERCE_BASH);
            break;

        case MonsterMoveId::THE_GUARDIAN_DEFENSIVE_MODE:
            buff<MS::SHARP_HIDE>(asc19 ? 4 : 3);
            setMove(MonsterMoveId::THE_GUARDIAN_ROLL_ATTACK);
            break;

        case MonsterMoveId::THE_GUARDIAN_FIERCE_BASH:
            attackPlayerHelper(bc, asc4 ? 36 : 32);
            setMove(MonsterMoveId::THE_GUARDIAN_VENT_STEAM);
            break;

        case MonsterMoveId::THE_GUARDIAN_ROLL_ATTACK:
            attackPlayerHelper(bc, asc4 ? 10 : 9);
            setMove(MonsterMoveId::THE_GUARDIAN_TWIN_SLAM);
            break;

        case MonsterMoveId::THE_GUARDIAN_TWIN_SLAM:
            attackPlayerHelper(bc, 8, 2);
            removeStatus<MS::SHARP_HIDE>();
            miscInfo += 10;
            setMove(MonsterMoveId::THE_GUARDIAN_WHIRLWIND);
            bc.addToBot( Actions::BuffEnemy<MS::MODE_SHIFT>(idx, miscInfo) );
            break;

        case MonsterMoveId::THE_GUARDIAN_VENT_STEAM:
            bc.addToBot( Actions::DebuffPlayer<PS::VULNERABLE>(2, true) );
            bc.addToBot( Actions::DebuffPlayer<PS::WEAK>(2, true) );
            setMove(MonsterMoveId::THE_GUARDIAN_WHIRLWIND);
            break;

        case MonsterMoveId::THE_GUARDIAN_WHIRLWIND:
            attackPlayerHelper(bc, 5, 4);
            setMove(MonsterMoveId::THE_GUARDIAN_CHARGING_UP);
            break;


        case MonsterMoveId::INVALID:
#ifdef sts_asserts
            assert(false);
#endif
        case MonsterMoveId::GENERIC_ESCAPE_MOVE:
        default:
            break;
    }
}

void Monster::setMoveFromRoll(BattleContext &bc, int roll) {
    bool asc17 = bc.ascension >= 17;
    bool asc18 = bc.ascension >= 18;

    switch (id) {

        case MonsterId::ACID_SLIME_S: {
            if (asc17) {
                setMove(MonsterMoveId::ACID_SLIME_S_LICK);
            } else if (bc.aiRng.randomBoolean()) {
                setMove(MonsterMoveId::ACID_SLIME_S_TACKLE);
            } else {
                setMove(MonsterMoveId::ACID_SLIME_S_LICK);
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
                    if (lastTwoMoves(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT)) {
                        if (bc.aiRng.randomBoolean()) {
                            setMove(MonsterMoveId::ACID_SLIME_M_TACKLE);
                        } else {
                            setMove(MonsterMoveId::ACID_SLIME_M_LICK);
                        }
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT);
                    }

                } else if (roll < 80) {
                    if (lastTwoMoves(MonsterMoveId::ACID_SLIME_M_TACKLE)) {
                        if (bc.aiRng.randomBoolean(0.5f)) {
                            setMove(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT);
                        } else {
                            setMove(MonsterMoveId::ACID_SLIME_M_LICK);
                        }
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_M_TACKLE);
                    }

                } else if (lastMove(MonsterMoveId::ACID_SLIME_M_LICK)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        setMove(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT);
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_M_TACKLE);
                    }
                } else {
                    setMove(MonsterMoveId::ACID_SLIME_M_LICK);
                }
                // END ASCENSION 17

            } else if (roll < 30) {
                if (lastTwoMoves(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT)) {
                    if (bc.aiRng.randomBoolean()) {
                        setMove(MonsterMoveId::ACID_SLIME_M_TACKLE);
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_M_LICK);
                    }
                } else {
                    setMove(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT);
                }

            } else if (roll < 70) {
                if (lastMove(MonsterMoveId::ACID_SLIME_M_TACKLE)) {
                    if (bc.aiRng.randomBoolean(0.4f)) {
                        setMove(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT);
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_M_LICK);
                    }
                } else {
                    setMove(MonsterMoveId::ACID_SLIME_M_TACKLE);
                }

            } else if (lastTwoMoves(MonsterMoveId::ACID_SLIME_M_LICK)) {
                if (bc.aiRng.randomBoolean(0.4F)) {
                    setMove(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT);
                } else {
                    setMove(MonsterMoveId::ACID_SLIME_M_TACKLE);
                }
            } else {
                setMove(MonsterMoveId::ACID_SLIME_M_LICK);
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
                    if (lastTwoMoves(MonsterMoveId::ACID_SLIME_M_CORROSIVE_SPIT)) {
                        if (bc.aiRng.randomBoolean(0.6F)) {
                            setMove(MonsterMoveId::ACID_SLIME_L_TACKLE);
                        } else {
                            setMove(MonsterMoveId::ACID_SLIME_L_LICK);
                        }
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT);
                    }

                } else if (roll < 70) {
                    if (lastTwoMoves(MonsterMoveId::ACID_SLIME_L_TACKLE)) {
                        if (bc.aiRng.randomBoolean(0.6F)) {
                            setMove(MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT);
                        } else {
                            setMove(MonsterMoveId::ACID_SLIME_L_LICK);
                        }
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_L_TACKLE);
                    }
                } else if (lastMove(MonsterMoveId::ACID_SLIME_L_LICK)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        setMove(MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT);
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_L_TACKLE);
                    }
                } else {
                    setMove(MonsterMoveId::ACID_SLIME_L_LICK);
                }

            } else if (roll < 30) {
                if (lastTwoMoves(MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT)) {
                    if (bc.aiRng.randomBoolean()) {
                        setMove(MonsterMoveId::ACID_SLIME_L_TACKLE);
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_L_LICK);
                    }
                } else {
                    setMove(MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT);
                }
            } else if (roll < 70) {
                if (lastMove(MonsterMoveId::ACID_SLIME_L_TACKLE)) {
                    if (bc.aiRng.randomBoolean(0.4F)) {
                        setMove(MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT);
                    } else {
                        setMove(MonsterMoveId::ACID_SLIME_L_LICK);
                    }
                } else {
                    setMove(MonsterMoveId::ACID_SLIME_L_TACKLE);
                }
            } else if (lastTwoMoves(MonsterMoveId::ACID_SLIME_L_LICK)) {
                if (bc.aiRng.randomBoolean(0.4F)) {
                    setMove(MonsterMoveId::ACID_SLIME_L_CORROSIVE_SPIT);
                } else {
                    setMove(MonsterMoveId::ACID_SLIME_L_TACKLE);
                }
            } else {
                setMove(MonsterMoveId::ACID_SLIME_L_LICK);
            }
            break;
        }

        case MonsterId::BLUE_SLAVER: {
            if (roll >= 40 && !lastTwoMoves(MonsterMoveId::BLUE_SLAVER_STAB)) {
                setMove(MonsterMoveId::BLUE_SLAVER_STAB);

            } else if ( !lastTwoMoves(MonsterMoveId::BLUE_SLAVER_RAKE) ||
                (asc17 && !lastMove(MonsterMoveId::BLUE_SLAVER_RAKE)) ) {
                setMove(MonsterMoveId::BLUE_SLAVER_RAKE);

            } else {
                setMove(MonsterMoveId::BLUE_SLAVER_STAB);
            }
            break;
        }

        case MonsterId::CULTIST: {
            if (lastMove(MonsterMoveId::INVALID)) {
                setMove(MonsterMoveId::CULTIST_INCANTATION);
            } else {
                setMove(MonsterMoveId::CULTIST_DARK_STRIKE);
            }
            break;
        }

        case MonsterId::FAT_GREMLIN: {
            setMove(MonsterMoveId::FAT_GREMLIN_SMASH);
            break;
        }

        case MonsterId::FUNGI_BEAST: {
            // 1 FUNGI_BEAST_BITE
            // 2 FUNGI_BEAST_GROW
            if (roll < 60) {
                if (lastTwoMoves(MonsterMoveId::FUNGI_BEAST_BITE)) {
                    setMove(MonsterMoveId::FUNGI_BEAST_GROW);
                } else {
                    setMove(MonsterMoveId::FUNGI_BEAST_BITE);
                }

            } else if (lastMove(MonsterMoveId::FUNGI_BEAST_GROW)) {
                setMove(MonsterMoveId::FUNGI_BEAST_BITE);

            } else {
                setMove(MonsterMoveId::FUNGI_BEAST_GROW);
            }
            break;
        }

        case MonsterId::GREMLIN_NOB: {
            // 1 Rush
            // 2 Skull Bash
            // 3 BELLOW

            if (lastMove(MonsterMoveId::INVALID)) {
                setMove(MonsterMoveId::GREMLIN_NOB_BELLOW);
                break;
            }

            if (asc18) {
                if (!lastTwoMoves(MonsterMoveId::GREMLIN_NOB_SKULL_BASH)) {
                    setMove(MonsterMoveId::GREMLIN_NOB_RUSH);
                    break;
                }

                if (lastTwoMoves(MonsterMoveId(MonsterMoveId::GREMLIN_NOB_RUSH))) {
                    setMove(MonsterMoveId::GREMLIN_NOB_SKULL_BASH);

                } else {
                    setMove(MonsterMoveId::GREMLIN_NOB_RUSH);
                }
                break;
            }

            if (roll < 33 || lastTwoMoves(MonsterMoveId::GREMLIN_NOB_RUSH)) {
                setMove(MonsterMoveId::GREMLIN_NOB_SKULL_BASH);

            } else {
                setMove(MonsterMoveId::GREMLIN_NOB_RUSH);
            }
            break;
        }

        case MonsterId::GREMLIN_WIZARD: {
            miscInfo = 1; // gremlin wizard charge
            setMove(MonsterMoveId::GREMLIN_WIZARD_CHARGING);
            break;
        }

        case MonsterId::GREEN_LOUSE: {
            if (roll < 25) {
                if (lastMove(MonsterMoveId::GREEN_LOUSE_SPIT_WEB) && (asc17 || lastTwoMoves(MonsterMoveId::GREEN_LOUSE_SPIT_WEB))) {
                    setMove(MonsterMoveId::GREEN_LOUSE_BITE);
                } else  {
                    setMove(MonsterMoveId::GREEN_LOUSE_SPIT_WEB);
                }

            } else if (lastTwoMoves(MonsterMoveId::GREEN_LOUSE_BITE)) {
                setMove(MonsterMoveId::GREEN_LOUSE_SPIT_WEB);

            } else {
                setMove(MonsterMoveId::GREEN_LOUSE_BITE);
            }
            break;
        }

        case MonsterId::HEXAGHOST: {
#ifdef sts_asserts
            assert(firstTurn());
#endif
            setMove(MonsterMoveId::HEXAGHOST_ACTIVATE);
            break;
        }

        case MonsterId::JAW_WORM: {
            if (firstTurn()) {
                setMove(MonsterMoveId::JAW_WORM_CHOMP);
                break;
            }

            if (roll < 25) {
                if (lastMove(MonsterMoveId::JAW_WORM_CHOMP)) {
                    if (bc.aiRng.randomBoolean(0.5625f)) {
                        setMove(MonsterMoveId::JAW_WORM_BELLOW);
                    } else {
                        setMove(MonsterMoveId::JAW_WORM_THRASH);
                    }

                } else {
                    setMove(MonsterMoveId::JAW_WORM_CHOMP);
                }

            } else if (roll < 55) {
                if (lastTwoMoves(MonsterMoveId::JAW_WORM_THRASH)) {
                    if (bc.aiRng.randomBoolean(0.357f)) {
                        setMove(MonsterMoveId::JAW_WORM_CHOMP);
                    } else {
                        setMove(MonsterMoveId::JAW_WORM_BELLOW);
                    }

                } else {
                    setMove(MonsterMoveId::JAW_WORM_THRASH);
                }

            } else if (lastMove(MonsterMoveId::JAW_WORM_BELLOW)) {
                if (bc.aiRng.randomBoolean(0.416f)) {
                    setMove(MonsterMoveId::JAW_WORM_CHOMP);
                } else {
                    setMove(MonsterMoveId::JAW_WORM_THRASH);
                }

            } else {
                setMove(MonsterMoveId::JAW_WORM_BELLOW);
            }
            break;
        }

        case MonsterId::LAGAVULIN: {
            if (hasStatus<MS::ASLEEP>()) {
                setMove(MonsterMoveId::LAGAVULIN_SLEEP);
            } else {
                setMove(MonsterMoveId::LAGAVULIN_SIPHON_SOUL);
            }
            break;
        }

        case MonsterId::LOOTER: {
#ifdef sts_asserts
            assert(firstTurn());
#endif
            setMove(MonsterMoveId::LOOTER_MUG);
            break;
        }

        case MonsterId::MAD_GREMLIN: {
            setMove(MonsterMoveId::MAD_GREMLIN_SCRATCH);
            break;
        }

        case MonsterId::RED_LOUSE: {
            if (roll < 25) {
                if (lastMove(MonsterMoveId::RED_LOUSE_GROW) && (asc17 || lastTwoMoves(MonsterMoveId::RED_LOUSE_GROW))) {
                    setMove(MonsterMoveId::RED_LOUSE_BITE);
                } else  {
                    setMove(MonsterMoveId::RED_LOUSE_GROW);
                }

            } else if (lastTwoMoves(MonsterMoveId::RED_LOUSE_BITE)) {
                setMove(MonsterMoveId::RED_LOUSE_GROW);

            } else {
                setMove(MonsterMoveId::RED_LOUSE_BITE);
            }
            break;
        }

        case MonsterId::SHIELD_GREMLIN: {
            setMove(MonsterMoveId::SHIELD_GREMLIN_PROTECT);
            break;
        }

        case MonsterId::SLIME_BOSS: {
            // 1 SLIME_BOSS_PREPARING
            // 2 SLIME_BOSS_SLAM
            // 3 SLIME_BOSS_SPLIT
            // 4 SLIME_BOSS_GOOP_SPRAY
            if (firstTurn()) {
                setMove(MonsterMoveId::SLIME_BOSS_GOOP_SPRAY);
            }
            break;
        }

        case MonsterId::SNEAKY_GREMLIN:
            setMove(MonsterMoveId::SNEAKY_GREMLIN_PUNCTURE);
            break;

        case MonsterId::SPIKE_SLIME_S:
            setMove(MonsterMoveId::SPIKE_SLIME_S_TACKLE);
            break;

        case MonsterId::RED_SLAVER: {
            // 1 Stab
            // 2 Entangle
            // 3 Scrape
            const bool usedEntangle = miscInfo;

            if (lastMove(MonsterMoveId::INVALID)) {
                setMove(MonsterMoveId::RED_SLAVER_STAB);

            } else if (roll >= 75 && !usedEntangle) {
                setMove(MonsterMoveId::RED_SLAVER_ENTANGLE);

            } else if (roll >= 50 && usedEntangle && !lastTwoMoves(MonsterMoveId::RED_SLAVER_STAB)) {
                setMove(MonsterMoveId::RED_SLAVER_STAB);

            } else if (!lastTwoMoves(MonsterMoveId::RED_SLAVER_SCRAPE) || (asc17 && !lastMove(MonsterMoveId::RED_SLAVER_SCRAPE))) {
                setMove(MonsterMoveId::RED_SLAVER_SCRAPE);

            } else {
                setMove(MonsterMoveId::RED_SLAVER_STAB);

            }
            break;
        }

        case MonsterId::SENTRY: {
            if (firstTurn()) {
                if (idx % 2 == 0) {
                    setMove(MonsterMoveId::SENTRY_BOLT);
                } else {
                    setMove(MonsterMoveId::SENTRY_BEAM);
                }
            }
            break;
        }

        case MonsterId::SPIKE_SLIME_L: {
            // 1 flame tackle
            // 3 split
            // 4 lick
           if (roll < 30) {
                if (lastTwoMoves(MonsterMoveId::SPIKE_SLIME_L_FLAME_TACKLE)) {
                    setMove(MonsterMoveId::SPIKE_SLIME_L_LICK);
                } else {
                    setMove(MonsterMoveId::SPIKE_SLIME_L_FLAME_TACKLE);
                }

            } else if (lastTwoMoves(MonsterMoveId::SPIKE_SLIME_L_LICK) || (asc17 && lastMove(MonsterMoveId::SPIKE_SLIME_L_LICK)) ) {
                setMove(MonsterMoveId::SPIKE_SLIME_L_FLAME_TACKLE);

            } else {
                setMove(MonsterMoveId::SPIKE_SLIME_L_LICK);

            }
            break;
        }

        case MonsterId::SPIKE_SLIME_M: {
            // 1 SPIKE_SLIME_M_FLAME_TACKLE
            // 4 SPIKE_SLIME_M_LICK
            if (roll < 30) {
                if (lastTwoMoves(MonsterMoveId::SPIKE_SLIME_M_FLAME_TACKLE)) {
                    setMove(MonsterMoveId::SPIKE_SLIME_M_LICK);
                } else {
                    setMove(MonsterMoveId::SPIKE_SLIME_M_FLAME_TACKLE);
                }
            } else if (lastTwoMoves(MonsterMoveId::SPIKE_SLIME_M_LICK) || (asc17 && lastMove(MonsterMoveId::SPIKE_SLIME_M_LICK))) {
                setMove(MonsterMoveId::SPIKE_SLIME_M_FLAME_TACKLE);
            } else {
                setMove(MonsterMoveId::SPIKE_SLIME_M_LICK);
            }
            break;
        }

        case MonsterId::THE_GUARDIAN: {
            setMove(MonsterMoveId::THE_GUARDIAN_CHARGING_UP);
            break;
        }


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
    const auto theftAmount = std::min(bc.player.gold, amount);
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

    bc.monsters.monsterCount = 3;
    bc.monsters.monstersAlive = 2;
    bc.monsterTurnIdx = 3;
}







