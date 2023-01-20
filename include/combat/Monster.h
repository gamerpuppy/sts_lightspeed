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
#include <cassert>

#include "constants/MonsterStatusEffects.h"
#include "constants/MonsterIds.h"
#include "constants/MonsterMoves.h"


namespace sts {

    class BattleContext;

    struct DamageInfo {
        int damage = 0;
        int attackCount = 1;
    };

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
        int block = 0;

        bool isEscapingB = false;
//        bool isDyingB = false;
        bool halfDead = false;
        bool escapeNext = false;
        MMID moveHistory[2] = {MMID::INVALID, MMID::INVALID };

        std::uint64_t statusBits = 0;
        std::int8_t artifact = 0;
        std::int8_t blockReturn = 0;
        std::int8_t choked = 0;
        std::int8_t corpseExplosion = 0;
        std::int8_t lockOn = 0;
        std::int16_t mark = 0;
        std::int8_t metallicize = 0;
        std::int8_t platedArmor = 0;
        std::int8_t poison = 0;
        std::int8_t regen = 0;
        std::int8_t shackled = 0;
        int strength = 0;
        int vulnerable = 0;
        int weak = 0;

        int uniquePower0 = 0; // unique powers, hexaghost orbCount
        std::int16_t uniquePower1 = 0; // Corrupt Heart invincible,

        // Shield Gremlin target
        // GreenLouse / RedLouse D
        // hexaghost divider damage
        // Gremlin wizard charge
        // book of stabbing n
        // champ phase2
        // bronze orb have used stasis
        // bronze automaton lastBoostWasFlail
        // spiker thorn buff count
        // writhing mass used implant
        // darkling d
        // time eater has used haste
        // awakened one isPhase2
        int miscInfo = 0;

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

        [[nodiscard]] const char *getName() const;

        [[nodiscard]] bool hasStatusInternal(MonsterStatus s) const; // only to be used by printLogs methods
        [[nodiscard]] int getStatusInternal(MonsterStatus s) const; // only to be used by printLogs methods


        template <MonsterStatus> [[nodiscard]] bool hasStatus() const;
        template <MonsterStatus> [[nodiscard]] int getStatus() const;
        void setHasStatus(MonsterStatus s, bool value=true);
        template <MonsterStatus> void setHasStatus(bool value=true);
        void setStatus(MonsterStatus s, int amount);
        template <MonsterStatus> void setStatus(int amount);
        template <MonsterStatus> void decrementStatus(int amount=1);
        template <MonsterStatus> void addDebuff(int amount, bool isSourceMonster=true);
        template <MonsterStatus> void removeStatus();
        template <MonsterStatus> void buff(int amount=1);


        void setJustApplied(MonsterStatus s, bool value);
        template <MonsterStatus> void setJustApplied(bool value);
        bool wasJustApplied(MonsterStatus s) const;
        template <MonsterStatus> [[nodiscard]] bool wasJustApplied() const;

        [[nodiscard]] bool isAlive() const;
        [[nodiscard]] bool isTargetable() const;
        [[nodiscard]] bool isDying() const;
        [[nodiscard]] bool isEscaping() const;
        [[nodiscard]] bool isDeadOrEscaped() const;
        [[nodiscard]] bool isHalfDead() const;
        [[nodiscard]] bool doesEscapeNext() const;
        [[nodiscard]] bool isAttacking() const;

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
        [[nodiscard]] DamageInfo getMoveBaseDamage(const BattleContext &bc) const;

        // monster specific functions

        void stealGoldFromPlayer(BattleContext &bc, int amount);
        static void largeSlimeSplit(BattleContext &bc, MonsterId mediumSlimeType, int placeIdx, int hp);
        static void slimeBossSplit(BattleContext &bc, int hp);
        void spawnBronzeOrbs(BattleContext &bc); // Bronze Automaton
        void stasisAction(BattleContext &bc); // Bronze Orb
        void returnStasisCard(BattleContext &bc);
        void reptomancerSummon(BattleContext &bc, int daggerCount);

