//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_POTIONS_H
#define STS_LIGHTSPEED_POTIONS_H

#include "constants/CharacterClasses.h"

namespace sts {

    enum class Potion : std::uint8_t {
        INVALID = 0,
        EMPTY_POTION_SLOT,
        AMBROSIA,
        ANCIENT_POTION,

        ATTACK_POTION,
        BLESSING_OF_THE_FORGE,
        BLOCK_POTION,
        BLOOD_POTION,

        BOTTLED_MIRACLE,
        COLORLESS_POTION,
        CULTIST_POTION,
        CUNNING_POTION,

        DEXTERITY_POTION,
        DISTILLED_CHAOS,
        DUPLICATION_POTION,
        ELIXIR_POTION,

        ENERGY_POTION,
        ENTROPIC_BREW,
        ESSENCE_OF_DARKNESS,
        ESSENCE_OF_STEEL,

        EXPLOSIVE_POTION,
        FAIRY_POTION,
        FEAR_POTION,
        FIRE_POTION,

        FLEX_POTION, // called SteroidPotion in game code
        FOCUS_POTION,
        FRUIT_JUICE,
        GAMBLERS_BREW,

        GHOST_IN_A_JAR,
        HEART_OF_IRON,
        LIQUID_BRONZE,
        LIQUID_MEMORIES,

        POISON_POTION,
        POTION_OF_CAPACITY,
        POWER_POTION,
        REGEN_POTION,

        SKILL_POTION,
        SMOKE_BOMB,
        SNECKO_OIL,
        SPEED_POTION,

        STANCE_POTION,
        STRENGTH_POTION,
        SWIFT_POTION,
        WEAK_POTION,
    };

    static constexpr const char* potionNames[] {
            "INVALID",
            "EMPTY_POTION_SLOT",
            "Ambrosia",
            "Ancient Potion",
            "Attack Potion",
            "Blessing Of The Forge",
            "Block Potion",
            "Blood Potion",
            "Bottled Miracle",
            "Colorless Potion",
            "Cultist Potion",
            "Cunning Potion",
            "Dexterity Potion",
            "Distilled Chaos",
            "Duplication Potion",
            "Elixir Potion",
            "Energy Potion",
            "Entropic Brew",
            "Essence Of Darkness",
            "Essence Of Steel",
            "Explosive Potion",
            "Fairy Potion",
            "Fear Potion",
            "Fire Potion",
            "Flex Potion",
            "Focus Potion",
            "Fruit Juice",
            "Gamblers Brew",
            "Ghost In A Jar",
            "Heart Of Iron",
            "Liquid Bronze",
            "Liquid Memories",
            "Poison Potion",
            "Potion Of Capacity",
            "Power Potion",
            "Regen Potion",
            "Skill Potion",
            "Smoke Bomb",
            "Snecko Oil",
            "Speed Potion",
            "Stance Potion",
            "Strength Potion",
            "Swift Potion",
            "Weak Potion",
    };

    static constexpr const char* const potionIds[] {
            "INVALID_ID",
            "EMPTY_POTION_ID",
            "Ambrosia",
            "Ancient Potion",
            "AttackPotion",
            "BlessingOfTheForge",
            "Block Potion",
            "BloodPotion",
            "BottledMiracle",
            "ColorlessPotion",
            "CultistPotion",
            "CunningPotion",
            "Dexterity Potion",
            "DistilledChaos",
            "DuplicationPotion",
            "ElixirPotion",
            "Energy Potion",
            "EntropicBrew",
            "EssenceOfDarkness",
            "EssenceOfSteel",
            "Explosive Potion",
            "FairyPotion",
            "FearPotion",
            "Fire Potion",
            "SteroidPotion",
            "FocusPotion",
            "Fruit Juice",
            "GamblersBrew",
            "GhostInAJar",
            "HeartOfIron",
            "LiquidBronze",
            "LiquidMemories",
            "Poison Potion",
            "PotionOfCapacity",
            "PowerPotion",
            "Regen Potion",
            "SkillPotion",
            "SmokeBomb",
            "SneckoOil",
            "SpeedPotion",
            "StancePotion",
            "Strength Potion",
            "Swift Potion",
            "Weak Potion",
    };

