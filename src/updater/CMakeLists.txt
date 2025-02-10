add_executable(updater "main.cpp")
target_compile_features(updater PRIVATE cxx_std_17)
target_precompile_headers(updater PRIVATE "pch.hpp")
target_link_libraries(updater PRIVATE fmt::fmt-header-only spdlog::spdlog_header_only)

install(TARGETS updater DESTINATION "bin")

if (COPY_PATH)
  add_custom_command(TARGET updater POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:updater> ${COPY_PATH}
    COMMAND_EXPAND_LISTS
  )
endif()