//
// Created by gamerpuppy on 7/8/2021.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdint>
#include <thread>
#include <memory>

#include "data_structure/fixed_list.h"
#include "constants/Cards.h"
#include "constants/Events.h"
#include "constants/CardPools.h"
#include "game/Game.h"
#include "game/Map.h"
#include "game/Neow.h"
#include "game/SaveFile.h"
#include "combat/BattleContext.h"
#include "sim/ConsoleSimulator.h"
#include "sim/PrintHelpers.h"
#include "sim/RandomAgent.h"
#include "sim/StateHandler.h"
#include "sim/BattleScumSearcher.h"
#include "sim/search/ScumSearchAgent2.h"

#include "sim/search/BattleScumSearcher2.h"

using namespace sts;

int findIdxInArr(const CardId *arr, CardId item) {
    int i = 0;
    while (arr[i] != item) {
        ++i;
    }
    return i;
}

std::map<std::string,CardId> getCardIdStringMap() {
    std::map<std::string,CardId> ret;
    for (int i = 0; i <= static_cast<int>(CardId::ZAP); ++i) {
        CardId id = static_cast<CardId>(i);
        std::string strId = getCardStringId(id);
        ret[strId] = id;
    }
    return ret;
}

void printBaseDamageNumbers() {
    std::array<std::string, 122> strings {"Immolate","Grand Finale","Go for the Eyes","Core Surge","Glass Knife","Consecrate","BowlingBash","Underhanded Strike","Anger","WheelKick","Cleave","Ball Lightning","Sunder","FlyingSleeves","Streamline","Dagger Spray","Reaper","Shiv","Bane","JustLucky","Unload","FlurryOfBlows","Compile Driver","TalkToTheHand","Dagger Throw","WindmillStrike","Iron Wave","Reckless Charge","All For One","Dramatic Entrance","Hemokinesis","Blizzard","Choke","Poisoned Stab","Body Slam","Barrage","Blood for Blood","Meteor Strike","Clash","CarveReality","Wallop","Thunderclap","Rebound","Endless Agony","SashWhip","Melter","Pummel","Riddle With Holes","Pommel Strike","Skewer","Quick Slash","Twin Strike","Bash","RitualDagger","Gash","Clothesline","Rampage","Sever Soul","Eruption","Whirlwind","Bite","LessonLearned","CutThroughFate","ReachHeaven","Finisher","Die Die Die","Ragnarok","FearNoEvil","SandsOfTime","Smite","Fiend Fire","Sweeping Beam","FTL","Rip and Tear","Heel Hook","Headbutt","Expunger","Wild Strike","HandOfGreed","Eviscerate","Flash of Steel","Heavy Blade","Searing Blow","Lockon","Dash","Conclude","ThroughViolence","Backstab","FollowUp","Scrape","Feed","Beam Cell","Bludgeon","Slice","Brilliance","Cold Snap","CrushJoints","Flechettes","Tantrum","Perfected Strike","Strike_B","Thunder Strike","Carnage","Masterful Stab","Dropkick","Swift Strike","Strike_G","Hyperbeam","Sword Boomerang","Weave","SignatureMove","Uppercut","Mind Blast","Neutralize","Doom and Gloom","Sucker Punch","Strike_R","Strike_P","EmptyFist","All Out Attack","Flying Knee","Predator",};
    std::array<int, 122> baseDamage {21,50,3,11,8,5,7,12,6,15,8,7,24,4,15,4,4,4,7,3,14,4,7,5,9,7,5,7,10,8,15,0,12,6,0,4,18,24,14,6,9,4,9,4,8,10,2,3,9,7,8,5,8,15,3,12,8,16,9,5,7,10,7,10,6,13,5,8,20,12,7,6,5,7,5,9,9,12,20,7,3,14,12,8,10,12,20,11,7,7,10,3,32,6,12,6,8,4,3,6,6,7,20,12,5,7,6,26,3,4,30,13,0,3,10,7,6,6,9,10,8,15,};
    std::array<int, 122> upDamage {28,60,4,15,12,8,10,16,8,20,11,10,32,6,20,6,5,6,10,4,18,6,10,7,12,10,7,10,14,12,20,0,12,8,0,6,22,30,18,10,12,7,12,6,10,14,2,4,10,10,12,7,10,15,5,14,8,22,9,8,8,13,9,15,8,17,6,11,26,16,10,9,6,9,8,12,15,17,25,9,6,14,16,11,13,16,30,15,11,10,12,4,42,9,16,9,10,6,3,6,9,9,28,16,8,10,9,34,3,6,40,13,0,4,14,9,9,9,14,14,11,20,};

    struct MyStruct {
        CardId id;
        int baseDamage = -1;
        int upDamage = -1;
    };

    std::array<MyStruct, 371> infos;
    auto map = getCardIdStringMap();

    for (int i = 0; i < 371; ++i) {
        auto id = static_cast<CardId>(i);
        infos[i].id = id;

        auto it = std::find(strings.begin(), strings.end(), std::string(cardStringIds[i]));
        if (it != strings.end()) {
            int origIdx = it-strings.begin();
            infos[i].baseDamage = baseDamage[origIdx];
            infos[i].upDamage = upDamage[origIdx];
        }
    }

    for (int i = 0; i < 371; ++i) {
        auto x = infos[i];
        std::cout << (x.baseDamage >= 10 ? "1" : "0");
    }
    std::cout << "\n";

    for (int i = 0; i < 371; ++i) {
        auto x = infos[i];
        std::cout << (x.upDamage >= 10 ? "1" : "0");
    }
    std::cout << "\n";
}

