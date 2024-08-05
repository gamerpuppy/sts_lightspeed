//
// Created by gamerpuppy on 6/25/2021.
//

#include "game/GameContext.h"

#include <algorithm>
#include <cassert>

#include "constants/RelicPools.h"
#include "constants/CardPools.h"
#include "combat/BattleContext.h"
#include "game/SaveFile.h"
#include "game/Game.h"
#include "sim/PrintHelpers.h"
#include "sts_common.h"

using namespace sts;

const GameContextAction returnToMapAction = [](auto &gs) {
    gs.screenState = ScreenState::MAP_SCREEN;
    gs.regainControlAction = nullptr;
};

int rollWeightedIdx(float roll, const float *weights, int weightSize);

bool isCampfireRelic(RelicId r) {
    return r == RelicId::PEACE_PIPE || r == RelicId::SHOVEL || r == RelicId::GIRYA;
}

SelectScreenCard::SelectScreenCard(const Card &card) : card(card) {}

SelectScreenCard::SelectScreenCard(const Card &card, int deckIdx) : card(card), deckIdx(deckIdx) {}

GameContext::GameContext(CharacterClass cc, std::uint64_t seed, int ascension)
    : seed(seed),
    neowRng(seed),
    treasureRng(seed),
    eventRng(seed),
    relicRng(seed),
    potionRng(seed),
    cardRng(seed),
    cardRandomRng(seed),
    merchantRng(seed),
    monsterRng(seed),
    shuffleRng(seed),
    miscRng(seed),
    mathUtilRng(seed-897897), // uses a time based seed -_-
    cc(cc),
    map(new Map(Map::fromSeed(seed, ascension, 1, true))),
    ascension(ascension) {
    eventList.insert(eventList.end(), EventPools::Act1::events.begin(), EventPools::Act1::events.end());
    shrineList.insert(shrineList.end(), EventPools::Act1::shrines.begin(), EventPools::Act1::shrines.end());
    if (ascension < 15) {
        specialOneTimeEventList.insert(specialOneTimeEventList.end(), EventPools::oneTimeEventsAsc0.begin(),
                                       EventPools::oneTimeEventsAsc0.end());
    } else {
        specialOneTimeEventList.insert(specialOneTimeEventList.end(), EventPools::oneTimeEventsAsc15.begin(),
                                       EventPools::oneTimeEventsAsc15.end());
    }

    generateMonsters();
    initRelics();
    initPlayer();

    potionCapacity = ascension < 11 ? 3 : 2;
    std::fill(potions.begin(), potions.end(), Potion::EMPTY_POTION_SLOT);

    curEvent = Event::NEOW;
    info.neowRewards = Neow::getOptions(neowRng);
    screenState = ScreenState::EVENT_SCREEN;
}

void GameContext::initFromSave(const SaveFile &s) {
    seed = s.seed;

    outcome = GameOutcome::UNDECIDED;
    ascension = s.ascension_level;
    act = s.act_num;
    floorNum = s.floor_num;
    curMapNodeX = s.room_x;
    curMapNodeY = s.room_y;

    cc = s.cc;
    curHp = s.current_health;
    maxHp = s.max_health;
    gold = s.gold;
    speedrunPace = s.play_time < (60*13 + 20); // speedrun pace is below 13m20s

    treasureRng = Random(seed, s.treasure_seed_count);
    eventRng = Random(seed, s.event_seed_count);
    relicRng = Random(seed, s.relic_seed_count);
    potionRng = Random(seed, s.potion_seed_count);
    cardRng = Random(seed, s.card_seed_count);
    cardRandomRng = Random(seed, s.card_random_seed_count);
    merchantRng = Random(seed, s.merchant_seed_count);
    mathUtilRng = Random(seed, 0);
    merchantRng = Random(seed, s.merchant_seed_count); // arbitrary
    miscRng = Random(seed+floorNum);
    monsterRng = Random(seed, s.monster_seed_count);

    cardRarityFactor = s.card_random_seed_randomizer;
    potionChance = s.potion_chance;
    monsterChance = s.monsterChance;
    shopChance = s.shopChance;
    treasureChance = s.treasureChance;

    if (s.has_emerald_key) {
        obtainKey(Key::EMERALD_KEY);
    }
    if (s.has_ruby_key) {
        obtainKey(Key::RUBY_KEY);
    }
    if (s.has_sapphire_key) {
        obtainKey(Key::SAPPHIRE_KEY);
    }

    // this is a game bug, the shrine list is not stored in the save file
    shrineList.clear();
    switch (act) {
        case 1:
            shrineList.insert(shrineList.begin(), EventPools::Act1::shrines.begin(), EventPools::Act1::shrines.end());
            break;

        case 2:
        case 3:
            shrineList.insert(shrineList.begin(), EventPools::Act2::shrines.begin(), EventPools::Act2::shrines.end());
            break;

        case 4:
        default:
            break;
    }

    eventList.insert(eventList.begin(), s.event_list.begin(), s.event_list.end());
    specialOneTimeEventList.insert(
            specialOneTimeEventList.begin(),
            s.one_time_event_list.begin(),
            s.one_time_event_list.end()
    );

    commonRelicPool.insert(commonRelicPool.begin(), s.common_relics.begin(), s.common_relics.end());
    uncommonRelicPool.insert(uncommonRelicPool.begin(), s.uncommon_relics.begin(), s.uncommon_relics.end());
    rareRelicPool.insert(rareRelicPool.begin(), s.rare_relics.begin(), s.rare_relics.end());
    shopRelicPool.insert(shopRelicPool.begin(), s.shop_relics.begin(), s.shop_relics.end());
    bossRelicPool.insert(bossRelicPool.begin(), s.boss_relics.begin(), s.boss_relics.end());


//    std::cout << "monsterListSize: " << s.monster_list.size() << std::endl;
//    std::cout << "eliteListSize: " << s.elite_monster_list.size() << std::endl;

    boss = s.boss_list[0];
    if (s.boss_list.size() > 1) {
        secondBoss = s.boss_list[1];
    }

    monsterListOffset = 0;
    monsterList = {};
    for (auto m : s.monster_list) {
        if (monsterList.size() == 16) {
            assert(false);
        }
        monsterList.push_back(m);
    }

    eliteMonsterListOffset = 0;
    eliteMonsterList = {};
    for (auto m : s.elite_monster_list) {
        if (eliteMonsterList.size() == 10) {
            assert(false);
        }
        eliteMonsterList.push_back(m);
    }

    MonsterEncounter encounter;
    switch (s.current_room) {

        case Save::RoomType::MONSTER_ROOM:
            curRoom = Room::MONSTER;
            encounter = getMonsterForRoomCreation();
            break;

        case Save::RoomType::MONSTER_ROOM_ELITE:
            curRoom = Room::ELITE;
            encounter = getEliteForRoomCreation();
            break;

        case Save::RoomType::MONSTER_ROOM_BOSS:
            curRoom = Room::BOSS;
            encounter = boss;
            break;

        default:
            std::cerr << "Only save files from the start of combat are supported right now. \n";
            assert(false);  // other rooms not supported right now.
    }

    deck.initFromSaveFile(s);

    initRelicsFromSave(s);

    potionCount = 0;
    potionCapacity = ascension >= 11 ? 2 : 3;
    if (relics.has(RelicId::POTION_BELT)) {
        potionCapacity += 2;
    }

    Potion tempPotion[6];
    for (int i = 0; i < s.potions.size(); ++i) {
        auto p = s.potions[i];
        if (p != Potion::EMPTY_POTION_SLOT) {
            ++potionCount;
        }
        potions[i] = p;
    }

    map = std::make_shared<Map>(Map::fromSeed(seed, ascension, act, true));

    regainControlAction = [](GameContext &gc) {
        gc.afterBattle();
    };
    enterBattle(encounter);
}

void GameContext::initRelicsFromSave(const SaveFile &s) {
    for (int i = 0; i < s.relics.size(); ++i) {
        auto r = s.relics[i];
//        std::cout << getRelicName(r) << " " << s.relic_counters[i] << '\n';

        // todo lizard tail, and others
        switch (r) {
            default:
                RelicInstance relic {r, s.relic_counters[i]};
                relics.add(relic);
        }
    }
}

const MapNode& GameContext::getCurMapNode() const {
    return map->getNode(curMapNodeX, curMapNodeY);
}

int GameContext::fractionMaxHp(float percent, HpType type) const {
    if (type == ROUND) {
        return static_cast<int>(std::round(static_cast<float>(maxHp) * percent));

    } else if (type == FLOOR) {
        return static_cast<int>(static_cast<float>(maxHp) * percent);

    } else {
        return static_cast<int>(std::ceil(static_cast<float>(maxHp) * percent));
    }
}

bool GameContext::hasRelic(RelicId r) const {
    return relics.has(r);
}

bool GameContext::hasKey(Key key) const {
    switch (key) {
        case Key::EMERALD_KEY:
            return greenKey;

        case Key::SAPPHIRE_KEY:
            return blueKey;

        case Key::RUBY_KEY:
            return redKey;

        default:
#ifdef sts_asserts
            assert(false);
#endif
            return false;
    }
}

bool GameContext::hasLessThanTwoCampfireRelics() const {
    int count = 0;
    if (hasRelic(RelicId::GIRYA)) {
        ++count;
    }
    if (hasRelic(RelicId::PEACE_PIPE)) {
        ++count;
    }
    if (hasRelic(RelicId::SHOVEL)) {
        ++count;
    }
    return count < 2;
}

bool GameContext::relicCanSpawn(RelicId relic, bool shopRoom) const {
    switch (relic) {

        case RelicId::BOTTLED_FLAME:
            return deck.getCountMatching([](auto c){
                return c.getType() == CardType::ATTACK && c.getRarity() != CardRarity::BASIC;
            }, 1);

        case RelicId::BOTTLED_LIGHTNING:
            return deck.getCountMatching([](auto c){
                return c.getType() == CardType::SKILL && c.getRarity() != CardRarity::BASIC;
            }, 1);

        case RelicId::BOTTLED_TORNADO:
            return deck.getCountMatching([](auto c){
                return c.getType() == CardType::POWER;
            }, 1);

        case RelicId::BLACK_BLOOD:
            return relics.has(RelicId::BURNING_BLOOD);

        case RelicId::FROZEN_CORE:
            return hasRelic(RelicId::CRACKED_CORE);

        case RelicId::BURNING_BLOOD:
            return hasRelic(RelicId::BURNING_BLOOD);

        case RelicId::RING_OF_THE_SERPENT:
            return hasRelic(RelicId::RING_OF_THE_SNAKE);

        case RelicId::HOLY_WATER:
            return hasRelic(RelicId::PURE_WATER);

        case RelicId::TINY_CHEST:
            return floorNum <= 35;

        case RelicId::WING_BOOTS:
        case RelicId::MATRYOSHKA:
            return floorNum <= 40;

        case RelicId::GIRYA:
        case RelicId::PEACE_PIPE:
        case RelicId::SHOVEL:
            return floorNum < 48 && hasLessThanTwoCampfireRelics();

        case RelicId::MAW_BANK:
        case RelicId::OLD_COIN:
        case RelicId::SMILING_MASK:
            return floorNum <= 48 && !shopRoom;

        case RelicId::ANCIENT_TEA_SET:
        case RelicId::CERAMIC_FISH:
        case RelicId::DARKSTONE_PERIAPT:
        case RelicId::DREAM_CATCHER:
        case RelicId::FROZEN_EGG:
        case RelicId::JUZU_BRACELET:
        case RelicId::MEAL_TICKET:
        case RelicId::MEAT_ON_THE_BONE:
        case RelicId::MOLTEN_EGG:
        case RelicId::OMAMORI:
        case RelicId::POTION_BELT:
        case RelicId::PRAYER_WHEEL:
        case RelicId::QUESTION_CARD:
        case RelicId::REGAL_PILLOW:
        case RelicId::SINGING_BOWL:
        case RelicId::THE_COURIER:
        case RelicId::TOXIC_EGG:
            return floorNum <= 48;

        case RelicId::PRESERVED_INSECT:
            return floorNum <= 52;

        default:
            return true;
    }
}

bool GameContext::canAddOneTimeEvent(Event shrine) const {
    switch (shrine) {
        case Event::THE_DIVINE_FOUNTAIN:
            return deck.hasCurse();

        case Event::DESIGNER_IN_SPIRE:
            return  (act == 2 || act == 3) && gold >= 75;

        case Event::DUPLICATOR:
            return act == 2 || act == 3;

        case Event::FACE_TRADER:
            return act == 1 || act == 2;

        case Event::KNOWING_SKULL:
            return act == 2 && curHp > 12;

        case Event::NLOTH:
            return act == 2 && relics.size() >= 2;

        case Event::THE_JOUST:
            return act == 2 && gold >= 50;

        case Event::THE_WOMAN_IN_BLUE:
            return gold >= 50;

        case Event::SECRET_PORTAL:
            return act == 3 && !speedrunPace;

        default:
            return true;
    }
}

bool GameContext::canAddEvent(Event event) const {
    switch (event) {
        case Event::DEAD_ADVENTURER:
        case Event::HYPNOTIZING_COLORED_MUSHROOMS:
            return floorNum > 6;

        case Event::THE_MOAI_HEAD: {
            bool playerAtLessHalf =
                    curHp <= maxHp/2;
            return playerAtLessHalf || hasRelic(RelicId::GOLDEN_IDOL);
        }

        case Event::THE_CLERIC:
            return gold >= 35;

        case Event::OLD_BEGGAR:
            return gold >= 75;

        case Event::COLOSSEUM:
            return curMapNodeY > 7 && !disableColosseum;

        default:
            return true;
    }
}

