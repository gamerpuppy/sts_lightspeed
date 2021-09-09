//
// Created by gamerpuppy on 7/5/2021.
//

#include "combat/Monster.h"

#include "game/Random.h"
#include "combat/BattleContext.h"
#include "game/GameContext.h"

using namespace sts;

MonsterAction::MonsterAction(MonsterMoveId id) : id(id), prob(0) { }

MonsterAction::MonsterAction(MonsterMoveId id, double prob) : id(id), prob(prob) { }

void Monster::setRandomHp(Random &hpRng, bool higherHp) {
    auto hpRange = monsterHpRange[static_cast<int>(id)][higherHp ? 1 : 0];
    curHp = hpRng.random(hpRange[0], hpRange[1]);
    maxHp = curHp;
}

void Monster::applyStartOfTurnPowers(BattleContext &bc) {
    if (!hasStatus<MS::BARRICADE>()) {
        block = 0;
    }

    if (hasStatus<MS::CHOKED>()) {
        setHasStatus<MS::CHOKED>(false);
    }

    if (hasStatus<MS::FLIGHT>()) {
        setStatus<MS::FLIGHT>(bc.ascension >= 17 ? 4 : 3);
    }

    if (hasStatus<MS::INVINCIBLE>()) {
        setStatus<MS::INVINCIBLE>(bc.ascension >= 19 ? 200 : 300);
    }

    if (hasStatus<MS::POISON>() && !bc.monsters.areMonstersBasicallyDead()) {
        bc.addToBot(Actions::PoisonLoseHpAction());
    }
}

void Monster::applyTurnPowers(BattleContext &bc) {
    // Monster Powers duringTurn()
    if (getStatus<MS::EXPLOSIVE>() == 1 && !isDying()) {
        bc.addToBot(Actions::SuicideAction(idx, true));
    }

    if (getStatus<MS::FADING>()) {
        bc.addToBot(Actions::SuicideAction(idx, true));
    }
}

void Monster::applyEndOfTurnTriggers(BattleContext &bc) {
    // Monster Powers atEndOfTurnPreEndTurnCards and atEndOfTurn
    if (hasStatus<MS::METALLICIZE>()) {
        addBlock(getStatus<MS::METALLICIZE>());
    }

    if (hasStatus<MS::PLATED_ARMOR>()) {
        addBlock(getStatus<MS::PLATED_ARMOR>());
    }

    if (getStatus<MS::INTANGIBLE>()) {
        if (wasJustApplied<MS::INTANGIBLE>()) {
            setJustApplied<MS::INTANGIBLE>(false);
        } else {
            decrementStatus<MS::INTANGIBLE>();
        }
    }

    if (hasStatus<MS::END_OF_TURN_GAIN_STRENGTH>()) {
        buff<MS::STRENGTH>(getStatus<MS::END_OF_TURN_GAIN_STRENGTH>());
        removeStatus<MS::END_OF_TURN_GAIN_STRENGTH>();
    }
}

void Monster::applyEndOfRoundPowers(BattleContext &bc) {
    // Monster Powers atEndOfRound
    if (hasStatus<MS::RITUAL>()) { // sort of a hack, RitualPower contains a skipFirst boolean
        if (wasJustApplied<MS::RITUAL>()) {
            setJustApplied<MS::RITUAL>(false);
        } else {
            buff<MS::STRENGTH>(getStatus<MS::RITUAL>());
        }
    }

    if (hasStatus<MS::SLOW>()) { // todo make this not remove it.
        setStatus<MS::SLOW>(0);
        setHasStatus<MS::SLOW>(true);
    }

    if (hasStatus<MS::LOCK_ON>()) {
        decrementStatus<MS::LOCK_ON>();
    }

    if (hasStatus<MS::WEAK>()) {
        if (wasJustApplied<MS::WEAK>()) {
            setJustApplied<MS::WEAK>(false);
        } else {
            decrementStatus<MS::WEAK>();
        }
    }

    if (hasStatus<MS::VULNERABLE>()) {
        if (wasJustApplied<MS::VULNERABLE>()) {
            setJustApplied<MS::VULNERABLE>(false);
        } else {
            decrementStatus<MS::VULNERABLE>();
        }
    }

}

void Monster::construct(BattleContext &bc, MonsterId monsterId, int monsterIdx) {
    this->id = monsterId;
    this->idx = monsterIdx;
    initHp(bc.monsterHpRng, bc.ascension);

    switch (id) {
        case MonsterId::GREEN_LOUSE:
        case MonsterId::RED_LOUSE:
            if (bc.ascension >= 2) {
                miscInfo = bc.monsterHpRng.random(6, 8);
            } else {
                miscInfo = bc.monsterHpRng.random(5, 7);
            }
            break;

        case MonsterId::DARKLING:
            if (bc.ascension >= 2) {
                miscInfo = bc.monsterHpRng.random(9, 13);
            } else {
                miscInfo = bc.monsterHpRng.random(7, 11);
            }
            break;

        default:
            break;
    }
}

