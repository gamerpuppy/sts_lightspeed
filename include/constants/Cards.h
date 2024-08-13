//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_CARDS_H
#define STS_LIGHTSPEED_CARDS_H

#include <cstdint>
#include <algorithm>
#include <array>
#include <cassert>
#include <string>

namespace sts {

    enum class CardId : std::uint16_t {
        INVALID = 0,
        ACCURACY,
        ACROBATICS,
        ADRENALINE,
        AFTER_IMAGE,
        AGGREGATE,
        ALCHEMIZE,
        ALL_FOR_ONE,
        ALL_OUT_ATTACK,
        ALPHA,
        AMPLIFY,
        ANGER,
        APOTHEOSIS,
        APPARITION,
        ARMAMENTS,
        ASCENDERS_BANE,
        AUTO_SHIELDS,
        A_THOUSAND_CUTS,
        BACKFLIP,
        BACKSTAB,
        BALL_LIGHTNING,
        BANDAGE_UP,
        BANE,
        BARRAGE,
        BARRICADE,
        BASH,
        BATTLE_HYMN,
        BATTLE_TRANCE,
        BEAM_CELL,
        BECOME_ALMIGHTY,
        BERSERK,
        BETA,
        BIASED_COGNITION,
        BITE,
        BLADE_DANCE,
        BLASPHEMY,
        BLIND,
        BLIZZARD,
        BLOODLETTING,
        BLOOD_FOR_BLOOD,
        BLUDGEON,
        BLUR,
        BODY_SLAM,
        BOOT_SEQUENCE,
        BOUNCING_FLASK,
        BOWLING_BASH,
        BRILLIANCE,
        BRUTALITY,
        BUFFER,
        BULLET_TIME,
        BULLSEYE,
        BURN,
        BURNING_PACT,
        BURST,
        CALCULATED_GAMBLE,
        CALTROPS,
        CAPACITOR,
        CARNAGE,
        CARVE_REALITY,
        CATALYST,
        CHAOS,
        CHARGE_BATTERY,
        CHILL,
        CHOKE,
        CHRYSALIS,
        CLASH,
        CLAW,
        CLEAVE,
        CLOAK_AND_DAGGER,
        CLOTHESLINE,
        CLUMSY,
        COLD_SNAP,
        COLLECT,
        COMBUST,
        COMPILE_DRIVER,
        CONCENTRATE,
        CONCLUDE,
        CONJURE_BLADE,
        CONSECRATE,
        CONSUME,
        COOLHEADED,
        CORE_SURGE,
        CORPSE_EXPLOSION,
        CORRUPTION,
        CREATIVE_AI,
        CRESCENDO,
        CRIPPLING_CLOUD,
        CRUSH_JOINTS,
        CURSE_OF_THE_BELL,
        CUT_THROUGH_FATE,
        DAGGER_SPRAY,
        DAGGER_THROW,
        DARKNESS,
        DARK_EMBRACE,
        DARK_SHACKLES,
        DASH,
        DAZED,
        DEADLY_POISON,
        DECAY,
        DECEIVE_REALITY,
        DEEP_BREATH,
        DEFEND_BLUE,
        DEFEND_GREEN,
        DEFEND_PURPLE,
        DEFEND_RED,
        DEFLECT,
        DEFRAGMENT,
        DEMON_FORM,
        DEUS_EX_MACHINA,
        DEVA_FORM,
        DEVOTION,
        DIE_DIE_DIE,
        DISARM,
        DISCOVERY,
        DISTRACTION,
        DODGE_AND_ROLL,
        DOOM_AND_GLOOM,
        DOPPELGANGER,
        DOUBLE_ENERGY,
        DOUBLE_TAP,
        DOUBT,
        DRAMATIC_ENTRANCE,
        DROPKICK,
        DUALCAST,
        DUAL_WIELD,
        ECHO_FORM,
        ELECTRODYNAMICS,
        EMPTY_BODY,
        EMPTY_FIST,
        EMPTY_MIND,
        ENDLESS_AGONY,
        ENLIGHTENMENT,
        ENTRENCH,
        ENVENOM,
        EQUILIBRIUM,
        ERUPTION,
        ESCAPE_PLAN,
        ESTABLISHMENT,
        EVALUATE,
        EVISCERATE,
        EVOLVE,
        EXHUME,
        EXPERTISE,
        EXPUNGER,
        FAME_AND_FORTUNE,
        FASTING,
        FEAR_NO_EVIL,
        FEED,
        FEEL_NO_PAIN,
        FIEND_FIRE,
        FINESSE,
        FINISHER,
        FIRE_BREATHING,
        FISSION,
        FLAME_BARRIER,
        FLASH_OF_STEEL,
        FLECHETTES,
        FLEX,
        FLURRY_OF_BLOWS,
        FLYING_KNEE,
        FLYING_SLEEVES,
        FOLLOW_UP,
        FOOTWORK,
        FORCE_FIELD,
        FOREIGN_INFLUENCE,
        FORESIGHT,
        FORETHOUGHT,
        FTL,
        FUSION,
        GENETIC_ALGORITHM,
        GHOSTLY_ARMOR,
        GLACIER,
        GLASS_KNIFE,
        GOOD_INSTINCTS,
        GO_FOR_THE_EYES,
        GRAND_FINALE,
        HALT,
        HAND_OF_GREED,
        HAVOC,
        HEADBUTT,
        HEATSINKS,
        HEAVY_BLADE,
        HEEL_HOOK,
        HELLO_WORLD,
        HEMOKINESIS,
        HOLOGRAM,
        HYPERBEAM,
        IMMOLATE,
        IMPATIENCE,
        IMPERVIOUS,
        INDIGNATION,
        INFERNAL_BLADE,
        INFINITE_BLADES,
        INFLAME,
        INJURY,
        INNER_PEACE,
        INSIGHT,
        INTIMIDATE,
        IRON_WAVE,
        JAX,
        JACK_OF_ALL_TRADES,
        JUDGMENT,
        JUGGERNAUT,
        JUST_LUCKY,
        LEAP,
        LEG_SWEEP,
        LESSON_LEARNED,
        LIKE_WATER,
        LIMIT_BREAK,
        LIVE_FOREVER,
        LOOP,
        MACHINE_LEARNING,
        MADNESS,
        MAGNETISM,
        MALAISE,
        MASTERFUL_STAB,
        MASTER_OF_STRATEGY,
        MASTER_REALITY,
        MAYHEM,
        MEDITATE,
        MELTER,
        MENTAL_FORTRESS,
        METALLICIZE,
        METAMORPHOSIS,
        METEOR_STRIKE,
        MIND_BLAST,
        MIRACLE,
        MULTI_CAST,
        NECRONOMICURSE,
        NEUTRALIZE,
        NIGHTMARE,
        NIRVANA,
        NORMALITY,
        NOXIOUS_FUMES,
        OFFERING,
        OMEGA,
        OMNISCIENCE,
        OUTMANEUVER,
        OVERCLOCK,
        PAIN,
        PANACEA,
        PANACHE,
        PANIC_BUTTON,
        PARASITE,
        PERFECTED_STRIKE,
        PERSEVERANCE,
        PHANTASMAL_KILLER,
        PIERCING_WAIL,
        POISONED_STAB,
        POMMEL_STRIKE,
        POWER_THROUGH,
        PRAY,
        PREDATOR,
        PREPARED,
        PRESSURE_POINTS,
        PRIDE,
        PROSTRATE,
        PROTECT,
        PUMMEL,
        PURITY,
        QUICK_SLASH,
        RAGE,
        RAGNAROK,
        RAINBOW,
        RAMPAGE,
        REACH_HEAVEN,
        REAPER,
        REBOOT,
        REBOUND,
        RECKLESS_CHARGE,
        RECURSION,
        RECYCLE,
        REFLEX,
        REGRET,
        REINFORCED_BODY,
        REPROGRAM,
        RIDDLE_WITH_HOLES,
        RIP_AND_TEAR,
        RITUAL_DAGGER,
        RUPTURE,
        RUSHDOWN,
        SADISTIC_NATURE,
        SAFETY,
        SANCTITY,
        SANDS_OF_TIME,
        SASH_WHIP,
        SCRAPE,
        SCRAWL,
        SEARING_BLOW,
        SECOND_WIND,
        SECRET_TECHNIQUE,
        SECRET_WEAPON,
        SEEING_RED,
        SEEK,
        SELF_REPAIR,
        SENTINEL,
        SETUP,
        SEVER_SOUL,
        SHAME,
        SHIV,
        SHOCKWAVE,
        SHRUG_IT_OFF,
        SIGNATURE_MOVE,
        SIMMERING_FURY,
        SKEWER,
        SKIM,
        SLICE,
        SLIMED,
        SMITE,
        SNEAKY_STRIKE,
        SPIRIT_SHIELD,
        SPOT_WEAKNESS,
        STACK,
        STATIC_DISCHARGE,
        STEAM_BARRIER,
        STORM,
        STORM_OF_STEEL,
        STREAMLINE,
        STRIKE_BLUE,
        STRIKE_GREEN,
        STRIKE_PURPLE,
        STRIKE_RED,
        STUDY,
        SUCKER_PUNCH,
        SUNDER,
        SURVIVOR,
        SWEEPING_BEAM,
        SWIFT_STRIKE,
        SWIVEL,
        SWORD_BOOMERANG,
        TACTICIAN,
        TALK_TO_THE_HAND,
        TANTRUM,
        TEMPEST,
        TERROR,
        THE_BOMB,
        THINKING_AHEAD,
        THIRD_EYE,
        THROUGH_VIOLENCE,
        THUNDERCLAP,
        THUNDER_STRIKE,
        TOOLS_OF_THE_TRADE,
        TRANQUILITY,
        TRANSMUTATION,
        TRIP,
        TRUE_GRIT,
        TURBO,
        TWIN_STRIKE,
        UNLOAD,
        UPPERCUT,
        VAULT,
        VIGILANCE,
        VIOLENCE,
        VOID,
        WALLOP,
        WARCRY,
        WAVE_OF_THE_HAND,
        WEAVE,
        WELL_LAID_PLANS,
        WHEEL_KICK,
        WHIRLWIND,
        WHITE_NOISE,
        WILD_STRIKE,
        WINDMILL_STRIKE,
        WISH,
        WORSHIP,
        WOUND,
        WRAITH_FORM,
        WREATH_OF_FLAME,
        WRITHE,
        ZAP,
    };