void GameContext::initRelics() {
    switch (cc) {

        case CharacterClass::IRONCLAD:
            commonRelicPool.insert(commonRelicPool.end(), Ironclad::commonRelicPool.begin(), Ironclad::commonRelicPool.end());
            uncommonRelicPool.insert(uncommonRelicPool.end(), Ironclad::uncommonRelicPool.begin(), Ironclad::uncommonRelicPool.end());
            rareRelicPool.insert(rareRelicPool.end(), Ironclad::rareRelicPool.begin(), Ironclad::rareRelicPool.end());
            shopRelicPool.insert(shopRelicPool.end(), Ironclad::shopRelicPool.begin(), Ironclad::shopRelicPool.end());
            bossRelicPool.insert(bossRelicPool.end(), Ironclad::bossRelicPool.begin(), Ironclad::bossRelicPool.end());
            break;

        case CharacterClass::SILENT:
            commonRelicPool.insert(commonRelicPool.end(), Silent::commonRelicPool.begin(), Silent::commonRelicPool.end());
            uncommonRelicPool.insert(uncommonRelicPool.end(), Silent::uncommonRelicPool.begin(), Silent::uncommonRelicPool.end());
            rareRelicPool.insert(rareRelicPool.end(), Silent::rareRelicPool.begin(), Silent::rareRelicPool.end());
            shopRelicPool.insert(shopRelicPool.end(), Silent::shopRelicPool.begin(), Silent::shopRelicPool.end());
            bossRelicPool.insert(bossRelicPool.end(), Silent::bossRelicPool.begin(), Silent::bossRelicPool.end());
            break;

        case CharacterClass::DEFECT:
            commonRelicPool.insert(commonRelicPool.end(), Defect::commonRelicPool.begin(), Defect::commonRelicPool.end());
            uncommonRelicPool.insert(uncommonRelicPool.end(), Defect::uncommonRelicPool.begin(), Defect::uncommonRelicPool.end());
            rareRelicPool.insert(rareRelicPool.end(), Defect::rareRelicPool.begin(), Defect::rareRelicPool.end());
            shopRelicPool.insert(shopRelicPool.end(), Defect::shopRelicPool.begin(), Defect::shopRelicPool.end());
            bossRelicPool.insert(bossRelicPool.end(), Defect::bossRelicPool.begin(), Defect::bossRelicPool.end());
            break;

        case CharacterClass::WATCHER:
            commonRelicPool.insert(commonRelicPool.end(), Watcher::commonRelicPool.begin(), Watcher::commonRelicPool.end());
            uncommonRelicPool.insert(uncommonRelicPool.end(), Watcher::uncommonRelicPool.begin(), Watcher::uncommonRelicPool.end());
            rareRelicPool.insert(rareRelicPool.end(), Watcher::rareRelicPool.begin(), Watcher::rareRelicPool.end());
            shopRelicPool.insert(shopRelicPool.end(), Watcher::shopRelicPool.begin(), Watcher::shopRelicPool.end());
            bossRelicPool.insert(bossRelicPool.end(), Watcher::bossRelicPool.begin(), Watcher::bossRelicPool.end());
            break;

        default:
            break;
    }

    java::Collections::shuffle(commonRelicPool.begin(), commonRelicPool.end(), java::Random(relicRng.nextLong()));
    java::Collections::shuffle(uncommonRelicPool.begin(), uncommonRelicPool.end(), java::Random(relicRng.nextLong()));
    java::Collections::shuffle(rareRelicPool.begin(), rareRelicPool.end(), java::Random(relicRng.nextLong()));
    java::Collections::shuffle(shopRelicPool.begin(), shopRelicPool.end(), java::Random(relicRng.nextLong()));
    java::Collections::shuffle(bossRelicPool.begin(), bossRelicPool.end(), java::Random(relicRng.nextLong()));

}


void GameContext::initPlayer() {

    if (ascension >= 10) {
        deck.obtain(*this, CardId::ASCENDERS_BANE);
    }

    switch (cc) {
        case CharacterClass::IRONCLAD:
            maxHp = ascension < 14 ? 80 : 75;
            obtainRelic(RelicId::BURNING_BLOOD);
            deck.obtain(*this, CardId::STRIKE_RED, 5);
            deck.obtain(*this, CardId::DEFEND_RED, 4);
            deck.obtain(*this, CardId::BASH);
            break;

        case CharacterClass::SILENT:
            maxHp = ascension < 14 ? 70 : 66;
            obtainRelic(RelicId::RING_OF_THE_SNAKE);
            deck.obtain(*this, CardId::STRIKE_GREEN, 5);
            deck.obtain(*this, CardId::DEFEND_GREEN, 5);
            deck.obtain(*this, CardId::SURVIVOR);
            deck.obtain(*this, CardId::NEUTRALIZE);
            break;

        case CharacterClass::DEFECT:
            maxHp = ascension < 14 ? 75 : 71;
            obtainRelic(RelicId::CRACKED_CORE);
            deck.obtain(*this, CardId::STRIKE_BLUE, 4);
            deck.obtain(*this, CardId::DEFEND_BLUE, 4);
            deck.obtain(*this, CardId::ZAP);
            deck.obtain(*this, CardId::DUALCAST);
            break;

        case CharacterClass::WATCHER:
            maxHp = ascension < 14 ? 72 : 68;
            obtainRelic(RelicId::PURE_WATER);
            deck.obtain(*this, CardId::STRIKE_PURPLE, 4);
            deck.obtain(*this, CardId::DEFEND_PURPLE, 4);
            deck.obtain(*this, CardId::ERUPTION);
            deck.obtain(*this, CardId::VIGILANCE);
            break;

        default:
            break;
    }
    curHp = ascension < 6 ? maxHp : std::round(static_cast<float>(maxHp) * 0.9f);
}

void GameContext::generateMonsters() {
    generateWeakMonsters();
    generateStrongMonsters();
    generateElites();
    generateBoss();
}

void GameContext::generateWeakMonsters() {
    populateMonsterList(
            MonsterEncounterPool::weakEnemies[act-1],
            MonsterEncounterPool::weakWeights[act - 1],
            MonsterEncounterPool::weakCount[act - 1],
            act == 1 ? 3 : 2);
}

void GameContext::generateStrongMonsters() {
    populateFirstStrongEnemy(
            MonsterEncounterPool::strongEnemies[act-1],
            MonsterEncounterPool::strongWeights[act-1],
            MonsterEncounterPool::strongCount[act-1]);

    populateMonsterList(
            MonsterEncounterPool::strongEnemies[act-1],
            MonsterEncounterPool::strongWeights[act-1],
            MonsterEncounterPool::strongCount[act-1], 12);
}

int rollElite(Random &monsterRng) {
    float roll = monsterRng.random();

    float currentWeight = 0.0f;
    currentWeight += 1.0f/3;
    if (roll < 1.0f/3) {
        return 0;
    }

    if (roll < 2.0f/3) {
        return 1;
    } else {
        return 2;
    }
}

void GameContext::generateElites() {
    for(int i = 0; i < 10; ++i) {
        if (eliteMonsterList.empty()) {
            eliteMonsterList.push_back(MonsterEncounterPool::elites[act-1][rollElite(monsterRng)]);
        } else {
            auto toAdd = MonsterEncounterPool::elites[act-1][rollElite(monsterRng)];
            if (toAdd != eliteMonsterList.back()) {
                eliteMonsterList.push_back(toAdd);
            } else {
                --i;
            }
        }
    }
}

void GameContext::generateBoss() {
    static const MonsterEncounter bosses[3][3] = {
            { ME::THE_GUARDIAN, ME::HEXAGHOST, ME::SLIME_BOSS },
            { ME::AUTOMATON, ME::COLLECTOR, ME::CHAMP },
            { ME::AWAKENED_ONE, ME::TIME_EATER, ME::DONU_AND_DECA },
    };

    int indices[3] = {0,1,2};

    java::Collections::shuffle(indices, indices+3, java::Random(monsterRng.randomLong()));

    boss = bosses[act-1][indices[0]];
    if (act == 3 && ascension >= 20) {
        secondBoss = bosses[act-1][indices[1]];
    }
}

int rollWeightedIdx(float roll, const float *weights, int weightSize) {
    // the base game method can return "ERROR"
    float curWeight = 0.0f;
    for (int i = 0; i < weightSize; ++i) {
        curWeight += weights[i];
        if (roll < curWeight) {
            return i;
        }
    }
    return weightSize - 1;
}

void GameContext::populateMonsterList(const MonsterEncounter monsters[], const float weights[], int monstersSize, int numMonsters) {
    for(int i = 0; i < numMonsters; ++i) {
        if (monsterList.empty()) {
            int idx = rollWeightedIdx(monsterRng.random(), weights, monstersSize);
            monsterList.push_back(monsters[idx]);

        } else {
            int idx = rollWeightedIdx(monsterRng.random(), weights, monstersSize);
            MonsterEncounter toAdd = monsters[idx];

            if (toAdd != monsterList.back() &&
                (monsterList.size() < 2 || toAdd != monsterList[monsterList.size()-2]) )
            {
                monsterList.push_back(toAdd);

            } else {
                --i;
            }
        }
    }
}

void GameContext::populateFirstStrongEnemy(const MonsterEncounter monsters[], const float weights[], int monstersSize) {
    auto lastMonster = monsterList.back();
    while (true) {
        int idx = rollWeightedIdx(monsterRng.random(), weights, monstersSize);
        auto toAdd = monsters[idx];

        switch (toAdd) {

            case MonsterEncounter::LARGE_SLIME:
            case MonsterEncounter::LOTS_OF_SLIMES:
                if (lastMonster == MonsterEncounter::SMALL_SLIMES) {
                    continue;
                }
                break;

            case MonsterEncounter::THREE_LOUSE:
                if (lastMonster == MonsterEncounter::TWO_LOUSE) {
                    continue;
                }
                break;

            case MonsterEncounter::CHOSEN_AND_BYRDS:
                break;
            case MonsterEncounter::SENTRY_AND_SPHERE:
                break;
            case MonsterEncounter::SNAKE_PLANT:
                break;
            case MonsterEncounter::SNECKO:
                break;
            case MonsterEncounter::CENTURION_AND_HEALER:
                break;
            case MonsterEncounter::CULTIST_AND_CHOSEN:
                break;
            case MonsterEncounter::THREE_CULTIST:
                break;
            case MonsterEncounter::SHELLED_PARASITE_AND_FUNGI:
                break;
            case MonsterEncounter::SPIRE_GROWTH:
                break;
            case MonsterEncounter::TRANSIENT:
                break;
            case MonsterEncounter::FOUR_SHAPES:
                break;
            case MonsterEncounter::MAW:
                break;
            case MonsterEncounter::SPHERE_AND_TWO_SHAPES:
                break;
            case MonsterEncounter::JAW_WORM_HORDE:
                break;
            case MonsterEncounter::WRITHING_MASS:
                break;

            default: // should not happen
                break;
        }

        monsterList.push_back(toAdd);
        return;
    }
}

void GameContext::transitionToAct(int targetAct) {
    act = targetAct;

    if (cardRng.counter < 250) {
        cardRng.setCounter(250);
    } else if (cardRng.counter < 500) {
        cardRng.setCounter(500);
    } else if (cardRng.counter < 750) {
        cardRng.setCounter(750);
    }

    curMapNodeX = -1;
    curMapNodeY = -1;
    if (targetAct == 2 || targetAct == 3) {
        *map = Map::fromSeed(seed, ascension, targetAct, !hasKey(Key::EMERALD_KEY));
    } else if (targetAct == 4) {
        *map = Map::act4Map();
    }

    colorlessCardPool = baseColorlessPool;
    if (ascension >= 5) {
        playerHeal(static_cast<int>(std::round(static_cast<float>(maxHp-curHp)*0.75f)));
    } else {
        playerHeal(maxHp);
    }

    monsterListOffset = 0;
    monsterList.clear();
    eliteMonsterListOffset = 0;
    eliteMonsterList.clear();

    if (targetAct == 4) {
        boss = MonsterEncounter::THE_HEART;
        eliteMonsterList.push_back(MonsterEncounter::SHIELD_AND_SPEAR);

    } else {
        generateMonsters();
    }

    monsterChance = 0.1f;
    shopChance = 0.03f;
    treasureChance = 0.02f;
    potionChance = 0;

    eventList.clear();
    shrineList.clear();
    if (targetAct == 2) {
        eventList.insert(eventList.end(), EventPools::Act2::events.begin(), EventPools::Act2::events.end());
        shrineList.insert(shrineList.end(), EventPools::Act2::shrines.begin(), EventPools::Act2::shrines.end());

    } else if (targetAct == 3) {
        eventList.insert(eventList.end(), EventPools::Act3::events.begin(), EventPools::Act3::events.end());
        shrineList.insert(shrineList.end(), EventPools::Act3::shrines.begin(), EventPools::Act3::shrines.end());
    }

    screenState = ScreenState::MAP_SCREEN;
}

