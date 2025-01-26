#include "dispatcher.hpp"
#include "utils/memory.hpp"
#include "game/game.hpp"

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

	static void preloop_dispatch()
	{
		dispatcher_run_callbacks(DispatcherCallbackType_PreLoop);
		game::get_current_loop()->preLoop();
	}

	static BOOL update_dispatch()
	{
		dispatcher_run_callbacks(DispatcherCallbackType_Update);
		return game::get_current_loop()->update();
	}

	static void postloop_dispatch()
	{
		dispatcher_run_callbacks(DispatcherCallbackType_PostLoop);
		game::get_current_loop()->postLoop();
	}

	static __declspec(naked) void dispatcher_preloop_hook()
	{
		__asm
		{
			mov eax, preloop_dispatch
			xor ebx, ebx
			push 0x00468AD9
			retn
		}
	}

	static __declspec(naked) void dispatcher_update_hook()
	{
		__asm
		{
			call update_dispatch
			mov ebx, eax
			push 0x00468B20
			retn
		}
	}

	static __declspec(naked) void dispatcher_player_render_hook()
	{
		__asm
		{
			push DispatcherCallbackType_Render
			call dispatcher_run_callbacks
			add esp, 0x4
			mov eax, 0x00465D10
			call eax
			push 0x0046641C
			retn
		}
	}

	static __declspec(naked) void dispatcher_server_render_hook()
	{
		__asm
		{
			push DispatcherCallbackType_Render
			call dispatcher_run_callbacks
			add esp, 0x4
			mov eax, 0x00465D10
			call eax
			push 0x0045F1FB
			retn
		}
	}

	static __declspec(naked) void dispatcher_postloop_hook()
	{
		__asm
		{
			mov eax, postloop_dispatch
			test eax, eax
			push 0x00468B7A
			retn
		}
	}

	void dispatcher::load()
	{
		utils::memory::jump(0x00468AD4, dispatcher_preloop_hook);
		utils::memory::jump(0x00468B1B, dispatcher_update_hook);
		utils::memory::jump(0x00466417, dispatcher_player_render_hook);
		utils::memory::jump(0x0045F1F6, dispatcher_server_render_hook);
		utils::memory::jump(0x00468B75, dispatcher_postloop_hook);
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