cmake_minimum_required (VERSION 3.24)

project(
    "df2x" 
    VERSION 0.1.1
    LANGUAGES C CXX RC)

string(REPLACE "." "," PROJECT_RC_FILEVERSION "${CMAKE_PROJECT_VERSION}")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(GetGitRevision)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(src/game_dll)
add_subdirectory(src/updater)

set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
include(CPack)