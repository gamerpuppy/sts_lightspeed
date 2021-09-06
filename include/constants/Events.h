//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_EVENTS_H
#define STS_LIGHTSPEED_EVENTS_H

#include <array>

namespace sts {

    enum class Event {
        INVALID = 0,
        MONSTER,
        REST,
        SHOP,
        TREASURE,
        NEOW,
        ACCURSED_BLACKSMITH,
        ADDICT,
        BACK_TO_BASICS,
        BEGGAR,
        BIG_FISH,
        BONFIRE_ELEMENTALS,
        COLOSSEUM,
        CURSED_TOME,
        DEAD_ADVENTURER,
        DESIGNER,
        DRUG_DEALER,
        DUPLICATOR,
        FACE_TRADER,
        FALLING,
        FORGOTTEN_ALTAR,
        FOUNTAIN_OF_CLEANSING,
        GHOSTS,
        GOLDEN_IDOL,
        GOLDEN_SHRINE,
        GOLDEN_WING,
        KNOWING_SKULL,
        LAB,
        LIARS_GAME,
        LIVING_WALL,
        MASKED_BANDITS,
        MATCH_AND_KEEP,
        MINDBLOOM,
        MUSHROOMS,
        MYSTERIOUS_SPHERE,
        NEST,
        NLOTH,
        NOTE_FOR_YOURSELF,
        PURIFIER,
        SCRAP_OOZE,
        SECRET_PORTAL,
        SENSORY_STONE,
        SHINING_LIGHT,
        THE_CLERIC,
        THE_JOUST,
        THE_LIBRARY,
        THE_MAUSOLEUM,
        THE_MOAI_HEAD,
        THE_WOMAN_IN_BLUE,
        TOMB_OF_LORD_RED_MASK,
        TRANSMORGRIFIER,
        UPGRADE_SHRINE,
        VAMPIRES,
        WE_MEET_AGAIN,
        WHEEL_OF_CHANGE,
        WINDING_HALLS,
        WORLD_OF_GOOP,
    };

    static constexpr const char *eventIdStrings[] = {
            "INVALID",
            "MONSTER",
            "REST",
            "SHOP",
            "TREASURE",
            "NEOW",
            "Accursed Blacksmith",
            "Addict",
            "Back to Basics",
            "Beggar",
            "Big Fish",
            "Bonfire Elementals",
            "Colosseum",
            "Cursed Tome",
            "Dead Adventurer",
            "Designer",
            "Drug Dealer",
            "Duplicator",
            "Face Trader",
            "Falling",
            "Forgotten Altar",
            "Fountain of Cleansing",
            "Ghosts",
            "Golden Idol",
            "Golden Shrine",
            "Golden Wing",
            "Knowing Skull",
            "Lab",
            "Liars Game",
            "Living Wall",
            "Masked Bandits",
            "Match and Keep",
            "Mindbloom",
            "Mushrooms",
            "Mysterious Sphere",
            "Nest",
            "Nloth",
            "Note For Yourself",
            "Purifier",
            "Scrap Ooze",
            "Secret Portal",
            "Sensory Stone",
            "Shining Light",
            "The Cleric",
            "The Joust",
            "The Library",
            "The Mausoleum",
            "The Moai Head",
            "The Woman in Blue",
            "Tomb of Lord Red Mask",
            "Transmorgrifier",
            "Upgrade Shrine",
            "Vampires",
            "WeMeetAgain",
            "Wheel of Change",
            "Winding Halls",
            "World of Goop",
    };

    static constexpr const char *eventEnumNames[] = {"INVALID","MONSTER","REST","SHOP","TREASURE","NEOW","ACCURSED_BLACKSMITH","ADDICT","BACK_TO_BASICS","BEGGAR","BIG_FISH","BONFIRE_ELEMENTALS","COLOSSEUM","CURSED_TOME","DEAD_ADVENTURER","DESIGNER","DRUG_DEALER","DUPLICATOR","FACE_TRADER","FALLING","FORGOTTEN_ALTAR","FOUNTAIN_OF_CLEANSING","GHOSTS","GOLDEN_IDOL","GOLDEN_SHRINE","GOLDEN_WING","KNOWING_SKULL","LAB","LIARS_GAME","LIVING_WALL","MASKED_BANDITS_EVENT","MATCH_AND_KEEP","MINDBLOOM","MUSHROOMS_EVENT","MYSTERIOUS_SPHERE_EVENT","NEST","NLOTH","NOTE_FOR_YOURSELF","PURIFIER","SCRAP_OOZE","SECRET_PORTAL","SENSORY_STONE","SHINING_LIGHT","THE_CLERIC","THE_JOUST","THE_LIBRARY","THE_MAUSOLEUM","THE_MOAI_HEAD","THE_WOMAN_IN_BLUE","TOMB_OF_LORD_RED_MASK","TRANSMORGRIFIER","UPGRADE_SHRINE","VAMPIRES","WE_MEET_AGAIN","WHEEL_OF_CHANGE","WINDING_HALLS","WORLD_OF_GOOP",};

