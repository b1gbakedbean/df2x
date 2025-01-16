#include "dispatcher.hpp"
#include "utils/memory.hpp"

namespace df2x::components
{
	static void dispatcher_run_callbacks(DispatcherCallbackType type)
	{
		static int ticks = 0;
		auto callbacks = dispatcher::get_callbacks();

		for (const auto& callback : callbacks)
		{
			if (callback.type == type)
			{
				callback.function(ticks);
			}
		}

		ticks++;
	}

	static __declspec(naked) void dispatcher_logic_loop_hook()
	{
		__asm
		{
			pushad
			push DispatcherCallbackType_Logic
			call dispatcher_run_callbacks
			add esp, 0x4
			popad

			mov eax, 0x00468BB0
			call eax

			push 0x00464CE5
			retn
		}
	}

	static __declspec(naked) void dispatcher_render_loop_hook()
	{
		__asm
		{
			pushad
			push DispatcherCallbackType_Render
			call dispatcher_run_callbacks
			add esp, 0x4
			popad

			mov eax, 0x00450070
			call eax

			push 0x004662B9
			retn
		}
	}

	void dispatcher::load()
	{
		// Hook the logic loop
		utils::memory::jump(0x00464CE0, dispatcher_logic_loop_hook);

		// Hook the render loop
		utils::memory::jump(0x004662B4, dispatcher_render_loop_hook);
	}

	void dispatcher::unload()
	{

	}

	void dispatcher::add_callback(DispatcherCallbackType type, std::function<void(int)> callback)
	{
		_callbacks.push_back({ type, callback });
	}

	std::vector<DispatcherCallback> dispatcher::get_callbacks()
	{
		return _callbacks;
	}
}