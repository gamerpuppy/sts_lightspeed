//
// Created by keega on 9/19/2021.
//

#include <sts/sim/search/expert.hpp>

using namespace sts;

int sts::search::Expert::getPlayOrdering(CardId id) {
    switch (id) {
        case CardId::NORMALITY:
            return -30;

        case CardId::PAIN:
            return -25;

        case CardId::APOTHEOSIS:
        case CardId::APPARITION:
            return -20;

        case CardId::ARMAMENTS:
            return -15;

        case CardId::CORRUPTION:
            return -10;

        case CardId::PANACHE:
        case CardId::RUPTURE:
            return -5;

        case CardId::DARK_EMBRACE:
        case CardId::FEEL_NO_PAIN:
        case CardId::BLOODLETTING:
        case CardId::SPOT_WEAKNESS:
        case CardId::OFFERING:
            return -2;

        case CardId::BARRICADE:
        case CardId::BERSERK:
        case CardId::DEMON_FORM:
            return 0;

        case CardId::EVOLVE:
        case CardId::MAYHEM:
        case CardId::BRUTALITY:
        case CardId::FIRE_BREATHING:
        case CardId::METALLICIZE:
            return 3;

        case CardId::MASTER_OF_STRATEGY:
            return 4;

        case CardId::SADISTIC_NATURE:
        case CardId::PANACEA:
        case CardId::SECRET_TECHNIQUE:
        case CardId::SECRET_WEAPON:
            return 5;

        case CardId::DARK_SHACKLES:
        case CardId::FINESSE:
        case CardId::BATTLE_TRANCE:
        case CardId::DISARM:
        case CardId::ENLIGHTENMENT:
        case CardId::INFLAME:
        case CardId::COMBUST:
        case CardId::INTIMIDATE:
        case CardId::BLIND:
        case CardId::IMPERVIOUS:
        case CardId::LIMIT_BREAK:
        case CardId::TRIP:
        case CardId::SHOCKWAVE:
        case CardId::SEEING_RED:
            return 10;

        case CardId::JUGGERNAUT:
        case CardId::MADNESS:
        case CardId::POWER_THROUGH:
        case CardId::RAGE:
        case CardId::TRANSMUTATION:
            return 15;

        case CardId::PANIC_BUTTON:
        case CardId::MAGNETISM:
        case CardId::JACK_OF_ALL_TRADES:
        case CardId::BURNING_PACT:
        case CardId::DOUBLE_TAP:
        case CardId::DUAL_WIELD:
        case CardId::CHRYSALIS:
        case CardId::DEEP_BREATH:
        case CardId::DISCOVERY:
        case CardId::INFERNAL_BLADE:
        case CardId::METAMORPHOSIS:
        case CardId::HAVOC:
        case CardId::FORETHOUGHT:
        case CardId::WARCRY:
        case CardId::SECOND_WIND:
        case CardId::THINKING_AHEAD:
        case CardId::SHRUG_IT_OFF:
        case CardId::JAX:
            return 20;

        case CardId::EXHUME:
        case CardId::IMPATIENCE:
        case CardId::TRUE_GRIT:
        case CardId::THE_BOMB:
            return 22;

        case CardId::BASH:
        case CardId::UPPERCUT:
        case CardId::THUNDERCLAP:
            return 25;

        case CardId::SEVER_SOUL:
        case CardId::WHIRLWIND:
        case CardId::GOOD_INSTINCTS:
        case CardId::GHOSTLY_ARMOR:
        case CardId::VIOLENCE:
        case CardId::SWIFT_STRIKE:
            return 27;

        case CardId::IMMOLATE:
        case CardId::CLASH:
        case CardId::BLOOD_FOR_BLOOD:
        case CardId::BLUDGEON:
        case CardId::CARNAGE:
        case CardId::REAPER:
        case CardId::DRAMATIC_ENTRANCE:
            return 30;

        case CardId::SEARING_BLOW:
        case CardId::RAMPAGE:
        case CardId::POMMEL_STRIKE:
        case CardId::HAND_OF_GREED:
        case CardId::PERFECTED_STRIKE:
        case CardId::HEMOKINESIS:
        case CardId::HEAVY_BLADE:
        case CardId::WILD_STRIKE:
        case CardId::SWORD_BOOMERANG:
            return 35;

        case CardId::DROPKICK:
        case CardId::FIEND_FIRE:
        case CardId::CLEAVE:
        case CardId::RECKLESS_CHARGE:
        case CardId::FLAME_BARRIER:
        case CardId::FLASH_OF_STEEL:
        case CardId::FEED:
        case CardId::RITUAL_DAGGER:
            return 40;

        case CardId::PUMMEL:
        case CardId::TWIN_STRIKE:
        case CardId::CLOTHESLINE:
        case CardId::ANGER:
        case CardId::BITE:
        case CardId::BANDAGE_UP:
        case CardId::BODY_SLAM:
        case CardId::MIND_BLAST:
            return 50;

        case CardId::SENTINEL:
        case CardId::IRON_WAVE:
        case CardId::ENTRENCH:
        case CardId::PURITY:
            return 52;

        case CardId::STRIKE_RED:
        case CardId::DEFEND_RED:
        case CardId::HEADBUTT:
            return 55;

        case CardId::BURN:
        case CardId::DECAY:
        case CardId::CLUMSY:
        case CardId::WOUND:
        case CardId::DAZED:
        case CardId::VOID:
            return 60;


        case CardId::REGRET:
        case CardId::SHAME:
        case CardId::DOUBT:
            return 65;

        case CardId::SLIMED:
        case CardId::NECRONOMICURSE:
        case CardId::WRITHE:
        case CardId::PRIDE:
        case CardId::PARASITE:
        case CardId::ASCENDERS_BANE:
        case CardId::CURSE_OF_THE_BELL:
        case CardId::INJURY:
            return 70;

        case CardId::INVALID:
        default:
            return 10000;
    }

}

