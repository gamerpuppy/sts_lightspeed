//
// Created by gamerpuppy on 6/24/2021.
//

#include <sts/game/neow.hpp>
#include <sts/common/random.hpp>
#include <sts/game/misc.hpp>

#include <sts/constants/card_pools.hpp>

using namespace sts;

std::array<Neow::Option, 4> Neow::getOptions(Random &r) {
    std::array<Option, 4> rewards {};
    rewards[0].r = static_cast<Bonus>(r.random(0, 5));
    rewards[0].d = Drawback::NONE;
    rewards[1].r = static_cast<Bonus>(6 + r.random(0, 4));
    rewards[1].d = Drawback::NONE;

    rewards[2].d = static_cast<Drawback>(2 + r.random(0, 3));
    switch (rewards[2].d) {
        case Drawback::TEN_PERCENT_HP_LOSS: {
            static constexpr Bonus myRewards[]{
                    Bonus::RANDOM_COLORLESS_2,
                    Bonus::REMOVE_TWO,
                    Bonus::ONE_RARE_RELIC,
                    Bonus::THREE_RARE_CARDS,
                    Bonus::TWO_FIFTY_GOLD,
                    Bonus::TRANSFORM_TWO_CARDS,
            };
            rewards[2].r = myRewards[r.random(0, 5)];
            break;
        }

        case Drawback::NO_GOLD: {
            static constexpr Bonus myRewards[]{
                    Bonus::RANDOM_COLORLESS_2,
                    Bonus::REMOVE_TWO,
                    Bonus::ONE_RARE_RELIC,
                    Bonus::THREE_RARE_CARDS,
                    Bonus::TRANSFORM_TWO_CARDS,
                    Bonus::TWENTY_PERCENT_HP_BONUS,
            };
            rewards[2].r = myRewards[r.random(0, 5)];
            break;
        }

        case Drawback::CURSE: {
            static constexpr Bonus myRewards[]{
                    Bonus::RANDOM_COLORLESS_2,
                    Bonus::ONE_RARE_RELIC,
                    Bonus::THREE_RARE_CARDS,
                    Bonus::TWO_FIFTY_GOLD,
                    Bonus::TRANSFORM_TWO_CARDS,
                    Bonus::TWENTY_PERCENT_HP_BONUS,
            };
            rewards[2].r = myRewards[r.random(0, 5)];
            break;
        }

        case Drawback::PERCENT_DAMAGE:
            rewards[2].r = static_cast<Bonus>(11 + r.random(0, 6));
            break;

        default:    // should not happen
            break;

    }

    rewards[3].r = Bonus::BOSS_RELIC;
    rewards[3].d = Drawback::LOSE_STARTER_RELIC;
    r.random(0, 0);

    return rewards;
}

CardReward sts::Neow::getColorlessCardReward(Random &neowRng, Random& cardRng, bool rareOnly) {
    CardReward reward;
    for (int i = 0; i < 3; ++i) {

        CardRarity rarity = neowRng.randomBoolean(0.33F) ? CardRarity::UNCOMMON : CardRarity::COMMON;
        if (rareOnly) {
            rarity = CardRarity::RARE;
        } else if (rarity == CardRarity::COMMON) {
            rarity = CardRarity::UNCOMMON;
        }

        Card card = getRandomColorlessCardNeow(cardRng, rarity);
        while (true) {
            bool containsCard = false;
            for (int x = 0; x < i; ++x) {
                if (reward[x].id == card.id) {
                    containsCard = true;
                    break;
                }
            }
            if (containsCard) {
                card = getRandomColorlessCardNeow(cardRng, rarity);
            } else {
                break;
            }
        }
        reward.push_back(card);
    }

    return reward;
}

CardReward sts::Neow::getCardReward(Random &rng, CharacterClass cc, bool rareOnly) {
    CardReward reward;

    for (int i = 0; i < 3; ++i) {
        CardRarity rarity = rng.randomBoolean(0.33F) ? CardRarity::UNCOMMON : CardRarity::COMMON;
        if (rareOnly) {
            rarity = CardRarity::RARE;
        }

        Card card = getRandomClassCardOfRarity(rng, cc, rarity);
        while (true) {
            bool containsCard = false;
            for (int x = 0; x < i; ++x) {
                if (reward[x].id == card.id) {
                    containsCard = true;
                    break;
                }
            }
            if (containsCard) {
                card = getRandomClassCardOfRarity(rng, cc, rarity);
            } else {
                break;
            }
        }
        reward.push_back(card);
    }
    return reward;
}
