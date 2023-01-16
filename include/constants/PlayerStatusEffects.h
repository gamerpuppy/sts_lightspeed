//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_PLAYERSTATUSEFFECTS_H
#define STS_LIGHTSPEED_PLAYERSTATUSEFFECTS_H

#include <cstdint>
#include <algorithm>
#include <array>
#include <cassert>
#include <string>

enum class PlayerStatus : std::uint8_t {

    // *********    Statuses that use JustApplied  ************
    INVALID=0,
    DOUBLE_DAMAGE,
    DRAW_REDUCTION,
    FRAIL,
    INTANGIBLE,
    VULNERABLE,
    WEAK,

    // *********    DEBUFFS   ************
    BIAS,
    CONFUSED,
    CONSTRICTED,
    ENTANGLED,
    FASTING,
    HEX,
    LOSE_DEXTERITY,
    LOSE_STRENGTH,
    NO_BLOCK,
    NO_DRAW,
    WRAITH_FORM,

    // *********    POWERS   ************
    // Bool
    BARRICADE,
    BLASPHEMER,
    CORRUPTION,
    ELECTRO,
    SURROUNDED,
    MASTER_REALITY,
    PEN_NIB,
    WRATH_NEXT_TURN,

    // Counter
    AMPLIFY,
    BLUR,
    BUFFER,
    COLLECT,
    DOUBLE_TAP,
    DUPLICATION,
    ECHO_FORM,
    FREE_ATTACK_POWER,
    REBOUND,
    MANTRA,

    // Intensity
    ACCURACY, // todo implement
    AFTER_IMAGE,
    BATTLE_HYMN,
    BRUTALITY,
    BURST,
    COMBUST,
    CREATIVE_AI,
    DARK_EMBRACE,
    DEMON_FORM,
    DEVA,
    DEVOTION,
    DRAW_CARD_NEXT_TURN,
    ENERGIZED,
    ENVENOM,
    ESTABLISHMENT,
    EVOLVE,
    FEEL_NO_PAIN,
    FIRE_BREATHING,
    FLAME_BARRIER,
    FOCUS,
    FORESIGHT,
    HELLO_WORLD,
    INFINITE_BLADES,
    JUGGERNAUT,
    LIKE_WATER,
    LOOP,
    MAGNETISM,
    MAYHEM,
    METALLICIZE,
    NEXT_TURN_BLOCK,
    NOXIOUS_FUMES,
    OMEGA,
    PANACHE,
    PHANTASMAL,
    PLATED_ARMOR,
    RAGE,
    REGEN,
    RITUAL,
    RUPTURE,
    SADISTIC,
    STATIC_DISCHARGE,
    THORNS,
    THOUSAND_CUTS,
    TOOLS_OF_THE_TRADE,
    VIGOR,
    WAVE_OF_THE_HAND,

    // Duration
    EQUILIBRIUM,
    ARTIFACT,
    DEXTERITY,
    STRENGTH,

    // special
    THE_BOMB
};

static constexpr const char* const playerStatusIds[] {
    "INVALID",
    "Double Damage",
    "Draw Reduction",
    "Frail",
    "IntangiblePlayer",
    "Vulnerable",
    "Weakened",

    "Bias",
    "Confusion",
    "Constricted",
    "Entangled",
    "EnergyDownPower",
    "Hex",
    "DexLoss",
    "Flex",
    "NoBlockPower",
    "No Draw",
    "Wraith Form v2",

    "Barricade",
    "EndTurnDeath",
    "Corruption",
    "Electro",
    "Surrounded",
    "MasterRealityPower",
    "Pen Nib",
    "WrathNextTurnPower",

    "Amplify",
    "Blur",
    "Buffer",
    "Collect",
    "Double Tap",
    "DuplicationPower",
    "Echo Form",
    "FreeAttackPower",
    "Rebound",
    "Mantra",

    "Accuracy",
    "After Image",
    "BattleHymn",
    "Brutality",
    "Burst",
    "Combust",
    "Creative AI",
    "Dark Embrace",
    "Demon Form",
    "DevaForm",
    "DevotionPower",
    "Draw Card",
    "Energized",
    "Envenom",
    "EstablishmentPower",
    "Evolve",
    "Feel No Pain",
    "Fire Breathing",
    "Flame Barrier",
    "Focus",
    "WireheadingPower",
    "Hello",
    "Infinite Blades",
    "Juggernaut",
    "LikeWaterPower",
    "Loop",
    "Magnetism",
    "Mayhem",
    "Metallicize",
    "Next Turn Block",
    "Noxious Fumes",
    "OmegaPower",
    "Panache",
    "Phantasmal",
    "Plated Armor",
    "Rage",
    "Regeneration",
    "Ritual",
    "Rupture",
    "Sadistic",
    "StaticDischarge",
    "Thorns",
    "Thousand Cuts",
    "Tools Of The Trade",
    "Vigor",
    "WaveOfTheHandPower",

    "Equilibrium",
    "Artifact",
    "Dexterity",
    "Strength",

    "TheBomb",
};

