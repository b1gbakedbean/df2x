#pragma comment(linker, "/export:DirectDrawCreate=_DirectDrawCreate@12")
#pragma comment(linker, "/export:DirectDrawEnumerateExA=_DirectDrawEnumerateExA@12")
#pragma comment(linker, "/export:DirectDrawEnumerateA=_DirectDrawEnumerateA@8")
#pragma comment(linker, "/export:AcquireDDThreadLock=_AcquireDDThreadLock@0")
#pragma comment(linker, "/export:ReleaseDDThreadLock=_ReleaseDDThreadLock@0")
#pragma comment(linker, "/export:DDInternalUnlock=_DDInternalUnlock@4")
#pragma comment(linker, "/export:DDInternalLock=_DDInternalLock@8")
#pragma comment(linker, "/export:D3DParseUnknownCommand=_D3DParseUnknownCommand@8")

typedef HRESULT(__stdcall* DirectDrawCreate_t)(GUID* pGUID, void** ppDD, void* pUnkOuter);
typedef HRESULT(__stdcall* DirectDrawEnumerateExA_t)(void* pCallback, void* pContext, DWORD dwFlags);
typedef HRESULT(__stdcall* DirectDrawEnumerateA_t)(void* pCallback, void* pContext);
typedef int(__stdcall* AcquireDDThreadLock_t)();
typedef void(__stdcall* ReleaseDDThreadLock_t)();
typedef int(__stdcall* DDInternalUnlock_t)(int unk1);
typedef int(__stdcall* DDInternalLock_t)(int unk1, int unk2);
typedef HRESULT(__stdcall* D3DParseUnknownCommand_t)(void* pvCommands, void** ppvReturnedCommand);

std::string GetPreferredDDrawDllPath();
std::string GetSystemDDrawDllPath();
std::string GetAlternateDDrawDllPath();

std::string preferredDDrawDllPath{};
HMODULE ddrawModule;

std::string GetPreferredDDrawDllPath()
{
	auto alternateDDrawDllPath = GetAlternateDDrawDllPath();

	// Check for the existence of the alternate DDraw.dll if the path isn't empty, as it takes priority over the system DDraw.dll
	if (!alternateDDrawDllPath.empty())
	{
		SPDLOG_INFO("Checking if \"{}\" exists in the current game directory...", alternateDDrawDllPath);

		if (std::filesystem::exists(alternateDDrawDllPath))
		{
			SPDLOG_INFO("\"{}\" exists!", alternateDDrawDllPath);
			return alternateDDrawDllPath;
		}

		SPDLOG_INFO("Alternate \"DDraw.dll\" doesn't exist", alternateDDrawDllPath);
	}

	auto systemDDrawDllPath = GetSystemDDrawDllPath();

	// Alternate DDraw.dll path was empty or didn't exist, check for the system DDraw.dll now
	if (!systemDDrawDllPath.empty())
	{
		SPDLOG_INFO("Checking if \"{}\" exists...", systemDDrawDllPath);

		if (std::filesystem::exists(systemDDrawDllPath))
		{
			SPDLOG_INFO("\"{}\" exists!", systemDDrawDllPath);
			return systemDDrawDllPath;
		}
	}

	// Neither the alternate or system DDraw.dll exist, uhhh?!
	SPDLOG_ERROR("No alternate or system \"DDraw.dll\" could be located and we cannot proceed, aborting!");
	MessageBoxA(nullptr, "No alternate or system \"DDraw.dll\" could be located and we cannot proceed, aborting!", "Fatal Error", MB_OK | MB_ICONERROR);
	exit(-1);

	return std::string();
}

std::string GetSystemDDrawDllPath()
{
	char rootPath[MAX_PATH];
	BOOL isWow64 = false;

	if (IsWow64Process(GetCurrentProcess(), &isWow64) == 0)
	{
		SPDLOG_ERROR("IsWow64Process() failed -> {}", GetLastError());
		return std::string();
	}

	if (isWow64)
	{
		if (GetSystemWow64DirectoryA(rootPath, sizeof(rootPath)) == 0)
		{
			SPDLOG_ERROR("GetSystemWow64DirectoryA() failed -> {}", GetLastError());
			return std::string();
		}
	}
	else
	{
		if (GetSystemDirectoryA(rootPath, sizeof(rootPath)) == 0)
		{
			SPDLOG_ERROR("GetSystemDirectoryA() failed -> {}", GetLastError());
			return std::string();
		}
	}

	return std::string(rootPath).append("\\ddraw.dll");
}