void GameContext::transitionToMapNode(int mapNodeX) {
    lastRoom = curRoom;
    curMapNodeX = mapNodeX;
    ++floorNum;
    ++curMapNodeY;

    const auto r = Random(seed + floorNum);
    miscRng = r;
    shuffleRng = r;
    cardRandomRng = r;

    regainControlAction = [](auto &gs) {
        gs.screenState = ScreenState::MAP_SCREEN;
    };

    if (curMapNodeY == 15) {
        curRoom = Room::BOSS;
    } else {
        curRoom = map->getNode(curMapNodeX, curMapNodeY).room;
    }
    relicsOnEnterRoom(curRoom);

    curEvent = Event::INVALID;
    if (curRoom == Room::EVENT) {
        curRoom = getEventRoomOutcomeHelper(lastRoom == Room::SHOP);
        switch (curRoom) {
            case Room::EVENT:
                curEvent = generateEvent(eventRng);
                break;

            case Room::SHOP:
            case Room::MONSTER:
            case Room::TREASURE:
                break;

            default:
                assert(false);
        }
    }

    switch (curRoom) {
        case Room::EVENT: {
            setupEvent();
            break;
        }

        case Room::BOSS: {
            regainControlAction = [](GameContext &gc) {
                gc.afterBattle();
            };
            enterBattle(boss);
            break;
        }

        case Room::ELITE: {
            auto encounter = getEliteForRoomCreation();
            regainControlAction = [](GameContext &gc) {
                gc.afterBattle();
            };
            enterBattle(encounter);
            break;
        }

        case Room::MONSTER: {
            auto encounter = getMonsterForRoomCreation();
            regainControlAction = [](GameContext &gc) {
                gc.afterBattle();
            };
            enterBattle(encounter);
            break;
        }

        case Room::REST: {
            screenState = ScreenState::REST_ROOM;
            break;
        }

        case Room::SHOP: {
            if (hasRelic(RelicId::MEAL_TICKET)) {
                playerHeal(15);
            }
            screenState = ScreenState::SHOP_ROOM;
            info.shop.setup(*this);
            break;
        }

        case Room::TREASURE: {
            setupTreasureRoom();
            break;
        }

        default:
            assert(false);
    }

}

void GameContext::setupEvent() { // todo necronomicon event
    screenState = ScreenState::EVENT_SCREEN;
    const bool unfavorable = ascension >= 15;

    switch (curEvent) {
        case Event::BIG_FISH:
            info.hpAmount0 = fractionMaxHp(1/3.0f, FLOOR);
            break;

        case Event::BONFIRE_SPIRITS:
            openCardSelectScreen(CardSelectScreenType::BONFIRE_SPIRITS, 1);
            break;

        case Event::CURSED_TOME:
            info.eventData = 0;
            break;

        case Event::DEAD_ADVENTURER: {
            info.phase = 0;
            info.rewards = {0,1,2};
            java::Collections::shuffle(info.rewards.begin(), info.rewards.end(), miscRng.randomLong());

            const MonsterEncounter encounters[] { ME::THREE_SENTRIES, ME::GREMLIN_NOB, ME::LAGAVULIN_EVENT };
            info.encounter = encounters[miscRng.random(2)];
            break;
        }

        case Event::DESIGNER_IN_SPIRE:
            info.upgradeOne = miscRng.randomBoolean();
            info.cleanUpIsRemoveCard = miscRng.randomBoolean();
            break;

        case Event::FACE_TRADER:
            info.hpAmount0 = std::max(1, fractionMaxHp(.10f, FLOOR));
            break;

        case Event::FALLING: { // todo test and CANNOT BE BOTTLED
            int counts[3] {0,0,0};
            for (const auto &c : deck.cards) {
                ++counts[static_cast<int>(c.getType())];
            }

            int attacksIdx = counts[0] > 0 ? miscRng.random(counts[0] - 1) : -1;
            int skillsIdx = counts[1] > 0 ? miscRng.random(counts[1] - 1) : -1;
            int powersIdx = counts[2] > 0 ? miscRng.random(counts[2] - 1) : -1;

            int attackCount = 0;
            int skillCount = 0;
            int powerCount = 0;

            info.attackCardDeckIdx = -1;
            info.skillCardDeckIdx = -1;
            info.powerCardDeckIdx = -1;

            for (int i = 0; i < deck.cards.size(); ++i) {
                switch (deck.cards[i].getType()) {
                    case CardType::ATTACK:
                        if (attackCount++ == attacksIdx) {
                            info.attackCardDeckIdx = i;
                        }
                        break;

                    case CardType::SKILL:
                        if (skillCount++ == skillsIdx) {
                            info.skillCardDeckIdx = i;
                        }
                        break;

                    case CardType::POWER:
                        if (powerCount++ == powersIdx) {
                            info.powerCardDeckIdx = i;
                        }
                        break;

                    default:
                        break;
                }
            }
            break;
        }

        case Event::FORGOTTEN_ALTAR:
            info.hpAmount0 = fractionMaxHp(unfavorable ? 0.35f : 0.25f, ROUND);
            break;

        case Event::GHOSTS:
            info.hpAmount0 = std::min(maxHp-1, fractionMaxHp(0.5f, CEIL));
            break;

        case Event::GOLDEN_IDOL:
            info.hpAmount0 = fractionMaxHp(unfavorable ? 0.35f : 0.25f);
            info.hpAmount1 = fractionMaxHp(unfavorable ? 0.10f : 0.08f);
            break;

        case Event::KNOWING_SKULL:
            info.hpAmount0 = 6;
            info.hpAmount1 = 6;
            info.hpAmount2 = 6;
            break;

        case Event::LAB: {
            Potion pReward[3];
            int pCount = 2;
            pReward[0] = getRandomPotion(potionRng, cc);
            pReward[1] = getRandomPotion(potionRng, cc);
            if (!unfavorable) {
                pReward[2] = getRandomPotion(potionRng, cc);
                ++pCount;
            }
            openCombatRewardScreen(Rewards(pReward, pCount));
            break;
        }

        case Event::MATCH_AND_KEEP: {
            if (disableMatchAndKeep) { // additional safeguard as it should also be prevented from spawning
                regainControl();
            }

            info.toSelectCards.clear();

            Card cards[6];
            cards[0] = previewObtainCard(getRandomClassCardOfRarity(cardRng, cc, CardRarity::RARE));
            cards[1] = previewObtainCard(getRandomClassCardOfRarity(cardRng, cc, CardRarity::UNCOMMON));
            cards[2] = previewObtainCard(getRandomClassCardOfRarity(cardRng, cc, CardRarity::COMMON));
            if (unfavorable) {
                cards[3] = getRandomCurse(cardRng);
            } else {
                cards[3] = previewObtainCard(returnColorlessCard(CardRarity::UNCOMMON));
            }
            cards[4] = getRandomCurse(cardRng);
            cards[5] = previewObtainCard(getStartCardForEvent(cc));

            int indices[12] = {0,1,2,3,4,5,0,1,2,3,4,5};
            java::Collections::shuffle(indices, indices+12, java::Random(miscRng.randomLong()));

            info.toSelectCards.resize(12);
            for (int i = 0; i < 12; ++i) {
                int cardIdx = indices[i];
                int gridX = i%4;
                int gridY = i%3;
                int selectIdx = gridY*4 + gridX;
                info.toSelectCards[selectIdx] = {cards[cardIdx], -1};
            }

            info.eventData = 5; // attempts remaining
            break;
        }

        case Event::NLOTH: {
            int relicIdxs[200]; // should be large enough
            for (int i = 0; i < relics.size(); ++i) {
                relicIdxs[i] = i;
            }
            java::Collections::shuffle(relicIdxs, relicIdxs+relics.size(), java::Random(miscRng.randomLong()));
            info.relicIdx0 = relicIdxs[0];
            info.relicIdx1 = relicIdxs[1];
            break;
        }

        case Event::SHINING_LIGHT:
            info.hpAmount0 = fractionMaxHp(unfavorable ? 0.30f : 0.20f, ROUND);
            break;

        case Event::THE_CLERIC:
            info.hpAmount0 = fractionMaxHp(0.25f);
            break;

        case Event::THE_LIBRARY:
            info.hpAmount0 = fractionMaxHp(unfavorable ? 0.20f : 0.33f, ROUND);
            break;

        case Event::THE_MOAI_HEAD:
            info.hpAmount0 = fractionMaxHp(unfavorable ? 0.18f : 0.125f, ROUND);
            break;

        case Event::THE_WOMAN_IN_BLUE:
            info.hpAmount0 = fractionMaxHp(0.05f, CEIL);
            break;

        case Event::VAMPIRES:
            info.hpAmount0 = std::min(maxHp-1, fractionMaxHp(0.3f, CEIL));
            break;

        case Event::WE_MEET_AGAIN: { //todo @FIX_BOTTLED
            info.potionIdx = getRandomPlayerPotionIdx();
            if (gold < 50) {
                info.gold = -1;
            } else {
                info.gold = miscRng.random(50, std::min(150, gold));
            }
            info.cardIdx = getRandomPlayerNonBasicCardIdx();
            break;
        }

        case Event::WINDING_HALLS:
            info.hpAmount0 = fractionMaxHp(unfavorable ? 0.18f : 0.125f, ROUND); // curHp loss
            info.hpAmount1 = fractionMaxHp(unfavorable ? 0.2f : 0.25f, ROUND);   // heal amount
            info.hpAmount2 = fractionMaxHp(0.05f, ROUND);   // maxHp loss
            break;

        case Event::WORLD_OF_GOOP:
            if (unfavorable) {
                info.goldLoss = std::min(gold, miscRng.random(35, 75));
            } else {
                info.goldLoss = std::min(gold, miscRng.random(20, 50));
            }
            break;

        default:
            break;
    }
}

void GameContext::setupTreasureRoom() {
    screenState = ScreenState::TREASURE_ROOM;
    info.chestSize = getRandomChestSize(treasureRng);

    int roll = treasureRng.random(99);
    info.haveGold = roll < chestGoldChances[static_cast<int>(info.chestSize)];

    const auto tierChances = chestRelicTierChances[static_cast<int>(info.chestSize)];
    const int commonChance = tierChances[static_cast<int>(RelicTier::COMMON)];
    const int uncommonChance = tierChances[static_cast<int>(RelicTier::UNCOMMON)];
    if (roll < commonChance) {
        info.tier = RelicTier::COMMON;
    } else if (roll < commonChance + uncommonChance) {
        info.tier = RelicTier::UNCOMMON;
    } else {
        info.tier = RelicTier::RARE;
    }
}


void GameContext::enterBossTreasureRoom() {
    ++floorNum;
    Random r(seed+floorNum);
    miscRng = r;
    shuffleRng = r;
    cardRandomRng = r;

    relicsOnEnterRoom(Room::BOSS_TREASURE);

    curRoom = Room::BOSS_TREASURE;
    screenState = ScreenState::BOSS_RELIC_REWARDS;

    for (int i = 0; i < 3; ++i) {
        info.bossRelics[i] = returnRandomRelic(RelicTier::BOSS);
    }

    regainControlAction = [=](GameContext &gc) {
        gc.transitionToAct(act+1);
    };
}

void GameContext::enterAct3VictoryRoom() {
    ++floorNum;
    relicsOnEnterRoom(Room::BOSS_TREASURE);

    if (hasKey(Key::SAPPHIRE_KEY) && hasKey(Key::EMERALD_KEY) && hasKey(Key::RUBY_KEY)) {
        transitionToAct(4);
    } else {
        outcome = GameOutcome::PLAYER_VICTORY;
    }
}

void GameContext::enterBattle(MonsterEncounter encounter) {
    screenState = ScreenState::BATTLE;
    info.encounter = encounter;
    if (skipBattles) {
        afterBattle();
    }
}

void GameContext::afterBattle() {
    if (hasRelic(RelicId::FACE_OF_CLERIC)) {
        playerIncreaseMaxHp(1);
    }

    switch (curRoom) {
        case Room::MONSTER: {
            regainControlAction = returnToMapAction;
            auto reward = createCombatReward();
            if (info.stolenGold != 0) { // todo stolen gold actually comes first in the list
                reward.addGold(info.stolenGold);
            }
            openCombatRewardScreen(reward);
            break;
        }

        case Room::ELITE:
            regainControlAction = returnToMapAction;
            openCombatRewardScreen(createEliteCombatReward());
            break;

        case Room::BOSS:
            if (act == 1 || act == 2) {
                regainControlAction = [=](GameContext &gc) {
                    gc.enterBossTreasureRoom();
                };
                openCombatRewardScreen(createBossCombatReward());

            } else if (act == 3) {

                if (ascension >= 20 && info.encounter == boss) {
                    // go to second boss
                    ++floorNum;
                    const auto r = Random(seed + floorNum);
                    miscRng = r;
                    shuffleRng = r;
                    cardRandomRng = r;
                    relicsOnEnterRoom(curRoom);

                    regainControlAction = [](GameContext &gc) {
                        gc.afterBattle();
                    };
                    enterBattle(secondBoss);

                } else {
                    // go to next act
                    regainControlAction = [](GameContext &gc) { gc.transitionToAct(gc.act + 1); };
                    enterAct3VictoryRoom();
                }

            } else if (act == 4) {
                outcome = GameOutcome::PLAYER_VICTORY;

            }
            break;

        case Room::EVENT:
            if (curEvent == Event::MYSTERIOUS_SPHERE) { // todo gold from golden idol?
                Rewards reward;
                reward.addGold(info.gold);
                reward.addRelic(info.bossRelics[0]);
                addPotionRewards(reward);
                reward.addCardReward(createCardReward(Room::EVENT));
                openCombatRewardScreen(reward);
                regainControlAction = returnToMapAction;
            } else {
                regainControl();
            }
            break;

        default:
#ifdef sts_asserts
            assert(false);
#endif
            break;
    }
}

