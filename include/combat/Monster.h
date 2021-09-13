//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_MONSTER_H
#define STS_LIGHTSPEED_MONSTER_H

#include <functional>
#include <random>
#include <map>
#include <game/Random.h>
#include <bitset>

#include "constants/MonsterStatusEffects.h"
#include "constants/MonsterIds.h"
#include "constants/MonsterMoves.h"


namespace sts {

    class BattleContext;

    struct Monster {

        /*
         * Things to possibly cache about a monster
         * - damage actions
         * - death actions
         * - whether they are a minion leader
         */

        int idx = -1;
        MonsterId id = MonsterId::INVALID;
        int curHp = 0;
        int maxHp = 0;

        bool isEscapingB = false;
//        bool isDyingB = false;
        bool halfDead = false;
        bool escapeNext = false;

        int block = 0;
        int strength = 0;

        std::uint64_t justAppliedBits = 0;
        std::uint64_t statusBits = 0;
        std::map<MonsterStatus, std::int16_t> statusMap;

        MMID moveHistory[2] = {MMID::INVALID, MMID::INVALID };

        // Shield Gremlin target
        // GreenLouse / RedLouse D
        // Red Slaver entangled turn
        // hexaghost divider damage
        // Gremlin wizard charge
        // book of stabbing n
        // champ phase2
        // bronze orb have used stasis
        // bronze automaton lastBoostWasFlail
        // spiker thorn buff count
        // writhing mass used implant
        // darkling d
        int miscInfo = 0;

        // Lagavulin asleep
        // Mugger/Looter ThieveryPower
        // Byrd FlightPower
        // Transient FadingPower
        // Time Eater TimeMazePower
        // Fungi Beast SporeCloudPower
        // Heart InvinciblePower
        // The Guardian ModeShiftPower
        // hexaghost orbCount
        int uniquePower0 = 0;

        // BeatOfDeathPower
        int uniquePower1 = 0;

        Monster() = default;
        Monster(const Monster& rhs) = default;

        void setRandomHp(Random &hpRng, bool higherHp);
        void initHp(Random &hpRng, int ascension);
        void construct(BattleContext &bc, MonsterId monsterId, int monsterIdx);
        void initSpawnedMonster(BattleContext &bc, MonsterId monsterId, int monsterIdx, int hp);

        void preBattleAction(BattleContext &bc);

        void applyStartOfTurnPowers(BattleContext &bc);
        void applyEndOfTurnTriggers(BattleContext &bc);
        void applyEndOfRoundPowers(BattleContext &bc);

        // ***********************

        int getStatusInternal(MonsterStatus s) const;

        template <MonsterStatus> void setHasStatus(bool value);
        template <MonsterStatus> void setStatus(int amount);
        template <MonsterStatus> void decrementStatus(int amount=1);
        template <MonsterStatus> void addDebuff(int amount, bool isSourceMonster=true);

        template <MonsterStatus> void removeStatus();
        template <MonsterStatus> void buff(int amount=1);


        template <MonsterStatus> void setJustApplied(bool value);
        template <MonsterStatus> [[nodiscard]] bool wasJustApplied() const;

        [[nodiscard]] bool isAlive() const;
        [[nodiscard]] bool isTargetable() const;
        [[nodiscard]] bool isDying() const;
        [[nodiscard]] bool isEscaping() const;
        [[nodiscard]] bool isDeadOrEscaped() const;
        [[nodiscard]] bool isHalfDead() const;
        [[nodiscard]] bool doesEscapeNext() const;
        [[nodiscard]] bool isAttacking() const;

        template <MonsterStatus> [[nodiscard]] bool hasStatus() const;
        template <MonsterStatus> [[nodiscard]] int getStatus() const;

        // ***********************

        void heal(int amount);
        void addBlock(int amount);
        void die(BattleContext &bc);
        void suicideAction(BattleContext &bc);

        void attackedUnblockedHelper(BattleContext &bc, int damage);
        void attacked(BattleContext &bc, int damage);
        void damageUnblockedHelper(BattleContext &bc, int damage);
        void damage(BattleContext &bc, int damage);
        void onHpLost(BattleContext &bc, int amount);
        void removeDebuffs();

        void resetAllStatusEffects();

        void setMove(MonsterMoveId moveId);
        [[nodiscard]] bool firstTurn() const; // only to be called in rollMove() before any moves are set
        [[nodiscard]] bool lastMove(MonsterMoveId moveId) const;
        [[nodiscard]] bool lastMoveBefore(MonsterMoveId moveId) const;
        [[nodiscard]] bool lastTwoMoves(MonsterMoveId moveId) const;
        [[nodiscard]] bool eitherLastTwo(MonsterMoveId moveId) const;

