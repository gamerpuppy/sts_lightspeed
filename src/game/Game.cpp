//
// Created by gamerpuppy on 6/24/2021.
//


#include "data_structure/fixed_list.h"
#include "game/Game.h"

#include <algorithm>
#include <cassert>

using namespace sts;

void RelicContainer::add(RelicInstance r) {
    setHasRelic(r.id, true);
    relics.push_back(r);
}

void RelicContainer::remove(RelicId r) {
    setHasRelic(r, false);
    for (auto it = relics.begin(); it != relics.end(); ++it) {
        if (it->id == r) {
            relics.erase(it);
            break;
        }
    }
}

void RelicContainer::replaceRelic(RelicId o, RelicId r) {
    setHasRelic(o, false);
    setHasRelic(r, true);
    for (auto & relic : relics) {
        if (relic.id == r) {
            relic.id = r;
            break;
        }
    }
}

void RelicContainer::setHasRelic(RelicId r, bool value) {
    const int rIdx = static_cast<int>(r);
    if (value) {
        if (rIdx < 64) {
            relicBits0 |= 1ULL << rIdx;
        } else if (rIdx < 128) {
            relicBits1 |= 1ULL << (rIdx-64);
        } else {
            relicBits2 |= 1ULL << (rIdx-128);
        }
    } else {
        if (rIdx < 64) {
            relicBits0 &= ~(1ULL << rIdx);
        } else if (rIdx < 128) {
            relicBits1 &= ~(1ULL << (rIdx-64));
        } else {
            relicBits2 &= ~(1ULL << (rIdx-128));
        }
    }
}

int RelicContainer::size() const {
    return static_cast<int>(relics.size());
}

bool RelicContainer::has(RelicId r) const {
    const int rIdx = static_cast<int>(r);
    if (rIdx < 64) {
        return relicBits0 & (1ULL << rIdx);
    } else if (rIdx < 128) {
        return relicBits1 & (1ULL << (rIdx-64));
    } else {
        return relicBits2 & (1ULL << (rIdx-128));
    }
}

int RelicContainer::getRelicValue(RelicId r) const {
    for (const auto x : relics) {
        if (x.id == r) {
            return x.data;
        }
    }
    return -1;
}

int& RelicContainer::getRelicValueRef(RelicId r) {
    for (auto &x : relics) {
        if (x.id == r) {
            return x.data;
        }
    }
    return relics[0].data;
}

int SeedHelper::getDigitValue(char c) {
    if (c < 'A') {
        return c - '0';
    }
    if (c < 'O') {
        return c - 'A' + 10;
    }
    return c - 'A' + 9;
}

std::string SeedHelper::getString(std::uint64_t seed) {
    constexpr auto chars = "0123456789ABCDEFGHIJKLMNPQRSTUVWXYZ";

    auto uSeed = static_cast<std::uint64_t>(seed);
    std::string str;

    do {
        int rem = static_cast<int>(uSeed % SEED_BASE);
        uSeed /= SEED_BASE;
        str += chars[rem];
    } while (uSeed != 0);

    for (int i = 0; i < str.size() / 2; i++) {
        std::swap(str[i], str[str.size() - 1 - i]);
    }
    return str;
}

std::uint64_t SeedHelper::getLong(const std::string &seed) {
    std::uint64_t ret = 0;
    for (auto it = seed.begin(); it != seed.end(); ++it) {
        ret *= SEED_BASE;
        int c = toupper(*it);
        int value = getDigitValue(c);
        ret += value;
    }
    return ret;
}

CardId sts::getAnyColorCard(Random &cardRng, CardRarity rarity) { // used for prismatic shard
    switch (rarity) {
        case CardRarity::COMMON: {
            cardRng.randomLong();
            int idx = cardRng.random(static_cast<int>(AnyColorTypeCardPool::commonPoolSize-1));
            return AnyColorTypeCardPool::commonCards[idx];
        }

        case CardRarity::UNCOMMON: {
            cardRng.randomLong();
            int idx = cardRng.random(static_cast<int>(AnyColorTypeCardPool::uncommonPoolSize-1));
            return AnyColorTypeCardPool::uncommonCards[idx];
        }

        case CardRarity::RARE: {
            cardRng.randomLong();
            int idx = cardRng.random(static_cast<int>(AnyColorTypeCardPool::rarePoolSize-1));
            return AnyColorTypeCardPool::rareCards[idx];
        }

        default:
            return CardId::INVALID;
    }
}

