#include "network.hpp"
#include "utils/memory.hpp"
#include "game/game.hpp"
#include "dispatcher.hpp"

namespace df2x::components
{
	int network::custom_resolve_hostname(char* hostName)
	{
		addrinfo hints{};
		addrinfo* addrs{};

		hints.ai_family = AF_INET;

		// Change the hostname to the new NovaWorld hostname if it isn't already
		if (strcmpi(hostName, "nw2.novaworld.net") != 0)
		{
			auto originalHostName = hostName;
			hostName = "nw2.novaworld.net";

			SPDLOG_INFO("\"{}\" -> \"{}\"", originalHostName, hostName);
		}

		auto result = getaddrinfo(hostName, nullptr, &hints, &addrs);

		if (result != 0)
		{
			SPDLOG_ERROR("getaddrinfo() failed -> {}", result);
			return -1;
		}

		auto addr = reinterpret_cast<sockaddr_in*>(addrs->ai_addr);

		SPDLOG_INFO("Resolved \"{}\" -> {}", hostName, inet_ntoa(addr->sin_addr));

		return addr->sin_addr.S_un.S_addr;
	}

	__declspec(naked) void network::resolve_hostname_hook()
	{
		__asm
		{
			push esi
			mov esi, [esp + 0x8] // hostName
			push esi
			call custom_resolve_hostname
			add esp, 0x4
			pop esi
			retn
		}
	}

	void network::cli_handle_custom_messages(unsigned int messageId, uint8_t* data)
	{
		switch (messageId)
		{
			case 91: // Subgoals update
			{
				uint32_t subGoalsWon = *reinterpret_cast<uint32_t*>(data);
				uint32_t subGoalsLost = *reinterpret_cast<uint32_t*>(data + 4);

				game::set_subgoals_won(subGoalsWon);
				game::set_subgoals_lost(subGoalsLost);
				break;
			}

			default:
				SPDLOG_DEBUG("Unhandled message id: {}", messageId);
		}
	}

	__declspec(naked) void network::cli_parse_incoming_message_hook()
	{
		__asm
		{
			pushad
			push ebx // data
			inc eax
			push eax // messageId
			call cli_handle_custom_messages
			add esp, 0x8
			popad

			test cl, 0x1
			jz label_244
			push 0x0040B0CB
			retn

		label_244:
			push 0x0040B0D0
			retn
		}
	}

	void network::svr_handle_custom_messages(unsigned int messageId, uint8_t* data)
	{
		switch (messageId)
		{
			default:
				SPDLOG_DEBUG("Unhandled message id: {}", messageId);
		}
	}

	__declspec(naked) void network::svr_parse_incoming_message_hook()
	{
		__asm
		{
			pushad
			push ebx // data
			inc eax
			push eax // messageId
			call svr_handle_custom_messages
			add esp, 0x8
			popad

			test cl, 0x1
			jz label_244
			push 0x0045DA69
			retn

		label_244:
			push 0x0045DA80
			retn
		}
	}

	void network::register_custom_messages()
	{
		game::add_custom_message(91, 0x0, 8); // Subgoals update
	}

	void network::load()
	{
		// Allows us to intercept the hostnames that Novalogics "ResolveHostname" function is provided.
		// Useful for changing the old NovaWorld hostname ("novaworld.net") to the new one ("nw2.novaworld.net").
		utils::memory::jump(0x004614D0, resolve_hostname_hook);

		// Disable VON (server & client)
		utils::memory::set<uint32_t>(0x004D935C, 0);
		utils::memory::nop(0x00440A25, 5);
		utils::memory::nop(0x00440464, 5);
		utils::memory::nop(0x00440470, 5);
		utils::memory::nop(0x00440477, 5);
		utils::memory::nop(0x0044048A, 5);
		utils::memory::nop(0x0044049B, 5);

		// Allow dynamic port usage (client)
		utils::memory::set<uint8_t>(0x0040869A, 1);
		utils::memory::set<uint8_t>(0x004086BF, 1);

		// Lower the delay used for transmission of certain network updates (client)
		utils::memory::set<uint8_t>(0x004421F5, 1);

		// Lower the delay used for transmission of certain network updates (server).
		// The default value for NovaWorld servers is 13 (~4.8pps) while 1 is the lowest possible value
		// and causes certain network updates to be sent as fast as the games tick rate (~62.5hz) acheiving ~62.5pps.
		// NOTE: A stock client cannot connect if the server is using a value below 7.
		utils::memory::set<uint8_t>(0x00441F7F, 1);

		// Lower the delay between checking for network messages and sending any queued messages
		// during the loading screen (client). The default is 100ms, this patch is a 4ms delay or 25x faster.
		// NOTE: This is required by the client if the server is using a value below 7 as mentioned above.
		utils::memory::set<uint8_t>(0x004089DB, 4);
		utils::memory::set<uint8_t>(0x00408B3E, 4);

		// Allow for more frequent updates of entity (organics) positions to the client (server)
		utils::memory::nop(0x0045FFA3, 2);

		register_custom_messages();

		utils::memory::jump(0x0040B0C6, cli_parse_incoming_message_hook);
		utils::memory::jump(0x0045DA64, svr_parse_incoming_message_hook);

		dispatcher::add_callback(DispatcherCallbackType_Update, [](int ticks)
			{
				// Make sure we're in the main game loop
				if (strcmpi(game::get_current_loop()->name, "Game Loop"))
				{
					return;
				}

				// Make sure we're the host
				if (!game::is_host())
				{
					return;
				}

				// Only broadcast the subgoals update every ~1 second
				if (ticks % 62 == 0)
				{
					uint32_t data[2]{ game::get_subgoals_won(), game::get_subgoals_lost() };

					game::svr_queue_broadcasted_message(91, &data, 8);
				}
			});
	}

	void network::unload()
	{

	}
}