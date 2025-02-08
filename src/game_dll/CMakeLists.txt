add_library(game_dll SHARED "pch.cpp" "dllmain.cpp" "components/misc.cpp" "component_loader.cpp" "game/game.cpp" "utils/memory.cpp" "components/discordrpc.cpp" "components/network.cpp" "components/dispatcher.cpp" "components/archive.cpp")
target_compile_features(game_dll PRIVATE cxx_std_17)
target_include_directories(game_dll PRIVATE .)
target_precompile_headers(game_dll PRIVATE "pch.hpp")

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_library(DISCORD_RPC_LIB discord-rpc)

target_link_libraries(game_dll PRIVATE fmt::fmt-header-only spdlog::spdlog_header_only ${DISCORD_RPC_LIB})
set_target_properties(game_dll PROPERTIES OUTPUT_NAME DDraw)

if (COPY_PATH)
  add_custom_command(TARGET game_dll POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:game_dll> ${COPY_PATH}
    COMMAND_EXPAND_LISTS
  )
endif()