std::string GetAlternateDDrawDllPath()
{
	return "DDraw.alt.dll";
}

extern "C" __declspec(dllexport) HRESULT __stdcall DirectDrawCreate(GUID* pGUID, void** ppDD, void* pUnkOuter)
{
	if (preferredDDrawDllPath.empty())
	{
		preferredDDrawDllPath = GetPreferredDDrawDllPath();
	}

	if (!ddrawModule)
	{
		if (!(ddrawModule = LoadLibrary(preferredDDrawDllPath.c_str())))
		{
			SPDLOG_ERROR("LoadLibrary(\"{}\") failed -> {}", preferredDDrawDllPath, GetLastError());
			return 0;
		}
	}

	auto fProc = reinterpret_cast<DirectDrawCreate_t>(GetProcAddress(ddrawModule, "DirectDrawCreate"));

	if (!fProc)
	{
		SPDLOG_ERROR("GetProcAddress({}, \"DirectDrawCreate\") failed -> {}", fmt::ptr(ddrawModule), GetLastError());
		return 0;
	}

	return fProc(pGUID, ppDD, pUnkOuter);
}

extern "C" __declspec(dllexport) HRESULT __stdcall DirectDrawEnumerateExA(void* pCallback, void* pContext, DWORD dwFlags)
{
	if (preferredDDrawDllPath.empty())
	{
		preferredDDrawDllPath = GetPreferredDDrawDllPath();
	}

	if (!ddrawModule)
	{
		if (!(ddrawModule = LoadLibrary(preferredDDrawDllPath.c_str())))
		{
			SPDLOG_ERROR("LoadLibrary(\"{}\") failed -> {}", preferredDDrawDllPath, GetLastError());
			return 0;
		}
	}

	auto fProc = reinterpret_cast<DirectDrawEnumerateExA_t>(GetProcAddress(ddrawModule, "DirectDrawEnumerateExA"));

	if (!fProc)
	{
		SPDLOG_ERROR("GetProcAddress({}, \"DirectDrawEnumerateExA\") failed -> {}", fmt::ptr(ddrawModule), GetLastError());
		return 0;
	}

	return fProc(pCallback, pContext, dwFlags);
}

extern "C" __declspec(dllexport) HRESULT __stdcall DirectDrawEnumerateA(void* pCallback, void* pContext)
{
	if (preferredDDrawDllPath.empty())
	{
		preferredDDrawDllPath = GetPreferredDDrawDllPath();
	}

	if (!ddrawModule)
	{
		if (!(ddrawModule = LoadLibrary(preferredDDrawDllPath.c_str())))
		{
			SPDLOG_ERROR("LoadLibrary(\"{}\") failed -> {}", preferredDDrawDllPath, GetLastError());
			return 0;
		}
	}

	auto fProc = reinterpret_cast<DirectDrawEnumerateA_t>(GetProcAddress(ddrawModule, "DirectDrawEnumerateA"));

	if (!fProc)
	{
		SPDLOG_ERROR("GetProcAddress({}, \"DirectDrawEnumerateA\") failed -> {}", fmt::ptr(ddrawModule), GetLastError());
		return 0;
	}

	return fProc(pCallback, pContext);
}

extern "C" __declspec(dllexport) int __stdcall AcquireDDThreadLock()
{
	if (preferredDDrawDllPath.empty())
	{
		preferredDDrawDllPath = GetPreferredDDrawDllPath();
	}

	if (!ddrawModule)
	{
		if (!(ddrawModule = LoadLibrary(preferredDDrawDllPath.c_str())))
		{
			SPDLOG_ERROR("LoadLibrary(\"{}\") failed -> {}", preferredDDrawDllPath, GetLastError());
			return 0;
		}
	}

	auto fProc = reinterpret_cast<AcquireDDThreadLock_t>(GetProcAddress(ddrawModule, "AcquireDDThreadLock"));

	if (!fProc)
	{
		SPDLOG_ERROR("GetProcAddress({}, \"AcquireDDThreadLock\") failed -> {}", fmt::ptr(ddrawModule), GetLastError());
		return 0;
	}

	return fProc();
}

