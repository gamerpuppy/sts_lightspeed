//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_BATTLECONTEXT_H
#define STS_LIGHTSPEED_BATTLECONTEXT_H

#include <vector>
#include <array>

#include "sts_common.h"

#include "data_structure/fixed_list.h"
#include "constants/Potions.h"
#include "constants/MonsterEncounters.h"
#include "combat/InputState.h"
#include "combat/Player.h"
#include "combat/Monster.h"
#include "combat/MonsterGroup.h"
#include "combat/ActionQueue.h"
#include "combat/CardQueue.h"
#include "combat/Actions.h"
#include "combat/CardManager.h"
#include "combat/CardSelectInfo.h"


namespace sts {



    enum class Outcome {
        UNDECIDED=0,
        PLAYER_VICTORY,
        PLAYER_LOSS,
    };

    static constexpr const char * battleOutcomeStrings[] {
            "UNDECIDED",
            "PLAYER_VICTORY",
            "PLAYER_LOSS",
    };

    class GameContext;

    extern thread_local BattleContext *g_debug_bc;

    struct BattleContext {

        // begin for debugging purposes
        inline static int sum = 0; // for preventing optimization in benchmarks
        bool haveUsedDiscoveryAction = false; // for tracking undefined behavior resulting from using the action
        bool undefinedBehaviorEvoked = false; // some cards cause inconsistent outcomes in games
        std::uint64_t seed = 0;
        int floorNum = 0;
        MonsterEncounter encounter = MonsterEncounter::INVALID;
        int loopCount = 0;
        int energyWasted = 0;
        int cardsDrawn = 0;
        // end for debugging purposes

        Random aiRng;
        Random cardRandomRng;
        Random miscRng;
        Random monsterHpRng;
        Random potionRng;
        Random shuffleRng;

        int ascension = 0;
        Outcome outcome = Outcome::UNDECIDED;
        InputState inputState = InputState::EXECUTING_ACTIONS;
        CardSelectInfo cardSelectInfo;

        int monsterTurnIdx = 6;

        bool isBattleOver = false;
        bool endTurnQueued = false;
        bool turnHasEnded = false;
        bool skipMonsterTurn = false;

        ActionQueue<50> actionQueue;
        CardQueue cardQueue;

        int potionCount = 0;
        int potionCapacity = 3;
        std::array<Potion, 5> potions;

        int turn = 0;
        Player player;
        MonsterGroup monsters;
        CardManager cards;

        CardQueueItem curCardQueueItem;

        std::bitset<32> miscBits; // 0 stolen gold check,

        BattleContext() = default;
        BattleContext(const BattleContext &rhs) = default;

// ****************************************

        void init(const GameContext &gc);
        void init(const GameContext &gc, MonsterEncounter encounterToInit);

        void initRelics(const GameContext &gc);

        void exitBattle(GameContext &g) const;
        void updateRelicsOnExit(GameContext &g) const;
        void updateCardsOnExit(Deck &d) const; // for cards like ritual dagger, and eventually lesson learned results

// ****************************************

        void setRequiresStolenGoldCheck(bool value);
        [[nodiscard]] bool requiresStolenGoldCheck() const;
        [[nodiscard]] int getMonsterTurnNumber() const;  // returns 1 for first turn, 2 for second, etc.

// ****************************************

        void executeActions();

        void playCardQueueItem(CardQueueItem);
        void useCard();
        void useNoTriggerCard();

        void useAttackCard();
        void useSkillCard();
        void usePowerCard();

        void onUseAttackCard();
        void onUseSkillCard();
        void onUsePowerCard();

        void onUseStatusOrCurseCard();
        void onAfterUseCard();

        void setState(InputState state);
        void addToTop(const Action &a);
        void addToBot(const Action &a);

        void addToTopCard(CardQueueItem item);
        void addToBotCard(CardQueueItem item);

        void checkCombat();
        void clearPostCombatActions();
        void cleanCardQueue();

        [[nodiscard]] bool isCardPlayAllowed() const;

        // **********************

        void endTurn(); // called when player clicks the end turn button
        void callEndOfTurnActions(); // GameActionManager.callEndTurnActions(), called when the end turn cardQueue item is reached.
        void onTurnEnding(); // AbstractRoom endTurn(), called when the turn actually ends.

        void callEndTurnEarlySequence(); // time eater

        void applyEndOfRoundPowers(); // game : MonsterGroup applyEndOfTurnPowers
        void afterMonsterTurns();

        void obtainPotion(Potion p);
        void discardPotion(int idx);
        void drinkPotion(int idx, int target=0);

        void drawCards(int count);
        void discardAtEndOfTurn();
        void discardAtEndOfTurnHelper();

        void playTopCardInDrawPile(int monsterTargetIdx, bool exhausts);

        void moveToHandHelper(CardInstance c);
        void exhaustSpecificCardInHand(int idx, std::int16_t uniqueId);
        void restoreRetainedCards(int count);
        void exhaustTopCardInHand();

