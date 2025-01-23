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
		static int custom_resolve_hostname(char* hostName);
		static void resolve_hostname_hook();
	};
}