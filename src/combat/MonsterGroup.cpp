//
// Created by gamerpuppy on 7/7/2021.
//

#include <cassert>

#include "combat/MonsterGroup.h"
#include "combat/BattleContext.h"


using namespace sts;

bool MonsterGroup::areMonstersBasicallyDead() const {
    return monstersAlive <= 0;
}

int MonsterGroup::getTargetableCount() const {
    int count = 0;
    for (int i = 0; i < monsterCount; ++i) {
        if (arr[i].isTargetable()) {
            ++count;
        }
    }
    return count;
}

int MonsterGroup::getFirstTargetable() const {
    for (int i = 0; i < monsterCount; ++i) {
        if (arr[i].isTargetable()) {
            return i;
        }
    }
    return -1;
}

int MonsterGroup::getAliveCount() const {
    return monstersAlive;
}

int MonsterGroup::getRandomMonsterIdx(Random &rng, bool aliveOnly) const {
    if (aliveOnly) {
        if (monstersAlive == 0) {
            return -1;
        } else {
            int aliveIdx = rng.random(monstersAlive-1);

            int i = 0;
            int aliveEncountered = 0;
            while (true) {
#ifdef sts_asserts
                // seed 11195
                if (i >= monsterCount) {
                    std::cerr << monsterIdStrings[static_cast<int>(arr[0].id)] << " "
                    << monsterIdStrings[static_cast<int>(arr[1].id)] << " "
                    << "count:" << monsterCount << " "
                    << "alive:" << monstersAlive << std::endl;
                    assert(false);
                }
#endif
                if (!arr[i].isDeadOrEscaped()) {
                    if (aliveEncountered == aliveIdx) {
                        return i;
                    } else {
                        ++aliveEncountered;
                    }
                }
                ++i;
            }

        }
    } else {
        return rng.random(monsterCount - 1);
    }
}

void MonsterGroup::init(BattleContext &bc, MonsterEncounter encounter) {
    createMonsters(bc, encounter);
    for (int i = 0; i < monsterCount; ++i) {
        if (arr[i].idx != -1) {
            arr[i].rollMove(bc);
        }
    }

    for (int i = 0; i < monsterCount; ++i) {
        if (arr[i].idx != -1) {
            arr[i].preBattleAction(bc);
        }
    }
}

void MonsterGroup::createMonster(BattleContext &bc, MonsterId id) {
    arr[monsterCount].construct(bc, id, monsterCount);
    ++monsterCount;
    ++monstersAlive;
}

