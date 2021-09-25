# sts_lightspeed

For tree search and simulation of the popular rogue-like deckbuilder game Slay The Spire

requires https://github.com/nlohmann/json
(download the zip and place the folder in the main project folder)

**Features**
* c++ 17 compiled with gcc
* Standalone
* Designed to be 100% RNG accurate*
* Playable in console
* Speed: 1M random playouts in 5s with 16 threads
* Loading from save files (loading into combat currently only supported)

**Planned Features**
* Tree search of game outcomes (best result, knowing the state of the game's rng)
* Tree search of possible game outcomes (not given the state of rng)

**Implementation Progress**
* All Enemies
* All relics (in testing)
* All Ironclad cards
* All Colorless cards
* Everything outside of combat is implemented apart from a couple events (Colosseum)

**Getting Started**
* The project was built with Clion2020 and the mingw64 toolchain on windows
* The main target creates a simulator of the game that can be played in console.
* The test target creates a program with various commands that can be run, including random simulation
* Click the star button at the top of the repo :)