    static constexpr const char *eventGameNames[] = {
            "INVALID",
            "MONSTER",
            "REST",
            "SHOP",
            "TREASURE",
            "NEOW",
            "Ominous Forge",
            "Pleading Vagrant",
            "Ancient Writing",
            "Old Beggar",
            "Big Fish",
            "Bonfire Spirits",
            "The Colosseum",
            "Cursed Tome",
            "Dead Adventurer",
            "Designer In-Spire",
            "Augmenter",
            "Duplicator",
            "Face Trader",
            "Falling",
            "Forgotten Altar",
            "The Divine Fountain",
            "Council of Ghosts",
            "Golden Idol",
            "Golden Shrine",
            "Wing Statue",
            "Knowing Skull",
            "Lab",
            "The Ssssserpent",
            "Living Wall",
            "Masked Bandits",
            "Match and Keep",
            "Mindbloom",
            "Hypnotizing Colored Mushrooms",
            "Mysterious Sphere",
            "The Nest",
            "N'loth",
            "Note For Yourself",
            "Purifier",
            "Scrap Ooze",
            "Secret Portal",
            "Sensory Stone",
            "Shining Light",
            "The Cleric",
            "The Joust",
            "The Library",
            "The Mausoleum",
            "The Moai Head",
            "The Woman in Blue",
            "Tomb of Lord Red Mask",
            "Transmorgrifier",
            "Upgrade Shrine",
            "Vampires(?)",
            "We Meet Again!",
            "Wheel of Change",
            "Winding Halls",
            "World of Goop",
    };

    namespace EventPools {

        const std::array<Event,14> oneTimeEventsAsc0 { Event::ACCURSED_BLACKSMITH, Event::BONFIRE_ELEMENTALS, Event::DESIGNER, Event::DUPLICATOR, Event::FACE_TRADER, Event::FOUNTAIN_OF_CLEANSING, Event::KNOWING_SKULL, Event::LAB, Event::NLOTH, Event::NOTE_FOR_YOURSELF, Event::SECRET_PORTAL, Event::THE_JOUST, Event::WE_MEET_AGAIN, Event::THE_WOMAN_IN_BLUE };
        const std::array<Event,13> oneTimeEventsAsc15 { Event::ACCURSED_BLACKSMITH, Event::BONFIRE_ELEMENTALS, Event::DESIGNER, Event::DUPLICATOR, Event::FACE_TRADER, Event::FOUNTAIN_OF_CLEANSING, Event::KNOWING_SKULL, Event::LAB, Event::NLOTH, Event::SECRET_PORTAL, Event::THE_JOUST, Event::WE_MEET_AGAIN, Event::THE_WOMAN_IN_BLUE };

        namespace Act1 {
            const std::array<Event,11> events { Event::BIG_FISH, Event::THE_CLERIC, Event::DEAD_ADVENTURER, Event::GOLDEN_IDOL, Event::GOLDEN_WING, Event::WORLD_OF_GOOP, Event::LIARS_GAME, Event::LIVING_WALL, Event::MUSHROOMS, Event::SCRAP_OOZE, Event::SHINING_LIGHT };
            const std::array<Event,6> shrines { Event::MATCH_AND_KEEP, Event::GOLDEN_SHRINE, Event::TRANSMORGRIFIER, Event::PURIFIER, Event::UPGRADE_SHRINE, Event::WHEEL_OF_CHANGE };
        }

        namespace Act2 {
            const std::array<Event,13> events { Event::ADDICT, Event::BACK_TO_BASICS, Event::BEGGAR, Event::COLOSSEUM, Event::CURSED_TOME, Event::DRUG_DEALER, Event::FORGOTTEN_ALTAR, Event::GHOSTS, Event::MASKED_BANDITS, Event::NEST, Event::THE_LIBRARY, Event::THE_MAUSOLEUM, Event::VAMPIRES };
            const std::array<Event,6> shrines { Event::MATCH_AND_KEEP, Event::WHEEL_OF_CHANGE, Event::GOLDEN_SHRINE, Event::TRANSMORGRIFIER, Event::PURIFIER, Event::UPGRADE_SHRINE };
        }

        namespace Act3 {
            const std::array<Event,7> events { Event::FALLING, Event::MINDBLOOM, Event::THE_MOAI_HEAD, Event::MYSTERIOUS_SPHERE, Event::SENSORY_STONE, Event::TOMB_OF_LORD_RED_MASK, Event::WINDING_HALLS };
            const std::array<Event,6> shrines { Event::MATCH_AND_KEEP, Event::WHEEL_OF_CHANGE, Event::GOLDEN_SHRINE, Event::TRANSMORGRIFIER, Event::PURIFIER, Event::UPGRADE_SHRINE };
        }

    }

}


#endif //STS_LIGHTSPEED_EVENTS_H