void printCardTargets() {
    std::array<std::string, 371> strings {"Immolate","Grand Finale","Regret","Crippling Poison","Storm","DeusExMachina","A Thousand Cuts","Spot Weakness","Genetic Algorithm","Go for the Eyes","Zap","Steam Power","Wound","Core Surge","Fission","Writhe","Beta","Hello World","Creative AI","Dark Shackles","Glass Knife","Consecrate","Cloak And Dagger","BowlingBash","Underhanded Strike","Anger","Storm of Steel","WheelKick","Cleave","Ball Lightning","Warcry","Sunder","Glacier","Inflame","Sadistic Nature","J.A.X.","Offering","Vengeance","FlyingSleeves","Exhume","Streamline","Wireheading","Consume","Power Through","Dual Wield","Deadly Poison","Leg Sweep","PanicButton","Flex","Redo","AscendersBane","Dagger Spray","Bullet Time","Fusion","Catalyst","Sanctity","Halt","Reaper","Shiv","Bane","Tactician","JustLucky","Infernal Blade","After Image","Unload","FlurryOfBlows","Blade Dance","Deflect","Compile Driver","TalkToTheHand","BattleHymn","Protect","Trip","Indignation","Dagger Throw","Amplify","ThirdEye","Brutality","Night Terror","WindmillStrike","Iron Wave","Reboot","Reckless Charge","All For One","ForeignInfluence","Decay","FameAndFortune","Tools of the Trade","Aggregate","Expertise","Dramatic Entrance","Hemokinesis","Blizzard","Chaos","LiveForever","Intimidate","Echo Form","Necronomicurse","Juggernaut","Choke","Caltrops","Impatience","DevaForm","Poisoned Stab","The Bomb","Blur","LikeWater","Body Slam","True Grit","Insight","Setup","Barrage","Crescendo","SpiritShield","Blood for Blood","Impervious","ClearTheMind","EmptyBody","Shrug It Off","Meteor Strike","Establishment","Fasting2","Clash","Stack","Miracle","CarveReality","Wallop","Thunderclap","Rebound","Flame Barrier","Seek","Endless Agony","WreathOfFlame","Collect","SashWhip","Wraith Form v2","Melter","Berserk","Pummel","Burning Pact","Riddle With Holes","Metallicize","Self Repair","Pommel Strike","Pain","Rainbow","InnerPeace","Burst","Acrobatics","Adaptation","Loop","Blind","Doppelganger","Skewer","Omniscience","Envenom","Chill","Adrenaline","Quick Slash","Twin Strike","BootSequence","Parasite","Bash","RitualDagger","Gash","Wish","Clothesline","DeceiveReality","MentalFortress","Shockwave","BecomeAlmighty","Rampage","Coolheaded","Static Discharge","Alpha","Heatsinks","Vault","Bandage Up","Scrawl","Sever Soul","Eruption","Whirlwind","Bite","LessonLearned","Secret Technique","Calculated Gamble","Tempest","Combust","Deep Breath","Doubt","Escape Plan","CutThroughFate","ReachHeaven","Finisher","Dark Embrace","Die Die Die","Well Laid Plans","Ragnarok","Buffer","Electrodynamics","FearNoEvil","Seeing Red","SandsOfTime","Smite","Violence","Disarm","Turbo","Panache","Undo","Fiend Fire","Terror","Force Field","Dazed","Barricade","Armaments","Havoc","Secret Weapon","Apotheosis","Sweeping Beam","Feel No Pain","FTL","Rip and Tear","Darkness","Corruption","Heel Hook","Blasphemy","Injury","Double Energy","Rage","Headbutt","Machine Learning","Reinforced Body","Defend_P","Limit Break","Entrench","Noxious Fumes","Infinite Blades","Phantasmal Killer","WaveOfTheHand","Malaise","Conserve Battery","Defend_R","Mayhem","Reflex","Study","Expunger","Sentinel","Survivor","Wild Strike","Defend_G","HandOfGreed","Meditate","Eviscerate","Flash of Steel","Defend_B","Battle Trance","Forethought","Dualcast","Auto Shields","Perseverance","Swivel","Heavy Blade","Slimed","Clumsy","Biased Cognition","Searing Blow","Devotion","Reprogram","Hologram","Corpse Explosion","Second Wind","Enlightenment","Purity","Panacea","Lockon","Dash","Worship","Conclude","ThroughViolence","Transmutation","Ghostly","Backstab","Chrysalis","FollowUp","Void","Scrape","Feed","Vigilance","Rupture","Venomology","Discovery","Beam Cell","Leap","CurseOfTheBell","Bouncing Flask","PathToVictory","Bludgeon","Finesse","Slice","Recycle","Backflip","Outmaneuver","Bloodletting","Brilliance","Magnetism","Concentrate","Skim","White Noise","Capacitor","Cold Snap","CrushJoints","Master of Strategy","Flechettes","Tantrum","Perfected Strike","Strike_B","Thunder Strike","Carnage","Masterful Stab","Nirvana","Evaluate","Prepared","Good Instincts","Dropkick","Swift Strike","Normality","Strike_G","MasterReality","Omega","Hyperbeam","Accuracy","Sword Boomerang","EmptyMind","Pride","Defragment","Jack Of All Trades","Demon Form","Fire Breathing","Ghostly Armor","Weave","Safety","Metamorphosis","Prostrate","SignatureMove","Uppercut","PiercingWail","Mind Blast","Neutralize","Multi-Cast","Shame","Doom and Gloom","Evolve","Double Tap","Sucker Punch","Burn","ConjureBlade","Strike_R","Judgement","Footwork","Strike_P","Steam","Distraction","Dodge and Roll","Thinking Ahead","EmptyFist","All Out Attack","Flying Knee","Predator","Pray","Madness",};
    std::array<std::string, 371> targets {"ALL_ENEMY","ALL_ENEMY","NONE","ALL_ENEMY","SELF","SELF","SELF","SELF_AND_ENEMY","SELF","ENEMY","SELF","SELF","NONE","ENEMY","NONE","NONE","NONE","SELF","SELF","ENEMY","ENEMY","ALL_ENEMY","SELF","ENEMY","ENEMY","ENEMY","NONE","ENEMY","ALL_ENEMY","ENEMY","SELF","ENEMY","SELF","SELF","SELF","SELF","SELF","NONE","ENEMY","NONE","ENEMY","NONE","SELF","SELF","NONE","ENEMY","ENEMY","SELF","SELF","SELF","NONE","ALL_ENEMY","NONE","SELF","ENEMY","SELF","SELF","ALL_ENEMY","ENEMY","ENEMY","NONE","ENEMY","NONE","SELF","ENEMY","ENEMY","NONE","SELF","ENEMY","ENEMY","SELF","SELF","ENEMY","NONE","ENEMY","SELF","SELF","SELF","NONE","ENEMY","ENEMY","SELF","ENEMY","ENEMY","NONE","NONE","NONE","SELF","SELF","SELF","ALL_ENEMY","ENEMY","ALL_ENEMY","SELF","NONE","ALL_ENEMY","SELF","NONE","SELF","ENEMY","SELF","NONE","SELF","ENEMY","SELF","SELF","NONE","ENEMY","SELF","SELF","NONE","ENEMY","SELF","SELF","ENEMY","SELF","SELF","SELF","SELF","ENEMY","SELF","SELF","ENEMY","SELF","NONE","ENEMY","ENEMY","ALL_ENEMY","ENEMY","SELF","NONE","ENEMY","SELF","SELF","ENEMY","SELF","ENEMY","SELF","ENEMY","NONE","ENEMY","SELF","SELF","ENEMY","NONE","SELF","SELF","SELF","NONE","SELF","SELF","ENEMY","SELF","ENEMY","NONE","SELF","SELF","SELF","ENEMY","ENEMY","SELF","NONE","ENEMY","ENEMY","ENEMY","NONE","ENEMY","SELF","SELF","ALL_ENEMY","NONE","ENEMY","SELF","SELF","NONE","SELF","ALL","SELF","NONE","ENEMY","ENEMY","ALL_ENEMY","ENEMY","ENEMY","NONE","NONE","SELF","SELF","SELF","NONE","SELF","ENEMY","ENEMY","ENEMY","SELF","ALL_ENEMY","NONE","ALL_ENEMY","SELF","SELF","ENEMY","NONE","ENEMY","ENEMY","NONE","ENEMY","SELF","SELF","SELF","ENEMY","ENEMY","SELF","NONE","SELF","SELF","NONE","NONE","NONE","ALL_ENEMY","SELF","ENEMY","ALL_ENEMY","SELF","SELF","ENEMY","SELF","NONE","SELF","SELF","ENEMY","SELF","SELF","SELF","SELF","SELF","SELF","SELF","SELF","SELF","ENEMY","SELF","SELF","SELF","NONE","SELF","ENEMY","SELF","SELF","ENEMY","SELF","ENEMY","NONE","ENEMY","ENEMY","SELF","NONE","NONE","NONE","SELF","SELF","SELF","ENEMY","SELF","NONE","SELF","ENEMY","NONE","NONE","SELF","ENEMY","SELF","SELF","NONE","SELF","ENEMY","ENEMY","SELF","ALL_ENEMY","ENEMY","SELF","SELF","ENEMY","NONE","ENEMY","NONE","ENEMY","ENEMY","SELF","SELF","SELF","SELF","ENEMY","SELF","NONE","ALL_ENEMY","ENEMY","ENEMY","SELF","ENEMY","SELF","SELF","NONE","SELF","ENEMY","SELF","SELF","NONE","NONE","SELF","ENEMY","ENEMY","NONE","ENEMY","ENEMY","ENEMY","ENEMY","ALL_ENEMY","ENEMY","ENEMY","SELF","SELF","NONE","SELF","ENEMY","ENEMY","NONE","ENEMY","SELF","SELF","ALL_ENEMY","SELF","ALL_ENEMY","SELF","SELF","SELF","NONE","NONE","SELF","SELF","ENEMY","SELF","NONE","SELF","ENEMY","ENEMY","ALL_ENEMY","ENEMY","ENEMY","NONE","NONE","ALL_ENEMY","SELF","SELF","ENEMY","NONE","SELF","ENEMY","ENEMY","SELF","ENEMY","SELF","NONE","SELF","NONE","ENEMY","ALL_ENEMY","ENEMY","ENEMY","SELF","SELF",};


    struct MyStruct {
        CardId id;
        bool requiresTarget = false;
    };

    std::array<MyStruct, 371> infos;
    auto map = getCardIdStringMap();

    for (int i = 0; i < 371; ++i) {
        auto id = static_cast<CardId>(i);
        infos[i].id = id;

        auto it = std::find(strings.begin(), strings.end(), std::string(cardStringIds[i]));
        if (it != strings.end()) {
            int origIdx = it-strings.begin();
            auto target = targets[origIdx];
            if (target == "ALL_ENEMY" ||
                target == "NONE" ||
                target == "SELF" ||
                target == "ALL") {
                infos[i].requiresTarget = false;

            } else if (target == "ENEMY" ||
                target == "ALL_ENEMY" ||
                target == "SELF_AND_ENEMY") {
                infos[i].requiresTarget = true;

            } else {
                assert(false);

            }
        }
    }

    std::cout << "{";
    for (int i = 0; i < 371; ++i) {
        auto x = infos[i];
        std::cout << (x.requiresTarget ? "true" : "false") << ",";
    }
    std::cout << "};\n";

    std::cout << "\n";
}


