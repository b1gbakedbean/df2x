#pragma once
#include "components/component.hpp"

namespace df2x
{
	class component_loader
	{
	public:
		template<typename T>
		static void add_component()
		{
			_components.push_back(std::make_shared<T>());
		}

		static void load_components();
		static void unload_components();

	private:
		static inline std::vector<std::shared_ptr<df2x::components::component>> _components;
	};
}