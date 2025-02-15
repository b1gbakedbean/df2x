#pragma once
#include "component.hpp"

namespace df2x::components
{
	class misc : public component
	{
	public:
		virtual void load();
		virtual void unload();

	private:
		typedef void (*ibrowse_SetCallback_t)(int, uintptr_t);
		static inline ibrowse_SetCallback_t o_ibrowse_SetCallback;
		
		static void ibrowse_SetCallback_hook(int callbackId, uintptr_t callback);

	};
}