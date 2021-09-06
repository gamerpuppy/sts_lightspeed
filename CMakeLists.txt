cmake_minimum_required(VERSION 3.19)
project(
        sts_lightspeed
        VERSION 0.1
        DESCRIPTION "High Performance, rng accurate implementation of Slay the Spire with tools for tree search and ml."
        LANGUAGES CXX)

file (GLOB_RECURSE sts_lightspeed_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-Wno-shift-count-overflow")

add_subdirectory(nlohmann_json)

add_executable(main apps/main.cpp ${sts_lightspeed_SOURCES})
target_link_directories(main PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(main PUBLIC include)
target_include_directories(main PUBLIC nlohmann_json/include)

add_executable(test apps/test.cpp ${sts_lightspeed_SOURCES})
target_link_directories(main PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(test PUBLIC include)
target_include_directories(test PUBLIC nlohmann_json/include)



