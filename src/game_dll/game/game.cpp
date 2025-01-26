#include "game.hpp"

namespace game
{
	// The internal namespace holds the raw functions and variables from the game
	namespace internal
	{
		// Functions
		draw_scaled_text_t draw_scaled_text = draw_scaled_text_t(0x004577A0);
		scale_coordinates_t scale_coordinates = scale_coordinates_t(0x00456B90);

		// Variables
		GameLoop** g_currentLoop = reinterpret_cast<GameLoop**>(0x006772F0);
		void* g_unknown01 = reinterpret_cast<void*>(0x0067D420);
		void* g_smallFont = reinterpret_cast<void*>(0x00A6E068);
	}

	// Everything below here is our custom functions that are mostly wrappers around the internal functions/variables

	GameLoop* get_current_loop()
	{
		return *internal::g_currentLoop;
	}

	void draw_text(int x, int y, char* text, int color, DrawTextAlignmentType alignmentType)
	{
		internal::scale_coordinates(internal::g_unknown01, &x, &y);
		internal::draw_scaled_text(internal::g_unknown01, x, y, 0, text, internal::g_smallFont, color, alignmentType);
	}
}