        // this is
        void triggerOnEndOfTurnForPlayingCards();
        void triggerOnOtherCardPlayed(const CardInstance &usedCard);

        template <MonsterStatus s>
        void debuffEnemy(int idx, int amount, bool isSourceMonster=true);

        [[nodiscard]] int calculateCardDamage(const CardInstance &card, int targetIdx, int baseDamage) const;
        [[nodiscard]] int calculateCardBlock(int baseBlock) const;

        void queuePurgeCard(const CardInstance &c, int target);
        void addPurgeCardToCardQueue(const CardQueueItem &item); // not really the front but hey
        void noOpRollMove(); // called by monsters to manipulate the aiRng counter when their rollMove function doesn't do anything

        void onManualDiscard(const CardInstance &c);
        void onShuffle();
        void triggerAndMoveToExhaustPile(CardInstance c);
        void mummifiedHandOnUsePower();

        // card select screens
        void openDiscoveryScreen(std::array<CardId, 3> discoveryCards, int copyCount);
        void openSimpleCardSelectScreen(CardSelectTask task, int count);

        // single card select helpers
        void chooseArmamentsCard(int handIdx);
        void chooseCodexCard(CardId id);
        void chooseDiscardToHandCard(int discardIdx, bool forZeroCost);
        void chooseDiscoveryCard(CardId id);
        void chooseDualWieldCard(int handIdx);
        void chooseExhaustOneCard(int handIdx);
        void chooseExhumeCard(int exhaustIdx);
        void chooseForethoughtCard(int handIdx);
        void chooseHeadbuttCard(int discardIdx);
        void chooseRecycleCard(int handIdx);
        void chooseWarcryCard(int handIdx);

        // multi card helpers
        void chooseDrawToHandCards(const int *idxs, int cardCount);
        void chooseExhaustCards(const fixed_list<int,10> &idxs);
        void chooseGambleCards(const fixed_list<int,10> &idxs);

    };

    std::ostream& operator<<(std::ostream &os, const BattleContext &bc);

    template <PlayerStatus s>
    Action Actions::BuffPlayer(int amount) {
        return {[=] (BattleContext &bc) {
            if (s == PlayerStatus::CORRUPTION && !bc.player.hasStatus<PS::CORRUPTION>()) {
                bc.cards.onBuffCorruption();
            }
            bc.player.buff<s>(amount);
        }};
    }

    template <PlayerStatus s>
    Action Actions::DebuffPlayer(int amount, bool isSourceMonster) {
        return {[=] (BattleContext &bc) {
            bc.player.debuff<s>(amount, isSourceMonster);
        }};
    }

    template<PlayerStatus s>
    Action Actions::DecrementStatus(int amount) {
        return {[=] (BattleContext &bc) {
            bc.player.decrementStatus<s>(amount);
        }};
    }

    template <PlayerStatus s>
    Action Actions::RemoveStatus() {
        return {[=] (BattleContext &bc) {
            bc.player.setHasStatus<s>(false);
        }};
    }

    template<MonsterStatus s>
    Action Actions::BuffEnemy(int idx, int amount) {
        return {[=] (BattleContext &bc) {
            // todo check if alive?
            bc.monsters.arr[idx].buff<s>(amount);
        }};
    }

    template <MonsterStatus s>
    Action Actions::DebuffEnemy(int idx, int amount, bool isSourceMonster) {
        return {[=] (BattleContext &bc) {
            bc.debuffEnemy<s>(idx, amount, isSourceMonster);
        }};
    }

    template <MonsterStatus s>
    Action Actions::DebuffAllEnemy(int amount, bool isSourceMonster) {
        // todo this should just add all to bot immediately, not be called first
        // ^^ never mind i think adding to top is a workaround here

        return {[=] (BattleContext &bc) {
            for (int i = bc.monsters.monsterCount-1; i >= 0; --i) {
                if (bc.monsters.arr[i].isTargetable()) {
                    bc.addToTop(Actions::DebuffEnemy<s>(i, amount, isSourceMonster));
                }
            }
        }};
    }

    template <MonsterStatus s>
    void BattleContext::debuffEnemy(int idx, int amount, bool isSourceMonster) {
        // todo poison and snake skull
        Monster &e = monsters.arr[idx];

        if (e.hasStatus<MS::ARTIFACT>()) {
            e.decrementStatus<MS::ARTIFACT>();
            return;
        }

        e.addDebuff<s>(amount, isSourceMonster);
        if (player.hasStatus<PS::SADISTIC>()) {
            addToBot( Actions::DamageEnemy(idx, player.getStatus<PS::SADISTIC>()) );
        }

        if (s == MS::VULNERABLE && player.hasRelic<RelicId::CHAMPION_BELT>()) {
            addToBot(Actions::DebuffEnemy<MS::WEAK>(idx, 1, false) );
        }
    }


}

#endif //STS_LIGHTSPEED_BATTLECONTEXT_H