        void rollMove(BattleContext &bc);

        // todo make monsterData const as well and move logic to move actions
        [[nodiscard]] MMID getMoveForRoll(BattleContext &bc, int &monsterData, int roll) const;

        [[nodiscard]] int calculateDamageToPlayer(const BattleContext &bc, int baseDamage) const;
        void attackPlayerHelper(BattleContext &bc, int baseDamage, int times=1);
        void takeTurn(BattleContext &bc);

        // monster specific functions
        void stealGoldFromPlayer(BattleContext &bc, int amount);
        static void largeSlimeSplit(BattleContext &bc, MonsterId mediumSlimeType, int placeIdx, int hp);
        static void slimeBossSplit(BattleContext &bc, int hp);
        void spawnBronzeOrbs(BattleContext &bc); // Bronze Automaton
        void stasisAction(BattleContext &bc); // Bronze Orb
        void returnStasisCard(BattleContext &bc);
        void reptomancerSummon(BattleContext &bc);

        [[nodiscard]] static int getAliveGremlinCount(const BattleContext &bc);

    };

    std::ostream& operator<<(std::ostream &os, const sts::Monster &m);

    template <MonsterStatus s>
    void Monster::setHasStatus(bool value) {
        if (s == MS::STRENGTH) {
            return; // should not be called
        }

        if (value) {
            statusBits |= (1ULL << (int)s);
        } else {
            statusBits &= ~(1ULL << (int)s);
        }
    }

    template <MonsterStatus s>
    void Monster::setStatus(int amount) {
        if (s == MS::STRENGTH) {
            strength = amount;
            return;
        }

        if (amount == 0) {
            setHasStatus<s>(false);
        } else {
            setHasStatus<s>(true);
            statusMap[s] = amount;
        }
    }

    template <MonsterStatus s>
    void Monster::decrementStatus(int amount) {
        if (s == MS::STRENGTH) {
            strength -= amount;
            return;
        }

        if (hasStatus<s>()) {
            statusMap[s] -= amount;
            if (statusMap.at(s) == 0) {
                setHasStatus<s>(false);
            }
        } else {
            setHasStatus<s>(true);
            statusMap[s] = -amount;
        }
    }

    template <MonsterStatus s>
    void Monster::addDebuff(int amount, bool isSourceMonster) {
        if (isSourceMonster && (s == MS::WEAK || s == MS::VULNERABLE || s == MS::INTANGIBLE)) {
            setJustApplied<s>(true);
        }

        if (s == MonsterStatus::STRENGTH) {
            strength += amount;
            return;
        }

        if (hasStatus<s>()) {
            statusMap[s] += amount;
        } else {
            setHasStatus<s>(true);
            statusMap[s] = amount;
        }
    }

    template <MonsterStatus s>
    void Monster::setJustApplied(bool value) {
        if (value) {
            justAppliedBits |= 1ULL << (int) s;
        } else {
            justAppliedBits &= ~(1ULL << (int) s);
        }
    }

    template <MonsterStatus s>
    bool Monster::wasJustApplied() const {
        return (justAppliedBits >> (int) s) & 1;
    }

    template <MonsterStatus s>
    bool Monster::hasStatus() const {
        if (s == MS::STRENGTH) {
            return strength;
        }

        return statusBits & (1ULL << (int)s);
    }

    template <MonsterStatus s>
    int Monster::getStatus() const {
        if (s == MS::STRENGTH) {
            return strength;

        } else if (hasStatus<s>()) {
            return statusMap.at(s);

        } else {
            return 0;

        }
    }


    template <MonsterStatus s>
    void Monster::removeStatus() {
        static_assert(s != MonsterStatus::STRENGTH);
        setHasStatus<s>(false);
    }

    template <MonsterStatus s>
    void Monster::buff(int amount) {
        // boolean powers
        if (s == MonsterStatus::BARRICADE
            || s == MonsterStatus::MINION
            || s == MonsterStatus::REGROW
            || s == MonsterStatus::ASLEEP
            || s == MonsterStatus::REACTIVE
            || s == MonsterStatus::SHIFTING
            || s == MonsterStatus::STASIS
        ) {
            setHasStatus<s>(true);
            return;
        }

        if (s == MS::STRENGTH) {
            strength += amount;
            return;
        }

        if (s == MS::RITUAL) {
            setJustApplied<s>(true);
        }

        if (hasStatus<s>()) {
            statusMap[s] += amount;
        } else {
            setHasStatus<s>(true);
            statusMap[s] = amount;
        }
    }

}

#endif //STS_LIGHTSPEED_MONSTER_H
