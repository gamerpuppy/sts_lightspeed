//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_MONSTERSTATUSEFFECTS_H
#define STS_LIGHTSPEED_MONSTERSTATUSEFFECTS_H

namespace sts {

    enum class MonsterStatus: std::uint8_t {
        ARTIFACT=0,
        BLOCK_RETURN,
        CHOKED,
        CORPSE_EXPLOSION,
        LOCK_ON,
        MARK,
        METALLICIZE,
        PLATED_ARMOR,
        POISON,
        REGEN,
        SHACKLED,
        STRENGTH,
        VULNERABLE,
        WEAK,

        // unique powers : two of these can't be on the same monster
        ANGRY,
        BEAT_OF_DEATH,
        CURIOSITY,
        CURL_UP,
        ENRAGE,
        FADING,
        FLIGHT,
        GENERIC_STRENGTH_UP,
        INTANGIBLE, // differs from the game in that it always decrements at end of round
        MALLEABLE,
        MODE_SHIFT,
        RITUAL, // todo just merge this with orb walker strength up
        SLOW, // this should be set just to
        SPORE_CLOUD,
        THIEVERY,
        THORNS,
        TIME_WARP,

        // unique powers 2
        INVINCIBLE,
        REACTIVE,
        SHARP_HIDE,

        // bool powers, stored in statusbits
        ASLEEP,
        BARRICADE,
        MINION,
        MINION_LEADER,
        PAINFUL_STABS,
        REGROW,
        SHIFTING,
        STASIS,

        INVALID,
    };


    typedef MonsterStatus MS;

    static constexpr const char* const enemyStatusStrings[]{
        "Artifact",
        "Block Return",
        "Choked",
        "Corpse Explosion",
        "Lock On",
        "Mark",
        "Metallicize",
        "Plated Armor",
        "Poison",
        "Regen",
        "Shackled",
        "Strength",
        "Vulnerable",
        "Weak",

        "Angry",
        "Beat Of Death",
        "Curiosity",
        "Curl Up",
        "Enrage",
        "Fading",
        "Flight",
        "Generic Strength Up",
        "Intangible",
        "Malleable",
        "Mode Shift",
        "Ritual",
        "Slow",
        "Spore Cloud",
        "Thievery",
        "Thorns",
        "Time Warp",

        "Invincible",
        "Reactive",
        "Sharp Hide",

        "Asleep",
        "Barricade",
        "Minion",
        "Minion Leader",
        "Painful Stabs",

        "Regrow",
        "Shifting",
        "Stasis",

        "INVALID",
    };

    static constexpr const char* const monsterStatusEnumStrings[]{
        "ARTIFACT",
        "BLOCK_RETURN",
        "CHOKED",
        "CORPSE_EXPLOSION",
        "LOCK_ON",
        "MARK",
        "METALLICIZE",
        "PLATED_ARMOR",
        "POISON",
        "REGEN",
        "SHACKLED",
        "STRENGTH",
        "VULNERABLE",
        "WEAK",

        "ANGRY",
        "BEAT_OF_DEATH",
        "CURIOSITY",
        "CURL_UP",
        "ENRAGE",
        "FADING",
        "FLIGHT",
        "GENERIC_STRENGTH_UP",
        "INTANGIBLE",
        "MALLEABLE",
        "MODE_SHIFT",
        "RITUAL",
        "SLOW",
        "SPORE_CLOUD",
        "THIEVERY",
        "THORNS",
        "TIME_WARP",

        "INVINCIBLE",
        "SHARP_HIDE",

        "ASLEEP",
        "BARRICADE",
        "MINION",
        "MINION_LEADER",
        "PAINFUL_STABS",
        "REACTIVE",
        "REGROW",
        "SHIFTING",
        "STASIS",
        "INVALID",
    };

    static constexpr bool isBooleanPower(MonsterStatus s) {
        switch (s) {
            case MS::ASLEEP:
            case MS::BARRICADE:
            case MS::MINION:
            case MS::MINION_LEADER:
            case MS::PAINFUL_STABS:
            case MS::REGROW:
            case MS::SHIFTING:
            case MS::STASIS:
                return true;

            default:
                return false;
        }
    }

}


#endif //STS_LIGHTSPEED_MONSTERSTATUSEFFECTS_H
