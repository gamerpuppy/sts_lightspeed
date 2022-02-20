cmake_minimum_required(VERSION 3.19)
project(
        sts_lightspeed
        VERSION 0.2
        DESCRIPTION "High Performance, rng accurate implementation of Slay the Spire with tools for tree search and ml."
        LANGUAGES CXX)

file (GLOB_RECURSE sts_lightspeed_SOURCES CONFIGURE_DEPENDS "sts/*.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-Wno-shift-count-overflow -O3")

add_executable(main apps/main.cpp ${sts_lightspeed_SOURCES})
target_include_directories(main PUBLIC ./)
target_include_directories(main PUBLIC third_party)