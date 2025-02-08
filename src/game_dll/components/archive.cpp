#include "archive.hpp"
#include "utils/memory.hpp"

namespace df2x::components
{
	static bool custom_find_file()
	{
		return false;
	}

	static __declspec(naked) void archive_find_file_hook()
	{
		__asm
		{
			mov esi, [esp]
			call custom_find_file
			push esi
			retn
		}
	}

	void archive::load()
	{
		//utils::memory::jump(0x0044AF30, archive_find_file_hook);
	}

	void archive::unload()
	{
	}
}