void MonsterGroup::createMonsters(BattleContext &bc, MonsterEncounter encounter) {
    switch (encounter) {

        case MonsterEncounter::GREMLIN_GANG: {
            MonsterId gremlinPool[] {
                    MonsterId::MAD_GREMLIN,
                    MonsterId::MAD_GREMLIN,
                    MonsterId::SNEAKY_GREMLIN,
                    MonsterId::SNEAKY_GREMLIN,
                    MonsterId::FAT_GREMLIN,
                    MonsterId::FAT_GREMLIN,
                    MonsterId::SHIELD_GREMLIN,
                    MonsterId::GREMLIN_WIZARD,
            };
            int lastIdx = 7;
            for (int i = 0; i < 4; ++i) {
                int idx = bc.miscRng.random(lastIdx);
                MonsterId gremlin = gremlinPool[idx];

                while (idx < lastIdx) {
                    gremlinPool[idx] = gremlinPool[idx + 1];
                    ++idx;
                }
                --lastIdx;
                createMonster(bc, gremlin);
            }
            break;
        }

        case MonsterEncounter::SMALL_SLIMES: {
            if (bc.miscRng.randomBoolean()) {
                createMonster(bc, MonsterId::SPIKE_SLIME_S);
                createMonster(bc, MonsterId::ACID_SLIME_M);
            } else {
                createMonster(bc, MonsterId::ACID_SLIME_S);
                createMonster(bc, MonsterId::SPIKE_SLIME_M);
            }
            break;
        }

        case MonsterEncounter::LOTS_OF_SLIMES: {
            MonsterId slimePool[]{
                    MonsterId::SPIKE_SLIME_S,
                    MonsterId::SPIKE_SLIME_S,
                    MonsterId::SPIKE_SLIME_S,
                    MonsterId::ACID_SLIME_S,
                    MonsterId::ACID_SLIME_S
            };
            for (int i = 4; i >= 0; --i) {
                int idx = bc.miscRng.random(i);
                MonsterId slime = slimePool[idx];
                while (idx < i) {
                    slimePool[idx] = slimePool[idx + 1];
                    ++idx;
                }
                createMonster(bc, slime);
            }
            break;
        }

        case MonsterEncounter::LARGE_SLIME: {
            auto id = bc.miscRng.randomBoolean() ? MonsterId::ACID_SLIME_L : MonsterId::SPIKE_SLIME_L;
            createMonster(bc, id);
            break;
        }

        case MonsterEncounter::EXORDIUM_THUGS:
            createWeakWildlife(bc);
            createStrongHumanoid(bc);
            break;

        case MonsterEncounter::EXORDIUM_WILDLIFE:
            createStrongWildlife(bc);
            createWeakWildlife(bc);
            break;

        case MonsterEncounter::AUTOMATON:
            monsterCount = 1;
            createMonster(bc, MonsterId::BRONZE_AUTOMATON);
            ++monsterCount;
            break;

        case MonsterEncounter::AWAKENED_ONE:
            createMonster(bc, MonsterId::CULTIST);
            createMonster(bc, MonsterId::CULTIST);
            createMonster(bc, MonsterId::AWAKENED_ONE);
            break;

        case MonsterEncounter::BOOK_OF_STABBING:
            createMonster(bc, MonsterId::BOOK_OF_STABBING);
            break;

        case MonsterEncounter::BLUE_SLAVER:
            createMonster(bc, MonsterId::BLUE_SLAVER);
            break;

        case MonsterEncounter::CENTURION_AND_HEALER:
            createMonster(bc, MonsterId::CENTURION);
            createMonster(bc, MonsterId::MYSTIC);
            break;

        case MonsterEncounter::COLLECTOR:
            monsterCount = 2;
            createMonster(bc, MonsterId::THE_COLLECTOR);
            break;

        case MonsterEncounter::COLOSSEUM_EVENT_NOBS:
            createMonster(bc, MonsterId::TASKMASTER);
            createMonster(bc, MonsterId::GREMLIN_NOB);
            break;

        case MonsterEncounter::COLOSSEUM_EVENT_SLAVERS:
            createMonster(bc, MonsterId::BLUE_SLAVER);
            createMonster(bc, MonsterId::RED_SLAVER);
            break;

        case MonsterEncounter::CHAMP:
            createMonster(bc, MonsterId::THE_CHAMP);
            break;

        case MonsterEncounter::CHOSEN:
            createMonster(bc, MonsterId::CHOSEN);
            break;

        case MonsterEncounter::CHOSEN_AND_BYRDS:
            createMonster(bc, MonsterId::BYRD);
            createMonster(bc, MonsterId::CHOSEN);
            break;

        case MonsterEncounter::CULTIST:
            createMonster(bc, MonsterId::CULTIST);
            break;

        case MonsterEncounter::CULTIST_AND_CHOSEN:
            createMonster(bc, MonsterId::CULTIST);
            createMonster(bc, MonsterId::CHOSEN);
            break;

        case MonsterEncounter::DONU_AND_DECA:
            createMonster(bc, MonsterId::DECA);
            createMonster(bc, MonsterId::DONU);
            break;

        case MonsterEncounter::FOUR_SHAPES:
            createShapes(bc, 4);
            break;

        case MonsterEncounter::GIANT_HEAD:
            createMonster(bc, MonsterId::GIANT_HEAD);
            break;

        case MonsterEncounter::GREMLIN_LEADER: {
            arr[1].construct(bc, getGremlin(bc.miscRng), 1);
            arr[1].buff<MS::MINION>();

            arr[2].construct(bc, getGremlin(bc.miscRng), 2);
            arr[2].buff<MS::MINION>();

            arr[3].construct(bc, MonsterId::GREMLIN_LEADER, 3);
            monstersAlive = 3;
            monsterCount = 4;
            break;
        }

        case MonsterEncounter::GREMLIN_NOB:
            createMonster(bc, MonsterId::GREMLIN_NOB);
            break;

        case MonsterEncounter::HEXAGHOST:
            createMonster(bc, MonsterId::HEXAGHOST);
            break;

        case MonsterEncounter::JAW_WORM:
            createMonster(bc, MonsterId::JAW_WORM);
            break;

        case MonsterEncounter::JAW_WORM_HORDE: {
            createMonster(bc, MonsterId::JAW_WORM);
            createMonster(bc, MonsterId::JAW_WORM);
            createMonster(bc, MonsterId::JAW_WORM);

            const auto strBuff = bc.ascension >= 17 ? 5 : (bc.ascension >= 2 ? 4 : 3);
            const auto blockBuff = bc.ascension >= 17 ? 9 : (bc.ascension >= 2 ? 6 : 5);
            for (int i = 0; i < 3; ++i) {
                auto &m = bc.monsters.arr[i];
                m.buff<MS::STRENGTH>(strBuff);
                m.addBlock(blockBuff);

                const auto firstMove = MMID::DARKLING_REGROW;
                m.moveHistory[0] = firstMove; // it doesn't matter what this is as long as it is not invalid
                static_assert(firstMove != MonsterMoveId::INVALID);
            }

            break;
        }

        case MonsterEncounter::LAGAVULIN:
            createMonster(bc, MonsterId::LAGAVULIN);
            bc.monsters.arr[0].setHasStatus<MS::ASLEEP>(true);
            break;

        case MonsterEncounter::LAGAVULIN_EVENT:
            createMonster(bc, MonsterId::LAGAVULIN);
            break;

        case MonsterEncounter::LOOTER:
            createMonster(bc, MonsterId::LOOTER);
            break;

        case MonsterEncounter::MASKED_BANDITS_EVENT:
            createMonster(bc, MonsterId::POINTY);
            createMonster(bc, MonsterId::ROMEO);
            createMonster(bc, MonsterId::BEAR);
            break;

        case MonsterEncounter::MAW:
            createMonster(bc, MonsterId::THE_MAW);
            break;

        case MonsterEncounter::MUSHROOMS_EVENT:
            createMonster(bc, MonsterId::FUNGI_BEAST);
            createMonster(bc, MonsterId::FUNGI_BEAST);
            createMonster(bc, MonsterId::FUNGI_BEAST);
            break;

        case MonsterEncounter::MYSTERIOUS_SPHERE_EVENT:
            createMonster(bc, MonsterId::ORB_WALKER);
            createMonster(bc, MonsterId::ORB_WALKER);
            break;

        case MonsterEncounter::NEMESIS:
            createMonster(bc, MonsterId::NEMESIS);
            break;

        case MonsterEncounter::ORB_WALKER:
            createMonster(bc, MonsterId::ORB_WALKER);
            break;

        case MonsterEncounter::RED_SLAVER:
            createMonster(bc, MonsterId::RED_SLAVER);
            break;

        case MonsterEncounter::REPTOMANCER:
            ++monsterCount;
            createMonster(bc, MonsterId::DAGGER);
            createMonster(bc, MonsterId::REPTOMANCER);
            ++monsterCount;
            createMonster(bc, MonsterId::DAGGER);
            break;

        case MonsterEncounter::SENTRY_AND_SPHERE:
            createMonster(bc, MonsterId::SENTRY);
            createMonster(bc, MonsterId::SPHERIC_GUARDIAN);
            break;

        case MonsterEncounter::SHELL_PARASITE:
            createMonster(bc, MonsterId::SHELLED_PARASITE);
            break;

        case MonsterEncounter::SHELLED_PARASITE_AND_FUNGI:
            createMonster(bc, MonsterId::SHELLED_PARASITE);
            createMonster(bc, MonsterId::FUNGI_BEAST);
            break;

        case MonsterEncounter::SHIELD_AND_SPEAR:
            createMonster(bc, MonsterId::SPIRE_SHIELD);
            createMonster(bc, MonsterId::SPIRE_SPEAR);
            break;

        case MonsterEncounter::SLAVERS:
            createMonster(bc, MonsterId::BLUE_SLAVER);
            createMonster(bc, MonsterId::TASKMASTER);
            createMonster(bc, MonsterId::RED_SLAVER);
            break;

        case MonsterEncounter::SNAKE_PLANT:
            createMonster(bc, MonsterId::SNAKE_PLANT);
            break;

        case MonsterEncounter::SNECKO:
            createMonster(bc, MonsterId::SNECKO);
            break;

        case MonsterEncounter::SPIRE_GROWTH:
            createMonster(bc, MonsterId::SPIRE_GROWTH);
            break;

        case MonsterEncounter::SPHERE_AND_TWO_SHAPES:
            createMonster(bc, getAncientShape(bc.miscRng));
            createMonster(bc, getAncientShape(bc.miscRng));
            createMonster(bc, MonsterId::SPHERIC_GUARDIAN);
            break;

        case MonsterEncounter::SPHERIC_GUARDIAN:
            createMonster(bc, MonsterId::SPHERIC_GUARDIAN);
            break;

        case MonsterEncounter::SLIME_BOSS:
            createMonster(bc, MonsterId::SLIME_BOSS);
            break;

        case MonsterEncounter::THE_GUARDIAN:
            createMonster(bc, MonsterId::THE_GUARDIAN);
            break;

        case MonsterEncounter::THE_HEART:
            createMonster(bc, MonsterId::CORRUPT_HEART);
            break;

        case MonsterEncounter::THREE_BYRDS:
            createMonster(bc, MonsterId::BYRD);
            createMonster(bc, MonsterId::BYRD);
            createMonster(bc, MonsterId::BYRD);
            break;

        case MonsterEncounter::THREE_CULTIST:
            createMonster(bc, MonsterId::CULTIST);
            createMonster(bc, MonsterId::CULTIST);
            createMonster(bc, MonsterId::CULTIST);
            break;

        case MonsterEncounter::THREE_DARKLINGS:
            createMonster(bc, MonsterId::DARKLING);
            createMonster(bc, MonsterId::DARKLING);
            createMonster(bc, MonsterId::DARKLING);
            break;

        case MonsterEncounter::THREE_LOUSE: {
            createMonster(bc, getLouse(bc.miscRng));
            createMonster(bc, getLouse(bc.miscRng));
            createMonster(bc, getLouse(bc.miscRng));
            break;
        }

        case MonsterEncounter::THREE_SENTRIES:
            createMonster(bc, MonsterId::SENTRY);
            createMonster(bc, MonsterId::SENTRY);
            createMonster(bc, MonsterId::SENTRY);
            break;

        case MonsterEncounter::THREE_SHAPES:
            createShapes(bc, 3);
            break;

        case MonsterEncounter::TIME_EATER:
            createMonster(bc, MonsterId::TIME_EATER);
            break;

        case MonsterEncounter::TRANSIENT:
            createMonster(bc, MonsterId::TRANSIENT);
            break;

        case MonsterEncounter::TWO_FUNGI_BEASTS:
            createMonster(bc, MonsterId::FUNGI_BEAST);
            createMonster(bc, MonsterId::FUNGI_BEAST);
            break;

        case MonsterEncounter::TWO_LOUSE: {
            createMonster(bc, getLouse(bc.miscRng));
            createMonster(bc, getLouse(bc.miscRng));
            break;
        }

        case MonsterEncounter::TWO_THIEVES:
            createMonster(bc, MonsterId::LOOTER);
            createMonster(bc, MonsterId::MUGGER);
            break;

        case MonsterEncounter::WRITHING_MASS:
            createMonster(bc, MonsterId::WRITHING_MASS);
            break;

        case MonsterEncounter::INVALID:
        default:
            assert(false);
            break;
    }
}

