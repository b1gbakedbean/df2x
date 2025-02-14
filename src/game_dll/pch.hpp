#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <functional>
#include <ws2tcpip.h>
#include <filesystem>

#pragma comment(lib, "Ws2_32")

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif

#define SPDLOG_FMT_EXTERNAL
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <discord_rpc.h>