#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/msvc_sink.h"
#include "cpr/cpr.h"

const std::string game_directory_files[3] = 
    {
        "Df2.pff",
        "Terrains.pff",
        "Netsock.dll"
    };

bool verify_game_directory()
{
    for (const auto& game_directory_file : game_directory_files)
    {
        if (!std::filesystem::exists(game_directory_file))
        {
            return false;
        }
    }

    return true;
}

void setup_logging()
{
    std::vector<spdlog::sink_ptr> sinks
    {
        std::make_shared<spdlog::sinks::rotating_file_sink_mt>("df2x/logs/updater.log", 1048576 * 5, 25, true),
#ifdef _DEBUG
        std::make_shared<spdlog::sinks::msvc_sink_mt>()
#endif
    };

    auto logger = std::make_shared<spdlog::logger>("updater", sinks.begin(), sinks.end());

#ifdef _DEBUG
    logger->set_level(spdlog::level::trace);
#else
    logger->set_level(spdlog::level::info);
#endif
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
}

void main(int argc, char* argv[])
{
    // Try to make sure the updater in located in the game directory.
    if (!verify_game_directory())
    {
        MessageBoxA(nullptr, "The updater doesn't appear to be located inside the game directory.\n\nPlace the updater inside game directory and re-run it.", "Updater Error", MB_ICONERROR | MB_OK);
        return;
    }

    // Setup logging after verifying the game directory, that way we don't end up creating the logs folder outside of the game directory.
    setup_logging();
}