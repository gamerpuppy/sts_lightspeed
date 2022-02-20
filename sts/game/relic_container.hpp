//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_RELICCONTAINER_H
#define STS_LIGHTSPEED_RELICCONTAINER_H

#include <vector>
#include <cstdint>

#include <sts/constants/relic_ids.hpp>

namespace sts {

    struct RelicInstance {
        RelicId id;
        int data;
    };

    class GameContext;

    struct RelicContainer {

        std::vector<RelicInstance> relics;

        std::uint64_t relicBits0 = 0;
        std::uint64_t relicBits1 = 0;
        std::uint64_t relicBits2 = 0;


        // private
        void setHasRelic(RelicId r, bool value);

        // public
        void add(RelicInstance r);
        void remove(RelicId r);
        void replaceRelic(RelicId o, RelicId r);

        int size() const;
        [[nodiscard]] bool has(RelicId r) const;
        int getRelicValue(RelicId r) const;
        int& getRelicValueRef(RelicId r);

    };


}



#endif //STS_LIGHTSPEED_RELICCONTAINER_H
