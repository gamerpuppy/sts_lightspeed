//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_CHARACTERCLASSES_H
#define STS_LIGHTSPEED_CHARACTERCLASSES_H

#include <cstdint>

namespace sts {

    enum class CharacterClass : std::uint8_t {
        IRONCLAD=0,
        SILENT,
        DEFECT,
        WATCHER,
        INVALID,
    };

    static constexpr const char* characterClassNames[] = {"Ironclad","Silent","Defect","Watcher"};
    static constexpr const char* characterClassEnumNames[] = {"IRONCLAD","SILENT","DEFECT","WATCHER"};

    static const char* getCharacterClassName(CharacterClass cc) {
        return characterClassNames[static_cast<int>(cc)];
    }

}

#endif //STS_LIGHTSPEED_CHARACTERCLASSES_H
