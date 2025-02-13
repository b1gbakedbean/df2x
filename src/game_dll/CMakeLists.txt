configure_file("${PROJECT_SOURCE_DIR}/src/version.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/version.hpp")
add_library(game_dll SHARED "pch.cpp" "dllmain.cpp" "components/misc.cpp" "component_loader.cpp" "game/game.cpp" "utils/memory.cpp" "components/discordrpc.cpp" "components/network.cpp" "components/dispatcher.cpp" "components/archive.cpp")
target_compile_features(game_dll PRIVATE cxx_std_17)
target_include_directories(game_dll PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_precompile_headers(game_dll PRIVATE "pch.hpp")

find_library(DISCORD_RPC_LIB discord-rpc)

target_link_libraries(game_dll PRIVATE fmt::fmt-header-only spdlog::spdlog_header_only ${DISCORD_RPC_LIB})
set_target_properties(game_dll PROPERTIES OUTPUT_NAME DDraw)

install(TARGETS game_dll
        LIBRARY EXCLUDE_FROM_ALL
        ARCHIVE EXCLUDE_FROM_ALL
        RUNTIME DESTINATION ".")

if (COPY_PATH)
  add_custom_command(TARGET game_dll POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:game_dll> ${COPY_PATH}
    COMMAND_EXPAND_LISTS
  )
endif()