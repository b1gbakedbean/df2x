#include "component_loader.hpp"

namespace df2x
{
	void component_loader::load_components()
	{
		for (const auto& component : _components)
		{
			component->load();
		}
	}

	void component_loader::unload_components()
	{
		for (const auto& component : _components)
		{
			component->unload();
		}
	}
}