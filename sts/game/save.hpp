//
// Created by gamerpuppy on 7/8/2021.
//

#ifndef STS_LIGHTSPEED_SAVEFILE_H
#define STS_LIGHTSPEED_SAVEFILE_H

#include <iostream>
#include <string>

#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>

#include <sts/constants/events.hpp>
#include <sts/constants/monster_encounters.hpp>
#include <sts/constants/relic_ids.hpp>
#include <sts/constants/character_classes.hpp>
#include <sts/constants/potions.hpp>

#include <sts/game/neow.hpp>
#include <sts/game/card.hpp>

namespace sts {

    class GameContext;

    namespace Save {

        enum class RoomType {
            INVALID,
            EMPTY_ROOM,
            EVENT_ROOM,
            MONSTER_ROOM,
            MONSTER_ROOM_BOSS,
            MONSTER_ROOM_ELITE,
            REST_ROOM,
            SHOP_ROOM,
            TREASURE_ROOM,
            TREASURE_ROOM_BOSS,
            TRUE_VICTORY_ROOM,
            VICTORY_ROOM,
        };

        enum class CombatRewardType {
            INVALID,
            GOLD,
            CARD,
            POTION,
            RELIC,
            STOLEN_GOLD,
            EMERALD_KEY,
            SAPPHIRE_KEY,
        };

        struct CombatReward {
            CombatRewardType type;
            int amount = -1;
            int bonusGold = -1;
            CardId cardId = CardId::INVALID;
            Potion potionId = Potion::INVALID;
            RelicId relicId = RelicId::INVALID;
        };

    }

    struct Base64 {
        static constexpr char chars[] = {
                'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                '0','1','2','3','4','5','6','7','8','9','+','/'
        };

        static char decodeChar(int c); // returns -1 if invalid char
        static char encodeChar(int dataMod64); // data in low bits

        static std::string decode(const std::string &base64Str);
        static std::string encode(const std::string &data);
    };

    struct SaveFile {
        std::string json;

        std::uint64_t seed;
        CharacterClass cc;
        int ascension_level;
        int act_num;
        int gold;
        int purgeCost;
        int current_health;
        int max_health;

        int play_time;
        int room_x;
        int room_y;
        int floor_num;
        bool post_combat;
        bool smoked;
        bool mugged;
        Save::RoomType current_room;

        int potion_seed_count;
        int relic_seed_count;
        int event_seed_count;
        int monster_seed_count;
        int merchant_seed_count;
        int card_random_seed_count;
        int card_seed_count;
        int treasure_seed_count;

        bool has_emerald_key;
        bool has_ruby_key;
        bool has_sapphire_key;

        int card_random_seed_randomizer;
        int potion_chance;
        float monsterChance;
        float shopChance;
        float treasureChance;

        bool chose_neow_reward;
        Neow::Bonus neow_bonus;
        Neow::Drawback neow_cost;

        std::vector<Potion> potions;

        std::vector<Card> cards;
        std::array<CardId, 3> bottledCards;

        std::vector<RelicId> relics;
        std::vector<int> relic_counters;
        std::vector<Save::CombatReward> combat_rewards;

        std::vector<RelicId> boss_relics;
        std::vector<RelicId> shop_relics;
        std::vector<RelicId> common_relics;
        std::vector<RelicId> uncommon_relics;
        std::vector<RelicId> rare_relics;

        std::vector<Event> one_time_event_list;
        std::vector<Event> event_list;
        std::vector<MonsterEncounter> monster_list;
        std::vector<MonsterEncounter> elite_monster_list;
        std::vector<MonsterEncounter> boss_list;

        SaveFile() = default;
        SaveFile(const SaveFile &rhs) = default;
        SaveFile(const std::string &json, CharacterClass cc);

        static SaveFile loadFromPath(const std::string& path, CharacterClass cc);

        static std::string getJsonFromSaveFile(const std::string &savePath);
        static void writeJsonToSaveFile(std::ifstream &jsonIs, const std::string &savePath);
        static std::string readFileToStringHelper(const std::string &path);
    };

}

#endif //STS_LIGHTSPEED_SAVEFILE_H
