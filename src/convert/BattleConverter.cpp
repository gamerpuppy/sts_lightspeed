#include "convert/BattleConverter.h"
#include "combat/BattleContext.h"
#include "game/GameContext.h"

#include <nlohmann/json.hpp>

using namespace sts;

BattleConverter::BattleConverter() {}

BattleConverter::~BattleConverter() {}

int countMonsterOccurrences(const nlohmann::json &monsters, const MonsterId id) {
    int count = 0;
    for (int i = 0; i < monsters.size(); ++i) {
        if (getMonsterIdFromId(monsters[i]["id"]) == id) {
            count += 1;
        }
    }
    return count;
}

int computePreplacedIdx(const nlohmann::json &monsters) {
    // the simulator special cases bronze automaton, collector, gremlin leader, and reptomancer with their summons
    // as such I can't just place every monster at an arbitrary position since these monsters must be positioned in specific locations
    // so assert that there is at most one of these (since multiple cannot be placed correctly)
    int countBronzeAutomaton = countMonsterOccurrences(monsters, MonsterId::BRONZE_AUTOMATON);
    int countTheCollector = countMonsterOccurrences(monsters, MonsterId::THE_COLLECTOR);
    int countGremlinLeader = countMonsterOccurrences(monsters, MonsterId::GREMLIN_LEADER);
    int countReptomancer = countMonsterOccurrences(monsters, MonsterId::REPTOMANCER);
    int countSpikeSlimeL = countMonsterOccurrences(monsters, MonsterId::SPIKE_SLIME_L);
    int countAcidSlimeL = countMonsterOccurrences(monsters, MonsterId::ACID_SLIME_L);
#ifdef sts_asserts
    assert(countBronzeAutomaton + countTheCollector + countGremlinLeader + countReptomancer <= 1);
    assert(countSpikeSlimeL <= 1);
    assert(countAcidSlimeL <= 1);
#endif
    
    if (countBronzeAutomaton >= 1) {
        return 1;
    } else if (countTheCollector >= 1) {
        return 2;
    } else if (countGremlinLeader >= 1) {
        return 3;
    } else if (countReptomancer >= 1) {
        return 2;
    } else if (countSpikeSlimeL + countAcidSlimeL >= 2) {
        return 2; // according to the simulator slime boss needs the second slime to be positioned at idx 2    
    } else {
        return -1;
    }
}

bool isSpecialCase(const MonsterId id) {
    return id == MonsterId::BRONZE_AUTOMATON || id == MonsterId::THE_COLLECTOR || id == MonsterId::GREMLIN_LEADER || id == MonsterId::REPTOMANCER
        || id == MonsterId::ACID_SLIME_L; // only the acid slime needs to be special-cased
                                          // note that in reality any L slime that has another monster after it needs to have the following monster special-cased 
                                          // to an idx 1 further over but in vanilla this only matters during slime boss so we can just move the following monster
                                          // ACID_SLIME_L to position 2 always
}

