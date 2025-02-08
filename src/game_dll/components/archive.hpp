#pragma once
#include "component.hpp"

namespace df2x::components
{
	class archive : public component
	{
	public:
		virtual void load();
		virtual void unload();
	};
}