void MonsterGroup::createStrongHumanoid(BattleContext &bc) {
    Monster temp[3] {};
    temp[0].construct(bc, MonsterId::CULTIST, monsterCount);
    temp[1].construct(bc, getSlaver(bc.miscRng), monsterCount);
    temp[2].construct(bc, MonsterId::LOOTER, monsterCount);

    const int idx = bc.miscRng.random(2);
    arr[monsterCount++] = temp[idx];
    monstersAlive++; // todo this is hacky
}

void MonsterGroup::createStrongWildlife(BattleContext &bc) {
    Monster temp[2] {};
    temp[0].construct(bc, MonsterId::FUNGI_BEAST, 0);
    temp[1].construct(bc, MonsterId::JAW_WORM, 0);
    const int idx = bc.miscRng.random(1);
    arr[monsterCount++] = temp[idx];
    monstersAlive++; // todo this is hacky
}


void MonsterGroup::createWeakWildlife(BattleContext &bc) {
    Monster temp[3] {};
    temp[0].construct(bc, getLouse(bc.miscRng), monsterCount);
    temp[1].construct(bc, MonsterId::SPIKE_SLIME_M, monsterCount);
    temp[2].construct(bc, MonsterId::ACID_SLIME_M, monsterCount);

    const int idx = bc.miscRng.random(2);
    arr[monsterCount++] = temp[idx];
    monstersAlive++; // todo this is hacky
}

