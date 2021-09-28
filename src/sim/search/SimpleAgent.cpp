//
// Created by keega on 9/27/2021.
//

#include <sim/search/SimpleAgent.h>
#include <game/Game.h>

void sts::search::SimpleAgent::takeAction(sts::GameContext &gc, sts::search::GameAction a) {
    actionHistory.emplace_back(a.bits);
    a.execute(gc);
}

void sts::search::SimpleAgent::takeAction(sts::BattleContext &bc, sts::search::Action a) {
    actionHistory.emplace_back(a.bits);
    a.execute(bc);
}

void sts::search::SimpleAgent::playout(GameContext &gc) {
    BattleContext bc;
    const auto seedStr = std::string(SeedHelper::getString(gc.seed));

    while (gc.outcome == GameOutcome::UNDECIDED) {
        if (gc.screenState == ScreenState::BATTLE) {
            bc = BattleContext();
            bc.init(gc);

            playoutBattle(bc);
            bc.exitBattle(gc);
            continue;
        }
    }
}

void sts::search::SimpleAgent::playoutBattle(sts::BattleContext &bc) {



}