int Monster::getStatusInternal(MonsterStatus s) const {
    if (s == MS::STRENGTH) {
        return strength;

    } else if (statusBits & (1ULL << (int)s)) {
        return statusMap.at(s);

    } else {
        return 0;

    }
}

bool Monster::isAlive() const {
    return curHp > 0;
}

bool Monster::isTargetable() const {
    return !isDeadOrEscaped();
}

bool Monster::isDying() const {
    return curHp <= 0;
}

bool Monster::isEscaping() const {
    return isEscapingB;
}

bool Monster::isDeadOrEscaped() const {
    return isDying() || isHalfDead() || isEscaping();
}

bool Monster::isHalfDead() const {
    return halfDead;
}

bool Monster::doesEscapeNext() const {
    return escapeNext;
}

bool Monster::isAttacking() const {
    return isMoveAttack(moveHistory[0]);
}

void Monster::addBlock(int amount) {
    block += amount;
}

void Monster::die(BattleContext &bc, bool triggerRelics) {
    //todo implement trigger relics
    --bc.monsters.monstersAlive;
    if (!hasStatus<MS::MINION>()) {
        const bool isMinionLeader =
            id == MonsterId::BRONZE_AUTOMATON ||
            id == MonsterId::GREMLIN_LEADER ||
            id == MonsterId::REPTOMANCER ||
            id == MonsterId::THE_COLLECTOR;
        if (bc.monsters.monstersAlive == 0 || isMinionLeader)
        {
            bc.cleanCardQueue();
            bc.outcome = Outcome::PLAYER_VICTORY;
            return;
        }
    }

    if (hasStatus<MS::SPORE_CLOUD>()) {
        // spore cloud always has a value of 2 in game
        bc.addToTop( Actions::DebuffPlayer<PS::VULNERABLE>(2, bc.turnHasEnded) );
    }

    if (hasStatus<MS::CORPSE_EXPLOSION>()) {
        int damage = maxHp * getStatus<MS::CORPSE_EXPLOSION>();
        bc.addToBot( Actions::DamageAllEnemy(damage) );
    }

    if (hasStatus<MS::STASIS>()) {
        // todo
    }

    if (bc.player.hasRelic<RelicId::GREMLIN_HORN>()) {
        bc.addToBot( Actions::GainEnergy(1) );
    }

    if (bc.player.hasRelic<RelicId::THE_SPECIMEN>()) {
        bc.addToBot( Actions::SetState(InputState::SELECT_ENEMY_THE_SPECIMEN_APPLY_POISON) );
    }

    // todo exhaust cards in limbo
}

void Monster::attackedUnblockedHelper(BattleContext &bc, int damage) { // todo, is this supposed to be only damage > 0 ????
    if (bc.player.hasRelic<RelicId::THE_BOOT>() && damage > 0 && damage < 5) {
        damage = 5;
    }

    if (hasStatus<MS::PLATED_ARMOR>()) {
        decrementStatus<MS::PLATED_ARMOR>();
    }

    if (bc.player.hasStatus<PS::ENVENOM>()) {
        bc.addToTop( Actions::DebuffEnemy<MS::POISON>(this->idx, bc.player.getStatus<PS::ENVENOM>()) );
    }

    if (hasStatus<MS::CURL_UP>()) {
        bc.addToBot(Actions::MonsterGainBlock(this->idx, getStatus<MS::CURL_UP>()) );
        setHasStatus<MS::CURL_UP>(false);
    }

    if (hasStatus<MS::FLIGHT>() && damage > 0) {
        auto flight = getStatus<MS::FLIGHT>();
        if (flight == 1) {
            setMove(MonsterMoveId::BYRD_STUNNED);
        }
        setStatus<MS::FLIGHT>(flight-1);
        // todo add to bot change enemy action to grounded?
    }

    if (hasStatus<MS::MALLEABLE>()) {
        const auto malleable = getStatus<MS::MALLEABLE>();
        bc.addToBot( Actions::MonsterGainBlock(this->idx, malleable) );
        setStatus<MS::MALLEABLE>(malleable+1);
    }

    if (hasStatus<MS::SHIFTING>()) {
        // todo addToBot roll move
    }

    if (hasStatus<MS::THORNS>()) {
        bc.addToTop(Actions::DamagePlayer(getStatus<MS::THORNS>()) );
    }

    if (hasStatus<MS::ASLEEP>()) {
        // lagavulin
        setHasStatus<MS::ASLEEP>(false);
        decrementStatus<MS::METALLICIZE>(8);
    }

    lastAttackInfo.unblockedDamage += std::min(curHp, damage);
    curHp -= damage;
    if (curHp <= 0) {
        curHp = 0;
        lastAttackInfo.fatal = true;
        die(bc);
    } else {
        onHpLost(bc, damage);
    }
}

