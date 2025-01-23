#include "network.hpp"
#include "utils/memory.hpp"

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

	void network::load()
	{
		// Allows us to intercept the hostnames that Novalogics "ResolveHostname" function is provided
		// Useful for changing the old NovaWorld hostname ("novaworld.net") to the new one ("nw2.novaworld.net")
		utils::memory::jump(0x004614D0, resolve_hostname_hook);

		// Disable VON (server)
		utils::memory::nop(0x00441EC2, 5);

		// Disable VON (client)
		utils::memory::nop(0x00442130, 5);

		// Allow dynamic port usage (client)
		utils::memory::set<uint8_t>(0x0040869A, 1);
		utils::memory::set<uint8_t>(0x004086BF, 1);
	}

	void network::unload()
	{

	}
}