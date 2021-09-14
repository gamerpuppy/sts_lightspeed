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
        PAINFUL_STABS, // not unique, used by book of stabbing and corrupt heart after buffs
        PLATED_ARMOR,
        POISON,
        REGEN,
        SHACKLED,
        STRENGTH,
        VULNERABLE,
        WEAK,

        // unique powers : two of these can't be on the same monster
        ANGRY,
        ASLEEP,
        BARRICADE,
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
        REACTIVE,
        REGROW,
        RITUAL, // todo just merge this with orb walker strength up
        SHIFTING,
        SLOW, // this should be set just to
        SPORE_CLOUD,
        STASIS,
        THIEVERY,
        THORNS,
        TIME_WARP,

        // special case for the heart's second unique power
        INVINCIBLE,
        MINION, // only stored in statusbits
        SHARP_HIDE,
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
        "Painful Stabs",
        "Plated Armor",
        "Poison",
        "Regen",
        "Shackled",
        "Strength",
        "Vulnerable",
        "Weak",

        "Angry",
        "Asleep",
        "Barricade",
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
        "Reactive",
        "Regrow",
        "Ritual",
        "Shifting",
        "Slow",
        "Spore Cloud",
        "Stasis",
        "Thievery",
        "Thorns",
        "Time Warp",

        "Invincible",
        "Minion",
        "Sharp Hide",
    };

    static constexpr bool isBooleanPower(MonsterStatus s) {
        switch (s) {
            case MS::ASLEEP:
            case MS::BARRICADE:
            case MS::MINION:
            case MS::REACTIVE:
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