    enum class CardRarity {
        COMMON = 0,
        UNCOMMON,
        RARE,
        BASIC,
        SPECIAL,
        CURSE,
        INVALID,
    };

    enum class CardColor {
        RED = 0,
        GREEN,
        BLUE,
        PURPLE,
        COLORLESS,
        CURSE,
        INVALID,
    };

    enum class CardType {
        ATTACK=0,
        SKILL,
        POWER,
        CURSE,
        STATUS,
        INVALID,
    };

    static constexpr int cardRarityPrices[] {50,75,150,-1,-1,-1,-1};
    static constexpr const char* cardRarityStrings[] = {"COMMON","UNCOMMON","RARE","BASIC","SPECIAL","CURSE","INVALID"};
    static constexpr const char* cardColorStrings[]  = {"RED","GREEN","BLUE","PURPLE","COLORLESS","CURSE","INVALID"};
    static constexpr const char* cardTypeStrings[]  = {"ATTACK","SKILL","POWER","CURSE","STATUS","INVALID"};

    static constexpr const char* cardEnumStrings[]  = {"INVALID", "ACCURACY", "ACROBATICS", "ADRENALINE", "AFTER_IMAGE", "AGGREGATE", "ALCHEMIZE", "ALL_FOR_ONE", "ALL_OUT_ATTACK", "ALPHA", "AMPLIFY", "ANGER", "APOTHEOSIS", "APPARITION", "ARMAMENTS", "ASCENDERS_BANE", "AUTO_SHIELDS", "A_THOUSAND_CUTS", "BACKFLIP", "BACKSTAB", "BALL_LIGHTNING", "BANDAGE_UP", "BANE", "BARRAGE", "BARRICADE", "BASH", "BATTLE_HYMN", "BATTLE_TRANCE", "BEAM_CELL", "BECOME_ALMIGHTY", "BERSERK", "BETA", "BIASED_COGNITION", "BITE", "BLADE_DANCE", "BLASPHEMY", "BLIND", "BLIZZARD", "BLOODLETTING", "BLOOD_FOR_BLOOD", "BLUDGEON", "BLUR", "BODY_SLAM", "BOOT_SEQUENCE", "BOUNCING_FLASK", "BOWLING_BASH", "BRILLIANCE", "BRUTALITY", "BUFFER", "BULLET_TIME", "BULLSEYE", "BURN", "BURNING_PACT", "BURST", "CALCULATED_GAMBLE", "CALTROPS", "CAPACITOR", "CARNAGE", "CARVE_REALITY", "CATALYST", "CHAOS", "CHARGE_BATTERY", "CHILL", "CHOKE", "CHRYSALIS", "CLASH", "CLAW", "CLEAVE", "CLOAK_AND_DAGGER", "CLOTHESLINE", "CLUMSY", "COLD_SNAP", "COLLECT", "COMBUST", "COMPILE_DRIVER", "CONCENTRATE", "CONCLUDE", "CONJURE_BLADE", "CONSECRATE", "CONSUME", "COOLHEADED", "CORE_SURGE", "CORPSE_EXPLOSION", "CORRUPTION", "CREATIVE_AI", "CRESCENDO", "CRIPPLING_CLOUD", "CRUSH_JOINTS", "CURSE_OF_THE_BELL", "CUT_THROUGH_FATE", "DAGGER_SPRAY", "DAGGER_THROW", "DARKNESS", "DARK_EMBRACE", "DARK_SHACKLES", "DASH", "DAZED", "DEADLY_POISON", "DECAY", "DECEIVE_REALITY", "DEEP_BREATH", "DEFEND_BLUE", "DEFEND_GREEN", "DEFEND_PURPLE", "DEFEND_RED", "DEFLECT", "DEFRAGMENT", "DEMON_FORM", "DEUS_EX_MACHINA", "DEVA_FORM", "DEVOTION", "DIE_DIE_DIE", "DISARM", "DISCOVERY", "DISTRACTION", "DODGE_AND_ROLL", "DOOM_AND_GLOOM", "DOPPELGANGER", "DOUBLE_ENERGY", "DOUBLE_TAP", "DOUBT", "DRAMATIC_ENTRANCE", "DROPKICK", "DUALCAST", "DUAL_WIELD", "ECHO_FORM", "ELECTRODYNAMICS", "EMPTY_BODY", "EMPTY_FIST", "EMPTY_MIND", "ENDLESS_AGONY", "ENLIGHTENMENT", "ENTRENCH", "ENVENOM", "EQUILIBRIUM", "ERUPTION", "ESCAPE_PLAN", "ESTABLISHMENT", "EVALUATE", "EVISCERATE", "EVOLVE", "EXHUME", "EXPERTISE", "EXPUNGER", "FAME_AND_FORTUNE", "FASTING", "FEAR_NO_EVIL", "FEED", "FEEL_NO_PAIN", "FIEND_FIRE", "FINESSE", "FINISHER", "FIRE_BREATHING", "FISSION", "FLAME_BARRIER", "FLASH_OF_STEEL", "FLECHETTES", "FLEX", "FLURRY_OF_BLOWS", "FLYING_KNEE", "FLYING_SLEEVES", "FOLLOW_UP", "FOOTWORK", "FORCE_FIELD", "FOREIGN_INFLUENCE", "FORESIGHT", "FORETHOUGHT", "FTL", "FUSION", "GENETIC_ALGORITHM", "GHOSTLY_ARMOR", "GLACIER", "GLASS_KNIFE", "GOOD_INSTINCTS", "GO_FOR_THE_EYES", "GRAND_FINALE", "HALT", "HAND_OF_GREED", "HAVOC", "HEADBUTT", "HEATSINKS", "HEAVY_BLADE", "HEEL_HOOK", "HELLO_WORLD", "HEMOKINESIS", "HOLOGRAM", "HYPERBEAM", "IMMOLATE", "IMPATIENCE", "IMPERVIOUS", "INDIGNATION", "INFERNAL_BLADE", "INFINITE_BLADES", "INFLAME", "INJURY", "INNER_PEACE", "INSIGHT", "INTIMIDATE", "IRON_WAVE", "JAX", "JACK_OF_ALL_TRADES", "JUDGMENT", "JUGGERNAUT", "JUST_LUCKY", "LEAP", "LEG_SWEEP", "LESSON_LEARNED", "LIKE_WATER", "LIMIT_BREAK", "LIVE_FOREVER", "LOOP", "MACHINE_LEARNING", "MADNESS", "MAGNETISM", "MALAISE", "MASTERFUL_STAB", "MASTER_OF_STRATEGY", "MASTER_REALITY", "MAYHEM", "MEDITATE", "MELTER", "MENTAL_FORTRESS", "METALLICIZE", "METAMORPHOSIS", "METEOR_STRIKE", "MIND_BLAST", "MIRACLE", "MULTI_CAST", "NECRONOMICURSE", "NEUTRALIZE", "NIGHTMARE", "NIRVANA", "NORMALITY", "NOXIOUS_FUMES", "OFFERING", "OMEGA", "OMNISCIENCE", "OUTMANEUVER", "OVERCLOCK", "PAIN", "PANACEA", "PANACHE", "PANIC_BUTTON", "PARASITE", "PERFECTED_STRIKE", "PERSEVERANCE", "PHANTASMAL_KILLER", "PIERCING_WAIL", "POISONED_STAB", "POMMEL_STRIKE", "POWER_THROUGH", "PRAY", "PREDATOR", "PREPARED", "PRESSURE_POINTS", "PRIDE", "PROSTRATE", "PROTECT", "PUMMEL", "PURITY", "QUICK_SLASH", "RAGE", "RAGNAROK", "RAINBOW", "RAMPAGE", "REACH_HEAVEN", "REAPER", "REBOOT", "REBOUND", "RECKLESS_CHARGE", "RECURSION", "RECYCLE", "REFLEX", "REGRET", "REINFORCED_BODY", "REPROGRAM", "RIDDLE_WITH_HOLES", "RIP_AND_TEAR", "RITUAL_DAGGER", "RUPTURE", "RUSHDOWN", "SADISTIC_NATURE", "SAFETY", "SANCTITY", "SANDS_OF_TIME", "SASH_WHIP", "SCRAPE", "SCRAWL", "SEARING_BLOW", "SECOND_WIND", "SECRET_TECHNIQUE", "SECRET_WEAPON", "SEEING_RED", "SEEK", "SELF_REPAIR", "SENTINEL", "SETUP", "SEVER_SOUL", "SHAME", "SHIV", "SHOCKWAVE", "SHRUG_IT_OFF", "SIGNATURE_MOVE", "SIMMERING_FURY", "SKEWER", "SKIM", "SLICE", "SLIMED", "SMITE", "SNEAKY_STRIKE", "SPIRIT_SHIELD", "SPOT_WEAKNESS", "STACK", "STATIC_DISCHARGE", "STEAM_BARRIER", "STORM", "STORM_OF_STEEL", "STREAMLINE", "STRIKE_BLUE", "STRIKE_GREEN", "STRIKE_PURPLE", "STRIKE_RED", "STUDY", "SUCKER_PUNCH", "SUNDER", "SURVIVOR", "SWEEPING_BEAM", "SWIFT_STRIKE", "SWIVEL", "SWORD_BOOMERANG", "TACTICIAN", "TALK_TO_THE_HAND", "TANTRUM", "TEMPEST", "TERROR", "THE_BOMB", "THINKING_AHEAD", "THIRD_EYE", "THROUGH_VIOLENCE", "THUNDERCLAP", "THUNDER_STRIKE", "TOOLS_OF_THE_TRADE", "TRANQUILITY", "TRANSMUTATION", "TRIP", "TRUE_GRIT", "TURBO", "TWIN_STRIKE", "UNLOAD", "UPPERCUT", "VAULT", "VIGILANCE", "VIOLENCE", "VOID", "WALLOP", "WARCRY", "WAVE_OF_THE_HAND", "WEAVE", "WELL_LAID_PLANS", "WHEEL_KICK", "WHIRLWIND", "WHITE_NOISE", "WILD_STRIKE", "WINDMILL_STRIKE", "WISH", "WORSHIP", "WOUND", "WRAITH_FORM", "WREATH_OF_FLAME", "WRITHE", "ZAP", };
    static constexpr const char* cardNames[] = { "INVALID","Accuracy","Acrobatics","Adrenaline","After Image","Aggregate","Alchemize","All for One","All-Out Attack","Alpha","Amplify","Anger","Apotheosis","Apparition","Armaments","Ascender's Bane","Auto-Shields","A Thousand Cuts","Backflip","Backstab","Ball Lightning","Bandage Up","Bane","Barrage","Barricade","Bash","Battle Hymn","Battle Trance","Beam Cell","Become Almighty","Berserk","Beta","Biased Cognition","Bite","Blade Dance","Blasphemy","Blind","Blizzard","Bloodletting","Blood for Blood","Bludgeon","Blur","Body Slam","Boot Sequence","Bouncing Flask","Bowling Bash","Brilliance","Brutality","Buffer","Bullet Time","Bullseye","Burn","Burning Pact","Burst","Calculated Gamble","Caltrops","Capacitor","Carnage","Carve Reality","Catalyst","Chaos","Charge Battery","Chill","Choke","Chrysalis","Clash","Claw","Cleave","Cloak and Dagger","Clothesline","Clumsy","Cold Snap","Collect","Combust","Compile Driver","Concentrate","Conclude","Conjure Blade","Consecrate","Consume","Coolheaded","Core Surge","Corpse Explosion","Corruption","Creative AI","Crescendo","Crippling Cloud","Crush Joints","Curse of the Bell","Cut Through Fate","Dagger Spray","Dagger Throw","Darkness","Dark Embrace","Dark Shackles","Dash","Dazed","Deadly Poison","Decay","Deceive Reality","Deep Breath","Defend","Defend","Defend","Defend","Deflect","Defragment","Demon Form","Deus Ex Machina","Deva Form","Devotion","Die Die Die","Disarm","Discovery","Distraction","Dodge and Roll","Doom and Gloom","Doppelganger","Double Energy","Double Tap","Doubt","Dramatic Entrance","Dropkick","Dualcast","Dual Wield","Echo Form","Electrodynamics","Empty Body","Empty Fist","Empty Mind","Endless Agony","Enlightenment","Entrench","Envenom","Equilibrium","Eruption","Escape Plan","Establishment","Evaluate","Eviscerate","Evolve","Exhume","Expertise","Expunger","Fame and Fortune","Fasting","Fear No Evil","Feed","Feel No Pain","Fiend Fire","Finesse","Finisher","Fire Breathing","Fission","Flame Barrier","Flash of Steel","Flechettes","Flex","Flurry of Blows","Flying Knee","Flying Sleeves","Follow-Up","Footwork","Force Field","Foreign Influence","Foresight","Forethought","FTL","Fusion","Genetic Algorithm","Ghostly Armor","Glacier","Glass Knife","Good Instincts","Go for the Eyes","Grand Finale","Halt","Hand of Greed","Havoc","Headbutt","Heatsinks","Heavy Blade","Heel Hook","Hello World","Hemokinesis","Hologram","Hyperbeam","Immolate","Impatience","Impervious","Indignation","Infernal Blade","Infinite Blades","Inflame","Injury","Inner Peace","Insight","Intimidate","Iron Wave","J.A.X.","Jack of All Trades","Judgment","Juggernaut","Just Lucky","Leap","Leg Sweep","Lesson Learned","Like Water","Limit Break","Live Forever","Loop","Machine Learning","Madness","Magnetism","Malaise","Masterful Stab","Master of Strategy","Master Reality","Mayhem","Meditate","Melter","Mental Fortress","Metallicize","Metamorphosis","Meteor Strike","Mind Blast","Miracle","Multi-Cast","Necronomicurse","Neutralize","Nightmare","Nirvana","Normality","Noxious Fumes","Offering","Omega","Omniscience","Outmaneuver","Overclock","Pain","Panacea","Panache","Panic Button","Parasite","Perfected Strike","Perseverance","Phantasmal Killer","Piercing Wail","Poisoned Stab","Pommel Strike","Power Through","Pray","Predator","Prepared","Pressure Points","Pride","Prostrate","Protect","Pummel","Purity","Quick Slash","Rage","Ragnarok","Rainbow","Rampage","Reach Heaven","Reaper","Reboot","Rebound","Reckless Charge","Recursion","Recycle","Reflex","Regret","Reinforced Body","Reprogram","Riddle with Holes","Rip and Tear","Ritual Dagger","Rupture","Rushdown","Sadistic Nature","Safety","Sanctity","Sands of Time","Sash Whip","Scrape","Scrawl","Searing Blow","Second Wind","Secret Technique","Secret Weapon","Seeing Red","Seek","Self Repair","Sentinel","Setup","Sever Soul","Shame","Shiv","Shockwave","Shrug It Off","Signature Move","Simmering Fury","Skewer","Skim","Slice","Slimed","Smite","Sneaky Strike","Spirit Shield","Spot Weakness","Stack","Static Discharge","Steam Barrier","Storm","Storm of Steel","Streamline","Strike","Strike","Strike","Strike","Study","Sucker Punch","Sunder","Survivor","Sweeping Beam","Swift Strike","Swivel","Sword Boomerang","Tactician","Talk to the Hand","Tantrum","Tempest","Terror","The Bomb","Thinking Ahead","Third Eye","Through Violence","Thunderclap","Thunder Strike","Tools of the Trade","Tranquility","Transmutation","Trip","True Grit","TURBO","Twin Strike","Unload","Uppercut","Vault","Vigilance","Violence","Void","Wallop","Warcry","Wave of the Hand","Weave","Well-Laid Plans","Wheel Kick","Whirlwind","White Noise","Wild Strike","Windmill Strike","Wish","Worship","Wound","Wraith Form","Wreath of Flame","Writhe","Zap", };
    static constexpr const char* cardStringIds[]  = {"INVALID", "Accuracy", "Acrobatics", "Adrenaline", "After Image", "Aggregate", "Venomology", "All For One", "All Out Attack", "Alpha", "Amplify", "Anger", "Apotheosis", "Ghostly", "Armaments", "AscendersBane", "Auto Shields", "A Thousand Cuts", "Backflip", "Backstab", "Ball Lightning", "Bandage Up", "Bane", "Barrage", "Barricade", "Bash", "BattleHymn", "Battle Trance", "Beam Cell", "BecomeAlmighty", "Berserk", "Beta", "Biased Cognition", "Bite", "Blade Dance", "Blasphemy", "Blind", "Blizzard", "Bloodletting", "Blood for Blood", "Bludgeon", "Blur", "Body Slam", "BootSequence", "Bouncing Flask", "BowlingBash", "Brilliance", "Brutality", "Buffer", "Bullet Time", "Lockon", "Burn", "Burning Pact", "Burst", "Calculated Gamble", "Caltrops", "Capacitor", "Carnage", "CarveReality", "Catalyst", "Chaos", "Conserve Battery", "Chill", "Choke", "Chrysalis", "Clash", "Gash", "Cleave", "Cloak And Dagger", "Clothesline", "Clumsy", "Cold Snap", "Collect", "Combust", "Compile Driver", "Concentrate", "Conclude", "ConjureBlade", "Consecrate", "Consume", "Coolheaded", "Core Surge", "Corpse Explosion", "Corruption", "Creative AI", "Crescendo", "Crippling Poison", "CrushJoints", "CurseOfTheBell", "CutThroughFate", "Dagger Spray", "Dagger Throw", "Darkness", "Dark Embrace", "Dark Shackles", "Dash", "Dazed", "Deadly Poison", "Decay", "DeceiveReality", "Deep Breath", "Defend_B", "Defend_G", "Defend_P", "Defend_R", "Deflect", "Defragment", "Demon Form", "DeusExMachina", "DevaForm", "Devotion", "Die Die Die", "Disarm", "Discovery", "Distraction", "Dodge and Roll", "Doom and Gloom", "Doppelganger", "Double Energy", "Double Tap", "Doubt", "Dramatic Entrance", "Dropkick", "Dualcast", "Dual Wield", "Echo Form", "Electrodynamics", "EmptyBody", "EmptyFist", "EmptyMind", "Endless Agony", "Enlightenment", "Entrench", "Envenom", "Undo", "Eruption", "Escape Plan", "Establishment", "Evaluate", "Eviscerate", "Evolve", "Exhume", "Expertise", "Expunger", "FameAndFortune", "Fasting2", "FearNoEvil", "Feed", "Feel No Pain", "Fiend Fire", "Finesse", "Finisher", "Fire Breathing", "Fission", "Flame Barrier", "Flash of Steel", "Flechettes", "Flex", "FlurryOfBlows", "Flying Knee", "FlyingSleeves", "FollowUp", "Footwork", "Force Field", "ForeignInfluence", "Wireheading", "Forethought", "FTL", "Fusion", "Genetic Algorithm", "Ghostly Armor", "Glacier", "Glass Knife", "Good Instincts", "Go for the Eyes", "Grand Finale", "Halt", "HandOfGreed", "Havoc", "Headbutt", "Heatsinks", "Heavy Blade", "Heel Hook", "Hello World", "Hemokinesis", "Hologram", "Hyperbeam", "Immolate", "Impatience", "Impervious", "Indignation", "Infernal Blade", "Infinite Blades", "Inflame", "Injury", "InnerPeace", "Insight", "Intimidate", "Iron Wave", "J.A.X.", "Jack Of All Trades", "Judgement", "Juggernaut", "JustLucky", "Leap", "Leg Sweep", "LessonLearned", "LikeWater", "Limit Break", "LiveForever", "Loop", "Machine Learning", "Madness", "Magnetism", "Malaise", "Masterful Stab", "Master of Strategy", "MasterReality", "Mayhem", "Meditate", "Melter", "MentalFortress", "Metallicize", "Metamorphosis", "Meteor Strike", "Mind Blast", "Miracle", "Multi-Cast", "Necronomicurse", "Neutralize", "Night Terror", "Nirvana", "Normality", "Noxious Fumes", "Offering", "Omega", "Omniscience", "Outmaneuver", "Steam Power", "Pain", "Panacea", "Panache", "PanicButton", "Parasite", "Perfected Strike", "Perseverance", "Phantasmal Killer", "PiercingWail", "Poisoned Stab", "Pommel Strike", "Power Through", "Pray", "Predator", "Prepared", "PathToVictory", "Pride", "Prostrate", "Protect", "Pummel", "Purity", "Quick Slash", "Rage", "Ragnarok", "Rainbow", "Rampage", "ReachHeaven", "Reaper", "Reboot", "Rebound", "Reckless Charge", "Redo", "Recycle", "Reflex", "Regret", "Reinforced Body", "Reprogram", "Riddle With Holes", "Rip and Tear", "RitualDagger", "Rupture", "Adaptation", "Sadistic Nature", "Safety", "Sanctity", "SandsOfTime", "SashWhip", "Scrape", "Scrawl", "Searing Blow", "Second Wind", "Secret Technique", "Secret Weapon", "Seeing Red", "Seek", "Self Repair", "Sentinel", "Setup", "Sever Soul", "Shame", "Shiv", "Shockwave", "Shrug It Off", "SignatureMove", "Vengeance", "Skewer", "Skim", "Slice", "Slimed", "Smite", "Underhanded Strike", "SpiritShield", "Spot Weakness", "Stack", "Static Discharge", "Steam", "Storm", "Storm of Steel", "Streamline", "Strike_B", "Strike_G", "Strike_P", "Strike_R", "Study", "Sucker Punch", "Sunder", "Survivor", "Sweeping Beam", "Swift Strike", "Swivel", "Sword Boomerang", "Tactician", "TalkToTheHand", "Tantrum", "Tempest", "Terror", "The Bomb", "Thinking Ahead", "ThirdEye", "ThroughViolence", "Thunderclap", "Thunder Strike", "Tools of the Trade", "ClearTheMind", "Transmutation", "Trip", "True Grit", "Turbo", "Twin Strike", "Unload", "Uppercut", "Vault", "Vigilance", "Violence", "Void", "Wallop", "Warcry", "WaveOfTheHand", "Weave", "Well Laid Plans", "WheelKick", "Whirlwind", "White Noise", "Wild Strike", "WindmillStrike", "Wish", "Worship", "Wound", "Wraith Form v2", "WreathOfFlame", "Writhe", "Zap", };

