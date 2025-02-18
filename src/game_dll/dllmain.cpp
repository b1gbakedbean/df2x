#include "utils/memory.hpp"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/msvc_sink.h"

#include "components/component.hpp"
#include "components/dispatcher.hpp"
#include "components/archive.hpp"
#include "components/network.hpp"
#include "components/discordrpc.hpp"
#include "components/misc.hpp"

#include "component_loader.hpp"
#include "version.hpp"

namespace df2x
{
    void setup_logging()
    {
        std::vector<spdlog::sink_ptr> sinks
        {
            std::make_shared<spdlog::sinks::rotating_file_sink_mt>("df2x/logs/df2x.log", 1048576 * 5, 25, true),
#ifdef _DEBUG
            std::make_shared<spdlog::sinks::msvc_sink_mt>()
#endif
        };

        auto logger = std::make_shared<spdlog::logger>("df2x", sinks.begin(), sinks.end());

#ifdef _DEBUG
        logger->set_level(spdlog::level::trace);
#else
        logger->set_level(spdlog::level::info);
#endif
        spdlog::register_logger(logger);
        spdlog::set_default_logger(logger);
    }

    void initialize()
    {
        setup_logging();
        SPDLOG_INFO("Initializing...");
#ifndef _DEBUG
        SPDLOG_INFO("Version: {}", DF2X_FULL_VERSION);
#else
        SPDLOG_INFO("Version: {}", DF2X_GIT_REVISION);
#endif
        component_loader::add_component<df2x::components::dispatcher>();
        component_loader::add_component<df2x::components::archive>();
        component_loader::add_component<df2x::components::network>();
        component_loader::add_component<df2x::components::discordrpc>();
        component_loader::add_component<df2x::components::misc>();
        component_loader::load_components();
    }

    void uninitialize()
    {
        SPDLOG_INFO("Uninitializing...");
        component_loader::unload_components();
    }
}

__declspec(naked) void entry_point_start_hook()
{
    __asm
    {
        call df2x::initialize
        push ebp
        mov ebp, esp
        push 0xFFFFFFFF
        push 0x004791C5
        retn
    }
}

__declspec(naked) void entry_point_end_hook()
{
    __asm
    {
        call df2x::uninitialize
        mov ecx, 0x00486B85
        call ecx
        push 0x00486292
        retn
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        utils::memory::jump(0x004791C0, entry_point_start_hook);
        utils::memory::jump(0x0048628D, entry_point_end_hook);
    }

    return TRUE;
}