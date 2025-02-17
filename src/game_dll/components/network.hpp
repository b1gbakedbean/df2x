#pragma once
#include "component.hpp"

namespace df2x::components
{
	class network : public component
	{
	public:
		virtual void load();
		virtual void unload();

	private:
		void register_custom_messages();

		static int custom_resolve_hostname(char* hostName);
		static void resolve_hostname_hook();

		static void cli_handle_custom_messages(unsigned int messageId, uint8_t* data);
		static void cli_parse_incoming_message_hook();

		static void svr_handle_custom_messages(unsigned int messageId, uint8_t* data);
		static void svr_parse_incoming_message_hook();
	};
}