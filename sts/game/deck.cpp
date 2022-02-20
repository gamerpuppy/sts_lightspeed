//
// Created by gamerpuppy on 7/10/2021.
//

#include <sts/game/deck.hpp>

#include <cassert>
#include <algorithm>

#include <sts/common/random.hpp>
#include <sts/common/java.hpp>

#include <sts/game/game_context.hpp>
#include <sts/game/save.hpp>

using namespace sts;

void Deck::initFromSaveFile(const SaveFile &s) {
    // the first card with a matching id will be bottled
    CardId bottledCardIds[] {s.bottledCards[0], s.bottledCards[1], s.bottledCards[2], CardId::INVALID};

    for (const auto &c : s.cards) {
        obtainRaw(c);

        if (c.getId() == bottledCardIds[static_cast<int>(c.getType())]) {
            bottleCard(static_cast<int>(cards.size()-1), c.getType());
            bottledCardIds[static_cast<int>(c.getType())] = CardId::INVALID;
        }
    }
}

int Deck::size() const {
    return cards.size();
}

bool Deck::hasCurse() const {
    return cardTypeCounts[static_cast<int>(CardType::CURSE)];
}

bool Deck::isCardBottled(int idx) const {
    return idx == bottleIdxs[0] || idx == bottleIdxs[1] || idx == bottleIdxs[2];
}

bool Deck::anyCardBottled() const {
    return bottleIdxs[0] == -1 || bottleIdxs[1] == -1 || bottleIdxs[2] == -1;
}

int Deck::getUpgradeableCount() const {
    return upgradeableCount;
}

int Deck::getTransformableCount(int limit, bool includeBottled) const {
    if (includeBottled) {
        int count = transformableCount;
        for (int i = 0; i < 3; ++i) {
            if (bottleIdxs[i] != -1) {
                ++count;
                if (limit != -1 && count >= limit) {
                    return count;
                }
            }
        }
        return count;

    } else {
        return transformableCount;
    }
}

bool Deck::hasCardForWingStatue() const {
    for (const auto &c : cards) {
        if (getBaseDamage(c.getId(), c.getUpgraded()) >= 10) {
            return true;
        }
    }
    return false;
}

int Deck::getCountMatching(const std::function<bool(const Card &)> &predicate, const int limit) const {
    int count = 0;
    for (const auto &card : cards) {
        if (predicate(card)) {
            ++count;
            if (limit != -1 && count > limit) {
                return count;
            }
        }
    }
    return count;
}

fixed_list<int, Deck::MAX_SIZE> Deck::getIdxsMatching(const CardPredicate &p) const {
    fixed_list<int, MAX_SIZE> list;
    for (int i = 0; i < cards.size(); ++i) {
        if (p(cards[i])) {
            list.push_back(i);
        }
    }
    return list;
}

void Deck::upgradeStrikesAndDefends() {
    for (auto &c : cards) {
        if (c.isStarterStrikeOrDefend() && c.canUpgrade()) {
            --upgradeableCount;
            c.upgrade();
        }
    }
}

void Deck::upgradeRandomCards(Random &miscRng, int count) {
    auto list = getUpgradeableCardIdxs();
    java::Collections::shuffle(list.begin(), list.end(), java::Random(miscRng.randomLong()));
    const int end = std::min(list.size(), count);
    for (int i = 0; i < end; ++i) {
        --upgradeableCount;
        cards[list[i]].upgrade();
    }
}

void Deck::transformRandomCards(Random &miscRng, int count) {
    // todo
}

void Deck::obtain(GameContext &gc, Card card, int count) {
    const auto type = card.getType();

    switch (type) {
        case CardType::ATTACK:
            transformableCount += count;
            cardTypeCounts[static_cast<int>(CardType::ATTACK)] += count;
            if (gc.hasRelic(RelicId::MOLTEN_EGG)) {
                card.upgrade();
            } else if (card.canUpgrade()) {
                upgradeableCount += count;
            }
            break;

        case CardType::SKILL:
            transformableCount += count;
            cardTypeCounts[static_cast<int>(CardType::SKILL)] += count;
            if (gc.hasRelic(RelicId::TOXIC_EGG)) {
                card.upgrade();
            } else if (card.canUpgrade()) {
                upgradeableCount += count;
            }
            break;

        case CardType::POWER:
            transformableCount += count;
            cardTypeCounts[static_cast<int>(CardType::POWER)] += count;
            if (gc.hasRelic(RelicId::FROZEN_EGG)) {
                card.upgrade();
            } else if (card.canUpgrade()) {
                upgradeableCount += count;
            }
            break;

        case CardType::CURSE:
            if (gc.relics.has(RelicId::OMAMORI)) {
                int &omamoriCount = gc.relics.getRelicValueRef(RelicId::OMAMORI);
                if (omamoriCount > 0) {
                    int decAmt = std::min(count, omamoriCount);
                    count -= decAmt;
                    omamoriCount -= decAmt;

                    if (count == 0) {
                        return;
                    }
                }
            }
            if (card.canTransform()) {
                transformableCount += count;
                cardTypeCounts[static_cast<int>(CardType::CURSE)] += count;
            }
            break;

        case CardType::STATUS:
        case CardType::INVALID:
        default:
            assert(false);
            break;
    }


    if (gc.relics.has(RelicId::CERAMIC_FISH)) { // todo check if ceramic fish activates before omamori
        gc.obtainGold(9 * count);
    }

    for (int i = 0; i < count; ++i) {
        cards.push_back(card);
    }
}

