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

	struct NetMessageSpec
	{
  		uint8_t id;
  		uint8_t flags;
  		int16_t size;
	};

	struct PffArchive
	{
		HFILE fileHandle;
		char pad_0004[136];
		uint32_t entryCount;
		char pad_0090[8];
		struct PffEntry* entries;
		struct PffEntry* cachedEntry;
		char pad_00A0[28];
	};

	struct PffEntry
	{
		char pad_0000[4];
		uint32_t offset;
		char pad_0008[24];
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