#pragma once
#include "structs.hpp"

namespace game
{
	// The internal namespace holds the raw functions and variables from the game
	namespace internal
	{
		// Functions

		// Variables
		extern GameLoop** g_currentLoop;
	}

	// Everything below here is our custom functions that are bascially wrappers around the internal functions/variables

	GameLoop* get_current_loop();
}