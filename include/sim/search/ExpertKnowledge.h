//
// Created by keega on 9/19/2021.
//

#ifndef STS_LIGHTSPEED_EXPERTKNOWLEDGE_H
#define STS_LIGHTSPEED_EXPERTKNOWLEDGE_H

#include "sts_common.h"
#include "constants/Cards.h"
#include "constants/Relics.h"

namespace sts::search::Expert {

    int getPlayOrdering(CardId id);
    int getBossRelicOrdering(RelicId id);
    int getObtainWeight(CardId id);
    int getObtainWeight(CardId id, bool upgraded);





}



#endif //STS_LIGHTSPEED_EXPERTKNOWLEDGE_H
