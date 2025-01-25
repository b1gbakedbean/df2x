#pragma once

namespace game
{
	// Internal game structures belong here

	struct GameLoop
	{
		char name[32];
		void (*preLoop)();
		BOOL(*update)();
		void (*render)();
		void (*postLoop)();
	};

}