//
// Created by gamerpuppy on 7/4/2021.
//

#include "combat/CardInstance.h"

#include "combat/BattleContext.h"

using namespace sts;

CardInstance::CardInstance(CardId id, bool upgraded) : id(id), upgraded(upgraded) {
    cost = getEnergyCost(id, upgraded);
    costForTurn = cost;
}

CardInstance::CardInstance(const Card &card) : CardInstance(card.getId(), card.isUpgraded()) {
    if (id == CardId::SEARING_BLOW) {
        specialData = card.getUpgraded();
    } else {
        specialData = card.misc;
    }
}

CardId CardInstance::getId() const {
    return id;
}

CardType CardInstance::getType() const {
    return getCardType(id);
}

const char *CardInstance::getName() const {
    if (upgraded) {

    }
    return cardNames[static_cast<int>(id)];
}

std::int16_t CardInstance::getUniqueId() const {
    return uniqueId;
}

bool CardInstance::isUpgraded() const {
    return upgraded;
}

int CardInstance::getUpgradeCount() const {
    if (id == CardId::SEARING_BLOW) {
        return specialData;
    } else {
        return upgraded & 0x1;
    }
}

bool CardInstance::canUpgrade() const {
    return (!upgraded || id == CardId::SEARING_BLOW) &&
        getType() != CardType::CURSE &&
        getType() != CardType::STATUS;
}

bool CardInstance::isEthereal() const {
    return isCardEthereal(id, upgraded);
}

bool CardInstance::isStrikeCard() const {
    return isCardStrikeCard(id);
}

bool CardInstance::doesExhaust() const {
    return doesCardExhaust(id, upgraded);
}

bool CardInstance::hasSelfRetain() const {
    return doesCardSelfRetain(id, upgraded);
}

bool CardInstance::requiresTarget() const {
    return cardTargetsEnemy(id, isUpgraded());
}

bool CardInstance::isXCost() const {
    return sts::isXCost(id);
}

bool CardInstance::isBloodCard() const {
    return getId() == CardId::BLOOD_FOR_BLOOD || getId() == CardId::MASTERFUL_STAB;
}

bool CardInstance::usesSpecialData() const {
    switch (getId()) {
        case CardId::SEARING_BLOW:
        case CardId::RAMPAGE:
        case CardId::GENETIC_ALGORITHM:
        case CardId::RITUAL_DAGGER:
            return true;

        default:
            return false;
    }
}

void CardInstance::upgradeBaseCost(int newBaseCost) {
    int diff = costForTurn - cost;
    cost = static_cast<int8_t>(newBaseCost);
    if (costForTurn > 0) {
        costForTurn = static_cast<int8_t>(std::max(0, cost + diff));
    }
}

void CardInstance::updateCost(int amount) {
    auto tmpCost = std::max(0, cost + amount);
    int diff = cost - costForTurn;

    if (tmpCost != cost) {
        cost = static_cast<int8_t>(tmpCost);
        costForTurn = static_cast<int8_t>(std::max(0, static_cast<int>(cost)-diff));
    }
}

void CardInstance::setCostForCombat(int newCost) {
    // todo should this set costForTurn to newCost? this isn't used exactly like this in game
    cost = static_cast<int8_t>(newCost);
}

void CardInstance::setCostForTurn(int newCost) {
//#ifdef sts_asserts
//    assert(newCost >= 0);
//#endif
    if (costForTurn >= 0) {
        costForTurn = static_cast<int8_t>(std::max(0,newCost));
    }
}

void CardInstance::setUniqueId(int _uniqueId) {
    this->uniqueId = static_cast<int16_t>(_uniqueId);
}

void CardInstance::upgrade() {

    // todo assert not upgraded here?
    // not sure where this is used

    switch (id) {
        case CardId::SEARING_BLOW:
            ++specialData;
            break;

        case CardId::BLOOD_FOR_BLOOD: // the game upgrades the cost to 3 if the cost is over 4 but would it ever be higher?
            if (!isUpgraded() && cost < 4 && cost > 0) {
                upgradeBaseCost(cost-1);
            }
            break;

        case CardId::BLIND:
        case CardId::TRIP:
            if (!isUpgraded()) {
                // todo change card target here when caching of card info is implemented
            }
            break;


        default:
            break;
    }
    if (!isUpgraded()) {
        upgraded = true;
        // TODO(dmz) is this logic right?
        int newcost = getEnergyCost(id, true);
        if (getEnergyCost(id, false) != newcost) {
            cost = costForTurn = newcost;
        }
    }
}

void CardInstance::tookDamage() {
    if (getId() == CardId::BLOOD_FOR_BLOOD) {
        updateCost(-1);
    } else {
        // masterful stab
        // todo check if it is above a limit
        updateCost(1);
    }

}

std::ostream &CardInstance::printSimpleDesc(std::ostream &os) const {
    os << getName();
    switch (id) {
        case CardId::RITUAL_DAGGER:
        case CardId::RAMPAGE:
            os << "=" << specialData;
            break;

        default:
            break;
    }

    if (upgraded) {
        os << "+";
        if (id == CardId::SEARING_BLOW) {
            os << upgraded;
        }
    }
    return os;
}

