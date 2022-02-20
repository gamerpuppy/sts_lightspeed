//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_MAP_HPP
#define STS_LIGHTSPEED_MAP_HPP

#include <array>
#include <string>

#include <sts/constants/rooms.hpp>

namespace sts {

    struct MapNode {
        int x = 0;
        int y = 0;

        int parentCount = 0;
        std::array<int, 6> parents{};

        int edgeCount = 0;
        std::array<int, 3> edges{};

        Room room = Room::NONE;

        void addParent(int parent);
        void addEdge(int edge);
        [[nodiscard]] char getRoomSymbol() const;
        [[nodiscard]] int getMaxEdge() const;
        [[nodiscard]] int getMinEdge() const;
        [[nodiscard]] int getMaxXParent() const;
        [[nodiscard]] int getMinXParent() const;
    };

    struct Map {
        int burningEliteX = -1;
        int burningEliteY = -1;
        int burningEliteBuff = -1;
        std::array<std::array<MapNode, 7>, 15> nodes;

        Map() = default;
        Map(std::uint64_t seed, int ascension, int act, bool assignBurningElite);

        MapNode &getNode(int x, int y);
        [[nodiscard]] const MapNode &getNode(int x, int y) const;
        [[nodiscard]] std::string toString(bool showRoomSymbols=true) const;
        static Map fromSeed(std::uint64_t seed, int ascension= 0, int act= 1, bool assignBurningElite=false);
        static Map act4Map();

        void normalizeParents();
    };

}

#endif //STS_LIGHTSPEED_MAP_HPP