        [[nodiscard]] static int getAliveGremlinCount(const BattleContext &bc);

    };

    std::ostream& operator<<(std::ostream &os, const sts::Monster &m);

    template <MonsterStatus s>
    void Monster::setHasStatus(bool value) {
        setHasStatus(s, value);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
    template <MonsterStatus s>
    void Monster::setStatus(int amount) {
        setStatus(s, amount);
    }
        
#pragma clang diagnostic pop

    template <MonsterStatus s>
    void Monster::decrementStatus(int amount) {
        if (isBooleanPower(s)) {
            setHasStatus<s>(false);
            return;
        }

        if (s == MonsterStatus::STRENGTH) {
            strength -= amount;
            return;
        }

        if (static_cast<int>(s) <= static_cast<int>(MS::WEAK)) {
            const auto newAmount = getStatus<s>()-amount;
            setStatus<s>(newAmount);
            setHasStatus<s>(newAmount);

        } else if (static_cast<int>(s) <= static_cast<int>(MS::TIME_WARP)) {
            uniquePower0 -= amount;
            if (uniquePower0 == 0) {
                setHasStatus<s>(false);
            }

        } else {
            uniquePower1 -= amount;
            if (uniquePower1 == 0) {
                setHasStatus<s>(false);
            }
        }
    }

    template <MonsterStatus s>
    void Monster::addDebuff(int amount, bool isSourceMonster) {
        if (isSourceMonster && (s == MS::WEAK || s == MS::VULNERABLE)) {
            setJustApplied<s>(true);
        }

        switch (s) {
            case MonsterStatus::BLOCK_RETURN:
                blockReturn += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::CHOKED:
                choked += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::CORPSE_EXPLOSION:
                corpseExplosion += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::LOCK_ON:
                lockOn += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::MARK:
                mark += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::POISON:
                poison += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::STRENGTH:
                strength += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::VULNERABLE:
                vulnerable += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::WEAK:
                weak += amount;
                setHasStatus<s>(true);
                return;

            default:
                assert(false);
                break;
        }
    }

    template <MonsterStatus s>
    void Monster::setJustApplied(bool value) {
        setJustApplied(s, value);
    }

    template <MonsterStatus s>
    bool Monster::wasJustApplied() const {
        return wasJustApplied(s);
    }

    template <MonsterStatus s>
    bool Monster::hasStatus() const {
        return statusBits & (1ULL << (int)s);
    }

    template <MonsterStatus s>
    int Monster::getStatus() const {
        if (s == MonsterStatus::STRENGTH) {
            return strength;
        }

        if (isBooleanPower(s)) {
            return hasStatus<s>();
        }

        if (!hasStatus<s>()) {
            return 0;
        }

        switch (s) {
            case MonsterStatus::ARTIFACT:
                return artifact;

            case MonsterStatus::BLOCK_RETURN:
                return blockReturn;

            case MonsterStatus::CHOKED:
                return choked;

            case MonsterStatus::CORPSE_EXPLOSION:
                return corpseExplosion;

            case MonsterStatus::LOCK_ON:
                return lockOn;

            case MonsterStatus::MARK:
                return mark;

            case MonsterStatus::METALLICIZE:
                return metallicize;

            case MonsterStatus::PLATED_ARMOR:
                return platedArmor;

            case MonsterStatus::POISON:
                return poison;

            case MonsterStatus::REGEN:
                return regen;

            case MonsterStatus::SHACKLED:
                return shackled;

            case MonsterStatus::VULNERABLE:
                return vulnerable;

            case MonsterStatus::WEAK:
                return weak;

            case MonsterStatus::ANGRY:
            case MonsterStatus::BEAT_OF_DEATH:
            case MonsterStatus::CURIOSITY:
            case MonsterStatus::CURL_UP:
            case MonsterStatus::ENRAGE:
            case MonsterStatus::FADING:
            case MonsterStatus::FLIGHT:
            case MonsterStatus::GENERIC_STRENGTH_UP:
            case MonsterStatus::INTANGIBLE:
            case MonsterStatus::MALLEABLE:
            case MonsterStatus::MODE_SHIFT:
            case MonsterStatus::RITUAL:
            case MonsterStatus::SLOW:
            case MonsterStatus::SPORE_CLOUD:
            case MonsterStatus::THIEVERY:
            case MonsterStatus::THORNS:
            case MonsterStatus::TIME_WARP:
                return uniquePower0;

            case MonsterStatus::INVINCIBLE:
            case MonsterStatus::REACTIVE:
            case MonsterStatus::SHARP_HIDE:
                return uniquePower1;

            default:
                return 0;
        }
    }

    template <MonsterStatus s>
    void Monster::removeStatus() {
        static_assert(s != MonsterStatus::STRENGTH);
        if (hasStatus<s>()) {
            setStatus<s>(0);
            setHasStatus<s>(false);
        }
    }

    template <MonsterStatus s>
    void Monster::buff(int amount) {
        if (isBooleanPower(s)) {
            setHasStatus<s>(true);
            return;
        }

        switch (s) {
            case MonsterStatus::ARTIFACT:
                artifact += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::METALLICIZE:
                metallicize += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::PLATED_ARMOR:
                platedArmor += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::REGEN:
                regen += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::SHACKLED:
                shackled += amount;
                setHasStatus<s>(true);
                return;

            case MonsterStatus::STRENGTH:
                strength += amount;
                return;

            case MS::ANGRY:
            case MS::BEAT_OF_DEATH:
            case MS::CURIOSITY:
            case MS::CURL_UP:
            case MS::ENRAGE:
            case MS::FADING:
            case MS::FLIGHT:
            case MS::GENERIC_STRENGTH_UP:
            case MS::INTANGIBLE:
            case MS::MALLEABLE:
            case MS::MODE_SHIFT:
            case MS::SLOW:
            case MS::SPORE_CLOUD:
            case MS::THIEVERY:
            case MS::THORNS:
            case MS::TIME_WARP:
                setHasStatus<s>(true);
                uniquePower0 += amount;
                return;

            case MS::RITUAL:
                setJustApplied<s>(true);
                setHasStatus<s>(true);
                uniquePower0 += amount;
                return;

            case MonsterStatus::INVINCIBLE:
            case MonsterStatus::REACTIVE:
            case MonsterStatus::SHARP_HIDE:
                setHasStatus<s>(true);
                uniquePower1 += amount;
                return;
        }
    }

}

#endif //STS_LIGHTSPEED_MONSTER_H