void Monster::attacked(BattleContext &bc, int damage) {
    if (isDeadOrEscaped()) { // these probably shouldn't happen
        return;
    }

    if (damage < 0) {// these probably shouldn't happen
        damage = 0;
    }

    if (hasStatus<MS::INTANGIBLE>()) {
        if (damage > 0) {
            damage = 1;
        }
    }

    if (hasStatus<MS::ANGRY>()) { // onAttacked
        buff<MS::STRENGTH>(getStatus<MS::ANGRY>());
    }

    if (hasStatus<MS::INVINCIBLE>()) {
        auto amount = getStatus<MS::INVINCIBLE>();
        int diff = amount - damage;
        if (diff >= 0) {
            decrementStatus<MS::INVINCIBLE>(damage);
        } else {
            damage += diff;
            setStatus<MS::INVINCIBLE>(0);
        }
    }

    const bool hadBlock = block > 0;
    const int tempDamage = damage;
    damage -= block;
    block = std::max(0, block - tempDamage);

    if (hadBlock && block == 0 && bc.player.hasRelic<RelicId::HAND_DRILL>()) {
        bc.addToBot(Actions::DebuffEnemy<MS::VULNERABLE>(idx, 2, false) );
    }

    if (damage > 0) { // todo can damage be zero???
        attackedUnblockedHelper(bc, damage);
    }
}

void Monster::damageUnblockedHelper(BattleContext &bc, int damage) {
    if (hasStatus<MS::PLATED_ARMOR>()) {
        decrementStatus<MS::PLATED_ARMOR>();
    }

    if (hasStatus<MS::ASLEEP>()) {
        // lagavulin
        setHasStatus<MS::ASLEEP>(false);
        decrementStatus<MS::METALLICIZE>(8);
    }

    curHp = std::max(0, curHp-damage);
    if (curHp == 0) {
        die(bc);
    } else {
        onHpLost(bc, damage);
    }
}

void Monster::damage(BattleContext &bc, int damage) {
    if (isDeadOrEscaped()) {
        return;
    }

    if (damage < 0) {
        damage = 0;
    }


    if (hasStatus<MS::INTANGIBLE>()) { // this is probably wrong with potions
        if (damage > 0) {
            damage = 1;
        }
    }

    if (hasStatus<MS::INVINCIBLE>()) {
        auto amount = getStatus<MS::INVINCIBLE>();
        int diff = amount - damage;
        if (diff >= 0) {
            decrementStatus<MS::INVINCIBLE>(damage);
        } else {
            damage += diff;
            setStatus<MS::INVINCIBLE>(0);
        }
    }

    const bool hadBlock = block > 0;
    const int tempDamage = damage;
    damage -= block;
    block = std::max(0, block-tempDamage);

    if (hadBlock && block == 0 && bc.player.hasRelic<RelicId::HAND_DRILL>()) {
        bc.addToBot(Actions::DebuffEnemy<MS::VULNERABLE>(idx, 2, false) );
    }

    if (damage > 0) {
        damageUnblockedHelper(bc, damage);
    } else {
        return;
    }
}

void Monster::onHpLost(BattleContext &bc, int amount) {
    const bool atOrBelowHalf = curHp <= maxHp/2;
    switch (id) {
        case MonsterId::ACID_SLIME_L:
            if (atOrBelowHalf) {
                moveHistory[0] = MonsterMoveId::ACID_SLIME_L_SPLIT;
            }
            break;

        case MonsterId::SLIME_BOSS:
            if (atOrBelowHalf) {
                moveHistory[0] = MonsterMoveId::SLIME_BOSS_SPLIT;
            }
            break;

        case MonsterId::SPIKE_SLIME_L:
            if (atOrBelowHalf) {
                moveHistory[0] = MonsterMoveId::SPIKE_SLIME_L_SPLIT;
            }
            break;

        case MonsterId::THE_GUARDIAN:
            if (hasStatus<MS::MODE_SHIFT>()) {
                const int newModeShiftAmount = getStatus<MS::MODE_SHIFT>() - amount;
                if (newModeShiftAmount <= 0) {
                    removeStatus<MS::MODE_SHIFT>();
                    setMove(MonsterMoveId::THE_GUARDIAN_DEFENSIVE_MODE);
                    bc.addToBot( Actions::MonsterGainBlock(idx, 20) );
                } else {
                    setStatus<MS::MODE_SHIFT>(newModeShiftAmount);
                }
            }
            break;

        default:
            break;
    }
}