static const PlayerStatus getPlayerStatusFromId(std::string id) {
    auto it = std::find(std::begin(playerStatusIds), std::end(playerStatusIds), id);
    if (it == std::end(playerStatusIds)) return PlayerStatus::INVALID;
    auto idx = it - std::begin(playerStatusIds);
    return static_cast<PlayerStatus>(idx);
}

typedef PlayerStatus PS;

static constexpr const char* const playerStatusStrings[] {
        "INVALID","Double Damage","Draw Reduction","Frail","Intangible","Vulnerable","Weak","Bias","Confused","Constricted","Entangled","Fasting","Hex","Lose Dexterity","Lose Strength","No Block","No Draw","Wraith Form","Barricade","Blasphemer","Corruption","Electro","Surrounded","Master Reality","Pen Nib","Wrath Next Turn","Amplify","Blur","Buffer","Collect","Double Tap","Duplication","Echo Form","Free Attack Power","Rebound","Mantra","Accuracy","After Image","Battle Hymn","Brutality","Burst","Combust","Creative Ai","Dark Embrace","Demon Form","Deva","Devotion","Draw Card Next Turn","Energized","Envenom","Establishment","Evolve","Feel No Pain","Fire Breathing","Flame Barrier","Focus","Foresight","Hello World","Infinite Blades","Juggernaut","Like Water","Loop","Magnetism","Mayhem","Metallicize","Next Turn Block","Noxious Fumes","Omega","Panache","Phantasmal","Plated Armor","Rage","Regen","Ritual","Rupture","Sadistic","Static Discharge","Thorns","Thousand Cuts","Tools Of The Trade","Vigor","Wave Of The Hand","Equilibrium","Artifact","Dexterity","Strength","The Bomb",
};

static constexpr const char* const playerStatusEnumStrings[] {
        "INVALID","DOUBLE_DAMAGE","DRAW_REDUCTION","FRAIL","INTANGIBLE","VULNERABLE","WEAK","BIAS","CONFUSED","CONSTRICTED","ENTANGLED","FASTING","HEX","LOSE_DEXTERITY","LOSE_STRENGTH","NO_BLOCK","NO_DRAW","WRAITH_FORM","BARRICADE","BLASPHEMER","CORRUPTION","ELECTRO","SURROUNDED","MASTER_REALITY","PEN_NIB","WRATH_NEXT_TURN","AMPLIFY","BLUR","BUFFER","COLLECT","DOUBLE_TAP","DUPLICATION","ECHO_FORM","FREE_ATTACK_POWER","REBOUND","MANTRA","ACCURACY","AFTER_IMAGE","BATTLE_HYMN","BRUTALITY","BURST","COMBUST","CREATIVE_AI","DARK_EMBRACE","DEMON_FORM","DEVA","DEVOTION","DRAW_CARD_NEXT_TURN","ENERGIZED","ENVENOM","ESTABLISHMENT","EVOLVE","FEEL_NO_PAIN","FIRE_BREATHING","FLAME_BARRIER","FOCUS","FORESIGHT","HELLO_WORLD","INFINITE_BLADES","JUGGERNAUT","LIKE_WATER","LOOP","MAGNETISM","MAYHEM","METALLICIZE","NEXT_TURN_BLOCK","NOXIOUS_FUMES","OMEGA","PANACHE","PHANTASMAL","PLATED_ARMOR","RAGE","REGEN","RITUAL","RUPTURE","SADISTIC","STATIC_DISCHARGE","THORNS","THOUSAND_CUTS","TOOLS_OF_THE_TRADE","VIGOR","WAVE_OF_THE_HAND","EQUILIBRIUM","ARTIFACT","DEXTERITY","STRENGTH","THE_BOMB",
};

enum class Stance : uint8_t {
    NEUTRAL=0,
    CALM,
    WRATH,
    DIVINITY,
};

static constexpr const char* const stanceStrings[] {
        "NEUTRAL",
        "WRATH",
        "CALM",
        "DIVINITY",
};

enum class Orb : uint8_t {
    EMPTY=0,
    DARK,
    FROST,
    FUSION,
    LIGHTNING,
};


#endif //STS_LIGHTSPEED_PLAYERSTATUSEFFECTS_H