    static const CardId getCardIdFromId(std::string id) {
        auto it = std::find(std::begin(cardStringIds), std::end(cardStringIds), id);
        if (it == std::end(cardStringIds)) return CardId::INVALID;
        auto idx = it - std::begin(cardStringIds);
        return static_cast<CardId>(idx);
    }

    static constexpr CardColor cardColors[] = { CardColor::INVALID, CardColor::GREEN,CardColor::GREEN,CardColor::GREEN,CardColor::GREEN,CardColor::BLUE,CardColor::GREEN,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::BLUE,CardColor::RED,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::RED,CardColor::CURSE,CardColor::BLUE,CardColor::GREEN,CardColor::GREEN,CardColor::GREEN,CardColor::BLUE,CardColor::COLORLESS,CardColor::GREEN,CardColor::BLUE,CardColor::RED,CardColor::RED,CardColor::PURPLE,CardColor::RED,CardColor::BLUE,CardColor::COLORLESS,CardColor::RED,CardColor::COLORLESS,CardColor::BLUE,CardColor::COLORLESS,CardColor::GREEN,CardColor::PURPLE,CardColor::COLORLESS,CardColor::BLUE,CardColor::RED,CardColor::RED,CardColor::RED,CardColor::GREEN,CardColor::RED,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::RED,CardColor::BLUE,CardColor::COLORLESS,CardColor::RED,CardColor::GREEN,CardColor::GREEN,CardColor::GREEN,CardColor::BLUE,CardColor::RED,CardColor::PURPLE,CardColor::GREEN,CardColor::BLUE,CardColor::BLUE,CardColor::BLUE,CardColor::GREEN,CardColor::COLORLESS,CardColor::RED,CardColor::BLUE,CardColor::RED,CardColor::GREEN,CardColor::RED,CardColor::CURSE,CardColor::BLUE,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::RED,CardColor::PURPLE,CardColor::PURPLE,CardColor::PURPLE,CardColor::BLUE,CardColor::BLUE,CardColor::BLUE,CardColor::GREEN,CardColor::RED,CardColor::BLUE,CardColor::PURPLE,CardColor::GREEN,CardColor::PURPLE,CardColor::CURSE,CardColor::PURPLE,CardColor::GREEN,CardColor::GREEN,CardColor::BLUE,CardColor::RED,CardColor::COLORLESS,CardColor::GREEN,CardColor::COLORLESS,CardColor::GREEN,CardColor::CURSE,CardColor::PURPLE,CardColor::COLORLESS,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::RED,CardColor::GREEN,CardColor::BLUE,CardColor::RED,CardColor::PURPLE,CardColor::PURPLE,CardColor::PURPLE,CardColor::GREEN,CardColor::RED,CardColor::COLORLESS,CardColor::GREEN,CardColor::GREEN,CardColor::BLUE,CardColor::GREEN,CardColor::BLUE,CardColor::RED,CardColor::CURSE,CardColor::COLORLESS,CardColor::RED,CardColor::BLUE,CardColor::RED,CardColor::BLUE,CardColor::BLUE,CardColor::PURPLE,CardColor::PURPLE,CardColor::PURPLE,CardColor::GREEN,CardColor::COLORLESS,CardColor::RED,CardColor::GREEN,CardColor::BLUE,CardColor::PURPLE,CardColor::GREEN,CardColor::PURPLE,CardColor::PURPLE,CardColor::GREEN,CardColor::RED,CardColor::RED,CardColor::GREEN,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::PURPLE,CardColor::PURPLE,CardColor::RED,CardColor::RED,CardColor::RED,CardColor::COLORLESS,CardColor::GREEN,CardColor::RED,CardColor::BLUE,CardColor::RED,CardColor::COLORLESS,CardColor::GREEN,CardColor::RED,CardColor::PURPLE,CardColor::GREEN,CardColor::PURPLE,CardColor::PURPLE,CardColor::GREEN,CardColor::BLUE,CardColor::PURPLE,CardColor::PURPLE,CardColor::COLORLESS,CardColor::BLUE,CardColor::BLUE,CardColor::BLUE,CardColor::RED,CardColor::BLUE,CardColor::GREEN,CardColor::COLORLESS,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::COLORLESS,CardColor::RED,CardColor::RED,CardColor::BLUE,CardColor::RED,CardColor::GREEN,CardColor::BLUE,CardColor::RED,CardColor::BLUE,CardColor::BLUE,CardColor::RED,CardColor::COLORLESS,CardColor::RED,CardColor::PURPLE,CardColor::RED,CardColor::GREEN,CardColor::RED,CardColor::CURSE,CardColor::PURPLE,CardColor::COLORLESS,CardColor::RED,CardColor::RED,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::PURPLE,CardColor::RED,CardColor::PURPLE,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::PURPLE,CardColor::RED,CardColor::COLORLESS,CardColor::BLUE,CardColor::BLUE,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::GREEN,CardColor::GREEN,CardColor::COLORLESS,CardColor::PURPLE,CardColor::COLORLESS,CardColor::PURPLE,CardColor::BLUE,CardColor::PURPLE,CardColor::RED,CardColor::COLORLESS,CardColor::BLUE,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::BLUE,CardColor::CURSE,CardColor::GREEN,CardColor::GREEN,CardColor::PURPLE,CardColor::CURSE,CardColor::GREEN,CardColor::RED,CardColor::COLORLESS,CardColor::PURPLE,CardColor::GREEN,CardColor::BLUE,CardColor::CURSE,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::CURSE,CardColor::RED,CardColor::PURPLE,CardColor::GREEN,CardColor::GREEN,CardColor::GREEN,CardColor::RED,CardColor::RED,CardColor::PURPLE,CardColor::GREEN,CardColor::GREEN,CardColor::PURPLE,CardColor::CURSE,CardColor::PURPLE,CardColor::PURPLE,CardColor::RED,CardColor::COLORLESS,CardColor::GREEN,CardColor::RED,CardColor::PURPLE,CardColor::BLUE,CardColor::RED,CardColor::PURPLE,CardColor::RED,CardColor::BLUE,CardColor::BLUE,CardColor::RED,CardColor::BLUE,CardColor::BLUE,CardColor::GREEN,CardColor::CURSE,CardColor::BLUE,CardColor::BLUE,CardColor::GREEN,CardColor::BLUE,CardColor::COLORLESS,CardColor::RED,CardColor::PURPLE,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::PURPLE,CardColor::PURPLE,CardColor::PURPLE,CardColor::BLUE,CardColor::PURPLE,CardColor::RED,CardColor::RED,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::RED,CardColor::BLUE,CardColor::BLUE,CardColor::RED,CardColor::GREEN,CardColor::RED,CardColor::CURSE,CardColor::COLORLESS,CardColor::RED,CardColor::RED,CardColor::PURPLE,CardColor::PURPLE,CardColor::GREEN,CardColor::BLUE,CardColor::GREEN,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::GREEN,CardColor::PURPLE,CardColor::RED,CardColor::BLUE,CardColor::BLUE,CardColor::BLUE,CardColor::BLUE,CardColor::GREEN,CardColor::BLUE,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::RED,CardColor::PURPLE,CardColor::GREEN,CardColor::BLUE,CardColor::GREEN,CardColor::BLUE,CardColor::COLORLESS,CardColor::PURPLE,CardColor::RED,CardColor::GREEN,CardColor::PURPLE,CardColor::PURPLE,CardColor::BLUE,CardColor::GREEN,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::PURPLE,CardColor::COLORLESS,CardColor::RED,CardColor::BLUE,CardColor::GREEN,CardColor::PURPLE,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::RED,CardColor::BLUE,CardColor::RED,CardColor::GREEN,CardColor::RED,CardColor::PURPLE,CardColor::PURPLE,CardColor::COLORLESS,CardColor::COLORLESS,CardColor::PURPLE,CardColor::RED,CardColor::PURPLE,CardColor::PURPLE,CardColor::GREEN,CardColor::PURPLE,CardColor::RED,CardColor::BLUE,CardColor::RED,CardColor::PURPLE,CardColor::PURPLE,CardColor::PURPLE,CardColor::COLORLESS,CardColor::GREEN,CardColor::PURPLE,CardColor::CURSE,CardColor::BLUE, };
    static constexpr CardRarity cardRarities[] = { CardRarity::INVALID, CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::COMMON,CardRarity::RARE,CardRarity::SPECIAL,CardRarity::COMMON,CardRarity::SPECIAL,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::RARE,CardRarity::BASIC,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::SPECIAL,CardRarity::RARE,CardRarity::SPECIAL,CardRarity::RARE,CardRarity::SPECIAL,CardRarity::COMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::CURSE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::RARE,CardRarity::RARE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::SPECIAL,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::CURSE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::BASIC,CardRarity::BASIC,CardRarity::BASIC,CardRarity::BASIC,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::RARE,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::CURSE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::BASIC,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::COMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::BASIC,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::SPECIAL,CardRarity::SPECIAL,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::RARE,CardRarity::COMMON,CardRarity::RARE,CardRarity::COMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::CURSE,CardRarity::UNCOMMON,CardRarity::SPECIAL,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::SPECIAL,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::COMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::SPECIAL,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::SPECIAL,CardRarity::RARE,CardRarity::SPECIAL,CardRarity::BASIC,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::CURSE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::SPECIAL,CardRarity::RARE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::CURSE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::CURSE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::SPECIAL,CardRarity::COMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::CURSE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::SPECIAL,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::SPECIAL,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::CURSE,CardRarity::SPECIAL,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::SPECIAL,CardRarity::COMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::COMMON,CardRarity::BASIC,CardRarity::BASIC,CardRarity::BASIC,CardRarity::BASIC,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::BASIC,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::COMMON,CardRarity::SPECIAL,CardRarity::COMMON,CardRarity::RARE,CardRarity::RARE,CardRarity::COMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::COMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::BASIC,CardRarity::RARE,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::UNCOMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::COMMON,CardRarity::RARE,CardRarity::UNCOMMON,CardRarity::CURSE,CardRarity::BASIC, };
    static constexpr CardType cardTypes[] = { CardType::INVALID, CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::CURSE,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::POWER,CardType::ATTACK,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::POWER,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::STATUS,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::POWER,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::CURSE,CardType::ATTACK,CardType::SKILL,CardType::POWER,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::CURSE,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::STATUS,CardType::SKILL,CardType::CURSE,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::POWER,CardType::POWER,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::CURSE,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::POWER,CardType::ATTACK,CardType::ATTACK,CardType::POWER,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::POWER,CardType::ATTACK,CardType::ATTACK,CardType::POWER,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::POWER,CardType::CURSE,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::POWER,CardType::SKILL,CardType::POWER,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::CURSE,CardType::ATTACK,CardType::SKILL,CardType::POWER,CardType::CURSE,CardType::POWER,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::CURSE,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::CURSE,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::CURSE,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::CURSE,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::POWER,CardType::POWER,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::CURSE,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::STATUS,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::POWER,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::POWER,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::POWER,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::SKILL,CardType::STATUS,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::ATTACK,CardType::POWER,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::ATTACK,CardType::ATTACK,CardType::SKILL,CardType::SKILL,CardType::STATUS,CardType::POWER,CardType::SKILL,CardType::CURSE,CardType::SKILL, };
    static constexpr std::int16_t cardSortedIdx[] = {184,1,2,4,5,6,349,7,8,9,10,11,12,166,13,14,15,0,16,17,18,19,20,21,22,23,25,24,26,27,28,29,30,31,32,33,34,35,37,36,38,39,40,41,42,43,44,45,46,47,208,48,49,50,51,52,53,54,55,56,57,75,58,59,60,61,164,63,64,65,66,67,68,69,70,71,72,73,74,76,77,78,79,80,81,82,83,84,85,86,87,88,91,89,90,92,93,94,95,96,97,100,101,99,98,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,121,120,122,123,124,125,126,127,128,129,130,344,131,132,133,134,135,136,137,138,139,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,363,162,140,163,165,167,168,169,171,170,172,173,174,175,176,177,178,179,180,181,182,183,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,209,210,211,212,213,216,214,215,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,310,237,238,239,240,241,243,244,245,246,247,248,249,250,251,252,242,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,269,268,270,271,272,273,274,275,276,277,3,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,348,300,301,302,303,304,343,305,306,307,308,309,311,312,313,314,315,317,316,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,336,335,337,62,338,339,340,341,342,345,346,347,350,351,352,353,354,355,356,357,358,359,360,361,362,364,365,366,367,368,369,370,};