int search::Expert::getBossRelicOrdering(RelicId id) {
    switch (id) {
        case RelicId::RUNIC_DOME:
            return -1;

        case RelicId::ECTOPLASM:
        case RelicId::COFFEE_DRIPPER:
        case RelicId::FUSION_HAMMER:
            return 0;

        case RelicId::PANDORAS_BOX:
        case RelicId::BUSTED_CROWN:
        case RelicId::CURSED_KEY:
        case RelicId::SOZU:
            return 1;

        case RelicId::VELVET_CHOKER:
        case RelicId::SNECKO_EYE:
        case RelicId::SLAVERS_COLLAR:
        case RelicId::MARK_OF_PAIN:
            return 2;

        case RelicId::BLACK_BLOOD:
            return 3;

        case RelicId::TINY_HOUSE:
        case RelicId::ASTROLABE:
        case RelicId::CALLING_BELL:
            return 5;

        case RelicId::RUNIC_CUBE:
        case RelicId::PHILOSOPHERS_STONE:
            return 5;

        case RelicId::SACRED_BARK:
        case RelicId::RUNIC_PYRAMID:
        case RelicId::BLACK_STAR:
            return 10;

        default:
            return 100;
    }
}

int search::Expert::getObtainWeight(CardId id) {
    switch (id) {
        case CardId::APOTHEOSIS:
        case CardId::APPARITION:
        case CardId::CORRUPTION:
            return 100;

        case CardId::IMPERVIOUS:
        case CardId::DEMON_FORM:
            return 95;

        case CardId::BARRICADE:
        case CardId::FEEL_NO_PAIN:
            return 85;

        case CardId::MASTER_OF_STRATEGY:
            return 70;

        case CardId::RITUAL_DAGGER:
        case CardId::BRUTALITY:
        case CardId::FIEND_FIRE:
        case CardId::SECRET_TECHNIQUE:
        case CardId::BLUDGEON:
        case CardId::IMMOLATE:
        case CardId::OFFERING:
        case CardId::FLEX:
        case CardId::SPOT_WEAKNESS:
            return 65;

        case CardId::LIMIT_BREAK:
            return 40;

        case CardId::BERSERK:
        case CardId::WHIRLWIND:
            return 35;

        case CardId::REAPER:
        case CardId::DARK_EMBRACE:
        case CardId::MAYHEM:
        case CardId::PANACEA:
        case CardId::SECRET_WEAPON:
        case CardId::INFLAME:
        case CardId::CARNAGE:
        case CardId::DRAMATIC_ENTRANCE:
        case CardId::JAX:
            return 30;

        case CardId::RUPTURE:
        case CardId::FEED:
            return 25;

        case CardId::BLOOD_FOR_BLOOD:
        case CardId::ARMAMENTS:
        case CardId::DARK_SHACKLES:
        case CardId::FINESSE:
        case CardId::BATTLE_TRANCE:
        case CardId::DISARM:
        case CardId::BLIND:
        case CardId::TRIP:
        case CardId::SHOCKWAVE:
        case CardId::PERFECTED_STRIKE:
        case CardId::EVOLVE:
            return 20;

        case CardId::UPPERCUT:
        case CardId::POWER_THROUGH:
        case CardId::SECOND_WIND:
        case CardId::FLAME_BARRIER:
            return 18;

        case CardId::SEVER_SOUL:
        case CardId::INTIMIDATE:
        case CardId::JUGGERNAUT:
        case CardId::MADNESS:
        case CardId::RAGE:
        case CardId::TRANSMUTATION:
        case CardId::HAND_OF_GREED:
        case CardId::SHRUG_IT_OFF:
            return 15;

        case CardId::CLOTHESLINE:
        case CardId::ENTRENCH:
        case CardId::ENLIGHTENMENT:
        case CardId::FLASH_OF_STEEL:
            return 12;

        case CardId::FIRE_BREATHING:
        case CardId::COMBUST:
        case CardId::CLEAVE:
        case CardId::PANACHE:
        case CardId::EXHUME:
        case CardId::SEEING_RED:
        case CardId::SADISTIC_NATURE:
        case CardId::PANIC_BUTTON:
        case CardId::BURNING_PACT:
        case CardId::DOUBLE_TAP:
        case CardId::DUAL_WIELD:
        case CardId::CHRYSALIS:
        case CardId::DISCOVERY:
        case CardId::INFERNAL_BLADE:
        case CardId::METAMORPHOSIS:
        case CardId::HAVOC:
        case CardId::SENTINEL:
        case CardId::HEMOKINESIS:
        case CardId::HEAVY_BLADE:
            return 8;

        case CardId::METALLICIZE:
        case CardId::ANGER:
        case CardId::THINKING_AHEAD:
        case CardId::MAGNETISM:
        case CardId::JACK_OF_ALL_TRADES:
        case CardId::DEEP_BREATH:
        case CardId::IMPATIENCE:
        case CardId::TRUE_GRIT:
        case CardId::THE_BOMB:
        case CardId::WARCRY:
        case CardId::GHOSTLY_ARMOR:
        case CardId::WILD_STRIKE:
        case CardId::POMMEL_STRIKE:
            return 6;

        case CardId::BLOODLETTING:
        case CardId::GOOD_INSTINCTS:
        case CardId::VIOLENCE:
        case CardId::SWIFT_STRIKE:
        case CardId::FORETHOUGHT:
        case CardId::HEADBUTT:
            return 5;

        case CardId::CLASH:
        case CardId::PUMMEL:
            return 4;

        case CardId::THUNDERCLAP:
        case CardId::RAMPAGE:
        case CardId::SWORD_BOOMERANG:
            return 3;

        case CardId::DROPKICK:
        case CardId::RECKLESS_CHARGE:
        case CardId::IRON_WAVE:
        case CardId::TWIN_STRIKE:
            return 2;

        case CardId::BITE:
        case CardId::BANDAGE_UP:
        case CardId::BASH:
        case CardId::BODY_SLAM:
            return 1;

        case CardId::MIND_BLAST:
        case CardId::PURITY:
        case CardId::SEARING_BLOW:
        case CardId::STRIKE_RED:
        case CardId::DEFEND_RED:
            return 0;

        default:
            return 0;
    }
}


int search::Expert::getObtainWeight(CardId id, bool upgraded) {
    if (upgraded) {
        return getObtainWeight(id) * 1.5;
    } else {
        return getObtainWeight(id);
    }
}






