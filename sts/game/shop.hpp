//
// Created by gamerpuppy on 7/11/2021.
//

#ifndef STS_LIGHTSPEED_SHOP_H
#define STS_LIGHTSPEED_SHOP_H

#include <sts/game/card.hpp>
#include <sts/common/random.hpp>

#include <sts/constants/potions.hpp>
#include <sts/constants/relic_ids.hpp>

namespace sts {

    class GameContext;

    struct Shop {
        int prices[13];
        int removeCost;
        Card cards[7];
        Potion potions[3];
        RelicId relics[3];

        static constexpr int REMOVE_PRICE_INCREASE = 25;
        static constexpr int BASE_REMOVE_PRICE = 75;
        static constexpr int SMILING_MASK_PRICE = 50;
        static constexpr float COURIER_FACTOR = 0.80f;
        static constexpr float MEMBERSHIP_CARD_FACTOR = 0.50f;

        void setup(GameContext &gc);
        void setupCards(GameContext &gc);
        void setupRelics(GameContext &gc);
        void setupPotions(GameContext &gc);

        void applyDiscount(float factor);

        void buyCard(GameContext &gc, int idx);
        void buyRelic(GameContext &gc, int idx);
        void buyPotion(GameContext &gc, int idx);
        void buyCardRemove(GameContext &gc);

        int& cardPrice(int idx);
        [[nodiscard]] int cardPrice(int idx) const;

        int& relicPrice(int idx);
        [[nodiscard]] int relicPrice(int idx) const;

        int& potionPrice(int idx);
        [[nodiscard]] int potionPrice(int idx) const;

        static CardRarity rollCardRarityShop(Random &cardRng, int cardRarityAdjustment);
        static int getNewCardPrice(GameContext &gc, CardRarity rarity, bool colorless); // Card
        static int getNewPrice(GameContext &gc, int basePrice); // Potion or Relic
        static int getRemoveCost(const GameContext &gc);
        static RelicTier rollRelicTier(Random &merchantRng);
        static void assignRandomCardExcluding(GameContext &gc, CardType type, CardId excludeCardId, Card &outCard, CardRarity &outRarity);
    };

}


#endif //STS_LIGHTSPEED_SHOP_H