    // todo need 2d array for trip card?
    static constexpr bool cardTargets[] = {false, false, false, false, false, false, false, true, false, false, false, true, false, false, false, false, false, false, false, true, true, false, true, true, false, true, false, false, true, false, false, false, false, true, false, false, true, false, false, true, true, false, true, false, false, true, true, false, false, false, true, false, false, false, false, false, false, true, true, true, false, false, false, true, false, true, true, false, false, true, false, true, false, false, true, false, false, false, false, false, false, true, true, false, false, false, false, true, false, true, false, true, false, false, true, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, true, false, true, false, false, false, false, true, false, false, false, true, false, false, false, true, false, false, true, true, false, true, false, true, false, false, false, true, true, false, true, true, true, true, false, false, false, false, false, true, false, false, false, false, true, false, true, false, false, true, false, true, false, true, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, true, false, true, false, true, true, false, false, false, false, false, false, false, true, true, false, false, false, false, true, false, false, false, true, true, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, true, true, false, false, true, false, true, false, false, false, true, false, true, false, false, false, true, true, false, false, true, true, false, false, false, false, false, false, true, false, true, false, false, false, false, false, true, true, true, false, true, false, false, false, false, false, false, false, false, true, false, true, false, false, true, false, true, false, true, false, true, true, false, true, false, false, false, false, false, true, true, true, true, true, false, true, true, false, false, true, false, false, false, true, true, false, true, false, false, false, true, false, false, false, false, false, true, false, false, true, true, true, false, false, false, false, true, false, false, true, false, true, false, false, true, true, false, false, false, false, false, false, false,};