void Monster::removeDebuffs() {
    if (getStatus<MS::STRENGTH>() < 0) {
        setStatus<MS::STRENGTH>(0);
    }

    removeStatus<MS::VULNERABLE>();
    removeStatus<MS::WEAK>();
    removeStatus<MS::END_OF_TURN_GAIN_STRENGTH>();

//    removeStatus<MS::POISON>();
//    removeStatus<MS::CHOKED>();
//    removeStatus<MS::CORPSE_EXPLOSION>();
//    removeStatus<MS::LOCK_ON>();

}

// this is calculated at the player when the damage occurs in game, consider testing whether there are any scenarios when it can't be done before
int Monster::calculateDamageToPlayer(const BattleContext &bc, int baseDamage) const {
    const auto &p = bc.player;
    auto damage = static_cast<float>(baseDamage + getStatus<MS::STRENGTH>());

    if (hasStatus<MS::WEAK>()) {
        if (p.hasRelic<RelicId::PAPER_KRANE>()) {
            damage *= 0.6f;
        } else {
            damage *= 0.75f;
        }
    }

    if (p.hasStatus<PS::VULNERABLE>()) {
        if (p.hasRelic<RelicId::ODD_MUSHROOM>()) {
            damage *= 1.25f;
        } else {
            damage *= 1.5f;
        }
    }

    if (p.stance == Stance::WRATH) {
        damage *= 2;
    }

    // apply backIdx attack for spire guard elites

    if (p.hasStatus<PS::INTANGIBLE>()) {
        damage = std::min(damage, static_cast<float>(1));
    }

    return std::max(static_cast<int>(std::floor(damage)), 0);
}

void Monster::attackPlayerHelper(BattleContext &bc, int baseDamage, int times) {
    const int damage = calculateDamageToPlayer(bc, baseDamage);

    for (int i = 0; i < times; i++) {
        bc.addToBot(Actions::AttackPlayer(idx, damage));
    }
}

bool Monster::firstTurn() {
    return moveHistory[0] == MonsterMoveId::INVALID;
}

bool Monster::lastMove(MonsterMoveId moveId) {
    return moveHistory[0] == moveId;
}

bool Monster::lastMoveBefore(MonsterMoveId moveId) {
    return moveHistory[1] == moveId;
}

bool Monster::lastTwoMoves(MonsterMoveId moveId) {
    return moveHistory[0] == moveId && moveHistory[1] == moveId;
}

void Monster::rollMove(BattleContext &bc) {
    setMoveFromRoll(bc, bc.aiRng.random(99));
}

void Monster::setMove(MonsterMoveId moveId) {
    moveHistory[1] = moveHistory[0];
    moveHistory[0] = moveId;
}

namespace sts {

    std::ostream& printIfHaveStatus(const Monster &m, std::ostream &os, MonsterStatus s, bool &havePrint) {
        int value = m.getStatusInternal(s);
        if (!value) {
            return os;
        }

        if (havePrint) {
            os << ", ";
        }
        havePrint = true;
        auto desc = enemyStatusStrings[(int)s];

        return os << "(" << desc << "," << value << ")";
    }

    std::ostream& printIfHaveBoolStatuses(std::ostream &os, const Monster &m,  bool &havePrint) {
        if (m.hasStatus<MS::ASLEEP>()) {
            if (havePrint) {
                os << ", ";
            }
            havePrint = true;
            auto desc = enemyStatusStrings[static_cast<int>(MS::ASLEEP)];
            os << "(" << desc << ")";
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Monster &m) {
        os << "{";
        os << m.idx << " " << sts::enemyIdStrings[(int) m.id]
           << " hp:(" << m.curHp << "/" << m.maxHp << ")"
           << " block:(" << m.block << ") {";

        bool havePrint = false;
        printIfHaveStatus(m, os, MS::STRENGTH, havePrint);
        printIfHaveBoolStatuses(os, m, havePrint);

        for (auto pair : m.statusMap) {
            printIfHaveStatus(m, os, pair.first, havePrint);
        }

        os << "}";

        os << " halfDead: " << m.halfDead
           << ", moveHistory: { " << monsterMoveStrings[(int) m.moveHistory[0]] << ", " << monsterMoveStrings[(int) m.moveHistory[1]] << "}"
           << " nextActionDamage: " << " miscInfo: " << m.miscInfo << " uniquePower0: " << m.uniquePower0 << " uniquePower1: " << m.uniquePower1
            << "}";
        return os;
    }

}
