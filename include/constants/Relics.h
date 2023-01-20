//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_RELICS_H
#define STS_LIGHTSPEED_RELICS_H

#include <cstdint>
#include <cassert>
#include <algorithm>
#include <array>
#include <string>

namespace sts {

    enum class RelicId : std::uint8_t {
        // battle relics -- relics which state is needed in battle
        AKABEKO = 0,
        ART_OF_WAR,
        BIRD_FACED_URN,
        BLOODY_IDOL,
        BLUE_CANDLE,
        BRIMSTONE,
        CALIPERS,
        CAPTAINS_WHEEL,
        CENTENNIAL_PUZZLE,
        CERAMIC_FISH,
        CHAMPION_BELT,
        CHARONS_ASHES,
        CHEMICAL_X,
        CLOAK_CLASP,
        DARKSTONE_PERIAPT,
        DEAD_BRANCH,
        DUALITY,
        ECTOPLASM,
        EMOTION_CHIP,
        FROZEN_CORE,
        FROZEN_EYE,
        GAMBLING_CHIP,
        GINGER,
        GOLDEN_EYE,
        GREMLIN_HORN,
        HAND_DRILL,
        HAPPY_FLOWER,
        HORN_CLEAT,
        HOVERING_KITE,
        ICE_CREAM,
        INCENSE_BURNER,
        INK_BOTTLE,
        INSERTER,
        KUNAI,
        LETTER_OPENER,
        LIZARD_TAIL,
        MAGIC_FLOWER,
        MARK_OF_THE_BLOOM,
        MEDICAL_KIT,
        MELANGE,
        MERCURY_HOURGLASS,
        MUMMIFIED_HAND,
        NECRONOMICON,
        NILRYS_CODEX,
        NUNCHAKU,
        ODD_MUSHROOM,
        OMAMORI,
        ORANGE_PELLETS,
        ORICHALCUM,
        ORNAMENTAL_FAN,
        PAPER_KRANE,
        PAPER_PHROG,
        PEN_NIB,
        PHILOSOPHERS_STONE,
        POCKETWATCH,
        RED_SKULL,
        RUNIC_CUBE,
        RUNIC_DOME,
        RUNIC_PYRAMID,
        SACRED_BARK,
        SELF_FORMING_CLAY,
        SHURIKEN,
        SNECKO_EYE,
        SNECKO_SKULL,
        SOZU,
        STONE_CALENDAR,
        STRANGE_SPOON,
        STRIKE_DUMMY,
        SUNDIAL,
        THE_ABACUS,
        THE_BOOT,
        THE_SPECIMEN,
        TINGSHA,
        TOOLBOX,
        TORII,
        TOUGH_BANDAGES,
        TOY_ORNITHOPTER,
        TUNGSTEN_ROD,
        TURNIP,
        TWISTED_FUNNEL,
        UNCEASING_TOP,
        VELVET_CHOKER,
        VIOLET_LOTUS,
        WARPED_TONGS,
        WRIST_BLADE,

        // end of battle relics
        BLACK_BLOOD,
        BURNING_BLOOD,
        MEAT_ON_THE_BONE,
        FACE_OF_CLERIC,

        // beginning of battle relics
        ANCHOR,
        ANCIENT_TEA_SET,
        BAG_OF_MARBLES,
        BAG_OF_PREPARATION,
        BLOOD_VIAL,
        BOTTLED_FLAME,
        BOTTLED_LIGHTNING,
        BOTTLED_TORNADO,
        BRONZE_SCALES,
        BUSTED_CROWN,
        CLOCKWORK_SOUVENIR,
        COFFEE_DRIPPER,
        CRACKED_CORE,
        CURSED_KEY,
        DAMARU,
        DATA_DISK,
        DU_VU_DOLL,
        ENCHIRIDION,
        FOSSILIZED_HELIX,
        FUSION_HAMMER,
        GIRYA,
        GOLD_PLATED_CABLES,
        GREMLIN_VISAGE,
        HOLY_WATER,
        LANTERN,
        MARK_OF_PAIN,
        MUTAGENIC_STRENGTH,
        NEOWS_LAMENT,
        NINJA_SCROLL,
        NUCLEAR_BATTERY,
        ODDLY_SMOOTH_STONE,
        PANTOGRAPH,
        PRESERVED_INSECT,
        PURE_WATER,
        RED_MASK,
        RING_OF_THE_SERPENT,
        RING_OF_THE_SNAKE,
        RUNIC_CAPACITOR,
        SLAVERS_COLLAR,
        SLING_OF_COURAGE,
        SYMBIOTIC_VIRUS,
        TEARDROP_LOCKET,
        THREAD_AND_NEEDLE,
        VAJRA,

