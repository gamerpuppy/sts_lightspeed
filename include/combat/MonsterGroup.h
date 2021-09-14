//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_MONSTERGROUP_H
#define STS_LIGHTSPEED_MONSTERGROUP_H

#include <array>
#include <iostream>

#include "constants/MonsterEncounters.h"
#include "constants/MonsterIds.h"
#include "combat/Monster.h"
#include "data_structure/fixed_list.h"

namespace sts {

    struct MonsterGroup {
        int monstersAlive = 0;
        int monsterCount = 0;
        std::array<Monster, 5> arr;

        std::bitset<5> extraRollMoveOnTurn;
        std::bitset<5> skipTurn;

        [[nodiscard]] bool areMonstersBasicallyDead() const;
        [[nodiscard]] int getAliveCount() const;
        [[nodiscard]] int getTargetableCount() const; // calculated here, not fast
        [[nodiscard]] int getFirstTargetable() const;
        [[nodiscard]] int getRandomMonsterIdx(Random &rng, bool aliveOnly=true) const;

        // initialization

        void init(BattleContext &bc, MonsterEncounter encounter);
        void createMonsters(BattleContext &bc, MonsterEncounter encounter);
        void createMonster(BattleContext &bc, MonsterId id);

        void createStrongHumanoid(BattleContext &bc);
        void createStrongWildlife(BattleContext &bc);
        void createWeakWildlife(BattleContext &bc);
        void createShapes(BattleContext &bc, int count);

        static MonsterId getAncientShape(Random &miscRng);
        static MonsterId getGremlin(Random &rng);
        static MonsterId getLouse(Random &miscRng);
        static MonsterId getSlaver(Random &miscRng);

        // actions
        void doMonsterTurn(BattleContext &bc); // take a turn for the next monster

        void applyPreTurnLogic(BattleContext &bc);
        void applyEmeraldEliteBuff(BattleContext &bc, int buffType, int act);

    };

    std::ostream &operator<<(std::ostream &os, const MonsterGroup &g);

}
#endif //STS_LIGHTSPEED_MONSTERGROUP_H