void GameContext::obtainKey(Key key) {
    switch (key) {
        case Key::EMERALD_KEY:
            greenKey = true;
            break;

        case Key::SAPPHIRE_KEY:
            blueKey = true;
            break;

        case Key::RUBY_KEY:
            redKey = true;
            break;

        default:
            break;
#ifdef sts_asserts
            assert(false);
#endif
    }
}

void GameContext::obtainCard(Card c, int count) {
    deck.obtain(*this, c, count);
}

void GameContext::obtainGold(int amount) {
    if (relics.has(R::ECTOPLASM)) {
        return;
    }

    gold += amount;
    if (relics.has(R::BLOODY_IDOL)) {
        playerHeal(5);
    }
}

void GameContext::obtainPotion(Potion p) {
    if (relics.has(RelicId::SOZU) || potionCount == potionCapacity) {
        return;
    }

    for (int i = 0; i < potionCapacity; ++i) {
        if (potions[i] == Potion::EMPTY_POTION_SLOT) {
            potions[i] = p;
            ++potionCount;
            return;
        }
    }

    // todo, just ignoring if there is not enough space for now
//#ifdef sts_asserts
//    assert(false);
//#endif
}


bool obtainBottleHelper(GameContext &gc, CardType bottleType) {
    gc.info.toSelectCards.clear();
    gc.deck.addMatchingToSelectList(
            gc.info.toSelectCards,
            [=](auto c) {return c.getType() == bottleType;}
    );
    if (gc.info.toSelectCards.empty()) {
        return false;
    } else {
        gc.openCardSelectScreen(CardSelectScreenType::BOTTLE, 1, false);
        return true;
    }
}


void upgradeRandomCardsMatching(GameContext &gc, CardType type) {
    auto list = gc.deck.getIdxsMatching([=](const auto &c){ return c.canUpgrade() && c.getType() == type; });
    java::Collections::shuffle(list.begin(), list.end(), java::Random(gc.miscRng.randomLong()));
    if (list.size() == 1) {
        gc.deck.upgrade(list[0]);
    } else if (list.size() >= 2) {
        gc.deck.upgrade(list[0]);
        gc.deck.upgrade(list[1]);
    }
}


// relics with screens will call regainControl() after obtaining a relic with a screen
bool GameContext::obtainRelic(RelicId r) {
//    std::cout << "obtained relic: " << relicNames[static_cast<int>(r)] << std::endl;
//    std::cout << *this << std::endl;

    if (relics.has(r)) {
        return false;
    }

#ifdef sts_asserts
    assert(!relics.has(r));
#endif

    relics.setHasRelic(r, true);

    int relicData = 0;
    bool opensScreen = false;

    switch (r) {

        case RelicId::ASTROLABE: {
            info.toSelectCards.clear();
            deck.addMatchingToSelectList(info.toSelectCards, [](auto c){ return c.canTransform();});

            if (info.toSelectCards.empty()) {
                break; // if we have no cards at all
            }

            openCardSelectScreen(CardSelectScreenType::TRANSFORM_UPGRADE, std::min(info.toSelectCards.size(), 3), false);
            opensScreen = true;
            break;
        }

        case RelicId::BLACK_BLOOD: {
            if (hasRelic(RelicId::BURNING_BLOOD)) {
                relics.remove(RelicId::BURNING_BLOOD);
            }
            break;
        }

        case RelicId::BOTTLED_FLAME: {
            opensScreen = obtainBottleHelper(*this, CardType::ATTACK);
            break;
        }

        case RelicId::BOTTLED_LIGHTNING: {
            opensScreen = obtainBottleHelper(*this, CardType::SKILL);
            break;
        }

        case RelicId::BOTTLED_TORNADO: {
            opensScreen = obtainBottleHelper(*this, CardType::POWER);
            break;
        }

        case RelicId::CAULDRON: {
            Potion pReward[5];
            for (int i = 0; i < 5; ++i) {
                pReward[i] = getRandomPotion(potionRng, cc);
            }
            openCombatRewardScreen({pReward, 5});
            opensScreen = true;
            break;
        }

        case RelicId::CALLING_BELL: {
            deck.obtain(*this, CardId::CURSE_OF_THE_BELL);
            Rewards reward;
            reward.addRelic(returnRandomScreenlessRelic(RelicTier::COMMON));
            reward.addRelic(returnRandomScreenlessRelic(RelicTier::UNCOMMON));
            reward.addRelic(returnRandomScreenlessRelic(RelicTier::RARE));
            openCombatRewardScreen(reward);
            opensScreen = true;
            break;
        }

        case RelicId::EMPTY_CAGE: {
            openCardSelectScreen(CardSelectScreenType::REMOVE, 2);
            opensScreen = true;
            break;
        }                                    

        case RelicId::LEES_WAFFLE: {
            playerIncreaseMaxHp(7);
            playerHeal(maxHp);
            break;
        }

        case RelicId::NEOWS_LAMENT: {
            relicData = 3;
            break;
        }

        case RelicId::MANGO: {
            playerIncreaseMaxHp(14);
            break;
        }

        case RelicId::MAW_BANK: {
            relicData = 1;
            break;
        }

        case RelicId::OMAMORI: {
            relicData = 2;
            break;
        }

        case RelicId::PANDORAS_BOX: {
            int transformCount = 0;
            for (int i = deck.size()-1; i >= 0; --i) {
                if (deck.cards[i].isStarterStrikeOrDefend()) {
                    deck.remove(*this, i);
                    ++transformCount;
                }
            }

            CardId toObtain[15]; // 15 is more strikes/defends then we can ever have right?

            for (int i = 0; i < transformCount; ++i) {
                toObtain[i] = getTrulyRandomCard(cardRandomRng, cc);
            }

            for (int i = transformCount-1; i >= 0; --i) {
                deck.obtain(*this, toObtain[i]);
            }
            break;
        }

        case RelicId::PEAR:
            playerIncreaseMaxHp(10);
            break;

        case RelicId::PRISMATIC_SHARD: {
            // todo if not defect: masterMaxOrbs = 1
            break;
        }

        case RelicId::POTION_BELT: {
            potionCapacity += 2;
            potions[potionCapacity-1] = Potion::EMPTY_POTION_SLOT;
            potions[potionCapacity-2] = Potion::EMPTY_POTION_SLOT;
            break;
        }

        case RelicId::STRAWBERRY: {
            playerIncreaseMaxHp(7);
            break;
        }

        case RelicId::TINY_HOUSE: {
            auto upgradeCards = deck.getUpgradeableCardIdxs();
            java::Collections::shuffle(upgradeCards.begin(), upgradeCards.end(), java::Random(miscRng.nextLong()));

            if (!upgradeCards.empty()) {
                deck.upgrade(upgradeCards[0]);
            }

            playerIncreaseMaxHp(5);
            Rewards reward;
            reward.addGold(50);
            reward.addPotion(getRandomPotion(miscRng, cc));
            reward.addCardReward(createCardReward(curRoom));
            openCombatRewardScreen(reward);
            opensScreen = true;
            break;
        }

        case RelicId::WAR_PAINT:
            upgradeRandomCardsMatching(*this, CardType::SKILL);
            break;

        case RelicId::WHETSTONE:
            upgradeRandomCardsMatching(*this, CardType::ATTACK);
            break;

        default:
            break;
    }

    relics.relics.push_back({r, relicData});
    return opensScreen;
}



RelicId GameContext::returnRandomRelic(RelicTier tier, bool shopRoom, bool fromFront) {
    RelicId retVal = RelicId::INVALID;
    std::vector<RelicId> *vec;

    switch(tier) {

        case RelicTier::COMMON:
            if (commonRelicPool.empty()) {
                retVal = returnRandomRelic(RelicTier::UNCOMMON, shopRoom);
            } else {
                vec = &commonRelicPool;
            }
            break;

        case RelicTier::UNCOMMON:
            if (uncommonRelicPool.empty()) {
                retVal = returnRandomRelic(RelicTier::RARE, shopRoom);
            } else {
                vec = &uncommonRelicPool;
            }
            break;

        case RelicTier::RARE:
            if (rareRelicPool.empty()) {
                retVal = RelicId::CIRCLET;
            } else {
                vec = &rareRelicPool;
            }
            break;

        case RelicTier::SHOP:
            if (shopRelicPool.empty()) {
                retVal = returnRandomRelic(RelicTier::UNCOMMON, shopRoom);
            } else {
                vec = &shopRelicPool;
            }
            break;

        case RelicTier::BOSS:
            if (bossRelicPool.empty()) {
                retVal = RelicId::RED_CIRCLET;
            } else {
                vec = &bossRelicPool;
            }
            break;

        default:
            break;
    };

    if (fromFront) {
        retVal = vec->front();
        vec->erase(vec->begin());
    } else {
        retVal = vec->back();
        vec->erase(vec->end()-1);
    }

    bool canSpawn = relicCanSpawn(retVal, shopRoom);
    if (canSpawn) {
        return retVal;
    } else {
        return returnRandomRelic(tier, shopRoom, false);
    }
}

RelicId GameContext::returnNonCampfireRelic(RelicTier tier, bool shopRoom) {
    RelicId relic;
    do {
        relic = returnRandomRelic(tier, shopRoom);
    } while(isCampfireRelic(relic));
    return relic;
}

RelicId GameContext::returnRandomScreenlessRelic(RelicTier tier, bool shopRoom) {
    RelicId r;
    do {
        r = returnRandomRelic(tier, shopRoom);
    }
    while(r == RelicId::BOTTLED_FLAME ||
          r == RelicId::BOTTLED_LIGHTNING ||
          r == RelicId::BOTTLED_TORNADO ||
          r == RelicId::WHETSTONE);

    return r;
}

Card GameContext::previewObtainCard(Card card) {
    switch (card.getType()) {
        case CardType::ATTACK:
            if (hasRelic(RelicId::MOLTEN_EGG)) {
                card.upgrade();
            }
            break;

        case CardType::SKILL:
            if (hasRelic(RelicId::TOXIC_EGG)) {
                card.upgrade();
            }
            break;

        case CardType::POWER:
            if (hasRelic(RelicId::FROZEN_EGG)) {
                card.upgrade();
            }
            break;

        default:
            break;
    }
    return card;
}

void GameContext::relicsOnEnterRoom(Room room) {
    if (hasRelic(RelicId::MAW_BANK) && relics.getRelicValue(RelicId::MAW_BANK) != 0) {
        obtainGold(12);
    }

    switch (room) {
        case Room::REST:
            if (hasRelic(RelicId::ETERNAL_FEATHER)) {
                playerHeal(deck.size() / 5 * 3);
            }
            break;

        case Room::EVENT:
            if (hasRelic(RelicId::SSSERPENT_HEAD)) {
                obtainGold(50);
            }
            break;

        default:
            break;
    }
}

CardRarity GameContext::rollCardRarity(Room room) {
    int roll = cardRng.random(99) + cardRarityFactor;

    if (room == Room::BOSS) {
        return CardRarity::RARE;
    }

    int rareChance = (room == Room::ELITE ? 10 : 3);
    const int uncommonChance = (room == Room::ELITE ? 40 : 37);

    if (room != Room::REST && hasRelic(RelicId::NLOTHS_GIFT)) {
        rareChance = rareChance * 3;
    }

    if (roll < rareChance) {
        return CardRarity::RARE;

    } else if (roll < rareChance + uncommonChance) {
        return CardRarity::UNCOMMON;

    } else {
        return CardRarity::COMMON;
    }
}

CardId GameContext::returnTrulyRandomCardFromAvailable(Random &rng, CardId exclude) {
    auto color = getCardColor(exclude);
    switch (color) {
        case CardColor::COLORLESS: {
            int idx = rng.random(static_cast<int>(colorlessCardPool.size()-2));
            if (colorlessCardPool[idx] == exclude) {
                return colorlessCardPool[idx + 1];
            } else {
                return colorlessCardPool[idx];
            }
        }

        case CardColor::CURSE: {
            return getRandomCurse(cardRng);
        }

        default: {
            const CardId* pool = TransformCardPool::getPoolForClass(cc);
            int poolSize = TransformCardPool::getPoolSizeForClass(cc);

            bool excludeInPool = cardRarities[static_cast<int>(exclude)] != CardRarity::BASIC &&
                                 static_cast<CardColor>(cc) == color;

            if (excludeInPool) {
                int idx = rng.random(poolSize-2);
                if (pool[idx] == exclude) {
                    return pool[idx+1];
                } else {
                    return pool[idx];
                }
            } else {
                return pool[rng.random(poolSize-1)];
            }
        }
    }
}

Card GameContext::getTransformedCard(Random &rng, CardId exclude, bool autoUpgrade) {
    auto color = getCardColor(exclude);

    if (color == CardColor::CURSE) {
        return getRandomCurse(rng, exclude);
    } else  if (color == CardColor::COLORLESS) {
        return returnTrulyRandomColorlessCardFromAvailable(rng, exclude);
    }

    CardId transformCard = returnTrulyRandomCardFromAvailable(rng, exclude);
    return Card(transformCard, autoUpgrade ? 1 : 0);
}

CardId GameContext::returnColorlessCard(CardRarity rarity) {
    java::Collections::shuffle(colorlessCardPool.begin(), colorlessCardPool.end(),
                               java::Random(shuffleRng.randomLong()));
    for (const auto &c : colorlessCardPool) { // todo optimize
        if (getCardRarity(c) == rarity) {
            return c;
        }
    }
    return CardId::SWIFT_STRIKE;
}

