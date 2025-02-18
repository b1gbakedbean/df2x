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

		typedef void (*svr_queue_broadcasted_message_t)(unsigned int messageId, const void *data, unsigned int length);
		extern svr_queue_broadcasted_message_t svr_queue_broadcasted_message;

		// Variables
		extern GameLoop** g_currentLoop;
		extern void* g_unknown01;
		extern void* g_smallFont;
		extern NetMessageSpec* g_messageSpecs;
		extern uint32_t* g_ticks;
		extern uint32_t* g_isHost;
		extern uint32_t* g_subGoalsWon;
		extern uint32_t* g_subGoalsLost;
	}

	// Everything below here is our custom functions that are bascially wrappers around the internal functions/variables

	GameLoop* get_current_loop();
	void draw_text(int x, int y, char* text, int color, DrawTextAlignmentType alignmentType);
	void svr_queue_broadcasted_message(unsigned int messageId, const void *data, unsigned int length);
	void add_custom_message(uint8_t id, uint8_t flags = 0x0, uint16_t size = 0);
	uint32_t get_ticks();
	bool is_host();
	uint32_t get_subgoals_won();
	uint32_t get_subgoals_lost();
	void set_subgoals_won(uint32_t value);
	void set_subgoals_lost(uint32_t value);

}