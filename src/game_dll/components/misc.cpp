#include "misc.hpp"
#include "utils/memory.hpp"
#include "version.hpp"

namespace df2x::components
{
	void misc::load()
	{
		// Prevent the game from overriding our title
		utils::memory::nop(0x00424A01, 5);
		utils::memory::nop(0x004249ED, 5);

		// Prevent the game from minimizing when focus is lost
		utils::memory::set<uint8_t>(0x00477F4D, 0xEB);
		utils::memory::set<uint8_t>(0x00477F77, 0xEB);

		// Keep the game rendering while it doesn't have focus
		utils::memory::nop(0x0047A8F7, 2);

		// Change the window title
		utils::memory::set_string(0x00A6E3C0, "DF2x, ");
#ifndef _DEBUG
		utils::memory::set_string(0x00A6E4C0, DF2X_FULL_VERSION);
#else
		utils::memory::set_string(0x00A6E4C0, DF2X_GIT_REVISION);
#endif

		// Allow the use of the alternate startup.htm (without this you cannot connect through NovaHQ)
		o_ibrowse_SetCallback = reinterpret_cast<ibrowse_SetCallback_t>(utils::memory::iat("iBrowse.dll", "ibrowse_SetCallback", ibrowse_SetCallback_hook));
	}

	void misc::unload()
	{
	}

    void misc::ibrowse_SetCallback_hook(int callbackId, uintptr_t callback)
    {
		if (callbackId == 0 && callback == 0x43DB50)
		{
			callbackId = 1;
		}
		else if (callbackId == 1 && callback == 0x43DDB0)
		{
			callbackId = 0;
		}

		o_ibrowse_SetCallback(callbackId, callback);
    }
}