    static constexpr std::int8_t cardBaseDamage[2][371] {
            {-1,-1,-1,-1,-1,-1,-1,10,10,-1,-1,6,-1,-1,-1,-1,-1,-1,-1,11,7,-1,7,4,-1,8,-1,-1,3,-1,-1,-1,-1,7,-1,-1,-1,0,-1,18,32,-1,0,-1,-1,7,12,-1,-1,-1,8,-1,-1,-1,-1,-1,-1,20,6,-1,-1,-1,-1,12,-1,14,3,8,-1,12,-1,6,-1,-1,7,-1,12,-1,5,-1,-1,11,-1,-1,-1,-1,-1,8,-1,7,4,9,-1,-1,-1,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,-1,-1,-1,-1,10,-1,-1,-1,-1,8,5,-1,-1,-1,-1,-1,9,-1,4,-1,-1,-1,-1,9,-1,-1,-1,7,-1,-1,-1,9,-1,-1,8,10,-1,7,-1,6,-1,-1,-1,3,4,-1,4,8,4,7,-1,-1,-1,-1,-1,5,-1,-1,-1,-1,8,-1,3,50,-1,20,-1,9,-1,14,5,-1,15,-1,26,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,-1,-1,-1,-1,3,-1,-1,10,-1,-1,-1,-1,-1,-1,-1,-1,12,-1,-1,-1,-1,10,-1,-1,-1,24,0,-1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,-1,-1,-1,6,9,-1,-1,15,-1,-1,-1,-1,-1,2,-1,8,-1,5,-1,8,10,4,-1,9,7,-1,-1,-1,-1,-1,-1,3,7,15,-1,-1,-1,-1,-1,20,8,7,-1,12,-1,-1,-1,-1,-1,-1,-1,-1,16,-1,4,-1,-1,30,-1,7,-1,6,-1,12,12,-1,-1,-1,-1,-1,-1,-1,15,6,6,6,6,-1,7,24,-1,6,7,-1,3,-1,5,3,-1,-1,-1,-1,-1,20,4,7,-1,-1,-1,-1,-1,-1,5,14,13,-1,-1,-1,-1,9,-1,-1,4,-1,15,5,-1,12,7,-1,-1,-1,-1,-1,-1,-1,},
            {-1,-1,-1,-1,-1,-1,-1,14,14,-1,-1,8,-1,-1,-1,-1,-1,-1,-1,15,10,-1,10,6,-1,10,-1,-1,4,-1,-1,-1,-1,8,-1,-1,-1,0,-1,22,42,-1,0,-1,-1,10,16,-1,-1,-1,11,-1,-1,-1,-1,-1,-1,28,10,-1,-1,-1,-1,12,-1,18,5,11,-1,14,-1,9,-1,-1,10,-1,16,-1,8,-1,-1,15,-1,-1,-1,-1,-1,10,-1,9,6,12,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,14,-1,-1,-1,-1,12,8,-1,-1,-1,-1,-1,14,-1,6,-1,-1,-1,-1,9,-1,-1,-1,9,-1,-1,-1,15,-1,-1,11,12,-1,10,-1,8,-1,-1,-1,6,6,-1,6,11,6,11,-1,-1,-1,-1,-1,6,-1,-1,-1,-1,12,-1,4,60,-1,25,-1,12,-1,14,8,-1,20,-1,34,28,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,7,-1,-1,-1,-1,4,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,16,-1,-1,-1,-1,14,-1,-1,-1,30,0,-1,-1,-1,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,-1,-1,-1,8,10,-1,-1,20,-1,-1,-1,-1,-1,2,-1,12,-1,6,-1,8,15,5,-1,12,10,-1,-1,-1,-1,-1,-1,4,9,15,-1,-1,-1,-1,-1,26,10,10,-1,16,-1,-1,-1,-1,-1,-1,-1,-1,22,-1,6,-1,-1,40,-1,10,-1,9,-1,16,16,-1,-1,-1,-1,-1,-1,-1,20,9,9,9,9,-1,9,32,-1,9,10,-1,3,-1,7,3,-1,-1,-1,-1,-1,30,7,9,-1,-1,-1,-1,-1,-1,7,18,13,-1,-1,-1,-1,12,-1,-1,6,-1,20,8,-1,17,10,-1,-1,-1,-1,-1,-1,-1,},
    };