void Deck::obtainRaw(Card card) {
    cards.push_back(card);
    ++transformableCount;
    ++cardTypeCounts[static_cast<int>(card.getType())];

    switch (card.getType()) {
        case CardType::ATTACK:
        case CardType::SKILL:
        case CardType::POWER:
            ++transformableCount;
            ++cardTypeCounts[static_cast<int>(card.getType())];
            if (card.canUpgrade()) {
                ++upgradeableCount;
            }
            break;

        case CardType::CURSE:
            if (card.canTransform()) {
                ++transformableCount;
                ++cardTypeCounts[static_cast<int>(card.getType())];
            }
            break;

        case CardType::STATUS:
        case CardType::INVALID:
        default:
            assert(false);
            break;
    }
}

void Deck::bottleCard(int idx, CardType bottleType) {
    bottleIdxs[static_cast<int>(bottleType)] = idx;
    --transformableCount;
}

void Deck::removeBottle(CardType bottleType) {
    bottleIdxs[static_cast<int>(bottleType)] = -1;
    ++transformableCount;
}

void Deck::remove(GameContext &gc, int idx) {
    const auto c = cards[idx];

    if (c.getType() == CardType::CURSE){
        if (c.canTransform()) {
            --cardTypeCounts[static_cast<int>(CardType::CURSE)];
            --transformableCount;
        }
        if (c.getId() == CardId::PARASITE) {
            gc.loseMaxHp(3);
        }
        cards.remove(idx);
        return;
    }

    --transformableCount;
    --cardTypeCounts[static_cast<int>(c.getType())];
    if (c.canUpgrade()) {
        --upgradeableCount;
    }

    if (!anyCardBottled()) { // if no bottled cards, can do simple remove
        cards.remove(idx);
        return;
    }

    // for erasing a card from the deck when there is a bottled card
    for (int i = 0; i < 3; ++i) {
        if (idx == bottleIdxs[i]) {
            bottleIdxs[i] = -1;
            ++transformableCount;  // if was in a bottle (not counted as transformable), we need to change the transformable count backIdx

        } else if (idx < bottleIdxs[i]) {
            --bottleIdxs[i]; // shift the bottleIdx to reflect shift in the cards list
        }
    }

    for (int i = idx; i+1 < cards.size(); ++i) {
        cards[i] = cards[i+1]; // shift cards to erase remove card
    }
    cards.resize(cards.size()-1);
}

void Deck::upgrade(int idx) {
    cards[idx].upgrade();
    --upgradeableCount;
}

void Deck::addMatchingToSelectList(fixed_list<SelectScreenCard, Deck::MAX_SIZE> &selectList, const CardPredicate &p) const {
    for (int i = 0; i < cards.size(); ++i) {
        if (p(cards[i])) {
            selectList.push_back({cards[i], i});
        }
    }
}

void Deck::removeSelected(GameContext &gc, const fixed_list<SelectScreenCard, 3> &selectList) {
    int removeIdx[3];
    for (int i = 0; i < selectList.size(); ++i) {
        removeIdx[i] = selectList[i].deckIdx;
    }
    std::sort(removeIdx, removeIdx+selectList.size());

    for (int i = selectList.size() - 1; i >= 0; --i) {
        remove(gc, selectList[i].deckIdx);
    }
}

void Deck::removeAllMatching(GameContext &gc, const std::function<bool(const Card &)> &p) {
//    int cumRemove[MAX_DECK_SIZE+1]; // OwO
//    cumRemove[cards.size()] = 0;
//
//    for (int i = cards.size()-1; i >= 0; --i) {
//        cumRemove[i] = cumRemove[i+1] + (predicate(cards[i]) ? 1 : 0);
//    }
//
//    int x = 0;
//    for (int i = 0; i < cards.size(); ++i) {
//        bool addCurCard = cumRemove[i+1] == cumRemove[i];
//        if (addCurCard) {
//            cards[x++] = cards[i];
//        }
//    }
//    cards.resize(x);

    for (int i = cards.size(); i >= 0; --i) {
        if (p(cards[i])) {
            remove(gc, i);
        }
    }
}

fixed_list<int, Deck::MAX_SIZE> Deck::getUpgradeableCardIdxs() const {
    fixed_list<int,Deck::MAX_SIZE> ret;
    for (int i = 0; i < cards.size(); ++i) {
        if (cards[i].canUpgrade()) {
            ret.push_back(i);
        }
    }
    return ret;
}