void CardInstance::triggerOnExhaust(BattleContext &bc) {
    if (id == CardId::SENTINEL) {
        bc.addToTop( Actions::GainEnergy(upgraded ? 3 : 2) );
    } else if (id == CardId::CURSE_OF_THE_BELL) {
        bc.addToBot(Actions::MakeTempCardInHand(CardId::CURSE_OF_THE_BELL) );
    }
}

void CardInstance::triggerOnManualDiscard(BattleContext &bc) {
    if (id == CardId::REFLEX) {
        bc.addToBot( Actions::DrawCards(upgraded ? 3 : 2) );
    } else if (id == CardId::TACTICIAN) {
        bc.addToTop( Actions::GainEnergy(upgraded ? 2 : 1) );
    }
}

void CardInstance::triggerWhenDrawn(BattleContext &bc, int myHandIdx) {

}

//void CardInstance::triggerWhenDrawn(BattleContext &bc, int myHandIdx) {
//    switch (id) {
//        case CardId::ENDLESS_AGONY:
//            //addToTop((AbstractGameAction)new MakeTempCardInHandAction(makeStatEquivalentCopy()));
//            bc.addToTop(Actions::MakeTempCardInHand(id, upgraded) );
//            break;
//        case CardId::EVISCERATE:
//            //setCostForTurn(this.cost - GameActionManager.totalDiscardedThisTurn);
//            costForTurn = this->cost - bc.player.cardsDiscardedThisTurn;
//            break;
//
//        case CardId::DEUS_EX_MACHINA:
//            bc.addToTop(Actions::MakeTempCardInHand(CardId::MIRACLE, upgraded ? 3 : 2));
//            bc.addToTop(Actions::ExhaustSpecificCardInHand(myHandIdx) );
//            break;
//
//        case CardId::VOID:
//            bc.addToBot( [](BattleContext &bc){ bc.player.energy = std::max(0, bc.player.energy-1); } );
//            break;
//
//            // case CardId::DOUBT:
//            // addToBot((AbstractGameAction)new SetDontTriggerAction(this, false));
//        default:
//            break;
//    }
//}

bool CardInstance::isFreeToPlay(const BattleContext &bc) const {
    return freeToPlayOnce ||
           getType() == CardType::ATTACK && bc.player.hasStatus<PS::FREE_ATTACK_POWER>();
}


bool canUseClash(const BattleContext &bc) {
    for (int i = 0; i < bc.cards.cardsInHand; ++i) {
        const auto &c = bc.cards.hand[i];
        if (c.getType() != sts::CardType::ATTACK) {
            return false;
        }
    }
    return true;
}

bool CardInstance::canUseOnAnyTarget(const BattleContext &bc) const {
    if (requiresTarget()) {
        return canUse(bc, bc.monsters.getFirstTargetable(), false);
    } else {
        return canUse(bc, 0, false);
    }
}

bool CardInstance::canUse(const BattleContext &bc, int target, const bool inAutoplay) const {
    if (this->requiresTarget() && (bc.monsters.areMonstersBasicallyDead() || !bc.monsters.arr[target].isTargetable())) {
        return false;
    }

    // this is handled elsewhere now
//    if (purgeOnUse) {
//        return true;
//    }


    // todo grand finale, signature move, reflex, deus ex machina, tactician
    switch (getType()) {
        case CardType::ATTACK:
            if (bc.player.hasStatus<PS::ENTANGLED>()) {
                return false;
            }
            if (getId() == CardId::CLASH && !canUseClash(bc)) {
                return false;
            }
            break;

        case CardType::SKILL:
            if (id == CardId::SECRET_TECHNIQUE) {
                const bool haveSkill = std::find_if(
                        bc.cards.drawPile.begin(),
                        bc.cards.drawPile.end(),
                        [](const auto &c) { return c.getType() == CardType::SKILL; })
                                != bc.cards.drawPile.end(); // todo maybe speed up
                if (!haveSkill) {
                    return false;
                }
            } else if (id == CardId::SECRET_WEAPON) {
                const bool haveAttack = std::find_if(
                        bc.cards.drawPile.begin(),
                        bc.cards.drawPile.end(),
                        [](const auto &c) { return c.getType() == CardType::ATTACK; })
                                       != bc.cards.drawPile.end(); // todo maybe speed up
                if (!haveAttack) {
                    return false;
                }
            }
            break;

        case CardType::CURSE:
            if (!bc.player.hasRelic<RelicId::BLUE_CANDLE>()) {
                return false;
            }
            break;

        case CardType::STATUS:
            if (!bc.player.hasRelic<RelicId::MEDICAL_KIT>() && id != CardId::SLIMED) {
                return false;
            }
            break;

        case CardType::INVALID:
            return false;

        default:
            break;
    }

    if (bc.player.energy < costForTurn && !isFreeToPlay(bc) && !inAutoplay) {
        return false;
    }

    return true;
}

namespace sts {

    std::ostream &operator<<(std::ostream &os, const CardInstance &c) {
        os << "("
           << c.getName()
           << (c.upgraded ? "+" : "")
           << "," << std::to_string(c.uniqueId)
           << "," << static_cast<int>(c.cost)
           << "," << static_cast<int>(c.costForTurn);

        if (c.usesSpecialData()) {
            os << ",s=" << static_cast<int>(c.specialData);
        }
        if (c.freeToPlayOnce) {
            os << ",f";
        }
        if (c.retain) {
            os << ",r";
        }
        return os  <<")";
    }

}