void printAnyColorCardPool(CardRarity rarity) {
    std::map<std::string, CardId> id2CardMap;
    std::vector<std::string> ids;
    for (int i = 0; i < 371; ++i) {
        if (
                cardRarities[i] == rarity &&
                cardTypes[i] != sts::CardType::STATUS &&
                cardTypes[i] != sts::CardType::CURSE)
        {
            id2CardMap[cardStringIds[i]] = static_cast<CardId>(i);
            ids.push_back(cardStringIds[i]);
        }
    }

    std::sort(ids.begin(), ids.end());

    std::cout << ids.size() << '\n';
    std::cout << "{";
    for (auto strId : ids) {
        auto cardId = id2CardMap[strId];
        std::cout << "CardId::" << getCardEnumName(cardId) << ",";
    }
    std::cout << "}\n";
}

void printAnyColorCardPools() {
    printAnyColorCardPool(CardRarity::COMMON);
    printAnyColorCardPool(CardRarity::UNCOMMON);
    printAnyColorCardPool(CardRarity::RARE);
}



void searchForNeowsBossRelic(RelicId id, CharacterClass cc) {
    for (std::uint64_t seed = 0; seed < 10000; seed++) {
        GameContext gc(seed, cc, 0);
        gc.chooseNeowOption(gc.info.neowRewards[3]);

        if (gc.relics.has(id)) {
            std::cout << SeedHelper::getString(seed) << '\n';
        }

    }
}