int GameContext::getRandomPlayerPotionIdx() {
    if (potionCount <= 0) {
        return -1;
    }

    fixed_list<int, 5> potionIdxs;
    for (int i = 0; i < potionCapacity; ++i) {
        if(potions[i] != Potion::EMPTY_POTION_SLOT) {
            potionIdxs.push_back(i);
        }
    }
    java::Collections::shuffle(potionIdxs.begin(), potionIdxs.end(), java::Random(miscRng.nextLong()));
    return potionIdxs[0];
}

int GameContext::getRandomPlayerNonBasicCardIdx() {
    fixed_list<int,Deck::MAX_SIZE> cardIdxs;

    for (int i = 0; i < deck.cards.size(); ++i) {
        const auto &c = deck.cards[i];
        if (c.getRarity() != CardRarity::BASIC && c.getType() != CardType::CURSE) {
            cardIdxs.push_back(i);
        }
    }

    if (cardIdxs.empty()) {
        return -1;
    }

    java::Collections::shuffle(cardIdxs.begin(), cardIdxs.end(), java::Random(miscRng.randomLong()));

    int retIdx = cardIdxs[0];
    for (int i = 0; i < retIdx; ++i) {
        if (deck.cards[i] == deck.cards[retIdx]) {
            return i;
        }
    }
    return retIdx;
}

MonsterEncounter GameContext::getMonsterForRoomCreation() {
    if (monsterListOffset == monsterList.size()) {
        // the number generated here won't be accurate but it should not matter
        // monster list is empty
        monsterListOffset = 0;
        monsterList.clear();
        generateStrongMonsters(); // game generates 12
    }
    return monsterList[monsterListOffset++];
}

MonsterEncounter GameContext::getEliteForRoomCreation() {
    if (eliteMonsterListOffset == eliteMonsterList.size()) {
        // the number generated here won't be accurate but it should not matter
        // monster list is empty
        eliteMonsterListOffset = 0;
        eliteMonsterList.clear();
        generateElites(); // game generates 10
    }
    return eliteMonsterList[eliteMonsterListOffset++];
}

void GameContext::addPotionRewards(Rewards &r) {
    // assume if in a monsters room, they didnt escape
    int chance = 40 + potionChance;

    if (hasRelic(RelicId::WHITE_BEAST_STATUE)) {
        chance = 100;
    }

    int rewardsSize = r.potionCount + r.relicCount + r.goldRewardCount + r.cardRewardCount;
    if (rewardsSize >= 4) {
        chance = 0;
    }

    if (potionRng.random(99) >= chance) {
        potionChance += 10;

    } else {
        r.addPotion(returnRandomPotion(potionRng, cc));
        potionChance -= 10;
    }
}

CardReward GameContext::createCardReward(Room room) {
    int numCards = 3;
    if (relics.has(RelicId::QUESTION_CARD)) {
        numCards += 1;
    }
    if (relics.has(RelicId::BUSTED_CROWN)) {
        numCards -= 2;
    }

    CardId cards[4];
    CardRarity rewardRarities[4];

    for (int i = 0; i < numCards; ++i) {
        CardRarity rarity = rollCardRarity(room);

        rewardRarities[i] = rarity;
        switch (rarity) {
            case CardRarity::COMMON:
                cardRarityFactor = std::max(cardRarityFactor - 1, -40);
                break;

            case CardRarity::RARE:
                cardRarityFactor = 5;
                break;

            default:
                break;
        }

        CardId id;
        bool hasDuplicate = true;
        while (hasDuplicate) {

            if (hasRelic(RelicId::PRISMATIC_SHARD) && !disablePrismaticShard) {
                id = getAnyColorCard(cardRng, rarity);
            } else {
                id = getRandomClassCardOfRarity(cardRng, cc, rarity);
            }

            hasDuplicate = false;
            for (int x = 0; x < i; ++x) {
                if (cards[x] == id) {
                    hasDuplicate = true;
                    break;
                }
            }
        }
        cards[i] = id;
    }

    float cardUpgradedChance = getUpgradedCardChance(act, ascension);

    CardReward reward;
    for (int i = 0; i < numCards; ++i) {
        if (rewardRarities[i] != CardRarity::RARE && cardRng.randomBoolean(cardUpgradedChance)) {
            reward.push_back(Card(cards[i], true));
        } else {
            reward.push_back(previewObtainCard(cards[i]));
        }
    }
    return reward;
}

CardReward GameContext::createColorlessCardReward() {
    int numCards = 3;
    if (hasRelic(RelicId::QUESTION_CARD)) {
        numCards += 1;
    }
    if (hasRelic(RelicId::BUSTED_CROWN)) {
        numCards -= 2;
    }

    CardReward reward;
    reward.resize(numCards);

    for (int i = 0; i < numCards; ++i) {
        const auto rarity = cardRng.randomBoolean(COLORLESS_RARE_CHANCE) ? CardRarity::RARE : CardRarity::UNCOMMON;
        switch (rarity) {
            case CardRarity::COMMON:
                cardRarityFactor = std::max(cardRarityFactor - 1, -40);
                break;

            case CardRarity::RARE:
                cardRarityFactor = 5;
                break;

            default:
                break;
        }

        CardId id;
        bool hasDuplicate = true;
        while (hasDuplicate) {
            id = getColorlessCardFromPool(cardRng, rarity);

            hasDuplicate = false;
            for (int x = 0; x < i; ++x) {
                if (reward[x].id == id) {
                    hasDuplicate = true;
                    break;
                }
            }
        }
        reward[i] = previewObtainCard(id);
    }
    return reward;
}

void GameContext::openTreasureRoomChest() {
    Rewards reward;

    if (hasRelic(RelicId::MATRYOSHKA) && relics.getRelicValue(RelicId::MATRYOSHKA) > 0) {
        auto matryoshkaTier = getMatryoshkaRelicTier(relicRng);
        auto relic = returnRandomRelic(matryoshkaTier, false);
        reward.addRelic(relic);
        --relics.getRelicValueRef(RelicId::MATRYOSHKA);
    }

    if (hasRelic(RelicId::CURSED_KEY)) {
        deck.obtain(*this, getRandomCurse(cardRng));
    }

    if (info.haveGold) {
        int amount = chestGoldAmounts[static_cast<int>(info.chestSize)];
        amount = std::round(treasureRng.random(amount*0.9f, amount*1.1f));
        reward.addGold(amount);
    }

    auto relic = returnRandomRelic(info.tier, false);
    if (hasRelic(RelicId::NLOTHS_HUNGRY_FACE) && relics.getRelicValue(RelicId::NLOTHS_HUNGRY_FACE) > 0) {
        --relics.getRelicValueRef(RelicId::NLOTHS_HUNGRY_FACE);
    } else {
        reward.addRelic(relic);
    }

    if (!hasKey(Key::SAPPHIRE_KEY)) {
        reward.sapphireKey = true;
    }

    openCombatRewardScreen(reward);
}

Rewards GameContext::createCombatReward() {
    Rewards reward;
    int goldAmt = treasureRng.random(10, 20);
    if (hasRelic(RelicId::GOLDEN_IDOL)) {
        goldAmt += std::round(static_cast<float>(goldAmt) * 0.25f);
    }
    reward.addGold(goldAmt);
    addPotionRewards(reward);
    reward.addCardReward(createCardReward(Room::MONSTER)); // TODO Prayer wheel
    if (hasRelic(RelicId::PRAYER_WHEEL)) {
        reward.addCardReward(createCardReward(Room::MONSTER));
    }
    return reward;
}

Rewards GameContext::createEliteCombatReward() {
    Rewards reward;

    int goldAmt = treasureRng.random(25, 35);
    if (hasRelic(RelicId::GOLDEN_IDOL)) {
        goldAmt += std::round(static_cast<float>(goldAmt) * 0.25f);
    }
    reward.addGold(goldAmt);

    reward.addRelic(returnRandomRelic(returnRandomRelicTierElite(relicRng)));
    if (hasRelic(RelicId::BLACK_STAR)) {
        reward.addRelic(returnNonCampfireRelic(returnRandomRelicTierElite(relicRng)));
    }
    reward.emeraldKey = map->burningEliteX == curMapNodeX && map->burningEliteY == curMapNodeY;
    addPotionRewards(reward);
    reward.addCardReward(createCardReward(Room::ELITE));
    return reward;
}

Rewards GameContext::createBossCombatReward() {
    Rewards reward;
    // room == BOSS
    int goldAmt = 100 + miscRng.random(-5, 5);
    if (ascension >= 13) {
        goldAmt = static_cast<int>(std::round((float)goldAmt * 0.75f));
    }
    if (hasRelic(RelicId::GOLDEN_IDOL)) {
        goldAmt += std::round(static_cast<float>(goldAmt) * 0.25f);
    }
    reward.addGold(goldAmt);
    if (act < 3) {
        addPotionRewards(reward);
        reward.addCardReward(createCardReward(Room::BOSS));
    }
    return reward;
}

Event GameContext::getShrine(Random &eventRngCopy) {  // todo fix, this is slow
    int tempLength = 0;
    Event tempShrines[20];

    for (auto shrine : shrineList) {
        if (shrine != Event::MATCH_AND_KEEP || !disableMatchAndKeep) {
            tempShrines[tempLength++] = shrine;
        }
    }

    for (auto event : specialOneTimeEventList) {
        if (canAddOneTimeEvent(event)) {
            tempShrines[tempLength++] = event;
        }
    }

    const auto idx = eventRngCopy.random(tempLength-1);
    const auto shrine = tempShrines[idx];
    bool didRemove = false;
    for (auto it = shrineList.begin(); it != shrineList.end(); ++it) {
        if (*it == shrine) {
            shrineList.erase(it);
            didRemove = true;
            break;
        }
    }
    for (auto it = specialOneTimeEventList.begin(); it != specialOneTimeEventList.end(); ++it) {
        if (*it == shrine) {
            specialOneTimeEventList.erase(it);
            didRemove = true;
            break;
        }
    }
#ifdef sts_asserts
    assert(didRemove);
#endif
    return shrine;
}

Event GameContext::getEvent(Random &eventRngCopy) {
    int tempLength = 0;
    Event tempEvents[14];

    for (auto event : eventList) {
        if (canAddEvent(event)) {
            tempEvents[tempLength++] = event;
        }
    }

    if (tempLength == 0) {
        return getShrine(eventRng);
    }

    auto idx = eventRngCopy.random(tempLength-1);
    Event event = tempEvents[idx];

    auto eventListPos = std::find(eventList.begin(), eventList.end(), event);
    eventList.erase(eventListPos);
    return tempEvents[idx];
}

Event GameContext::generateEvent(Random eventRngCopy) {
    if (eventRngCopy.random(1.0f) < SHRINE_CHANCE) {
        if (shrineList.empty() && specialOneTimeEventList.empty()) {
            if (eventList.empty()) {
                return sts::Event::INVALID;
            } else {
                return getEvent(eventRngCopy);
            }

        } else {
            return getShrine(eventRngCopy);
        }
    } else {
        return getEvent(eventRngCopy);
    }
}

Room GameContext::getEventRoomOutcomeHelper(bool lastRoomWasShop) {
    float roll = eventRng.random();
    Room choice = Room::NONE;

    if (hasRelic(RelicId::TINY_CHEST)) {
        auto &value = relics.getRelicValueRef(RelicId::TINY_CHEST);
        if (value == 3) {
            value = 0;
            choice = sts::Room::TREASURE;
        } else {
            ++value;
        }
    }

    if (choice != sts::Room::TREASURE) {
        int monsterSize = (int) (monsterChance * 100);
        int shopSize = (lastRoomWasShop ? 0 : (int) (shopChance * 100)) + monsterSize;
        int treasureSize = (int) (treasureChance * 100) + shopSize;

        int idx = (int) (roll * 100);

        if (idx < monsterSize) {
            choice = Room::MONSTER;
        } else if (idx < shopSize) {
            choice = Room::SHOP;
        } else if (idx < treasureSize) {
            choice = Room::TREASURE;
        } else {
            choice = Room::EVENT;
        }
    }

    if (choice == Room::MONSTER) {
        if (hasRelic(RelicId::JUZU_BRACELET)) {
            choice = Room::EVENT;
        }
        monsterChance = 0.1F;
    } else {
        monsterChance += 0.1F;
    }

    if (choice == Room::SHOP) {
        shopChance = 0.03F;
    } else {
        shopChance += 0.03F;
    }

    if (choice == Room::TREASURE) {
        treasureChance = 0.02F;
    } else {
        treasureChance += 0.02F;
    }

    return choice;
}

void GameContext::damagePlayer(int amount) {
#ifdef sts_asserts
    assert(amount > 0);
#endif
    if (amount <= 5 && relics.has(RelicId::TORII)) {
        amount = 1;
    }
    playerLoseHp(amount);
}

void GameContext::playerLoseHp(int amount) {
#ifdef sts_asserts
    assert(amount > 0);
#endif

    if (hasRelic(RelicId::TUNGSTEN_ROD)) {
        amount -= 1;
    }
    curHp -= amount;

    if (curHp <= 0) {
        curHp = 0;
        playerOnDie();
    }
}