BattleContext BattleConverter::convertFromJson(const nlohmann::json &json) {
    GameContext gc;
    gc.initFromJson(json);
    BattleContext bc;
    bc.partialInitOne(gc, MonsterEncounter::INVALID);
    auto monsters = json["game_state"]["combat_state"]["monsters"];
    int monstersIdx = 0;
    int preplacedIdx = computePreplacedIdx(monsters);
    for (int i = 0; i < monsters.size(); ++i) {
        auto m = monsters[i];
        MonsterId monsterId = getMonsterIdFromId(m["id"]);
        
        // any monster that has been defeated can be removed from consideration entirely
        // this is necessary because the simulator expects at most 5 monsters to exist
        // and during some fights (ex. slime boss) there exist more than 5 monsters if
        // we consider the "gone" ones (which communication mod does)
        if (m["is_gone"]) {
            continue;
        }

        Monster *monster;
        
        // ensure that monstersIdx and the MonsterGroup always skips past the preplaced position
        if (monstersIdx == preplacedIdx) {
            monstersIdx += 1;
            bc.monsters.monsterCount += 1;
        }

        if (isSpecialCase(monsterId)) {
            // preplaced monster gets put into its position
            int cachedCount = bc.monsters.monsterCount;
            bc.monsters.monsterCount = preplacedIdx;
            bc.monsters.createMonster(bc, monsterId);
            monster = &bc.monsters.arr[preplacedIdx];
            // restore the previous position in the MonsterGroup
            bc.monsters.monsterCount = cachedCount;
        } else {
            bc.monsters.createMonster(bc, monsterId);
            monster = &bc.monsters.arr[monstersIdx++];
        }

        monster->curHp = m["current_hp"];
        monster->maxHp = m["max_hp"];
        monster->block = m["block"];

        monster->isEscapingB = m["is_escaping"];
        monster->halfDead = m["half_dead"];

        // createMonster increments monstersAlive,
        // which gets deceremented when a monster leaves battle
        // in a typical fashion (hp goes to 0 or escapes)
        // but that has to be explicitly done during this conversion process
        if (monster->curHp <= 0 || monster->isEscapingB) {
            --bc.monsters.monstersAlive;
        }

        monster->moveHistory[0] = getMonsterMoveFromId(monsterId, m["move_id"]);
        if (m.contains("last_move_id")) {
            monster->moveHistory[1] = getMonsterMoveFromId(monsterId, m["last_move_id"]);
        }

        auto powers = m["powers"];
        for (int j = 0; j < powers.size(); ++j) {
            auto p = powers[j];
            MonsterStatus monsterStatus = getMonsterStatusFromId(p["id"]);
            monster->setStatus(monsterStatus, p["amount"]);
            if (m.contains("just_applied")) {
                monster->setJustApplied(monsterStatus, p["just_applied"]);
            }
        }
    }
    // ensure the MonsterGroup position includes the preplacedIdx
    bc.monsters.monsterCount = std::max(preplacedIdx + 1, bc.monsters.monsterCount);
    // special case for bronze automaton, ensure monsterCount is at least preplacedIdx + 2
    if (countMonsterOccurrences(monsters, MonsterId::BRONZE_AUTOMATON) >= 1) {
        bc.monsters.monsterCount = std::max(preplacedIdx + 2, bc.monsters.monsterCount);
    }
    bc.partialInitTwo(gc);
    bc.player.energy = json["game_state"]["combat_state"]["player"]["energy"];

    auto drawPile = json["game_state"]["combat_state"]["draw_pile"];
    for (int i = 0; i < drawPile.size(); ++i) {
        auto c = drawPile[i];
        CardId cardId = getCardIdFromId(c["id"]);
        CardInstance cardInstance(cardId, c["upgrades"] > 0);
        cardInstance.costForTurn = c["cost"];
        bc.cards.moveToDrawPileTop(cardInstance);
        bc.cards.notifyAddCardToCombat(cardInstance);
    }

    auto discardPile = json["game_state"]["combat_state"]["discard_pile"];
    for (int i = 0; i < discardPile.size(); ++i) {
        auto c = discardPile[i];
        CardId cardId = getCardIdFromId(c["id"]);
        CardInstance cardInstance(cardId, c["upgrades"] > 0);
        cardInstance.costForTurn = c["cost"];
        bc.cards.moveToDiscardPile(cardInstance);
        bc.cards.notifyAddCardToCombat(cardInstance);
    }

    auto hand = json["game_state"]["combat_state"]["hand"];
    for (int i = 0; i < hand.size(); ++i) {
        auto c = hand[i];
        CardId cardId = getCardIdFromId(c["id"]);
        CardInstance cardInstance(cardId, c["upgrades"] > 0);
        cardInstance.costForTurn = c["cost"];
        bc.cards.moveToHand(cardInstance);
        bc.cards.notifyAddCardToCombat(cardInstance);
    }

    // TODO: powers for the player
    // auto powers = json["game_state"]["player"]["powers"];
    // for (int j = 0; j < powers.size(); ++j) {
    //     auto p = powers[j];
    //     PlayerStatus playerStatus = getPlayerStatusFromId(p["id"]);
    //     bc.player.setStatus(playerStatus, p["amount"]);
    //     bc.player.setJustApplied(playerStatus, p["just_applied"]);
    // }

    // these would typically be initialized in BattleContext::initRelics
    // but that performs additional initialization that only occurs at the start of battle
    // which would be invalid for loading a state that is already in the middle of combat
    bc.player.happyFlowerCounter = gc.relics.getRelicValue(RelicId::HAPPY_FLOWER);
    bc.player.incenseBurnerCounter = gc.relics.getRelicValue(RelicId::INCENSE_BURNER);
    bc.player.inkBottleCounter = gc.relics.getRelicValue(RelicId::INK_BOTTLE);
    bc.player.inserterCounter = gc.relics.getRelicValue(RelicId::INSERTER);
    bc.player.nunchakuCounter = gc.relics.getRelicValue(RelicId::NUNCHAKU);
    bc.player.penNibCounter = gc.relics.getRelicValue(RelicId::PEN_NIB);
    bc.player.sundialCounter = gc.relics.getRelicValue(RelicId::SUNDIAL);

    // TODO: have communication mod provide the "activated" state of the necronomicon

    // TODO: have communication mod provide the cardsPlayedThisTurn, attacksPlayedthisTurn, skillsPlayedThisTurn, orangePelletsCardTypesPlayed, cardsDiscardedThisTurn

    bc.executeActions();
    return bc;
}