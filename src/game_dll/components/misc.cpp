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

		// Change the window title
		utils::memory::set_string(0x00A6E3C0, "DF2x, ");
#ifndef _DEBUG
		utils::memory::set_string(0x00A6E4C0, DF2X_FULL_VERSION);
#else
		utils::memory::set_string(0x00A6E4C0, fmt::format("{}-{}", DF2X_FULL_VERSION, DF2X_GIT_REVISION).c_str());
#endif
	}

	void misc::unload()
	{
	}
}