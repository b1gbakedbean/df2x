#include "game.hpp"

namespace game
{
	// The internal namespace holds the raw functions and variables from the game
	namespace internal
	{
		// Functions

		// Variables
		GameLoop** g_currentLoop = reinterpret_cast<GameLoop**>(0x006772F0);
	}

	// Everything below here is our custom functions that are mostly wrappers around the internal functions/variables

	GameLoop* get_current_loop()
	{
		return *internal::g_currentLoop;
	}
}