void MonsterGroup::createShapes(BattleContext &bc, int count) {
    MonsterId shapePool[] {
        MonsterId::REPULSOR,
        MonsterId::REPULSOR,
        MonsterId::EXPLODER,
        MonsterId::EXPLODER,
        MonsterId::SPIKER,
        MonsterId::SPIKER,
        };
    int lastIdx = 5;
    for (int i = 0; i < count; ++i) {
        int idx = bc.miscRng.random(lastIdx);
        MonsterId shape = shapePool[idx];

        while (idx < lastIdx) {
            shapePool[idx] = shapePool[idx + 1];
            ++idx;
        }
        --lastIdx;
        createMonster(bc, shape);
    }
}


MonsterId MonsterGroup::getAncientShape(Random &miscRng) {
    const MonsterId shapes[] {
            MonsterId::SPIKER,
            MonsterId::REPULSOR,
            MonsterId::EXPLODER,
    };
    return shapes[miscRng.random(2)];
}

MonsterId MonsterGroup::getGremlin(Random &rng) {
    const MonsterId gremlins[] {
            MonsterId::MAD_GREMLIN,
            MonsterId::MAD_GREMLIN,
            MonsterId::SNEAKY_GREMLIN,
            MonsterId::SNEAKY_GREMLIN,
            MonsterId::FAT_GREMLIN,
            MonsterId::FAT_GREMLIN,
            MonsterId::SHIELD_GREMLIN,
            MonsterId::GREMLIN_WIZARD,
    };
    return gremlins[rng.random(7)];
}