    static constexpr const char *potionEnumNames[] = {"INVALID","EMPTY_POTION_SLOT","AMBROSIA","ANCIENT_POTION","ATTACK_POTION","BLESSING_OF_THE_FORGE","BLOCK_POTION","BLOOD_POTION","BOTTLED_MIRACLE","COLORLESS_POTION","CULTIST_POTION","CUNNING_POTION","DEXTERITY_POTION","DISTILLED_CHAOS","DUPLICATION_POTION","ELIXIR_POTION","ENERGY_POTION","ENTROPIC_BREW","ESSENCE_OF_DARKNESS","ESSENCE_OF_STEEL","EXPLOSIVE_POTION","FAIRY_POTION","FEAR_POTION","FIRE_POTION","FLEX_POTION","FOCUS_POTION","FRUIT_JUICE","GAMBLERS_BREW","GHOST_IN_A_JAR","HEART_OF_IRON","LIQUID_BRONZE","LIQUID_MEMORIES","POISON_POTION","POTION_OF_CAPACITY","POWER_POTION","REGEN_POTION","SKILL_POTION","SMOKE_BOMB","SNECKO_OIL","SPEED_POTION","STANCE_POTION","STRENGTH_POTION","SWIFT_POTION","WEAK_POTION",};

    enum class PotionRarity {
        COMMON=0,
        UNCOMMON,
        RARE,
        PLACEHOLDER,
    };


    static constexpr PotionRarity potionRarities[] = {
            PotionRarity::PLACEHOLDER,
            PotionRarity::PLACEHOLDER,
            PotionRarity::RARE,     // Ambrosia
            PotionRarity::UNCOMMON, // Ancient Potion

            PotionRarity::COMMON,   // Attack Potion
            PotionRarity::COMMON,   // Blessing of the Forge
            PotionRarity::COMMON,   // Block Potion
            PotionRarity::COMMON,   // Blood Potion

            PotionRarity::COMMON,   // Bottled Miracle
            PotionRarity::COMMON,   // Colorless Potion
            PotionRarity::RARE,     // Cultist Potion
            PotionRarity::UNCOMMON, // Cunning Potion

            PotionRarity::COMMON,   // Dexterity Potion
            PotionRarity::UNCOMMON, // Distilled Chaos
            PotionRarity::UNCOMMON, // Duplication Potion
            PotionRarity::UNCOMMON, // Elixir Potion

            PotionRarity::COMMON,   // Energy Potion
            PotionRarity::RARE,     // Entropic Brew
            PotionRarity::RARE,     // Essence of Darkness
            PotionRarity::UNCOMMON, // Essence of Steel

            PotionRarity::COMMON,   // Explosive Potion
            PotionRarity::RARE,     // Fairy Potion
            PotionRarity::COMMON,   // Fear Potion
            PotionRarity::COMMON,   // Fire Potion

            PotionRarity::COMMON,   // Flex Potion
            PotionRarity::COMMON,   // Focus Potion
            PotionRarity::RARE,     // Fruit Juice
            PotionRarity::UNCOMMON, // Gamblers Brew

            PotionRarity::RARE,     // Ghost in a Jar
            PotionRarity::RARE,     // Heart of Iron
            PotionRarity::UNCOMMON, // Liquid Bronze
            PotionRarity::UNCOMMON, // Liquid Memories

            PotionRarity::COMMON,   // Poison Potion
            PotionRarity::UNCOMMON, // Potion of Capacity
            PotionRarity::COMMON,   // Power Potion
            PotionRarity::UNCOMMON, // Regen Potion

            PotionRarity::COMMON,   // Skill Potion
            PotionRarity::RARE,     // Smoke Bomb
            PotionRarity::RARE,     // Snecko Oil
            PotionRarity::COMMON,   // Speed Potion

            PotionRarity::UNCOMMON, // Stance Potion
            PotionRarity::COMMON,   // Strength Potion
            PotionRarity::COMMON,   // Swift Potion
            PotionRarity::COMMON,   // Weak Potion
    };


    // common, uncommon, rare
    static constexpr int potionRarityPrices[] {50,75,100};


    namespace PotionPool {

