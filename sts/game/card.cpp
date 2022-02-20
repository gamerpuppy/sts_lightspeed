//
// Created by gamerpuppy on 6/27/2021.
//

#include <sts/game/card.hpp>

using namespace sts;

void Card::upgrade() {
    upgraded = true;
    if (id == CardId::SEARING_BLOW) {
        ++misc;
    }
}

CardId Card::getId() const {
    return id;
}

int Card::getUpgraded() const {
    if (id == CardId::SEARING_BLOW) {
        return misc;
    } else {
        return upgraded;
    }
}

bool Card::isUpgraded() const {
    return upgraded;
}

bool Card::isInnate() const {
    return isCardInnate(id, upgraded);
}

bool Card::isStrikeCard() const {
    return isCardStrikeCard(id);
}

CardType Card::getType() const {
    return getCardType(id);
}

const char *Card::getName() const { // todo show if upgraded
    return getCardName(id);
}

CardRarity Card::getRarity() const {
    return getCardRarity(id);
}

int Card::getBaseDamage() const {
    return cardBaseDamage[upgraded ? 1 : 0][static_cast<int>(id)];
}

bool Card::isStarterStrikeOrDefend() const {
    return sts::isStarterStrikeOrDefend(id);
}

bool Card::isStarterStrike() const {
    int idValue = static_cast<int>(id);

    return idValue >= static_cast<int>(CardId::STRIKE_BLUE) &&
        idValue <= static_cast<int>(CardId::STRIKE_RED);
}

bool Card::canUpgrade() const {
    switch (getType()) {
        case CardType::ATTACK:
            return !upgraded || getId() == CardId::SEARING_BLOW;
        case CardType::SKILL:
        case CardType::POWER:
            return !upgraded;

        case CardType::CURSE:
        case CardType::STATUS:
        case CardType::INVALID:
        default:
            return false;
    }
}

bool Card::canTransform() const {
    bool transformable = !(id == CardId::ASCENDERS_BANE || id == CardId::NECRONOMICURSE || id == CardId::CURSE_OF_THE_BELL);
    return transformable;
}

bool Card::operator==(const Card &rhs) const {
    return id == rhs.id &&
           misc == rhs.misc &&
           upgraded == rhs.upgraded;
}

bool Card::operator!=(const Card &rhs) const {
    return !(rhs == *this);
}
