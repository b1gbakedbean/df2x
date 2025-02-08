#include "misc.hpp"
#include "utils/memory.hpp"

namespace df2x::components
{
	void misc::load()
	{
		// Change the window title
		utils::memory::set_string(0x004C329C, "DF2x, ");
	}

	void misc::unload()
	{

	}
}