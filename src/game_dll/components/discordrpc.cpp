#include "discordrpc.hpp"
#include "dispatcher.hpp"

namespace df2x::components
{
	void discordrpc::load()
	{
		initialize_discordrpc();
	}

	void discordrpc::unload()
	{
		Discord_ClearPresence();
		Discord_Shutdown();
	}

	void discordrpc::discordrpc_ready_callback(const DiscordUser* user)
	{
		SPDLOG_DEBUG("User connected {}", user->username);
	}

	void discordrpc::discordrpc_error_callback(int errorCode, const char* message)
	{
		SPDLOG_ERROR("{} - {}", errorCode, message);
	}

	void discordrpc::initialize_discordrpc()
	{
		DiscordEventHandlers handlers{};

		handlers.ready = discordrpc_ready_callback;
		handlers.errored = discordrpc_error_callback;

		Discord_Initialize("1320979085513723976", &handlers, TRUE, nullptr);

		dispatcher::add_callback(DispatcherCallbackType_Update, [](int ticks)
			{
				// Should be called every ~1s
				if (ticks % 62 == 0)
				{
					Discord_RunCallbacks();
				}
			});
	}
}