    static const char * getCardEnumName(CardId id) {
        return cardEnumStrings[static_cast<int>(id)];
    }

    static const char * getCardName(CardId id) {
        return cardNames[static_cast<int>(id)];
    }

    static const char * getCardStringId(CardId id) {
        return cardStringIds[static_cast<int>(id)];
    }

    static CardColor getCardColor(CardId id) {
        return cardColors[static_cast<int>(id)];
    }

    static CardRarity getCardRarity(CardId id) {
        return cardRarities[static_cast<int>(id)];
    }

    static CardType getCardType(CardId id) {
        return cardTypes[static_cast<int>(id)];
    }

    static int getBaseDamage(CardId id, bool upgraded) {
        return cardBaseDamage[upgraded ? 1 : 0][static_cast<int>(id)];
    }

    static constexpr bool isCardEthereal(CardId id, bool upgraded=false) {
        switch (id) {
            case CardId::APPARITION:
            case CardId::ECHO_FORM:
            case CardId::DEVA_FORM:
                return !upgraded;

            case CardId::ASCENDERS_BANE:
            case CardId::CARNAGE:
            case CardId::CLUMSY:
            case CardId::DAZED:
            case CardId::GHOSTLY_ARMOR:
            case CardId::VOID:
                return true;

            default:
                return false;
        }
    }

