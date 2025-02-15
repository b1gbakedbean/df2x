#include "memory.hpp"

namespace utils::memory
{
    uintptr_t iat(const std::string& libraryName, const std::string& functionName, void* func)
    {
		auto moduleHandle = reinterpret_cast<uint8_t*>(GetModuleHandle(nullptr));
		auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(moduleHandle);
		auto ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(moduleHandle + dosHeader->e_lfanew);
		auto importDirectory = &ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
		auto importDescriptor = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(moduleHandle + importDirectory->VirtualAddress);

		while (importDescriptor->Name)
		{
			std::string importLibraryName = reinterpret_cast<char*>(moduleHandle + importDescriptor->Name);

			if (importLibraryName == libraryName)
			{
				auto nameTableEntry = reinterpret_cast<uintptr_t*>(moduleHandle + importDescriptor->OriginalFirstThunk);
				auto addressTableEntry = reinterpret_cast<uintptr_t*>(moduleHandle + importDescriptor->FirstThunk);

				if (!importDescriptor->OriginalFirstThunk)
				{
					nameTableEntry = reinterpret_cast<uintptr_t*>(moduleHandle + importDescriptor->FirstThunk);
				}

				while (*nameTableEntry)
				{
					std::string importFunctionName;

					if (IMAGE_SNAP_BY_ORDINAL(*nameTableEntry))
					{
						importFunctionName = "#" + std::to_string(IMAGE_ORDINAL(*nameTableEntry));
					}
					else
					{
						importFunctionName = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(moduleHandle + *nameTableEntry)->Name;
					}
					
					if (importFunctionName == functionName)
					{
						auto originalAddressTableEntry = *addressTableEntry;

						set<uintptr_t>(reinterpret_cast<uintptr_t>(addressTableEntry), reinterpret_cast<uintptr_t>(func));
						return originalAddressTableEntry;
					}

					nameTableEntry++;
					addressTableEntry++;
				}
			}

			importDescriptor++;
		}

		return 0;
    }

    void jump(uintptr_t address, void* func)
	{
		auto target = reinterpret_cast<uint8_t*>(address);
		auto jmpValue = reinterpret_cast<uint32_t>(func) - (address + 5);
		DWORD oldProtectValue;

		VirtualProtect(target, 5, PAGE_EXECUTE_READWRITE, &oldProtectValue);
		*target = 0xE9;
		*reinterpret_cast<size_t*>(target + 1) = jmpValue;
		VirtualProtect(target, 5, oldProtectValue, &oldProtectValue);
		FlushInstructionCache(GetCurrentProcess(), target, 5);
	}

	void set_string(uintptr_t address, const char* str)
	{
		strncpy(reinterpret_cast<char*>(address), str, strlen(str) + 1);
	}

	void nop(uintptr_t address, size_t length)
	{
		auto target = reinterpret_cast<uint8_t*>(address);
		DWORD oldProtectValue;

		VirtualProtect(target, length, PAGE_EXECUTE_READWRITE, &oldProtectValue);
		memset(target, 0x90, length);
		VirtualProtect(target, length, oldProtectValue, &oldProtectValue);
		FlushInstructionCache(GetCurrentProcess(), target, length);
	}
};