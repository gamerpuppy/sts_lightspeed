//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_MONSTERSTATUSEFFECTS_H
#define STS_LIGHTSPEED_MONSTERSTATUSEFFECTS_H

namespace sts {

    enum class MonsterStatus: std::uint8_t {

        // statuses that use just applied
        VULNERABLE=0,
        WEAK,

        // *********    DEBUFFS   ************

        BLOCK_RETURN,
        CHOKED,
        CORPSE_EXPLOSION,
        LOCK_ON,
        MARK,
        POISON,
        SHACKLED,
        SLOW, // this should be set just to
        STRENGTH_DOWN,
        STUNNED,

        // *********    POWERS   ************

        ARTIFACT,
        GENERIC_STRENGTH_UP,
        END_OF_TURN_GAIN_STRENGTH,
        MALLEABLE,
        METALLICIZE,
        MINION,
        PAINFUL_STABS, // not unique, used by book of stabbing and corrupt heart after buffs and
        PLATED_ARMOR,
        REGEN,
        STRENGTH,
        THIEVERY,

        // unique powers
        ANGRY,
        ASLEEP,
        BARRICADE,
        BEAT_OF_DEATH,
        CURIOSITY,
        CURL_UP,
        ENRAGE,
        EXPLOSIVE,
        FADING,
        FLIGHT,
        INTANGIBLE,
        INVINCIBLE,
        MODE_SHIFT,
        RITUAL,
        SHARP_HIDE,
        SHIFTING,
        SPORE_CLOUD,
        STASIS,
        THORNS,
        TIME_WARP,
    };


    typedef MonsterStatus MS;

    // todo
    static constexpr const char* const enemyStatusStrings[]{
            "Vulnerable","Weak","Block Return","Choked","Corpse Explosion","Lock On","Mark","Poison","Shackled","Slow","Strength Down","Stunned","Artifact","Generic Strength Up","End of Turn Gain Strength","Malleable","Metallicize","Minion","Painful Stabs","Plated Armor","Regen","Strength","Thievery","Angry","Asleep","Barricade","Beat Of Death","Curiosity","Curl Up","Enrage","Explosive","Fading","Flight","Intangible","Invincible","Mode Shift","Ritual","Sharp Hide","Shifting","Spore Cloud","Stasis","Thorns","Time Warp",
    };

}


#endif //STS_LIGHTSPEED_MONSTERSTATUSEFFECTS_H