CardId sts::getRandomClassCardOfTypeAndRarity(Random &cardRng, CharacterClass cc, CardType type, CardRarity rarity) {
    int size = TypeRarityCardPool::getPoolSize(cc, type, rarity);
    int idx = cardRng.random(size-1);
    return TypeRarityCardPool::getCardFromPool(cc, type, rarity, idx);
}

CardId sts::getRandomClassCardOfRarity(Random &rng, CharacterClass cc, CardRarity rarity) {
    auto groupSize = RarityCardPool::getPoolSize(cc, rarity);
    auto idx = rng.random(groupSize-1);
    return RarityCardPool::getCardFromPool(cc, rarity, idx);
}

CardId sts::getRandomColorlessCardNeow(Random &rng, CardRarity rarity) {
    int size = ColorlessRarityCardPool::getGroupSize(rarity);
    int idx = rng.random(size-1);
    return ColorlessRarityCardPool::getCardAt(rarity, idx);
}

CardId sts::getColorlessCardFromPool(Random &cardRng, CardRarity rarity) {
    int idx = cardRng.random(ColorlessRarityCardPool::getGroupSize(rarity) - 1);
    return ColorlessRarityCardPool::getCardAt(rarity, idx);
}

CardId sts::getRandomCurse(Random &cardRng) {
    int idx = cardRng.random(curseCardPoolSize-1);
    return curseCardPool[idx];
}

CardId sts::getRandomCurse(Random &rng, CardId exclude) {
    int idx = rng.random(curseCardPoolSize-2);
    if (curseCardPool[idx] == exclude) {
        return curseCardPool[idx+1];
    } else {
        return curseCardPool[idx];
    }
}

CardId sts::getTrulyRandomCard(Random &cardRandomRng, CharacterClass cc) {
    int idx = cardRandomRng.random(TrulyRandomCardPool::getPoolSizeForClass(cc)-1);
    return TrulyRandomCardPool::getPoolForClass(cc)[idx];
}

CardId sts::returnTrulyRandomColorlessCardFromAvailable(Random &rng, CardId exclude) {
    int idx = rng.random(srcColorlessCardPoolSize-2);
    if (srcColorlessCardPool[idx] == exclude) {
        return srcColorlessCardPool[idx + 1];
    } else {
        return srcColorlessCardPool[idx];
    }
}

CardId sts::getTrulyRandomColorlessCardInCombat(Random &cardRandomRng) {
    const auto poolSize = CombatColorlessCardPool::getPoolSize();
    const auto idx = cardRandomRng.random(poolSize-1);
    return CombatColorlessCardPool::getCardAt(idx);
}

CardId sts::getTrulyRandomCardInCombat(Random &cardRandomRng, CharacterClass cc) {
    const auto poolSize = CombatCardPool::getPoolSize(cc);
    const auto idx = cardRandomRng.random(poolSize-1);
    return CombatCardPool::getCardAt(cc, idx);
}

CardId sts::getTrulyRandomCardInCombat(Random &cardRandomRng, const CharacterClass cc, const CardType type) {
    const auto poolSize = CombatTypeCardPool::getPoolSize(cc, type);
    const auto idx = cardRandomRng.random(poolSize-1);
    return CombatTypeCardPool::getCardAt(cc, type, idx);
}

// using hacky arguments: status = colorless cards, invalid = any type
std::array<CardId, 3> sts::generateDiscoveryCards(Random &cardRandomRng, CharacterClass cc, CardType type) {
    int cardCount = 0;
    std::array<CardId, 3> cards;

    while (cardCount < 3) {
        CardId id;
        if (type == CardType::INVALID) {
            id = getTrulyRandomCardInCombat(cardRandomRng, cc);

        } else if (type == CardType::STATUS) {
            id = getTrulyRandomColorlessCardInCombat(cardRandomRng);

        } else {
            id = getTrulyRandomCardInCombat(cardRandomRng, cc, type);

        }

        bool haveDuplicate = false;
        for (int i = 0; i < cardCount; ++i) {
            if (cards[i] == id) {
                haveDuplicate = true;
                break;
            }
        }

        if (!haveDuplicate) {
            cards[cardCount++] = id;
        }
    }

#ifdef sts_asserts
    for (auto x: cards) {
        if (x == CardId::INVALID) {
            assert(false);
        }
    }
#endif
    return cards;
}