    static constexpr bool isCardInnate(CardId id, bool upgraded=false) {
        switch (id) {
            case CardId::BACKSTAB:
            case CardId::BOOT_SEQUENCE:
            case CardId::DRAMATIC_ENTRANCE:
            case CardId::MIND_BLAST:
            case CardId::WRITHE:
                return true;

            case CardId::BRUTALITY:
            case CardId::INFINITE_BLADES:
            case CardId::AFTER_IMAGE:
            case CardId::CHILL:
            case CardId::HELLO_WORLD:
            case CardId::STORM:
            case CardId::MACHINE_LEARNING:
            case CardId::BATTLE_HYMN:
            case CardId::ALPHA:
            case CardId::ESTABLISHMENT:
                return upgraded;

            default:
                return false;
        }
    }

    static constexpr bool isCardStrikeCard(CardId id) {
        switch (id) {
            case CardId::METEOR_STRIKE:
            case CardId::PERFECTED_STRIKE:
            case CardId::POMMEL_STRIKE:
            case CardId::SNEAKY_STRIKE:
            case CardId::STRIKE_BLUE:
            case CardId::STRIKE_GREEN:
            case CardId::STRIKE_RED:
            case CardId::STRIKE_PURPLE:
            case CardId::SWIFT_STRIKE:
            case CardId::THUNDER_STRIKE:
            case CardId::TWIN_STRIKE:
            case CardId::WILD_STRIKE:
            case CardId::WINDMILL_STRIKE:
                return true;

            default:
                return false;
        }
    }

    static constexpr bool doesCardExhaust(CardId id, bool upgraded) {
        switch (id) {
            case CardId::WARCRY:
            case CardId::DISARM:
            case CardId::INFERNAL_BLADE:
            case CardId::INTIMIDATE:
            case CardId::PUMMEL:
            case CardId::SHOCKWAVE:
            case CardId::EXHUME:
            case CardId::FEED:
            case CardId::FIEND_FIRE:
            case CardId::IMPERVIOUS:
            case CardId::OFFERING:
            case CardId::REAPER:

            case CardId::PIERCING_WAIL:
            case CardId::BACKSTAB:
            case CardId::CATALYST:
            case CardId::CRIPPLING_CLOUD:
            case CardId::DISTRACTION:
            case CardId::ENDLESS_AGONY:
            case CardId::ADRENALINE:
            case CardId::ALCHEMIZE:
            case CardId::DIE_DIE_DIE:
            case CardId::DOPPELGANGER:
            case CardId::MALAISE:
            case CardId::NIGHTMARE:

            case CardId::BOOT_SEQUENCE:
            case CardId::CHILL:
            case CardId::DOUBLE_ENERGY:
            case CardId::GENETIC_ALGORITHM:
            case CardId::TEMPEST:
            case CardId::WHITE_NOISE:
            case CardId::CORE_SURGE:
            case CardId::FISSION:
            case CardId::REBOOT:
            case CardId::SEEK:

            case CardId::CRESCENDO:
            case CardId::TRANQUILITY:
            case CardId::COLLECT:
            case CardId::FOREIGN_INFLUENCE:
            case CardId::TALK_TO_THE_HAND:
            case CardId::ALPHA:
            case CardId::BLASPHEMY:
            case CardId::CONJURE_BLADE:
            case CardId::DEUS_EX_MACHINA:
            case CardId::LESSON_LEARNED:
            case CardId::OMNISCIENCE:
            case CardId::SCRAWL:
            case CardId::VAULT:
            case CardId::WISH:

            case CardId::BANDAGE_UP:
            case CardId::DARK_SHACKLES:
            case CardId::DRAMATIC_ENTRANCE:
            case CardId::JACK_OF_ALL_TRADES:
            case CardId::MADNESS:
            case CardId::PANACEA:
            case CardId::PANIC_BUTTON:
            case CardId::PURITY:
            case CardId::APOTHEOSIS:
            case CardId::CHRYSALIS:
            case CardId::MASTER_OF_STRATEGY:
            case CardId::METAMORPHOSIS:
            case CardId::TRANSMUTATION:
            case CardId::VIOLENCE:
            case CardId::APPARITION:
            case CardId::BETA:
            case CardId::INSIGHT:
            case CardId::MIRACLE:
            case CardId::RITUAL_DAGGER:
            case CardId::SAFETY:
            case CardId::SHIV:
            case CardId::SMITE:
            case CardId::THROUGH_VIOLENCE:

            case CardId::SLIMED:
            case CardId::PRIDE:
                return true;

            case CardId::LIMIT_BREAK:
            case CardId::CALCULATED_GAMBLE:
            case CardId::HOLOGRAM:
            case CardId::RAINBOW:
            case CardId::DISCOVERY:
            case CardId::SECRET_TECHNIQUE:
            case CardId::SECRET_WEAPON:
            case CardId::THINKING_AHEAD:
                return !upgraded;

            default:
                return false;
        }

    }