void GameContext::playerOnDie() {
    if (hasRelic(RelicId::MARK_OF_THE_BLOOM)) {
        outcome = GameOutcome::PLAYER_LOSS;
        return;
    }

    const auto hasBark = relics.has(RelicId::SACRED_BARK);
    auto it = std::find(potions.begin(), potions.end(), Potion::FAIRY_POTION);
    if (it != potions.end()) {
        *it = Potion::EMPTY_POTION_SLOT;
        --potionCount;
        curHp = std::max(1, static_cast<int>(hasBark ? 0.6f : 0.3f * static_cast<float>(maxHp)));
        return;
    }

    if (hasRelic(RelicId::LIZARD_TAIL)) {
        auto &lizardValue = relics.getRelicValueRef(RelicId::LIZARD_TAIL);
        if (lizardValue != 0) {
            lizardValue = 0;
            curHp = std::max(1, maxHp/2);
            return;
        }
    }

    outcome = GameOutcome::PLAYER_LOSS;
}


void GameContext::playerHeal(int amount) {
    if (hasRelic(RelicId::MARK_OF_THE_BLOOM)) {
        return;
    }
    curHp = std::min(curHp + amount, maxHp);
}

void GameContext::playerIncreaseMaxHp(int amount) {
    maxHp += amount;
    playerHeal(amount);
}

void GameContext::loseGold(int amount, bool inShop) {
    if (inShop && relics.has(RelicId::MAW_BANK)) {
        relics.getRelicValueRef(RelicId::MAW_BANK) = 0;
    }
    gold = std::max(0, gold-amount);
}

void GameContext::loseMaxHp(int amount) {
    maxHp -= amount;
    curHp = std::min(curHp, maxHp);
}

void GameContext::drinkPotion(Potion p) {
    switch (p) {
        case Potion::BLOOD_POTION:
            playerHeal(fractionMaxHp(hasRelic(RelicId::SACRED_BARK) ? 0.40f : 0.20f));
            break;

        case Potion::ENTROPIC_BREW: {
            Potion randPotions[5];
            for (int i = 0 ; i < potionCapacity; ++i) {
                randPotions[i] = returnRandomPotion(potionRng, cc);
                if (potions[i] == Potion::EMPTY_POTION_SLOT) {
                    potions[i] = randPotions[i];
                }
            }
            potionCount = potionCapacity;
            break;
        }

        case Potion::FRUIT_JUICE:
            playerIncreaseMaxHp(hasRelic(RelicId::SACRED_BARK) ? 10 : 5);
            break;

        case Potion::INVALID:
        case Potion::EMPTY_POTION_SLOT:
        default:
#ifdef sts_asserts
            assert(false);
#endif
            break;
    }
}

void GameContext::drinkPotionAtIdx(int idx) {
#ifdef sts_asserts
    assert(!(curEvent == Event::WE_MEET_AGAIN && screenState == ScreenState::EVENT_SCREEN));
    assert(potions[idx] != Potion::EMPTY_POTION_SLOT);
    assert(potions[idx] != Potion::INVALID);
#endif
    const Potion p = potions[idx];
    discardPotionAtIdx(idx);
    drinkPotion(p);
}

void GameContext::discardPotionAtIdx(int idx) {
#ifdef sts_asserts
    assert(!(curEvent == Event::WE_MEET_AGAIN && screenState == ScreenState::EVENT_SCREEN));
    assert(potions[idx] != Potion::EMPTY_POTION_SLOT);
    assert(potions[idx] != Potion::INVALID);
#endif
    potions[idx] = Potion::EMPTY_POTION_SLOT;
    --potionCount;
}

void GameContext::chooseNeowOption(const Neow::Option &o) {

    switch (o.d) {
        case Neow::Drawback::TEN_PERCENT_HP_LOSS:
            maxHp = static_cast<int>(0.9f * static_cast<float>(maxHp));
            curHp = std::min(curHp, maxHp);
            break;

        case Neow::Drawback::NO_GOLD:
            gold = 0;
            break;

        case Neow::Drawback::PERCENT_DAMAGE:
            curHp = curHp / 10 * 7;
            break;

        case Neow::Drawback::LOSE_STARTER_RELIC:
            relics.remove(getStarterRelicForClass(cc));
            break;

        case Neow::Drawback::NONE:
        default:
            break;
    }

    if (o.d == Neow::Drawback::CURSE) {
        regainControlAction = [](auto &gc) {
            int roll = gc.cardRng.random(static_cast<int>(9));
            gc.deck.obtain(gc, curseCardPool[roll]);
            gc.screenState = ScreenState::MAP_SCREEN;
            gc.regainControlAction = returnToMapAction;
        };
    } else {
        regainControlAction = returnToMapAction;
    }

    switch (o.r) {
        case Neow::Bonus::THREE_CARDS:
            regainControlAction(*this); // hack because curse is received first
            regainControlAction = returnToMapAction;
            openCombatRewardScreen(Neow::getCardReward(neowRng, cc, false));
            break;

        case Neow::Bonus::ONE_RANDOM_RARE_CARD: {
            int idx = neowRng.random(RarityCardPool::getPoolSize(cc, CardRarity::RARE) - 1);
            deck.obtain(*this, RarityCardPool::getCardFromPool(cc, CardRarity::RARE, idx), 1);
            regainControlAction(*this);
            break;
        }

        case Neow::Bonus::REMOVE_CARD:
            openCardSelectScreen(CardSelectScreenType::REMOVE, 1);
            break;

        case Neow::Bonus::UPGRADE_CARD:
            openCardSelectScreen(CardSelectScreenType::UPGRADE, 1);
            break;

        case Neow::Bonus::TRANSFORM_CARD:
            info.transformRng = NEOW_RNG;
            openCardSelectScreen(CardSelectScreenType::TRANSFORM, 1);
            break;

        case Neow::Bonus::RANDOM_COLORLESS:
            regainControlAction(*this); // hack because curse is received first
            regainControlAction = returnToMapAction;
            openCombatRewardScreen(Neow::getColorlessCardReward(neowRng, cardRng, false));
            break;

        case Neow::Bonus::THREE_SMALL_POTIONS:  {
            createCardReward(Room::EVENT);
            Potion potions[3];
            for (int i = 0; i < 3; ++i) {
                potions[i] = getRandomPotion(potionRng, cc);
            }
            openCombatRewardScreen(Rewards(potions, 3));
            break;
        }

        case Neow::Bonus::RANDOM_COMMON_RELIC:
            obtainRelic(returnRandomRelic(RelicTier::COMMON, false, true));
            regainControlAction(*this);
            break;

        case Neow::Bonus::TEN_PERCENT_HP_BONUS:
            maxHp += static_cast<int>(static_cast<float>(maxHp) * 0.1f);
            regainControlAction(*this);
            break;

        case Neow::Bonus::THREE_ENEMY_KILL:
            obtainRelic(RelicId::NEOWS_LAMENT);
            regainControlAction(*this);
            break;

        case Neow::Bonus::HUNDRED_GOLD:
            obtainGold(100);
            regainControlAction(*this);
            break;

        case Neow::Bonus::RANDOM_COLORLESS_2:
            regainControlAction(*this); // hack because curse is received first
            regainControlAction = returnToMapAction;
            openCombatRewardScreen(Neow::getColorlessCardReward(neowRng, cardRng, true));
            break;

        case Neow::Bonus::REMOVE_TWO:
            openCardSelectScreen(CardSelectScreenType::REMOVE, 2);
            break;

        case Neow::Bonus::ONE_RARE_RELIC:
            obtainRelic(returnRandomRelic(RelicTier::RARE, false, true));
            regainControlAction(*this);
            break;

        case Neow::Bonus::THREE_RARE_CARDS:
            regainControlAction(*this); // hack because curse is received first
            regainControlAction = returnToMapAction;
            openCombatRewardScreen(Neow::getCardReward(neowRng, cc, true));
            break;

        case Neow::Bonus::TWO_FIFTY_GOLD:
            obtainGold(250);
            regainControlAction(*this);
            break;

        case Neow::Bonus::TRANSFORM_TWO_CARDS:
            info.transformRng = NEOW_RNG;
            openCardSelectScreen(CardSelectScreenType::TRANSFORM, 2);
            {
                int roll = cardRng.random(static_cast<int>(9));
                deck.obtain(*this, curseCardPool[roll]);
            }
            regainControlAction = returnToMapAction;
            break;

        case Neow::Bonus::TWENTY_PERCENT_HP_BONUS:
            maxHp += static_cast<int>(static_cast<float>(maxHp) * 0.2f);
            regainControlAction(*this);
            break;

        case Neow::Bonus::BOSS_RELIC: {
            bool openedScreen = obtainRelic(returnRandomRelic(RelicTier::BOSS, false, true));
            if (!openedScreen) {
                regainControlAction(*this);
            }
            break;
        }

        case Neow::Bonus::INVALID:
            assert(false);
            break;
    }

}

void GameContext::chooseBossRelic(int idx) {
    if (idx >= 0 && idx < 3) {
        bool screenUp = obtainRelic(info.bossRelics[idx]);
        if (!screenUp) {
            regainControl();
        }
    } else if (idx == 3) {
        regainControl();
    }
}


void GameContext::chooseMatchAndKeepCards(int idx1, int idx2) {
    if (idx1 < 0 || idx1 >= 12 || info.toSelectCards[idx1].card.id == CardId::INVALID
        || idx2 == idx1 || idx2 < 0 || idx2 >= 12 || info.toSelectCards[idx2].card.id == CardId::INVALID
            ) {
        assert(false);}

    auto &s1 = info.toSelectCards[idx1].card;
    auto &s2 = info.toSelectCards[idx2].card;

    if (s1.id == s2.id) {
        deck.obtain(*this, s1, 1);
        info.toSelectCards[idx1].deckIdx = 0;
        info.toSelectCards[idx2].deckIdx = 0;
    } else {
        info.toSelectCards[idx1].deckIdx = 1;
        info.toSelectCards[idx2].deckIdx = 1;
    }

    if (--info.eventData == 0) {
        info.toSelectCards.clear();
        info.eventData = 0;
        regainControl();
    }
}

void GameContext::chooseTreasureRoomOption(bool openChest) {
    if (openChest) {
        openTreasureRoomChest();
    } else {
        regainControl();
    }
}