        // out of battle relics
        ASTROLABE,
        BLACK_STAR,
        CALLING_BELL,
        CAULDRON,
        CULTIST_HEADPIECE,
        DOLLYS_MIRROR,
        DREAM_CATCHER,
        EMPTY_CAGE,
        ETERNAL_FEATHER,
        FROZEN_EGG,
        GOLDEN_IDOL,
        JUZU_BRACELET,
        LEES_WAFFLE,
        MANGO,
        MATRYOSHKA,
        MAW_BANK,
        MEAL_TICKET,
        MEMBERSHIP_CARD,
        MOLTEN_EGG,
        NLOTHS_GIFT,
        NLOTHS_HUNGRY_FACE,
        OLD_COIN,
        ORRERY,
        PANDORAS_BOX,
        PEACE_PIPE,
        PEAR,
        POTION_BELT,
        PRAYER_WHEEL,
        PRISMATIC_SHARD,
        QUESTION_CARD,
        REGAL_PILLOW,
        SSSERPENT_HEAD,
        SHOVEL,
        SINGING_BOWL,
        SMILING_MASK,
        SPIRIT_POOP,
        STRAWBERRY,
        THE_COURIER,
        TINY_CHEST,
        TINY_HOUSE,
        TOXIC_EGG,
        WAR_PAINT,
        WHETSTONE,
        WHITE_BEAST_STATUE,
        WING_BOOTS,

        CIRCLET,
        RED_CIRCLET,
        INVALID,
    };

    enum class RelicTier {
        COMMON=0,
        UNCOMMON,
        RARE,
        BOSS,
        SHOP,
        STARTER,
        SPECIAL,
        INVALID,
    };