        static constexpr Potion potionPool[4][33] {
                { Potion::BLOOD_POTION, Potion::ELIXIR_POTION, Potion::HEART_OF_IRON, Potion::BLOCK_POTION, Potion::DEXTERITY_POTION, Potion::ENERGY_POTION, Potion::EXPLOSIVE_POTION, Potion::FIRE_POTION, Potion::STRENGTH_POTION, Potion::SWIFT_POTION, Potion::WEAK_POTION, Potion::FEAR_POTION, Potion::ATTACK_POTION, Potion::SKILL_POTION, Potion::POWER_POTION, Potion::COLORLESS_POTION, Potion::FLEX_POTION, Potion::SPEED_POTION, Potion::BLESSING_OF_THE_FORGE, Potion::REGEN_POTION, Potion::ANCIENT_POTION, Potion::LIQUID_BRONZE, Potion::GAMBLERS_BREW, Potion::ESSENCE_OF_STEEL, Potion::DUPLICATION_POTION, Potion::DISTILLED_CHAOS, Potion::LIQUID_MEMORIES, Potion::CULTIST_POTION, Potion::FRUIT_JUICE, Potion::SNECKO_OIL, Potion::FAIRY_POTION, Potion::SMOKE_BOMB, Potion::ENTROPIC_BREW },
                { Potion::POISON_POTION, Potion::CUNNING_POTION, Potion::GHOST_IN_A_JAR, Potion::BLOCK_POTION, Potion::DEXTERITY_POTION, Potion::ENERGY_POTION, Potion::EXPLOSIVE_POTION, Potion::FIRE_POTION, Potion::STRENGTH_POTION, Potion::SWIFT_POTION, Potion::WEAK_POTION, Potion::FEAR_POTION, Potion::ATTACK_POTION, Potion::SKILL_POTION, Potion::POWER_POTION, Potion::COLORLESS_POTION, Potion::FLEX_POTION, Potion::SPEED_POTION, Potion::BLESSING_OF_THE_FORGE, Potion::REGEN_POTION, Potion::ANCIENT_POTION, Potion::LIQUID_BRONZE, Potion::GAMBLERS_BREW, Potion::ESSENCE_OF_STEEL, Potion::DUPLICATION_POTION, Potion::DISTILLED_CHAOS, Potion::LIQUID_MEMORIES, Potion::CULTIST_POTION, Potion::FRUIT_JUICE, Potion::SNECKO_OIL, Potion::FAIRY_POTION, Potion::SMOKE_BOMB, Potion::ENTROPIC_BREW },
                { Potion::FOCUS_POTION, Potion::POTION_OF_CAPACITY, Potion::ESSENCE_OF_DARKNESS, Potion::BLOCK_POTION, Potion::DEXTERITY_POTION, Potion::ENERGY_POTION, Potion::EXPLOSIVE_POTION, Potion::FIRE_POTION, Potion::STRENGTH_POTION, Potion::SWIFT_POTION, Potion::WEAK_POTION, Potion::FEAR_POTION, Potion::ATTACK_POTION, Potion::SKILL_POTION, Potion::POWER_POTION, Potion::COLORLESS_POTION, Potion::FLEX_POTION, Potion::SPEED_POTION, Potion::BLESSING_OF_THE_FORGE, Potion::REGEN_POTION, Potion::ANCIENT_POTION, Potion::LIQUID_BRONZE, Potion::GAMBLERS_BREW, Potion::ESSENCE_OF_STEEL, Potion::DUPLICATION_POTION, Potion::DISTILLED_CHAOS, Potion::LIQUID_MEMORIES, Potion::CULTIST_POTION, Potion::FRUIT_JUICE, Potion::SNECKO_OIL, Potion::FAIRY_POTION, Potion::SMOKE_BOMB, Potion::ENTROPIC_BREW },
                { Potion::BOTTLED_MIRACLE, Potion::STANCE_POTION, Potion::AMBROSIA, Potion::BLOCK_POTION, Potion::DEXTERITY_POTION, Potion::ENERGY_POTION, Potion::EXPLOSIVE_POTION, Potion::FIRE_POTION, Potion::STRENGTH_POTION, Potion::SWIFT_POTION, Potion::WEAK_POTION, Potion::FEAR_POTION, Potion::ATTACK_POTION, Potion::SKILL_POTION, Potion::POWER_POTION, Potion::COLORLESS_POTION, Potion::FLEX_POTION, Potion::SPEED_POTION, Potion::BLESSING_OF_THE_FORGE, Potion::REGEN_POTION, Potion::ANCIENT_POTION, Potion::LIQUID_BRONZE, Potion::GAMBLERS_BREW, Potion::ESSENCE_OF_STEEL, Potion::DUPLICATION_POTION, Potion::DISTILLED_CHAOS, Potion::LIQUID_MEMORIES, Potion::CULTIST_POTION, Potion::FRUIT_JUICE, Potion::SNECKO_OIL, Potion::FAIRY_POTION, Potion::SMOKE_BOMB, Potion::ENTROPIC_BREW },
        };
        constexpr int poolSize = 33;

        static constexpr Potion getPotionForClass(CharacterClass cc, int idx) {
            return potionPool[static_cast<int>(cc)][idx];
        }

    }

    static const char* getPotionName(Potion p) {
        return potionNames[static_cast<int>(p)];
    }

    static constexpr PotionRarity getPotionRarity(Potion p) {
        return potionRarities[static_cast<int>(p)];
    }

    static constexpr int getPotionBaseCost(Potion p) {
        return potionRarityPrices[static_cast<int>(getPotionRarity(p))];
    }

    static constexpr bool potionRequiresTarget(Potion p) {
        switch (p) {
            case Potion::FEAR_POTION:
            case Potion::FIRE_POTION:
            case Potion::POISON_POTION:
            case Potion::WEAK_POTION:
                return true;

            default:
                return false;
        }
    }

}

#endif //STS_LIGHTSPEED_POTIONS_H
