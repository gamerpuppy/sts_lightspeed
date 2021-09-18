//
// Created by keega on 9/16/2021.
//

#include <pybind11/pybind11.h>
#include "sim/ConsoleSimulator.h"
#include "sim/SimHelpers.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

using namespace sts;

void play() {
    sts::SimulatorContext ctx;
    sts::ConsoleSimulator sim;
    sim.play(std::cin, std::cout, ctx);
}

CardId getCardIdForString(const std::string &s) {
    return SimHelpers::getCardIdForString(s);
}

Card getCardForString(const std::string &s) {
    return {SimHelpers::getCardIdForString(s)};
}

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(slaythespire, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", &add, "A function which adds two numbers");
    m.def("play", &play, "play Slay the Spire Console");
    m.def("getCardForString", &getCardIdForString, "");

    py::class_<Card> card(m, "Card");
    card.def(py::init<CardId>());
    card.def("upgrade", &Card::upgrade);
    card.def_property_readonly("id", &Card::getId)
        .def_property_readonly("upgraded", &Card::isUpgraded)
        .def_property_readonly("upgradeCount", &Card::getUpgraded)
        .def_property_readonly("isInnate", &Card::isInnate)
        .def_property_readonly("isTransformable", &Card::canTransform)
        .def_property_readonly("isUpgradable", &Card::canUpgrade)
        .def_property_readonly("isStrikeCard", &Card::isStrikeCard)
        .def_property_readonly("isStarterStrikeOrDefend", &Card::isStarterStrikeOrDefend)
        .def_property_readonly("rarity", &Card::getRarity)
        .def_property_readonly("type", &Card::getType);

    py::enum_<CardRarity>(m, "CardRarity")
            .value("COMMON", CardRarity::COMMON)
            .value("UNCOMMON", CardRarity::UNCOMMON)
            .value("RARE", CardRarity::RARE)
            .value("BASIC", CardRarity::BASIC)
            .value("SPECIAL", CardRarity::SPECIAL)
            .value("CURSE", CardRarity::CURSE)
            .value("INVALID", CardRarity::INVALID);

    py::enum_<CardColor>(m, "CardColor")
            .value("RED", CardColor::RED)
            .value("GREEN", CardColor::GREEN)
            .value("PURPLE", CardColor::PURPLE)
            .value("COLORLESS", CardColor::COLORLESS)
            .value("CURSE", CardColor::CURSE)
            .value("INVALID", CardColor::INVALID);

    py::enum_<CardType>(m, "CardType")
            .value("ATTACK", CardType::ATTACK)
            .value("SKILL", CardType::SKILL)
            .value("POWER", CardType::POWER)
            .value("CURSE", CardType::CURSE)
            .value("STATUS", CardType::STATUS)
            .value("INVALID", CardType::INVALID);

    py::enum_<CardId>(m, "CardId")
            .value("INVALID", CardId::INVALID)
            .value("ACCURACY", CardId::ACCURACY)
            .value("ZAP", CardId::ZAP);


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

// os.add_dll_directory("C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\bin")