void GameContext::chooseEventOption(int idx) {
//    regainControlAction = [](auto &gs) {
//        gs.screenState = ScreenState::MAP_SCREEN;
//        gs.regainControlAction = nullptr;
//    };
    const bool unfavorable = ascension >= 15;

    switch (curEvent) {
        case Event::NEOW:
            chooseNeowOption(info.neowRewards[idx]);
            break;

        case Event::OMINOUS_FORGE: { // Ominous Forge
            switch (idx) {
                case 0:
                    openCardSelectScreen(CardSelectScreenType::UPGRADE, 1);
                    break;

                case 1:
                    deck.obtain(*this, CardId::PAIN);
                    obtainRelic(RelicId::WARPED_TONGS);
                    regainControl();
                    break;

                case 2:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::PLEADING_VAGRANT: { // Pleading Vagrant
            switch (idx) {
                case 0: {
                    loseGold(85);
                    auto r = returnRandomScreenlessRelic(returnRandomRelicTier(relicRng, act), false);
                    obtainRelic(r);
                    regainControl();
                    break;
                }

                case 1: {
                    auto r = returnRandomScreenlessRelic(returnRandomRelicTier(relicRng, act), false);
                    obtainRelic(r);
                    deck.obtain(*this, CardId::SHAME);
                    regainControl();
                    break;
                }

                case 2:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::ANCIENT_WRITING: { // Ancient Writing
            if (idx == 0) {
                openCardSelectScreen(CardSelectScreenType::REMOVE, 1);

            } else if (idx == 1) {
                deck.upgradeStrikesAndDefends();
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::OLD_BEGGAR: { // Old Beggar
            if (idx == 0) {
                loseGold(75);
                openCardSelectScreen(CardSelectScreenType::REMOVE, 1);

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::BIG_FISH: { // Big Fish
            switch (idx) {
                case 0: {
                    playerHeal(fractionMaxHp(1 / 3.0f));
                    regainControl();
                    break;
                }

                case 1: {
                    playerIncreaseMaxHp(5);
                    regainControl();
                    break;
                }

                case 2: {
                    auto r = returnRandomScreenlessRelic(returnRandomRelicTier(relicRng, act));
                    obtainRelic(r);
                    deck.obtain(*this, CardId::REGRET);
                    regainControl();
                    break;
                }

                default:
                    assert(false);
            }
            break;
        }

        case Event::COLOSSEUM: // todo
            regainControl();
            break;

        case Event::CURSED_TOME: {
            int &phase = info.eventData;
            switch (idx) {
                case 0:
                    ++phase;
                    break;

                case 1:
                    regainControl();
                    break;

                case 2:
                case 3:
                case 4:
                    playerLoseHp(phase);
                    ++phase;
                    break;

                case 5: {
                    playerLoseHp(unfavorable ? 15 : 10);
                    auto res = miscRng.random(2);
                    auto book = res == 0 ? RelicId::NECRONOMICON :
                                (res == 1 ? RelicId::ENCHIRIDION : RelicId::NILRYS_CODEX);

                    Rewards reward;
                    reward.addRelic(book);
                    openCombatRewardScreen(reward);
                    break;
                }

                case 6: {
                    playerLoseHp(3);
                    regainControl();
                    break;
                }

                default:
                    break;
            }
            break;
        }

        case Event::DEAD_ADVENTURER: { // todo map onto unique idxs
            if (idx == 0) {
                int encounterChance = info.phase * 25 + (unfavorable ? 35 : 25);
                bool didEncounter = miscRng.random(99) < encounterChance;

                if (didEncounter) {
                    int goldAmt = miscRng.random(25, 35);
                    bool addRelic = false;
                    RelicId combatRewardRelic;

                    for (int i = info.phase; i < 3; ++i) {
                        if (info.rewards[i] == 0) {
                            goldAmt += 30;
                        } else if (info.rewards[i]  == 2) {
                            addRelic = true;
                            combatRewardRelic = returnRandomRelic(returnRandomRelicTier(relicRng, 1));
                        }
                    }

                    regainControlAction = [=](GameContext &gc) {
                        Rewards reward;
                        reward.addGold(goldAmt);
                        if (addRelic) {
                            reward.addRelic(combatRewardRelic);
                        }
                        reward.addCardReward(createCardReward(Room::EVENT));
                        addPotionRewards(reward);
                        gc.openCombatRewardScreen(reward);
                        gc.regainControlAction = returnToMapAction;
                    };
                    enterBattle(info.encounter);

                } else {
                    auto reward = info.rewards[info.phase];
                    if (reward == 0) {
                        // GOLD
                        obtainGold(30);

                    } else if (reward == 2) {
                        // RELIC
                        auto relic = returnRandomScreenlessRelic(returnRandomRelicTier(relicRng, 1));
                        obtainRelic(relic);
                    }

                    ++info.phase;
                }

            } else if (idx == 1) {
                regainControl();

            } else {
                std::cerr << idx << " " << *this << std::endl;
                assert(false);
            }
            break;
        }

        case Event::DESIGNER_IN_SPIRE: { // Designer In-Spire
            switch (idx) {
                case 0:
                    loseGold(unfavorable ? 50 : 40);
                    openCardSelectScreen(CardSelectScreenType::UPGRADE, 1);
                    break;

                case 1:
                    deck.upgradeRandomCards(miscRng, 2);
                    regainControl();
                    break;

                case 2:
                    loseGold(unfavorable ? 75 : 60);
                    openCardSelectScreen(CardSelectScreenType::REMOVE, 1);
                    break;

                case 3:
                    loseGold(unfavorable ? 75 : 60);
                    deck.transformRandomCards(miscRng, 2); // TODO
                    regainControl();

                case 4:
                    loseGold(unfavorable ? 110 : 90);
                    regainControlAction = [=](GameContext &gc) {
                        gc.deck.upgradeRandomCards(miscRng, 1);
                        returnToMapAction(gc);
                    };
                    openCardSelectScreen(CardSelectScreenType::REMOVE, 1);

                case 5:
                    playerLoseHp(unfavorable ? 5 : 3);
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::AUGMENTER: { // Augmenter
            switch (idx) {
                case 0:
                    deck.obtain(*this, CardId::JAX);
                    regainControl();
                    break;

                case 1:
                    info.transformRng = MISC_RNG;
                    openCardSelectScreen(CardSelectScreenType::TRANSFORM, 2);
                    break;

                case 2:
                    obtainRelic(RelicId::MUTAGENIC_STRENGTH);
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::DUPLICATOR: { //
            if (idx == 0) {
                openCardSelectScreen(CardSelectScreenType::DUPLICATE, 1);

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::FACE_TRADER: { // Face Trader
            switch (idx) {
                case 0:
                    obtainGold(unfavorable ? 50 : 75);
                    damagePlayer(std::max(fractionMaxHp(.10f), 1));
                    regainControl();
                    break;

                case 1:
                    obtainRelic(getRandomFace(relics, miscRng));
                    regainControl();
                    break;

                case 2:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::FALLING: {
            switch (idx) {
                case 0:
                    // Warn!! replace might go around future obtainRelic logic
                    deck.remove(*this, info.skillCardDeckIdx);
                    regainControl();
                    break;

                case 1:
                    deck.remove(*this, info.powerCardDeckIdx);
                    regainControl();
                    break;

                case 2:
                    deck.remove(*this, info.attackCardDeckIdx);
                    regainControl();
                    break;

                case 3:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::FORGOTTEN_ALTAR: {
            switch (idx) {
                case 0:
                    // Warn!! replace might go around future obtainRelic logic
                    relics.replaceRelic(RelicId::GOLDEN_IDOL, RelicId::BLOODY_IDOL);
                    regainControl();
                    break;

                case 1:
                    playerIncreaseMaxHp(5);
                    playerLoseHp(info.hpAmount0);
                    regainControl();
                    break;

                case 2:
                    deck.obtain(*this, CardId::DECAY);
                    regainControl();
                    break;

                default:
                    assert(false);
            }

            break;
        }

        case Event::THE_DIVINE_FOUNTAIN: {
            if (idx == 0) {
                for (int i = deck.size()-1; i >= 0; --i) {
                    if (deck.cards[i].getType() == CardType::CURSE && deck.cards[i].canTransform()) {
                        deck.remove(*this, i);
                    }
                }
                regainControl();

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::GHOSTS: {
            if (idx == 0) {
                loseMaxHp(info.hpAmount0);
                deck.obtain(*this, CardId::APPARITION, unfavorable ? 3 : 5);
                regainControl();

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::GOLDEN_IDOL: {
            switch (idx) {
                case 0:
                    obtainRelic(RelicId::GOLDEN_IDOL);
                    break;

                case 1:
                    regainControl();
                    break;

                case 2:
                    obtainCard(CardId::INJURY);
                    regainControl();
                    break;

                case 3:
                    damagePlayer(info.hpAmount0);
                    regainControl();
                    break;

                case 4:
                    loseMaxHp(info.hpAmount1);
                    regainControl();
                    break;

                default:
                    break;
            }
            break;
        }

        case Event::GOLDEN_SHRINE: { // Golden Shrine
            switch (idx) {
                case 0:
                    obtainGold(unfavorable ? 50 : 100);
                    regainControl();
                    break;

                case 1:
                    obtainGold(275);
                    deck.obtain(*this, CardId::REGRET);
                    regainControl();
                    break;

                case 2:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::WING_STATUE: {
            switch (idx) {
                case 0:
                    damagePlayer(7);
                    openCardSelectScreen(CardSelectScreenType::REMOVE, 1);
                    break;

                case 1:
                    obtainGold(miscRng.random(50, 80));
                    regainControl();
                    break;

                case 2:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::KNOWING_SKULL: {
            switch (idx) {
                case 0:
                    playerLoseHp(info.hpAmount0++);
                    obtainGold(90);
                    break;

                case 1:
                    playerLoseHp(info.hpAmount1++);
                    deck.obtain(*this, returnColorlessCard(CardRarity::UNCOMMON));
                    break;

                case 2:
                    playerLoseHp(info.hpAmount2++);
                    obtainPotion(getRandomPotion(potionRng, cc));
                    break;

                case 3:
                    playerLoseHp(6);
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::LAB:
            // no actions to take
            assert(false);
            break;

        case Event::THE_SSSSSERPENT: { // The Ssssserpent
            if (idx == 0) {
                obtainGold(unfavorable ? 150 : 175);
                deck.obtain(*this, CardId::DOUBT);
                regainControl();

            } else  if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::LIVING_WALL: {
            switch (idx) {
                case 0:
                    openCardSelectScreen(CardSelectScreenType::REMOVE, 1);
                    break;
                case 1:
                    info.transformRng = MISC_RNG;
                    openCardSelectScreen(CardSelectScreenType::TRANSFORM, 1);
                    break;
                case 2:
                    openCardSelectScreen(CardSelectScreenType::UPGRADE, 1);
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::MASKED_BANDITS: {
            if (idx == 0) {
                loseGold(gold);
                regainControl();

            } else if (idx == 1) {
                const int goldAmt = miscRng.random(25, 35);
                regainControlAction = [goldAmt](auto &gc) {
                    Rewards reward;
                    reward.addGold(goldAmt);
                    reward.addRelic(RelicId::RED_MASK);
                    reward.addCardReward(gc.createCardReward(Room::EVENT));
                    gc.openCombatRewardScreen(reward);
                    gc.regainControlAction = returnToMapAction;
                };
                enterBattle(MonsterEncounter::MASKED_BANDITS_EVENT);

            } else {
                assert(false);
            }
            break;
        }

        case Event::MATCH_AND_KEEP:
            // handled elsewhere
            assert(false);
            break;

        case Event::MINDBLOOM: {
            switch (idx) {
                case 0: {
                    MonsterEncounter bosses[3] {
                        MonsterEncounter::THE_GUARDIAN,
                        MonsterEncounter::HEXAGHOST,
                        MonsterEncounter::SLIME_BOSS,
                    };
                    java::Collections::shuffle(bosses, bosses+3, java::Random(miscRng.randomLong()));

                    const int goldAmt = unfavorable ? 25 : 50;
                    const RelicId rareRelic = returnRandomRelic(RelicTier::RARE);
                    regainControlAction = [=](GameContext &gc) {
                        Rewards reward;
                        reward.addGold(goldAmt);
                        reward.addRelic(rareRelic);
                        addPotionRewards(reward);
                        reward.addCardReward(gc.createCardReward(Room::EVENT));
                        gc.openCombatRewardScreen(reward);
                        gc.regainControlAction = returnToMapAction;
                    };
                    enterBattle(bosses[0]);
                    break;
                }

                case 1:
                    for (int i = 0; i < deck.cards.size(); ++i) {
                        if (deck.cards[i].canUpgrade()) {
                            deck.upgrade(i);
                        }
                    }

                    obtainRelic(RelicId::MARK_OF_THE_BLOOM);
                    regainControl();
                    break;

                case 2:
                    obtainGold(999);
                    deck.obtain(*this, CardId::NORMALITY, 2);
                    regainControl();
                    break;

                case 3:
                    playerHeal(maxHp);
                    deck.obtain(*this, CardId::DOUBT);
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::HYPNOTIZING_COLORED_MUSHROOMS: {
            if (idx == 0) {
                const int goldAmt = miscRng.random(20, 30);
                regainControlAction = [=](GameContext &gc) {
                    Rewards reward;
                    reward.addGold(goldAmt);
                    reward.addRelic(RelicId::ODD_MUSHROOM);
                    addPotionRewards(reward);
                    reward.addCardReward(createCardReward(Room::EVENT));
                    gc.openCombatRewardScreen(reward);
                    gc.regainControlAction = returnToMapAction;
                };
                enterBattle(MonsterEncounter::MUSHROOMS_EVENT);

            } else if (idx == 1) {
                obtainGold(unfavorable ? 50 : 99);
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::MYSTERIOUS_SPHERE: {
            if (idx == 0) {
                const int goldAmt = miscRng.random(45, 55);
                const RelicId rareRelic = returnRandomScreenlessRelic(RelicTier::RARE);
                info.bossRelics[0] = rareRelic;
                info.gold = goldAmt;
                regainControlAction = [=](GameContext &gc) {
                    gc.afterBattle();
                };
                enterBattle(MonsterEncounter::MYSTERIOUS_SPHERE_EVENT);

            } else if (idx == 1) {
                obtainGold(unfavorable ? 50 : 99);
                regainControl();

            } else {
                assert(false);
            }

            break;
        }

        case Event::THE_NEST: {
            if (idx == 0) {
                obtainGold(unfavorable ? 50 : 99);
                regainControl();

            } else if (idx == 1) {
                damagePlayer(6);
                deck.obtain(*this, CardId::RITUAL_DAGGER);
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::NLOTH: {
            switch (idx) {
                case 0:
                    relics.remove(relics.relics[info.relicIdx0].id); // maybe add a removeAtIdx method?
                    obtainRelic(RelicId::NLOTHS_GIFT);
                    regainControl();
                    break;

                case 1:
                    relics.remove(relics.relics[info.relicIdx1].id); // maybe add a removeAtIdx method?
                    obtainRelic(RelicId::NLOTHS_GIFT);
                    regainControl();
                    break;

                case 2:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::NOTE_FOR_YOURSELF:
            if (idx == 0) {
                deck.obtain(*this, noteForYourselfCard);
                openCardSelectScreen(CardSelectScreenType::REMOVE, 1);
            } else if (idx == 1) {
                regainControl();
            } else {
                assert(false);
            }
            break;

        case Event::PURIFIER: {
            if (idx == 0) {
                openCardSelectScreen(CardSelectScreenType::REMOVE, 1);

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::SCRAP_OOZE:
            if (idx == 0) {
                damagePlayer(unfavorable ? 5 : 3);
                const int roll = miscRng.random(99);
                const int relicChance = info.eventData*10 + 25;
                if (roll >= 99 - relicChance) {
                    auto relic = returnRandomScreenlessRelic(returnRandomRelicTier(relicRng, 1));
                    obtainRelic(relic);
                    regainControl();
                } else {
                    ++info.eventData;
                }

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;

        case Event::SECRET_PORTAL: {
            if (idx == 0) {
                curMapNodeY = 14;
                transitionToMapNode(0);

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::SENSORY_STONE: {
            if (idx < 0 || idx > 3) {
                assert(false);
            }

            if (idx == 1) {
                playerLoseHp(5);
            } else if (idx == 2) {
                playerLoseHp(10);
            }

            Rewards reward;
            for (int i = 0; i <= idx; ++i) {
                reward.addCardReward(createColorlessCardReward());
            }
            openCombatRewardScreen(reward);
            break;
        }

        case Event::SHINING_LIGHT: {
            if (idx == 0) {
                damagePlayer(info.hpAmount0);
                auto list = deck.getUpgradeableCardIdxs();
                java::Collections::shuffle(list.begin(), list.end(), java::Random(miscRng.randomLong()));
                if (!list.empty()) {
                    if (list.size() == 1) {
                        deck.upgrade(list[0]);
                    } else {
                        deck.upgrade(list[0]);
                        deck.upgrade(list[1]);
                    }
                }
                regainControl();

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::THE_CLERIC: {
            switch (idx) {
                case 0:
                    loseGold(35);
                    playerHeal(info.hpAmount0);
                    regainControl();
                    break;

                case 1:
                    loseGold(unfavorable ? 75 : 50);
                    openCardSelectScreen(CardSelectScreenType::REMOVE, 1);
                    break;

                case 2:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::THE_JOUST: {
            bool ownerWins = miscRng.randomBoolean(0.3f);
            loseGold(50);
            if (idx == 0) {
                if (!ownerWins) {
                    obtainGold(100);
                }

            } else if (idx == 1) {
                if (ownerWins) {
                    obtainGold(250);
                }

            } else {
                assert(false);
            }
            regainControl();
            break;
        }

        case Event::THE_LIBRARY: {
            if (idx == 0) {
                std::bitset<384> selectedBits;
                info.toSelectCards.clear();
                info.toSelectCards.resize(20);
                // cards are added in displayed in reverse order that they are generated
                for (int i = 19; i >= 0; --i) {
                    CardId id;
                    do {
                        id = getRandomClassCardOfRarity(cardRng, cc, rollCardRarity(Room::EVENT));
                    } while (selectedBits.test(static_cast<int>(id)));
                    selectedBits.set(static_cast<int>(id));
                    info.toSelectCards[i] = previewObtainCard(id);
                }
                openCardSelectScreen(CardSelectScreenType::OBTAIN, 1, false);

            } else if (idx == 1) {
                playerHeal(info.hpAmount0);
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::THE_MAUSOLEUM: {
            if (idx == 0) {
                auto result = miscRng.randomBoolean();
                if (result || unfavorable) {
                    deck.obtain(*this, CardId::WRITHE);
                }
                regainControl();

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::THE_MOAI_HEAD: {
            switch (idx) {
                case 0:
                    loseMaxHp(fractionMaxHp(unfavorable ? 0.18f : 0.125f, ROUND));
                    if (maxHp < 1) {
                        maxHp = 1;
                    }
                    if (curHp <= 0 && hasRelic(RelicId::MARK_OF_THE_BLOOM)) {
                        outcome = GameOutcome::PLAYER_LOSS;
                    }
                    playerHeal(maxHp);
                    regainControl();
                    break;


                case 1:
                    obtainGold(333);
                    relics.remove(RelicId::GOLDEN_IDOL);
                    regainControl();
                    break;

                case 2:
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::THE_WOMAN_IN_BLUE: { // The Woman in Blue
            Potion pArr[3];
            switch (idx) {
                case 0: {
                    pArr[0] = getRandomPotion(potionRng, cc);
                    openCombatRewardScreen(Rewards(pArr, 1));
                    break;
                }
                case 1: {
                    pArr[0] = getRandomPotion(potionRng, cc);
                    pArr[1] = getRandomPotion(potionRng, cc);
                    openCombatRewardScreen(Rewards(pArr, 2));
                    break;
                }
                case 2: {
                    pArr[0] = getRandomPotion(potionRng, cc);
                    pArr[1] = getRandomPotion(potionRng, cc);
                    pArr[2] = getRandomPotion(potionRng, cc);
                    openCombatRewardScreen(Rewards(pArr, 3));
                    break;
                }
                case 3: {
                    if (unfavorable) {
                        int hpLoss = std::ceil(static_cast<float>(maxHp) * 0.05f);
                        playerLoseHp(hpLoss);
                        regainControl();
                    } else {
                        regainControl();
                    }
                    break;
                }
            }
            break;
        }

        case Event::TOMB_OF_LORD_RED_MASK: {
            switch (idx) {
                case 0: {
                    obtainGold(222);
                    regainControl();
                    break;
                }
                case 1: {
                    loseGold(gold);
                    obtainRelic(RelicId::RED_MASK);
                    regainControl();
                    break;
                }
                case 2: {
                    regainControl();
                    break;
                }
                default:
                    assert(false);
            }

            break;
        }

        case Event::TRANSMORGRIFIER: {
            if (idx == 0) {
                info.transformRng = MISC_RNG;
                openCardSelectScreen(CardSelectScreenType::TRANSFORM, 1);

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::UPGRADE_SHRINE: {
            if (idx == 0) {
                openCardSelectScreen(CardSelectScreenType::UPGRADE, 1);

            } else if (idx == 1) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::VAMPIRES: { // Vampires(?)
            if (idx == 0) {
                relics.remove(RelicId::BLOOD_VIAL);
            } else if (idx == 1) {
                int maxHpLoss = std::min(maxHp-1, fractionMaxHp(0.3f, CEIL));
                loseMaxHp(maxHpLoss);
            }

            if (idx == 0 || idx == 1) {
                deck.removeAllMatching(*this, []( const auto &card) {
                    return card.isStarterStrike();
                });
                deck.obtain(*this, CardId::BITE, 5);
                regainControl();

            } else if (idx == 2) {
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::WE_MEET_AGAIN: {
            if (idx == 3) {
                regainControl();
                break;
            }

            switch (idx) {
                case 0:
                    potions[info.potionIdx] = Potion::EMPTY_POTION_SLOT;
                    --potionCount; // doing this instead of calling discardPotion because drinkPotionAtIdx/discardPotionAtIdx is not allowed during this event
                    break;

                case 1:
                    loseGold(info.gold);
                    break;

                case 2:
                    deck.remove(*this, info.cardIdx);
                    break;

                default:
                    assert(false);
            }

            auto r = returnRandomScreenlessRelic(returnRandomRelicTier(relicRng, act));
            obtainRelic(r);
            regainControl();
            break;
        }


        case Event::WHEEL_OF_CHANGE: {
            if (idx != 0) {
                assert(false);
            }
            int result = miscRng.random(5);
            switch (result) {
                case 0:
                    obtainGold(act * 100);
                    regainControl();
                    break;

                case 1:
                    openCombatRewardScreen(
                        returnRandomScreenlessRelic(returnRandomRelicTier(relicRng, act))
                    );
                    break;

                case 2:
                    playerHeal(maxHp);
                    regainControl();
                    break;

                case 3:
                    deck.obtain(*this, CardId::DECAY);
                    regainControl();
                    break;

                case 4:
                    openCardSelectScreen(CardSelectScreenType::REMOVE, 1);
                    break;

                case 5:
                default:
                    int hpLoss = fractionMaxHp(unfavorable ? 0.15f : 0.10f);
                    playerLoseHp(hpLoss);
                    regainControl();
                    break;
            }
            break;
        }

        case Event::WINDING_HALLS: {
            switch (idx) {
                case 0:
                    playerLoseHp(info.hpAmount0);
                    deck.obtain(*this, CardId::MADNESS, 2);
                    regainControl();
                    break;

                case 1:
                    playerHeal(info.hpAmount1);
                    deck.obtain(*this, CardId::WRITHE);
                    regainControl();
                    break;

                case 2:
                    loseMaxHp(info.hpAmount2);
                    regainControl();
                    break;

                default:
                    assert(false);
            }
            break;
        }

        case Event::WORLD_OF_GOOP: {
            if (idx == 0) {
                damagePlayer(11);
                obtainGold(75);
                regainControl();

            } else if (idx == 1) {
                loseGold(info.goldLoss);
                regainControl();

            } else {
                assert(false);
            }
            break;
        }

        case Event::INVALID:
        case Event::MONSTER:
        case Event::REST:
        case Event::SHOP:
        case Event::TREASURE:
        default:
            assert(false);
            break;
    }
}

void GameContext::chooseSelectCardScreenOption(int idx) {
    bool isLastCard = info.haveSelectedCards.size() + 1 == info.toSelectCount;
    info.haveSelectedCards.push_back(info.toSelectCards[idx]);
    info.toSelectCards.remove(idx);

    if (!isLastCard) { // todo maybe refactor so dont remove card if last
        return;
    }

    switch (info.selectScreenType) {
        case CardSelectScreenType::TRANSFORM:
            selectScreenTransform();
            regainControl();
            break;

        case CardSelectScreenType::UPGRADE:
            for (const auto &c : info.haveSelectedCards) {
                deck.upgrade(c.deckIdx);
            }
            regainControl();
            break;

        case CardSelectScreenType::REMOVE: {
            deck.removeSelected(*this, info.haveSelectedCards);
            regainControl();
            break;
        }

        case CardSelectScreenType::DUPLICATE:
        case CardSelectScreenType::OBTAIN:
            for (const auto &c : info.haveSelectedCards) {
                deck.obtain(*this, c.card, 1);
            }
            regainControl();
            break;

        case CardSelectScreenType::BOTTLE: {
            const auto selected = info.haveSelectedCards[0];
            deck.bottleCard(selected.deckIdx, selected.card.getType());
            regainControl();
            break;
        }

        case CardSelectScreenType::TRANSFORM_UPGRADE: { // astrolabe
            deck.removeSelected(*this, info.haveSelectedCards);
            for (int i = 0; i < info.haveSelectedCards.size(); ++i) {
                auto id = info.haveSelectedCards[i].card.id;
                deck.obtain(*this, getTransformedCard(miscRng, id, true));
            }
            regainControl();
            break;
        }

        case CardSelectScreenType::BONFIRE_SPIRITS: {
            const SelectScreenCard &c = info.haveSelectedCards[0];
            switch (c.card.getRarity()) {
                case CardRarity::CURSE:
                    obtainRelic(RelicId::SPIRIT_POOP);
                    break;

                case CardRarity::BASIC:
                    break;

                case CardRarity::COMMON:
                case CardRarity::SPECIAL:
                    playerHeal(5);
                    break;

                case CardRarity::UNCOMMON:
                    playerHeal(10);
                    break;

                case CardRarity::RARE:
                    playerIncreaseMaxHp(10);
                    playerHeal(maxHp);
                    break;

                case CardRarity::INVALID:
                default:
                    assert(false);
                    break;
            }
            deck.remove(*this, c.deckIdx);
            screenState = ScreenState::MAP_SCREEN;
            break;
        }


        default:
            assert(false);
            break;
    }

}

void GameContext::chooseCampfireOption(int idx) {
    switch (idx) {
        case 0: { // REST
            int healAmt = fractionMaxHp(0.30f) + (hasRelic(RelicId::REGAL_PILLOW) ? 15 : 0);
            playerHeal(healAmt);
            if (hasRelic(RelicId::DREAM_CATCHER)) {
                openCombatRewardScreen(createCardReward(Room::REST));
            } else {
                regainControl();
            }
            break;
        }

        case 1: { // SMITH
            openCardSelectScreen(CardSelectScreenType::UPGRADE, 1);
            break;
        }

        case 2: { // RECALL
            obtainKey(Key::RUBY_KEY);
            regainControl();
            break;
        }

        case 3: { // LIFT
            ++relics.getRelicValueRef(RelicId::GIRYA);
            regainControl();
            break;
        }

        case 4: { // TOKE
            openCardSelectScreen(CardSelectScreenType::REMOVE, 1);
            regainControl();
            break;
        }

        case 5: { // DIG
            auto r = returnRandomRelic(returnRandomRelicTier(relicRng, act));
            openCombatRewardScreen(Rewards(r));
            break;
        }

        case 6: { // SKIP
            regainControl();
            break;
        }

        default:
            assert(false);
            break;
    }
}

void GameContext::selectScreenTransform() {
    Random *rng;
    switch (info.transformRng) {
        case MISC_RNG:
            rng = &miscRng;
            break;
        case CARD_RNG:
            rng = &cardRng;
            break;
        case NEOW_RNG:
            rng = &neowRng;
            break;
        default:
            assert(false);
            break;
    }

    deck.removeSelected(*this, info.haveSelectedCards);
    for (const auto &c : info.haveSelectedCards) {
        Card transformCard = getTransformedCard(*rng, c.card.id);
        deck.obtain(*this, transformCard);
    }
    info.transformRng = CARD_RNG;
}

void GameContext::openCombatRewardScreen(Rewards reward) {
    info.rewardsContainer = reward;
    screenState = ScreenState::REWARDS;
}

void GameContext::openCardSelectScreen(CardSelectScreenType type, int selectCount, bool initSelectCards) {
    screenState = ScreenState::CARD_SELECT;
    info.selectScreenType = type;
    info.toSelectCount = selectCount;
    info.haveSelectedCards.clear();

    if (initSelectCards) {
        info.toSelectCards.clear();
        switch (type) {

            case CardSelectScreenType::DUPLICATE:
                for (int i = 0; i < deck.size(); ++i) {
                    info.toSelectCards.push_back({deck.cards[i], i});
                }
                break;

            case CardSelectScreenType::REMOVE:

            case CardSelectScreenType::BONFIRE_SPIRITS:
            case CardSelectScreenType::TRANSFORM:
                for (int i = 0; i < deck.size(); ++i) {
                    const auto &c = deck.cards[i];
                    if (c.canTransform() && !deck.isCardBottled(i)) {
                        info.toSelectCards.push_back({c, i});
                    }
                }
                break;

            case CardSelectScreenType::UPGRADE:
                deck.addMatchingToSelectList(info.toSelectCards, [](auto c){ return c.canUpgrade(); });
                break;

            case CardSelectScreenType::TRANSFORM_UPGRADE:
                deck.addMatchingToSelectList(info.toSelectCards, [](auto c){ return c.canTransform(); });
                break;

            default:
                break;
        }
    }
}

void GameContext::regainControl() {
    if (regainControlAction == nullptr) {
        std::cerr << "regain control lambda was null" << "\n";
//        assert(false);
    }
    regainControlAction(*this);
}

