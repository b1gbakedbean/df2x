cmake_minimum_required (VERSION 3.24)

project ("df2x" VERSION 0.1.0)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(src/game_dll)
add_subdirectory(src/updater)