    static const char* relicTierStrings[] = {
            "Common",
            "Uncommon",
            "Rare",
            "Boss",
            "Shop",
            "Starter",
            "Special",
            "Invalid",
    };
    static constexpr const char *const relicEnumNames[] {"AKABEKO","ART_OF_WAR","BIRD_FACED_URN","BLOODY_IDOL","BLUE_CANDLE","BRIMSTONE","CALIPERS","CAPTAINS_WHEEL","CENTENNIAL_PUZZLE","CERAMIC_FISH","CHAMPION_BELT","CHARONS_ASHES","CHEMICAL_X","CLOAK_CLASP","DARKSTONE_PERIAPT","DEAD_BRANCH","DUALITY","ECTOPLASM","EMOTION_CHIP","FROZEN_CORE","FROZEN_EYE","GAMBLING_CHIP","GINGER","GOLDEN_EYE","GREMLIN_HORN","HAND_DRILL","HAPPY_FLOWER","HORN_CLEAT","HOVERING_KITE","ICE_CREAM","INCENSE_BURNER","INK_BOTTLE","INSERTER","KUNAI","LETTER_OPENER","LIZARD_TAIL","MAGIC_FLOWER","MARK_OF_THE_BLOOM","MEDICAL_KIT","MELANGE","MERCURY_HOURGLASS","MUMMIFIED_HAND","NECRONOMICON","NILRYS_CODEX","NUNCHAKU","ODD_MUSHROOM","OMAMORI","ORANGE_PELLETS","ORICHALCUM","ORNAMENTAL_FAN","PAPER_KRANE","PAPER_PHROG","PEN_NIB","PHILOSOPHERS_STONE","POCKETWATCH","RED_SKULL","RUNIC_CUBE","RUNIC_DOME","RUNIC_PYRAMID","SACRED_BARK","SELF_FORMING_CLAY","SHURIKEN","SNECKO_EYE","SNECKO_SKULL","SOZU","STONE_CALENDAR","STRANGE_SPOON","STRIKE_DUMMY","SUNDIAL","THE_ABACUS","THE_BOOT","THE_SPECIMEN","TINGSHA","TOOLBOX","TORII","TOUGH_BANDAGES","TOY_ORNITHOPTER","TUNGSTEN_ROD","TURNIP","TWISTED_FUNNEL","UNCEASING_TOP","VELVET_CHOKER","VIOLET_LOTUS","WARPED_TONGS","WRIST_BLADE","BLACK_BLOOD","BURNING_BLOOD","MEAT_ON_THE_BONE","FACE_OF_CLERIC","ANCHOR","ANCIENT_TEA_SET","BAG_OF_MARBLES","BAG_OF_PREPARATION","BLOOD_VIAL","BOTTLED_FLAME","BOTTLED_LIGHTNING","BOTTLED_TORNADO","BRONZE_SCALES","BUSTED_CROWN","CLOCKWORK_SOUVENIR","COFFEE_DRIPPER","CRACKED_CORE","CURSED_KEY","DAMARU","DATA_DISK","DU_VU_DOLL","ENCHIRIDION","FOSSILIZED_HELIX","FUSION_HAMMER","GIRYA","GOLD_PLATED_CABLES","GREMLIN_VISAGE","HOLY_WATER","LANTERN","MARK_OF_PAIN","MUTAGENIC_STRENGTH","NEOWS_LAMENT","NINJA_SCROLL","NUCLEAR_BATTERY","ODDLY_SMOOTH_STONE","PANTOGRAPH","PRESERVED_INSECT","PURE_WATER","RED_MASK","RING_OF_THE_SERPENT","RING_OF_THE_SNAKE","RUNIC_CAPACITOR","SLAVERS_COLLAR","SLING_OF_COURAGE","SYMBIOTIC_VIRUS","TEARDROP_LOCKET","THREAD_AND_NEEDLE","VAJRA","ASTROLABE","BLACK_STAR","CALLING_BELL","CAULDRON","CULTIST_HEADPIECE","DOLLYS_MIRROR","DREAM_CATCHER","EMPTY_CAGE","ETERNAL_FEATHER","FROZEN_EGG","GOLDEN_IDOL","JUZU_BRACELET","LEES_WAFFLE","MANGO","MATRYOSHKA","MAW_BANK","MEAL_TICKET","MEMBERSHIP_CARD","MOLTEN_EGG","NLOTHS_GIFT","NLOTHS_HUNGRY_FACE","OLD_COIN","ORRERY","PANDORAS_BOX","PEACE_PIPE","PEAR","POTION_BELT","PRAYER_WHEEL","PRISMATIC_SHARD","QUESTION_CARD","REGAL_PILLOW","SSSERPENT_HEAD","SHOVEL","SINGING_BOWL","SMILING_MASK","SPIRIT_POOP","STRAWBERRY","THE_COURIER","TINY_CHEST","TINY_HOUSE","TOXIC_EGG","WAR_PAINT","WHETSTONE","WHITE_BEAST_STATUE","WING_BOOTS","CIRCLET","RED_CIRCLET","INVALID",};
    static constexpr const char* const relicNames[] {"Akabeko","Art Of War","Bird Faced Urn","Bloody Idol","Blue Candle","Brimstone","Calipers","Captains Wheel","Centennial Puzzle","Ceramic Fish","Champion Belt","Charons Ashes","Chemical X","Cloak Clasp","Darkstone Periapt","Dead Branch","Duality","Ectoplasm","Emotion Chip","Frozen Core","Frozen Eye","Gambling Chip","Ginger","Golden Eye","Gremlin Horn","Hand Drill","Happy Flower","Horn Cleat","Hovering Kite","Ice Cream","Incense Burner","Ink Bottle","Inserter","Kunai","Letter Opener","Lizard Tail","Magic Flower","Mark Of The Bloom","Medical Kit","Melange","Mercury Hourglass","Mummified Hand","Necronomicon","Nilrys Codex","Nunchaku","Odd Mushroom","Omamori","Orange Pellets","Orichalcum","Ornamental Fan","Paper Krane","Paper Phrog","Pen Nib","Philosophers Stone","Pocketwatch","Red Skull","Runic Cube","Runic Dome","Runic Pyramid","Sacred Bark","Self Forming Clay","Shuriken","Snecko Eye","Snecko Skull","Sozu","Stone Calendar","Strange Spoon","Strike Dummy","Sundial","The Abacus","The Boot","The Specimen","Tingsha","Toolbox","Torii","Tough Bandages","Toy Ornithopter","Tungsten Rod","Turnip","Twisted Funnel","Unceasing Top","Velvet Choker","Violet Lotus","Warped Tongs","Wrist Blade","Black Blood","Burning Blood","Meat On The Bone","Face Of Cleric","Anchor","Ancient Tea Set","Bag Of Marbles","Bag Of Preparation","Blood Vial","Bottled Flame","Bottled Lightning","Bottled Tornado","Bronze Scales","Busted Crown","Clockwork Souvenir","Coffee Dripper","Cracked Core","Cursed Key","Damaru","Data Disk","Du Vu Doll","Enchiridion","Fossilized Helix","Fusion Hammer","Girya","Goldplated Cables","Gremlin Visage","Holy Water","Lantern","Mark Of Pain","Mutagenic Strength","Neows Lament","Ninja Scroll","Nuclear Battery","Oddly Smooth Stone","Pantograph","Preserved Insect","Pure Water","Red Mask","Ring Of The Serpent","Ring Of The Snake","Runic Capacitor","Slavers Collar","Sling Of Courage","Symbiotic Virus","Teardrop Locket","Thread And Needle","Vajra","Astrolabe","Black Star","Calling Bell","Cauldron","Cultist Headpiece","Dollys Mirror","Dream Catcher","Empty Cage","Eternal Feather","Frozen Egg","Golden Idol","Juzu Bracelet","Lees Waffle","Mango","Matryoshka","Maw Bank","Meal Ticket","Membership Card","Molten Egg","Nloths Gift","Nloths Hungry Face","Old Coin","Orrery","Pandoras Box","Peace Pipe","Pear","Potion Belt","Prayer Wheel","Prismatic Shard","Question Card","Regal Pillow","Ssserpent Head","Shovel","Singing Bowl","Smiling Mask","Spirit Poop","Strawberry","The Courier","Tiny Chest","Tiny House","Toxic Egg","War Paint","Whetstone","White Beast Statue","Wing Boots","Circlet","Red Circlet","Invalid",};
    static constexpr const char* const relicIds[] {"Akabeko","Art of War","Bird Faced Urn","Bloody Idol","Blue Candle","Brimstone","Calipers","CaptainsWheel","Centennial Puzzle","CeramicFish","Champion Belt","Charon's Ashes","Chemical X","CloakClasp","Darkstone Periapt","Dead Branch","Yang","Ectoplasm","Emotion Chip","FrozenCore","Frozen Eye","Gambling Chip","Ginger","GoldenEye","Gremlin Horn","HandDrill","Happy Flower","HornCleat","HoveringKite","Ice Cream","Incense Burner","InkBottle","Inserter","Kunai","Letter Opener","Lizard Tail","Magic Flower","Mark of the Bloom","Medical Kit","Melange","Mercury Hourglass","Mummified Hand","Necronomicon","Nilry's Codex","Nunchaku","Odd Mushroom","Omamori","OrangePellets","Orichalcum","Ornamental Fan","Paper Crane","Paper Frog","Pen Nib","Philosopher's Stone","Pocketwatch","Red Skull","Runic Cube","Runic Dome","Runic Pyramid","SacredBark","Self Forming Clay","Shuriken","Snecko Eye","Snake Skull","Sozu","StoneCalendar","Strange Spoon","StrikeDummy","Sundial","TheAbacus","Boot","The Specimen","Tingsha","Toolbox","Torii","Tough Bandages","Toy Ornithopter","TungstenRod","Turnip","TwistedFunnel","Unceasing Top","Velvet Choker","VioletLotus","WarpedTongs","WristBlade","Black Blood","Burning Blood","Meat on the Bone","FaceOfCleric","Anchor","Ancient Tea Set","Bag of Marbles","Bag of Preparation","Blood Vial","Bottled Flame","Bottled Lightning","Bottled Tornado","Bronze Scales","Busted Crown","ClockworkSouvenir","Coffee Dripper","Cracked Core","Cursed Key","Damaru","DataDisk","Du-Vu Doll","Enchiridion","FossilizedHelix","Fusion Hammer","Girya","Cables","GremlinMask","HolyWater","Lantern","Mark of Pain","MutagenicStrength","NeowsBlessing","Ninja Scroll","Nuclear Battery","Oddly Smooth Stone","Pantograph","PreservedInsect","PureWater","Red Mask","Ring of the Serpent","Ring of the Snake","Runic Capacitor","SlaversCollar","Sling","Symbiotic Virus","TeardropLocket","Thread and Needle","Vajra","Astrolabe","Black Star","Calling Bell","Cauldron","CultistMask","DollysMirror","Dream Catcher","Empty Cage","Eternal Feather","Frozen Egg 2","Golden Idol","Juzu Bracelet","Lee's Waffle","Mango","Matryoshka","MawBank","MealTicket","Membership Card","Molten Egg 2","Nloth's Gift","NlothsMask","Old Coin","Orrery","Pandora's Box","Peace Pipe","Pear","Potion Belt","Prayer Wheel","PrismaticShard","Question Card","Regal Pillow","SsserpentHead","Shovel","Singing Bowl","Smiling Mask","Spirit Poop","Strawberry","The Courier","Tiny Chest","Tiny House","Toxic Egg 2","War Paint","Whetstone","White Beast Statue","WingedGreaves","Circlet","Red Circlet","INVALID",};
    static constexpr const RelicTier relicTiers[] {RelicTier::COMMON,RelicTier::COMMON,RelicTier::RARE,RelicTier::SPECIAL,RelicTier::UNCOMMON,RelicTier::SHOP,RelicTier::RARE,RelicTier::RARE,RelicTier::COMMON,RelicTier::COMMON,RelicTier::RARE,RelicTier::RARE,RelicTier::SHOP,RelicTier::RARE,RelicTier::UNCOMMON,RelicTier::RARE,RelicTier::UNCOMMON,RelicTier::BOSS,RelicTier::RARE,RelicTier::BOSS,RelicTier::SHOP,RelicTier::RARE,RelicTier::RARE,RelicTier::RARE,RelicTier::UNCOMMON,RelicTier::SHOP,RelicTier::COMMON,RelicTier::UNCOMMON,RelicTier::BOSS,RelicTier::RARE,RelicTier::RARE,RelicTier::UNCOMMON,RelicTier::BOSS,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::RARE,RelicTier::RARE,RelicTier::SPECIAL,RelicTier::SHOP,RelicTier::SHOP,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::SPECIAL,RelicTier::SPECIAL,RelicTier::COMMON,RelicTier::SPECIAL,RelicTier::COMMON,RelicTier::SHOP,RelicTier::COMMON,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::BOSS,RelicTier::RARE,RelicTier::COMMON,RelicTier::BOSS,RelicTier::BOSS,RelicTier::BOSS,RelicTier::BOSS,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::BOSS,RelicTier::COMMON,RelicTier::BOSS,RelicTier::RARE,RelicTier::SHOP,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::SHOP,RelicTier::COMMON,RelicTier::RARE,RelicTier::RARE,RelicTier::SHOP,RelicTier::RARE,RelicTier::RARE,RelicTier::COMMON,RelicTier::RARE,RelicTier::RARE,RelicTier::SHOP,RelicTier::RARE,RelicTier::BOSS,RelicTier::BOSS,RelicTier::SPECIAL,RelicTier::BOSS,RelicTier::BOSS,RelicTier::STARTER,RelicTier::UNCOMMON,RelicTier::SPECIAL,RelicTier::COMMON,RelicTier::COMMON,RelicTier::COMMON,RelicTier::COMMON,RelicTier::COMMON,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::BOSS,RelicTier::SHOP,RelicTier::BOSS,RelicTier::STARTER,RelicTier::BOSS,RelicTier::COMMON,RelicTier::COMMON,RelicTier::RARE,RelicTier::SPECIAL,RelicTier::RARE,RelicTier::BOSS,RelicTier::RARE,RelicTier::UNCOMMON,RelicTier::SPECIAL,RelicTier::BOSS,RelicTier::COMMON,RelicTier::BOSS,RelicTier::SPECIAL,RelicTier::SPECIAL,RelicTier::UNCOMMON,RelicTier::BOSS,RelicTier::COMMON,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::STARTER,RelicTier::SPECIAL,RelicTier::BOSS,RelicTier::STARTER,RelicTier::SHOP,RelicTier::BOSS,RelicTier::SHOP,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::RARE,RelicTier::COMMON,RelicTier::BOSS,RelicTier::BOSS,RelicTier::BOSS,RelicTier::SHOP,RelicTier::SPECIAL,RelicTier::SHOP,RelicTier::COMMON,RelicTier::BOSS,RelicTier::UNCOMMON,RelicTier::UNCOMMON,RelicTier::SPECIAL,RelicTier::COMMON,RelicTier::SHOP,RelicTier::RARE,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::COMMON,RelicTier::SHOP,RelicTier::UNCOMMON,RelicTier::SPECIAL,RelicTier::SPECIAL,RelicTier::RARE,RelicTier::SHOP,RelicTier::BOSS,RelicTier::RARE,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::RARE,RelicTier::SHOP,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::SPECIAL,RelicTier::RARE,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::SPECIAL,RelicTier::COMMON,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::BOSS,RelicTier::UNCOMMON,RelicTier::COMMON,RelicTier::COMMON,RelicTier::UNCOMMON,RelicTier::RARE,RelicTier::SPECIAL,RelicTier::SPECIAL,RelicTier::INVALID,};

    static const RelicId getRelicFromId(std::string id) {
        auto it = std::find(std::begin(relicIds), std::end(relicIds), id);
        assert(it != std::end(relicIds));
        auto idx = it - std::begin(relicIds);
        return static_cast<RelicId>(idx);
    }

            // COMMON, UNCOMMON, RARE, BOSS, SHOP, STARTER, SPECIAL
    static constexpr const int  relicTierPrices[] = {150,250,300,999,150,300,400};


    static const char* getRelicName(RelicId id) {
        return relicNames[static_cast<int>(id)];
    }

    static RelicTier getRelicTier(RelicId id) {
        return relicTiers[static_cast<int>(id)];
    }

    static int getRelicBasePrice(RelicId id) {
        return relicTierPrices[static_cast<int>(getRelicTier(id))];
    }

    static constexpr bool isEggRelic(RelicId id) {
        return id == RelicId::MOLTEN_EGG || id == RelicId::FROZEN_EGG || id == RelicId::TOXIC_EGG;
    }

    typedef RelicId R;

}

#endif //STS_LIGHTSPEED_RELICS_H