extern "C" __declspec(dllexport) void __stdcall ReleaseDDThreadLock()
{
	if (preferredDDrawDllPath.empty())
	{
		preferredDDrawDllPath = GetPreferredDDrawDllPath();
	}

	if (!ddrawModule)
	{
		if (!(ddrawModule = LoadLibrary(preferredDDrawDllPath.c_str())))
		{
			SPDLOG_ERROR("LoadLibrary(\"{}\") failed -> {}", preferredDDrawDllPath, GetLastError());
			return;
		}
	}

	auto fProc = reinterpret_cast<ReleaseDDThreadLock_t>(GetProcAddress(ddrawModule, "ReleaseDDThreadLock"));

	if (!fProc)
	{
		SPDLOG_ERROR("GetProcAddress({}, \"ReleaseDDThreadLock\") failed -> {}", fmt::ptr(ddrawModule), GetLastError());
		return;
	}

	fProc();
}

extern "C" __declspec(dllexport) int __stdcall DDInternalUnlock(int unk1)
{
	if (preferredDDrawDllPath.empty())
	{
		preferredDDrawDllPath = GetPreferredDDrawDllPath();
	}

	if (!ddrawModule)
	{
		if (!(ddrawModule = LoadLibrary(preferredDDrawDllPath.c_str())))
		{
			SPDLOG_ERROR("LoadLibrary(\"{}\") failed -> {}", preferredDDrawDllPath, GetLastError());
			return 0;
		}
	}

	auto fProc = reinterpret_cast<DDInternalUnlock_t>(GetProcAddress(ddrawModule, "DDInternalUnlock"));

	if (!fProc)
	{
		SPDLOG_ERROR("GetProcAddress({}, \"DDInternalUnlock\") failed -> {}", fmt::ptr(ddrawModule), GetLastError());
		return 0;
	}

	return fProc(unk1);
}

extern "C" __declspec(dllexport) int __stdcall DDInternalLock(int unk1, int unk2)
{
	if (preferredDDrawDllPath.empty())
	{
		preferredDDrawDllPath = GetPreferredDDrawDllPath();
	}

	if (!ddrawModule)
	{
		if (!(ddrawModule = LoadLibrary(preferredDDrawDllPath.c_str())))
		{
			SPDLOG_ERROR("LoadLibrary(\"{}\") failed -> {}", preferredDDrawDllPath, GetLastError());
			return 0;
		}
	}

	auto fProc = reinterpret_cast<DDInternalLock_t>(GetProcAddress(ddrawModule, "DDInternalLock"));

	if (!fProc)
	{
		SPDLOG_ERROR("GetProcAddress({}, \"DDInternalLock\") failed -> {}", fmt::ptr(ddrawModule), GetLastError());
		return 0;
	}

	return fProc(unk1, unk2);
}

extern "C" __declspec(dllexport) HRESULT __stdcall D3DParseUnknownCommand(void* pvCommands, void** ppvReturnedCommand)
{
	if (preferredDDrawDllPath.empty())
	{
		preferredDDrawDllPath = GetPreferredDDrawDllPath();
	}

	if (!ddrawModule)
	{
		if (!(ddrawModule = LoadLibrary(preferredDDrawDllPath.c_str())))
		{
			SPDLOG_ERROR("LoadLibrary(\"{}\") failed -> {}", preferredDDrawDllPath, GetLastError());
			return 0;
		}
	}

	auto fProc = reinterpret_cast<D3DParseUnknownCommand_t>(GetProcAddress(ddrawModule, "D3DParseUnknownCommand"));

	if (!fProc)
	{
		SPDLOG_ERROR("GetProcAddress({}, \"D3DParseUnknownCommand\") failed -> {}", fmt::ptr(ddrawModule), GetLastError());
		return 0;
	}

	return fProc(pvCommands, ppvReturnedCommand);
}