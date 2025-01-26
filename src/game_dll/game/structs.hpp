#pragma once

namespace game
{
	// Internal game structures belong here

	enum DrawTextAlignmentType
	{
		DrawTextAlignmentType_Left = 0x0,
		DrawTextAlignmentType_Right = 0x1,
		DrawTextAlignmentType_Center = 0x2,
	};

	struct GameLoop
	{
		char name[32];
		void (*preLoop)();
		BOOL(*update)();
		void (*render)();
		void (*postLoop)();
	};

}