//
// Created by gamerpuppy on 7/4/2021.
//

#ifndef STS_LIGHTSPEED_ACTIONS_H
#define STS_LIGHTSPEED_ACTIONS_H

#include <utility>
#include <functional>
#include <cstdint>

#include <sts/constants/player_status_effects.hpp>
#include <sts/constants/monster_status_effects.hpp>
#include <sts/constants/card_ids.hpp>
#include <sts/constants/potions.hpp>

#include <sts/combat/input_state.hpp>
#include <sts/combat/card_select_info.hpp>

#include <sts/combat/action_queue.hpp>

namespace sts {

    class BattleContext;
    class CardInstance;
    class CardQueueItem;

    typedef std::array<std::uint16_t,5> DamageMatrix;

    struct Actions {
        static Action SetState(InputState state);

        // implemented in BattleContext.h because of template
        template <PlayerStatus s> static Action BuffPlayer(int amount=1);
        template <PlayerStatus s> static Action DebuffPlayer(int amount=1, bool isSourceMonster=true);

        template <PlayerStatus s> static Action DecrementStatus(int amount=1);
        template <PlayerStatus s> static Action RemoveStatus();

        template <MonsterStatus> static Action BuffEnemy(int idx, int amount=1);
        template <MonsterStatus> static Action DebuffEnemy(int idx, int amount, bool isSourceMonster=true);
        template <MonsterStatus> static Action DebuffAllEnemy(int amount, bool isSourceMonster=true);


        // todo, action and damage actions should clear certain actions from the queue if the player kills all enemies
        static Action AttackEnemy(int idx, int damage);

        // for cards that do one hit, like cleave
        static Action AttackAllEnemy(int baseDamage);

        // for cards like whirlwind, where damage is calculated once
        static Action AttackAllEnemy(DamageMatrix damageMatrix);

        static Action DamageEnemy(int idx, int damage);

        // all usages of this in game have pureDamage set to true
        static Action DamageAllEnemy(int damage); // todo maybe need to add boolean isPlayerOwner

        static Action AttackPlayer(int idx, int damage);
        static Action DamagePlayer(int damage, bool selfDamage=false);
        
        static Action VampireAttack(int damage); // only used by shelled parasite
        
        static Action PlayerLoseHp(int hp, bool selfDamage=false);
        static Action HealPlayer(int amount);

        static Action MonsterGainBlock(int idx, int amount);
        static Action RollMove(int monsterIdx);
        static Action ReactiveRollMove(); // for writhing mass

        static Action NoOpRollMove();

        static Action ChangeStance(Stance stance);
        static Action GainEnergy(int amount);
        static Action GainBlock(int amount);
        static Action DrawCards(int amount);


        // the draw pile doesn't actually have to be empty
        // the game calls on shuffle when this is created
        static Action EmptyDeckShuffle();

        // used by Deep Breath and Reboot, does not trigger onShuffle relics
        static Action ShuffleDrawPile();

        static Action ShuffleTempCardIntoDrawPile(CardId id, int count=1);

        static Action PlayTopCard(int monsterTargetIdx, bool exhausts);
        static Action MakeTempCardInHand(CardId card, bool upgraded= false, int amount = 1);
        static Action MakeTempCardInHand(CardInstance c, int amount = 1);
        static Action MakeTempCardInDrawPile(const CardInstance &c, int amount, bool shuffleInto);
        static Action MakeTempCardInDiscard(const CardInstance &c, int amount = 1);
        static Action MakeTempCardsInHand(std::vector<CardInstance> cards);

        static Action DiscardNoTriggerCard(); // for doubt, shame, etc, discards the BattleContext.curCard


        static Action ClearCardQueue();
        static Action DiscardAtEndOfTurn();
        static Action DiscardAtEndOfTurnHelper();
        static Action RestoreRetainedCards(int count);

        static Action UnnamedEndOfTurnAction();
        static Action MonsterStartTurnAction();
        static Action TriggerEndOfTurnOrbsAction();

        static Action ExhaustTopCardInHand();
        static Action ExhaustSpecificCardInHand(int idx, std::int16_t uniqueId);

        // Random Actions

        static Action DamageRandomEnemy(int damage); // juggernaut
        static Action GainBlockRandomEnemy(int sourceMonster, int amount);

        static Action SummonGremlins();
        static Action SpawnTorchHeads();
        static Action SpireShieldDebuff(); // only called if player has orb slots

        static Action ExhaustRandomCardInHand(int count);
        static Action MadnessAction();
        static Action RandomizeHandCost();
        static Action UpgradeRandomCardAction(); // Warped Tongs Relic

        static Action CodexAction(); // Nilrys Codex onPlayerEndTurn
        static Action ExhaustMany(int limit);
        static Action GambleAction();
        static Action ToolboxAction();
        static Action FiendFireAction(int targetIdx, int calculatedDamage); // Fiend Fire Card
        static Action SwordBoomerangAction(int baseDamage); // Sword Boomerang

        static Action PutRandomCardsInDrawPile(CardType type, int count); // used by chrysalis and metamorphosis
        static Action DiscoveryAction(CardType type, int amount); // attack potion, skill potion
        static Action InfernalBladeAction();
        static Action JackOfAllTradesAction(bool upgraded);
        static Action TransmutationAction(bool upgraded, int energy, bool useEnergy);
        static Action ViolenceAction(int count);

        static Action BetterDiscardPileToHandAction(int amount, CardSelectTask task); // used by hologram and liquid memories
        static Action ArmamentsAction();
        static Action DualWieldAction(int copyCount);
        static Action ExhumeAction();
        static Action ForethoughtAction(bool upgraded);
        static Action HeadbuttAction();
        static Action ChooseExhaustOne();
        static Action DrawToHandAction(CardSelectTask task, CardType cardType);
        static Action WarcryAction();

        // ************

        static Action TimeEaterPlayCardQueueItem(const CardQueueItem &item);
        static Action UpgradeAllCardsInHand();
        static Action OnAfterCardUsed(); // called UseCardAction in game
        static Action EssenceOfDarkness(int darkOrbsPerSlot); // handle sacred bark
        static Action IncreaseOrbSlots(int count);

        static Action SuicideAction(int monsterIdx, bool triggerRelics);

        static Action PoisonLoseHpAction();
        static Action RemovePlayerDebuffs(); // Orange Pellets Relic

        static Action DualityAction(); // Duality Relic

        static Action ApotheosisAction();
        static Action DropkickAction(int targetIdx);
        static Action EnlightenmentAction(bool upgraded);
        static Action EntrenchAction();
        static Action FeedAction(int idx, int damage, bool upgraded);
        static Action HandOfGreedAction(int idx, int damage, bool upgraded);
        static Action LimitBreakAction();
        static Action ReaperAction(int baseDamage);
        static Action RitualDaggerAction(int idx, int damage);
        static Action SecondWindAction(int blockPerCard);
        static Action SeverSoulExhaustAction();
        static Action SpotWeaknessAction(int target, int strength); // Spot Weakness
        static Action WhirlwindAction(int baseDamage, int energy, bool useEnergy);

        static Action AttackAllMonsterRecursive(DamageMatrix matrix, int timesRemaining);
    };

}

#endif //STS_LIGHTSPEED_ACTIONS_H
