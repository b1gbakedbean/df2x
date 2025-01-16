#pragma once
#include "component.hpp"

namespace df2x::components
{
	class misc : public component
	{
	public:
		virtual void load();
		virtual void unload();
	};
}