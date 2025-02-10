#pragma once
#include <Windows.h>
#include <filesystem>

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>