    static constexpr bool doesCardSelfRetain(CardId id, bool upgraded) {
        switch (id) {
            case CardId::CRESCENDO:
            case CardId::FLYING_SLEEVES:
            case CardId::PROTECT:
            case CardId::TRANQUILITY:
            case CardId::PERSEVERANCE:
            case CardId::SANDS_OF_TIME:
            case CardId::WINDMILL_STRIKE:
            case CardId::INSIGHT:
            case CardId::MIRACLE:
            case CardId::SAFETY:
            case CardId::SMITE:
            case CardId::THROUGH_VIOLENCE:
                return true;

            case CardId::WORSHIP:
            case CardId::BLASPHEMY:
                return upgraded;

            default:
                return false;
        }
    }

    static constexpr bool isStarterStrikeOrDefend(CardId id) {
        switch(id) {
            case CardId::STRIKE_BLUE:
            case CardId::STRIKE_GREEN:
            case CardId::STRIKE_PURPLE:
            case CardId::STRIKE_RED:
            case CardId::DEFEND_BLUE:
            case CardId::DEFEND_GREEN:
            case CardId::DEFEND_PURPLE:
            case CardId::DEFEND_RED:
                return true;
            default:
                return false;
        }
    }

    static constexpr bool cardTargetsEnemy(CardId id, bool upgraded) {
        switch (id) {
            case CardId::BLIND:
            case CardId::TRIP:
                return !upgraded;

            default:
                return cardTargets[static_cast<int>(id)];
        }
    }

    static constexpr bool isXCost(CardId id) {
        switch (id) {
            case CardId::WHIRLWIND:
            case CardId::REINFORCED_BODY:
            case CardId::TRANSMUTATION:
            case CardId::MALAISE:
            case CardId::SKEWER:
            case CardId::DOPPELGANGER:
            case CardId::TEMPEST:
            case CardId::MULTI_CAST:
            case CardId::COLLECT:
            case CardId::CONJURE_BLADE:
                return true;

            default:
                return false;
        }
    }

    static constexpr int getEnergyCost(CardId id, bool upgraded) {
        switch (id) {
            case CardId::ANGER:
            case CardId::BACKSTAB:
            case CardId::BANDAGE_UP:
            case CardId::BATTLE_TRANCE:
            case CardId::BERSERK:
            case CardId::BLIND:
            case CardId::BLOODLETTING:
            case CardId::BRUTALITY:
            case CardId::CLASH:
            case CardId::DARK_SHACKLES:
            case CardId::DEEP_BREATH:
            case CardId::DRAMATIC_ENTRANCE:
            case CardId::ENLIGHTENMENT:
            case CardId::FINESSE:
            case CardId::FLASH_OF_STEEL:
            case CardId::FLEX:
            case CardId::FORETHOUGHT:
            case CardId::GOOD_INSTINCTS:
            case CardId::IMPATIENCE:
            case CardId::INSIGHT:
            case CardId::INTIMIDATE:
            case CardId::JACK_OF_ALL_TRADES:
            case CardId::JAX:
            case CardId::MASTER_OF_STRATEGY:
            case CardId::MIRACLE:
            case CardId::OFFERING:
            case CardId::PANACEA:
            case CardId::PANACHE:
            case CardId::PANIC_BUTTON:
            case CardId::PURITY:
            case CardId::RECKLESS_CHARGE:
            case CardId::SADISTIC_NATURE:
            case CardId::SECRET_TECHNIQUE:
            case CardId::SECRET_WEAPON:
            case CardId::SWIFT_STRIKE:
            case CardId::THINKING_AHEAD:
            case CardId::VIOLENCE:
            case CardId::WARCRY:
                return 0;

            case CardId::DEFEND_BLUE:
            case CardId::DEFEND_GREEN:
            case CardId::DEFEND_PURPLE:
            case CardId::DEFEND_RED:
            case CardId::DOUBLE_TAP:
            case CardId::STRIKE_BLUE:
            case CardId::STRIKE_GREEN:
            case CardId::STRIKE_PURPLE:
            case CardId::STRIKE_RED:
            case CardId::SWORD_BOOMERANG:
            case CardId::TRIP:
            case CardId::TRUE_GRIT:
            case CardId::WILD_STRIKE:
                return 1;

            case CardId::BASH:
            case CardId::BETA:
            case CardId::CARNAGE:
            case CardId::CHRYSALIS:
            case CardId::CLOTHESLINE:
            case CardId::FIEND_FIRE:
            case CardId::FLAME_BARRIER:
            case CardId::HAND_OF_GREED:
            case CardId::HEAVY_BLADE:
            case CardId::IMMOLATE:
            case CardId::IMPERVIOUS:
            case CardId::JUGGERNAUT:
            case CardId::METAMORPHOSIS:
            case CardId::PERFECTED_STRIKE:
            case CardId::REAPER:
            case CardId::SHOCKWAVE:
            case CardId::THE_BOMB:
            case CardId::UPPERCUT:
            case CardId::SEARING_BLOW:
            case CardId::SEVER_SOUL:
                return 2;

            case CardId::BARRICADE:
            case CardId::CORRUPTION:
                return upgraded ? 2 : 3;

            case CardId::BLUDGEON:
            case CardId::OMEGA:
            case CardId::WRAITH_FORM:
            case CardId::DEMON_FORM:
                return 3;

            case CardId::BLOOD_FOR_BLOOD:
                return upgraded ? 3 : 4;

            case CardId::APOTHEOSIS:
            case CardId::MAGNETISM:
            case CardId::MAYHEM:
            case CardId::MIND_BLAST:
            case CardId::DARK_EMBRACE:
            case CardId::ENTRENCH:
                return upgraded ? 1 : 2;

            case CardId::HAVOC:
            case CardId::SEEING_RED:
            case CardId::BODY_SLAM:
            case CardId::EXHUME:
            case CardId::MADNESS:
                return upgraded ? 0 : 1;


            case CardId::WHIRLWIND:
            case CardId::TRANSMUTATION:
            case CardId::MALAISE:
            case CardId::MULTI_CAST:
            case CardId::REINFORCED_BODY:
                return -1;

            case CardId::BURN:
            case CardId::DAZED:
            case CardId::VOID:
            case CardId::WOUND:
                return -2;

            case CardId::ASCENDERS_BANE:
            case CardId::CLUMSY:
            case CardId::DECAY:
            case CardId::DOUBT:
            case CardId::INJURY:
            case CardId::NORMALITY:
            case CardId::PAIN:
            case CardId::PARASITE:
            case CardId::REGRET:
            case CardId::SHAME:
            case CardId::WRITHE:
            case CardId::NECRONOMICURSE:
            case CardId::CURSE_OF_THE_BELL:
                return -3;


            default:
                return 1;
        }
    }

}


#endif //STS_LIGHTSPEED_CARDS_H