void printCardInfo(CardId id) {
    std::cout << getCardName(id) << " " << getCardEnumName(id) << " " << getCardStringId(id) << " " << cardTypeStrings[static_cast<int>(
            getCardType(id))] << " " << cardRarityStrings[static_cast<int>(getCardRarity(id))] << " " << cardColorStrings[static_cast<int>(getCardColor(id))] <<'\n';
}

void runBenchmark3(std::string saveFile, std::string actionList, int loopCountThousands) {
    BattleContext::sum = 0;

    std::ifstream fis(actionList);
    std::string actionListStr;
    copy(std::istreambuf_iterator<char>(fis), std::istreambuf_iterator<char>(), back_inserter(actionListStr));


    auto save = SaveFile::loadFromPath(saveFile, sts::CharacterClass::IRONCLAD);
    GameContext gameContext;
    gameContext.initFromSave(save);

    auto start = std::chrono::high_resolution_clock::now();

    for (std::int64_t i = 0; i < loopCountThousands * 1000LL; ++i) {
        std::istringstream iss(actionListStr);

        SimulatorContext simContext;
        simContext.printInput = false;
        simContext.printFirstLine = false;
        simContext.printLogActions = false;
        simContext.printPrompts = false;

        ConsoleSimulator sim;
        sim.gc = new GameContext(gameContext);
        if (sim.gc->screenState == ScreenState::BATTLE) {
            sim.battleSim.initBattle(*sim.gc);
        }

        sim.play(iss, std::cout, simContext);
        delete sim.gc;
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish-start;
    std::int64_t timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "saveFile: " << saveFile << " actionList: " << actionList << '\n';
    std::cout << "millis: " << timeMillis << " runsPerSecond: " << ((double)(loopCountThousands*1000*1000)/timeMillis) << '\n';
    std::cout << "sum: " << BattleContext::sum << '\n';
}

std::string getSeedWithGuardian() {
    std::uint64_t seed = 40;
    while (true) {
        GameContext gc(seed, sts::CharacterClass::IRONCLAD, 0);
        if (gc.boss == sts::MonsterEncounter::THE_GUARDIAN) {
            break;
        }

        ++seed;
    }

    return SeedHelper::getString(seed);
}

void replayActionList(std::string filename) {
    std::ifstream fis(filename);

    ConsoleSimulator sim;
    SimulatorContext simContext;

    sim.play(fis, std::cout, simContext);
    simContext.printFirstLine = true;
    sim.play(std::cin, std::cout, simContext);
}

void printSizes() {
    std::cout << "sizeof Map:" << sizeof(Map) << '\n';
    std::cout << "sizeof Player: " << sizeof(Player) << '\n';
    std::cout << "sizeof Monster: " << sizeof(Monster) << '\n';
    std::cout << "sizeof MonsterGroup : " << sizeof(MonsterGroup) << '\n';
    std::cout << "sizeof CardInstance: " << sizeof(CardInstance) << '\n';
    std::cout << "sizeof CardManager : " << sizeof(CardManager) << '\n';
    std::cout << "sizeof ActionFunction : " << sizeof(ActionFunction) << '\n';
    std::cout << "sizeof ActionQueue<40> : " << sizeof(ActionQueue<40>) << '\n';
    std::cout << "sizeof BattleContext: " << sizeof(BattleContext) << '\n';


    std::cout << "sizeof GameContext: " << sizeof(GameContext) << '\n';
    std::cout << "sizeof Deck: " << sizeof(Deck) << '\n';
    std::cout << "sizeof Card: " << sizeof(Card) << '\n';
    std::cout << "sizeof SelectScreenCard: " << sizeof(SelectScreenCard) << '\n';

    std::cout << "sizeof RandomBattleStateHandler: " << sizeof(RandomBattleStateHandler) << '\n';
}

void playFromSaveFile(const std::string &fname, const std::string &actionFile) {

    CharacterClass cc;
    switch (tolower(fname[0])) {
        case 'i':
            cc = sts::CharacterClass::IRONCLAD;
        default:
            cc = sts::CharacterClass::IRONCLAD;
    }

    SaveFile saveFile = SaveFile::loadFromPath(fname, cc);

    ConsoleSimulator sim;
    sim.setupGameFromSaveFile(saveFile);
    SimulatorContext simContext;


    std::ifstream actionListInputStream(actionFile);

    sim.play(actionListInputStream, std::cout, simContext);
    actionListInputStream.close();

//    simContext.printFirstLine = true;
    simContext.quitCommandGiven = false;
    sim.play(std::cin, std::cout, simContext);
}

//void playRandom() {
//    RandomAgent agent(std::default_random_engine(0));
//    for (std::uint64_t seed = 1; seed < 100; ++seed) {
//        GameContext gc(seed, CharacterClass::IRONCLAD, 0);
//        gc.skipBattles = true;
//        agent.playoutBattle(gc);
//    }
//}

void playRandom2(std::uint64_t startSeed, int playoutCount) {
    int lossCount = 0;
    int victoryCount = 0;
    int maxVictoryHp = 0;
    std::int64_t maxVictoryHpSeed = 0;

    for (std::int64_t seed = startSeed; seed < startSeed+playoutCount; ++seed) {
        RandomAgent agent((std::default_random_engine(seed)));
        agent.print = true;
        agent.printStartingAt = 0;
        GameContext gc(seed, CharacterClass::IRONCLAD, 0);
        agent.playoutWithBattles(gc);

        if (gc.act == 2) {
            ++victoryCount;
            if (gc.curHp > maxVictoryHp) {
                maxVictoryHp = gc.curHp;
                maxVictoryHpSeed = seed;
            }
        } else {
            ++lossCount;
        }
    }

    std::cout << "w/l: (" << victoryCount  << ", " << lossCount
        << ") maxVictoryHp: " << maxVictoryHp
        << " seed: " << SeedHelper::getString(maxVictoryHpSeed)
        << '\n';
}


struct PlayRandomInfo {
    std::uint64_t startSeed;
    std::uint64_t endSeed;
    int seedIncrement;
    int seedOffset;

    std::int64_t winCount = 0;
    std::int64_t lossCount = 0;
    std::int64_t floorSum = 0;

    std::int64_t nodeEvalTotal = 0;
};

void playRandom3(PlayRandomInfo *info) {

    for (std::uint64_t seed = info->startSeed + info->seedOffset; seed < info->endSeed; seed += info->seedIncrement) {
        RandomAgent agent( (std::default_random_engine(seed)) );
        agent.print = false;

        GameContext gc(seed, CharacterClass::IRONCLAD, 20);
        agent.playoutWithBattles(gc);

        if (gc.act == 2) {
            ++info->winCount;
        } else {
            ++info->lossCount;
        }
        info->floorSum += gc.floorNum;
//        std::cout << seed << '\n';
    }

//    std::cout << "thread finished: " << info->seedOffset << std::endl;
}

static int g_searchAscension = 0;
static int g_simulationCount = 5;

void playRandom4(PlayRandomInfo *info) {
    for (std::uint64_t seed = info->startSeed + info->seedOffset; seed < info->endSeed; seed += info->seedIncrement) {
        GameContext gc(seed, CharacterClass::IRONCLAD, g_searchAscension);
        search::ScumSearchAgent2::Settings settings;
        settings.simulationCountBase = g_simulationCount;
        settings.rng = std::default_random_engine(gc.seed);
        settings.printLogs = false;

        search::ScumSearchAgent2 agent(settings);
        agent.playout(gc);

        info->floorSum += gc.floorNum;

        if (gc.outcome == sts::GameOutcome::PLAYER_VICTORY) {
            ++info->winCount;
//            std::cout << gc << std::endl;
            std::cout << seed << " won at floor " << gc.floorNum << " against "
                << monsterEncounterStrings[static_cast<int>(gc.info.encounter)];
            std::cout << " " << gc.deck << " " << gc.relics << std::endl;

        } else {
            ++info->lossCount;
            std::cout << seed << " lost at floor " << gc.floorNum << " "
                << roomStrings[static_cast<int>(gc.curRoom)] << " ";
            if (gc.curRoom == sts::Room::EVENT) {
                std::cout << eventGameNames[static_cast<int>(gc.curEvent)];
            } else if (gc.curRoom == Room::BOSS || gc.curRoom == Room::ELITE || gc.curRoom == Room::MONSTER) {
                std::cout << monsterEncounterStrings[static_cast<int>(gc.info.encounter)];
            }
            std::cout << " " << gc.deck << " " << gc.relics << std::endl;
        }
    }
}

void playRandomMt(int threadCount, std::uint64_t startSeed, int playoutCount) {
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::unique_ptr<std::thread>> threads;

    std::vector<PlayRandomInfo> infos;
    for (int tid = 0; tid < threadCount; ++tid) {
        PlayRandomInfo info;
        info.startSeed = startSeed;
        info.seedOffset = tid;
        info.endSeed = startSeed + playoutCount;
        info.seedIncrement = threadCount;

        infos.push_back(info);
    }
    if (threadCount == 1) { // doing this for more consistency when benchmarking
        playRandom4(&infos[0]);
    } else {
        for (int tid = 0; tid < threadCount; ++tid) {
            threads.emplace_back(new std::thread(playRandom4, &infos[tid]));
        }
    }

    std::int64_t winCount = 0;
    std::int64_t lossCount = 0;
    std::int64_t floorSum = 0;
    std::int64_t nodeSearchSum = 0;


    for (int tid = 0; tid < threadCount; ++tid) {
        if (threadCount > 1) {
            threads[tid]->join();
        }
        winCount += infos[tid].winCount;
        lossCount += infos[tid].lossCount;
        floorSum += infos[tid].floorSum;
        nodeSearchSum += infos[tid].nodeEvalTotal;
    }


    auto endTime = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(endTime-startTime).count();

    std::cout << "w/l: (" << winCount  << ", " << lossCount << ")"
        << " percentWin: " << static_cast<double>(winCount) / playoutCount * 100 << "%"
        << " avgFloorReached: " << static_cast<double>(floorSum) / playoutCount << '\n'
        << " nodesSearched: " << nodeSearchSum << " avgPerFloor: " << (double)nodeSearchSum/floorSum << '\n';

    std::cout << "threads: " << threadCount
              << " playoutCount: " << playoutCount
              << " depth: " << g_simulationCount
        << " asc: " << g_searchAscension
        << " elapsed: " << duration
        << std::endl;
}

int scumSearch(int argc, const char *argv[]) {
    const auto depth = std::stoi(argv[2]);
    const auto minTurnLookahead = std::stoi(argv[3]);
    const auto saveFilePath = argv[4];

    SaveFile saveFile = SaveFile::loadFromPath(saveFilePath, sts::CharacterClass::IRONCLAD);
    GameContext gc;
    gc.initFromSave(saveFile);

    BattleContext bc;
    bc.init(gc);

    BattleScumSearcher scumSearcher(depth, minTurnLookahead);

    auto startTime = std::chrono::high_resolution_clock::now();

    scumSearcher.search(bc);

    auto endTime = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(endTime-startTime).count();

    std::cout << " search time: " << duration << "s\n";

    for (int i = 0; i < scumSearcher.bestInfos.size(); ++i) {
        std::cout << "depth [" << i << "]: " << scumSearcher.bestInfos[i] << '\n';

    }
    return 0;
}


int mcts(int argc, const char *argv[]) {
    const auto saveFilePath = argv[2];
    const auto simulationCount = std::stoll(argv[3]);

    SaveFile saveFile = SaveFile::loadFromPath(saveFilePath, sts::CharacterClass::IRONCLAD);
    GameContext gc;
    gc.initFromSave(saveFile);

    std::cout << SeedHelper::getString(gc.seed) << std::endl;

    BattleContext bc = BattleContext();
    bc.init(gc);

    search::BattleScumSearcher2 searcher(bc);

    auto startTime = std::chrono::high_resolution_clock::now();
    searcher.search(simulationCount);
    auto endTime = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(endTime-startTime).count();

    std::cout << "steps: " << simulationCount << " search time: " << duration << "s\n";
    std::cout << "best search value: " << searcher.bestActionValue << " depth: " << searcher.bestActionSequence.size() << '\n';
    if (searcher.bestActionSequence.empty()) {
        std::cout << "bestActionSequenceIsEmpty" << std::endl;
        return 0;
    }

    for (auto bestAction : searcher.bestActionSequence) {
        bestAction.printDesc(std::cout, bc) << '\n';
        bestAction.execute(bc);
    }

    std::cout << "ending hp: " << bc.player.curHp << '\n';

    searcher.printSearchTree(std::cout, 3);

    std::cout.flush();
    return 0;
}

int main(int argc, const char* argv[]) {

    if (argc < 2) {
        std::cout << "incorrect arguments" << std::endl;
        return 0;
    }

    const std::string command(argv[1]);

    if (command == "save") {
        playFromSaveFile(argv[2], argv[3]);

    } else if (command == "benchmark") {
        runBenchmark3(argv[2], argv[3], 3);

    } else if (command == "random") {
        const std::uint64_t startSeedLong(std::stoull(argv[2]));
        const int playoutCount(std::stoi(argv[3]));
        playRandom2(startSeedLong, playoutCount);

    } else if (command == "random_mt") { // actually doing tree search now
        const int threadCount(std::stoi(argv[2]));
        const int depthArg = std::stoi(argv[3]);
        const int ascensionIn = std::stoi(argv[4]);
        const std::uint64_t startSeedLong(std::stoull(argv[5]));
        const int playoutCount(std::stoi(argv[6]));

        g_searchAscension = ascensionIn;
        g_simulationCount = depthArg;

        playRandomMt(threadCount, startSeedLong, playoutCount);

    } else if (command == "scum_search") {
        return scumSearch(argc, argv);

    } else if (command == "json") {
        const std::string saveFilePath(argv[2]);
        const std::string jsonOutPath(argv[3]);
        std::ofstream outFileStream(jsonOutPath);
        outFileStream << SaveFile::getJsonFromSaveFile(saveFilePath);
        outFileStream.close();

    } else if (command == "json_to_save") {
        const std::string jsonInPath(argv[2]);
        const std::string saveFileOutPath(argv[3]);

        std::ifstream jsonIfStream(jsonInPath);
        SaveFile::writeJsonToSaveFile(jsonIfStream, saveFileOutPath);

    }  else if (command == "scum_searcher") {
        const std::uint64_t startSeedLong(std::stoull(argv[2]));
        const int playoutCount(std::stoi(argv[3]));

        for (std::uint64_t seed = startSeedLong; seed < startSeedLong+playoutCount; ++seed) {
//            playRandom4(startSeedLong);
        }


    } else if (command == "mcts_save") {
        return mcts(argc, argv);
    }

    //    printSizes();
//    std::cout << SeedHelper::getString(77) << '\n';
//    playRandom();
//    std::cout << getSeedWithGuardian();
//    replayActionList(argv[1]);

    return 0;
}