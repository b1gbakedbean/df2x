#include "game.hpp"

namespace game
{
	// The internal namespace holds the raw functions and variables from the game
	namespace internal
	{
		// Functions
		draw_scaled_text_t draw_scaled_text = draw_scaled_text_t(0x004577A0);
		scale_coordinates_t scale_coordinates = scale_coordinates_t(0x00456B90);
		pff_load_t pff_load = pff_load_t(0x0047D720);
		svr_queue_broadcasted_message_t svr_queue_broadcasted_message = svr_queue_broadcasted_message_t(0x00459A30);

		// Variables
		GameLoop** g_currentLoop = reinterpret_cast<GameLoop**>(0x006772F0);
		void* g_unknown01 = reinterpret_cast<void*>(0x0067D420);
		void* g_smallFont = reinterpret_cast<void*>(0x00A6E068);
		NetMessageSpec* g_messageSpecs = reinterpret_cast<NetMessageSpec*>(0x004B06A8);
		uint32_t* g_ticks = reinterpret_cast<uint32_t*>(0x0067D414);
		uint32_t* g_isHost = reinterpret_cast<uint32_t*>(0x0067CD34);
		uint32_t* g_subGoalsWon = reinterpret_cast<uint32_t*>(0x00AB39E8);
		uint32_t* g_subGoalsLost = reinterpret_cast<uint32_t*>(0x00ABC000);
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

	void game::svr_queue_broadcasted_message(unsigned int messageId, const void *data, unsigned int length)
	{
		if (!is_host())
		{
			return;
		}

		internal::svr_queue_broadcasted_message(messageId, data, length);
	}

	void game::add_custom_message(uint8_t id, uint8_t flags, uint16_t size)
	{
		internal::g_messageSpecs[id].id = id;
		internal::g_messageSpecs[id].flags = flags;
		internal::g_messageSpecs[id].size = size;
	}

	uint32_t game::get_ticks()
	{
    	return *internal::g_ticks;
	}

	bool game::is_host()
	{
    	return *internal::g_isHost;
	}

	uint32_t game::get_subgoals_won()
	{
    	return *internal::g_subGoalsWon;
	}

	uint32_t game::get_subgoals_lost()
	{
    	return *internal::g_subGoalsLost;
	}

	void game::set_subgoals_won(uint32_t value)
	{
		*internal::g_subGoalsWon = value;
	}

	void game::set_subgoals_lost(uint32_t value)
	{
		*internal::g_subGoalsLost = value;
	}
}