#pragma once
#include "structs.hpp"

namespace game
{
	// The internal namespace holds the raw functions and variables from the game
	namespace internal
	{
		// Functions
		typedef void (*draw_scaled_text_t)(void* resolution, int x, int y, int unknown1, char* text, void* font, int color, DrawTextAlignmentType alignmentType);
		extern draw_scaled_text_t draw_scaled_text;

		typedef void (*scale_coordinates_t)(void* resolution, int* x, int* y);
		extern scale_coordinates_t scale_coordinates;

		typedef game::PffArchive* (*pff_load_t)(const char* name);
		extern pff_load_t pff_load;

		// Variables
		extern GameLoop** g_currentLoop;
		extern void* g_unknown01;
		extern void* g_smallFont;
	}

	// Everything below here is our custom functions that are bascially wrappers around the internal functions/variables

	GameLoop* get_current_loop();
	void draw_text(int x, int y, char* text, int color, DrawTextAlignmentType alignmentType);
}