MonsterId MonsterGroup::getLouse(Random &miscRng) {
    if (miscRng.randomBoolean()) {
        return MonsterId::RED_LOUSE;
    } else {
        return MonsterId::GREEN_LOUSE;
    }
}

MonsterId MonsterGroup::getSlaver(Random &miscRng) {
    if (miscRng.randomBoolean()) {
        return MonsterId::RED_SLAVER;
    } else {
        return MonsterId::BLUE_SLAVER;
    }
}

void MonsterGroup::doMonsterTurn(BattleContext &bc) {
    auto &m = arr[bc.monsterTurnIdx];
    if ((!m.isDeadOrEscaped() || m.isHalfDead())
        && !skipTurn[bc.monsterTurnIdx])
    {
        if (skipTurn[bc.monsterTurnIdx]) {
            skipTurn.set(bc.monsterTurnIdx, false);
        } else {
            m.takeTurn(bc);
        }
    }

    if (extraRollMoveOnTurn.test(bc.monsterTurnIdx)) {
        bc.noOpRollMove();
        extraRollMoveOnTurn.set(bc.monsterTurnIdx, false);
    }

    ++bc.monsterTurnIdx;
}

void MonsterGroup::applyPreTurnLogic(BattleContext &bc) {
    for (int i = 0; i < monsterCount; ++i) {
        auto &m = arr[i];
        if (m.isDying() || m.isEscaping()) { // todo fix this line
            continue;
        }
        m.applyStartOfTurnPowers(bc); // dont need to do this before I think
    }
}

void MonsterGroup::applyEmeraldEliteBuff(BattleContext &bc, const int buffType, const int act) {
    for (int i = 0; i < monsterCount; ++i) {
        Monster &m = arr[i];

        switch (buffType) {
            case 0:
                m.buff<MS::STRENGTH>(act);
                break;

            case 1: {
                const auto increaseAmount = static_cast<int>(std::round(static_cast<float>(m.maxHp) * 0.25f));
                m.maxHp += increaseAmount;
                m.curHp += increaseAmount;
                break;
            }

            case 2:
                m.buff<MS::METALLICIZE>(act * 2 + 2);
                break;

            case 3:
                m.buff<MS::REGEN>(act * 2 + 1);
                break;

            default:
                // unreachable
                break;
        }
    }

}

namespace sts {

    std::ostream &operator<<(std::ostream &os, const MonsterGroup &g) {
        const std::string s = "\n\t";

        os << "MonsterGroup { ";
        os << s << "monsterCount: " << g.monsterCount;
        os << s << "monstersAlive: " << g.monstersAlive;
        os << s << "extraRollMoveOnTurnBits: " << g.extraRollMoveOnTurn.to_string();

        for (int i = 0; i < g.monsterCount; ++i) {
            os << s << g.arr[i];
        }
        os << "\n}\n";
        return os;
    }

}