RelicTier sts::returnRandomRelicTier(Random &relicRng, int act) {
    int commonChance = act == 4 ? 0 : 50;
    int uncommonChance = act == 4 ? 100 : 33;

    int roll = relicRng.random(0,99);
    if (roll < commonChance) {
        return RelicTier::COMMON;
    } else if (roll < uncommonChance+commonChance) {
        return RelicTier::UNCOMMON;
    } else {
        return RelicTier::RARE;
    }

}

RelicTier sts::returnRandomRelicTierElite(Random &relicRng) {
    int roll = relicRng.random(99);
    if (roll < 50) {
        return RelicTier::COMMON;
    } else if (roll > 82) {
        return RelicTier::RARE;
    } else {
        return RelicTier::UNCOMMON;
    }
}

Potion sts::returnRandomPotion(Random &potionRng, CharacterClass cc, bool limited) {
    PotionRarity rarity;

    int roll = potionRng.random(0,99);
    if (roll < 65) {
        rarity = PotionRarity::COMMON;
    } else if (roll < 90) {
        rarity = PotionRarity::UNCOMMON;
    } else {
        rarity = PotionRarity::RARE;
    }

    return returnRandomPotionOfRarity(potionRng, rarity, cc, limited);
}

Potion sts::returnRandomPotionOfRarity(Random &potionRng, PotionRarity rarity, CharacterClass cc, bool limited) {
    // this is dumb.
    Potion temp = getRandomPotion(potionRng, cc);
    bool spamCheck = limited;
    while(potionRarities[static_cast<int>(temp)] != rarity || spamCheck) {
        spamCheck = limited;
        temp = getRandomPotion(potionRng, cc);
        if (temp != Potion::FRUIT_JUICE) {
            spamCheck = false;
        }
    }
    return temp;
}

Potion sts::getRandomPotion(Random &potionRng, CharacterClass cc) {
    int idx = potionRng.random(PotionPool::poolSize-1); // all characters have 33 possible potions
    return PotionPool::getPotionForClass(cc, idx);
}


RelicId sts::getRandomFace(const RelicContainer &relics, Random &miscRng) {
    fixed_list<RelicId, 5> tmpList;

    if (!relics.has(sts::RelicId::CULTIST_HEADPIECE)) {
        tmpList.push_back(sts::RelicId::CULTIST_HEADPIECE);
    }

    if (!relics.has(sts::RelicId::FACE_OF_CLERIC)) {
        tmpList.push_back(sts::RelicId::FACE_OF_CLERIC);
    }

    if (!relics.has(sts::RelicId::GREMLIN_VISAGE)) {
        tmpList.push_back(sts::RelicId::GREMLIN_VISAGE);
    }

    if (!relics.has(sts::RelicId::NLOTHS_HUNGRY_FACE)) {
        tmpList.push_back(sts::RelicId::NLOTHS_HUNGRY_FACE);
    }

    if (!relics.has(sts::RelicId::SSSERPENT_HEAD)) {
        tmpList.push_back(sts::RelicId::SSSERPENT_HEAD);
    }

    if (tmpList.empty()) {
        tmpList.push_back(sts::RelicId::CIRCLET);
    }

    java::Collections::shuffle(tmpList.begin(), tmpList.end(), java::Random(miscRng.randomLong()));
    return tmpList[0];
}

CardId sts::getStartCardForEvent(CharacterClass cc) {
    CardId cards[4] {CardId::BASH, CardId::NEUTRALIZE, CardId::ZAP, CardId::ERUPTION};
    return cards[static_cast<int>(cc)];
}

ChestSize sts::getRandomChestSize(Random &treasureRng) {
    int roll = treasureRng.random(99);
    if (roll < SMALL_CHEST_CHANCE) {
        return ChestSize::SMALL;

    } else  if (roll < SMALL_CHEST_CHANCE + MEDIUM_CHEST_CHANCE) {
        return ChestSize::MEDIUM;

    } else {
        return ChestSize::LARGE;
    }
}

RelicTier sts::getMatryoshkaRelicTier(Random &relicRng) {
    if (relicRng.randomBoolean(0.75f)) {
        return sts::RelicTier::COMMON;
    } else {
        return sts::RelicTier::UNCOMMON;
    }
}

float sts::getUpgradedCardChance(int act, int ascension) {
    if (act < 2) {
        return 0.0f;
    } else if (act == 2) {
        if (ascension < 12) {
            return 0.25f;
        } else {
            return 0.125f;
        }
    } else {
        if (ascension < 12) {
            return 0.50f;
        } else {
            return 0.25f;
        }
    }
}
