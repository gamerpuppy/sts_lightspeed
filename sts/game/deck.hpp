//
// Created by gamerpuppy on 7/10/2021.
//

#ifndef STS_LIGHTSPEED_DECK_H
#define STS_LIGHTSPEED_DECK_H

#include <functional>

#include <sts/data_structure/fixed_list.hpp>
#include <sts/constants/character_classes.hpp>
#include <sts/game/card.hpp>

namespace sts {

    class Random;
    class GameContext;
    class SelectScreenCard;
    class SaveFile;

    typedef std::function<bool(const Card &)> CardPredicate;

    struct Deck {
        static constexpr int MAX_SIZE = 96;

        fixed_list<Card,MAX_SIZE> cards;

        std::array<int,4> cardTypeCounts {0,0,0,0};
        std::array<int,3> bottleIdxs {-1,-1,-1};

        int upgradeableCount = 0;
        int transformableCount = 0; // does not include cards which are bottled

        void initFromSaveFile(const SaveFile &s);

        [[nodiscard]] int size() const;
        [[nodiscard]] bool hasCurse() const;
        [[nodiscard]] bool isCardBottled(int idx) const;
        [[nodiscard]] bool anyCardBottled() const;

        [[nodiscard]] int getUpgradeableCount() const;
        [[nodiscard]] int getTransformableCount(int limit=-1, bool includeBottled=false) const;
        [[nodiscard]] int getCountMatching(const CardPredicate &predicate, int limit=-1) const;
        [[nodiscard]] fixed_list<int, MAX_SIZE> getIdxsMatching(const CardPredicate &p) const;

        [[nodiscard]] bool hasCardForWingStatue() const;

        void upgradeStrikesAndDefends();
        void upgradeRandomCards(Random &miscRng, int count);
        void transformRandomCards(Random &miscRng, int count);

        void obtain(GameContext &gc, Card card, int count=1); // for all cards
        void obtainRaw(Card card);

        void remove(GameContext &gc, int idx);
        void removeSelected(GameContext &gc, const fixed_list<SelectScreenCard, 3> &selectList);
        void upgrade(int idx);
        void addMatchingToSelectList(fixed_list<SelectScreenCard, MAX_SIZE> &selectList, const CardPredicate &p) const;

        void bottleCard(int idx, CardType bottleType);
        void removeBottle(CardType bottleType);

        void removeAllMatching(GameContext &gc, const CardPredicate &p);

        [[nodiscard]] fixed_list<int, MAX_SIZE> getUpgradeableCardIdxs() const;
    };

}


#endif //STS_LIGHTSPEED_DECK_H
