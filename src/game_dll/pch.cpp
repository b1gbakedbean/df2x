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

std::string GetSystemDDrawDllPath();
std::string GetAlternateDDrawDllPath();

std::string systemDDrawDllPath = GetSystemDDrawDllPath();
std::string alternateDDrawDllPath = GetAlternateDDrawDllPath();
HMODULE ddrawModule;

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
	return "DDraw.dgVoodoo.dll";
}

extern "C" __declspec(dllexport) HRESULT __stdcall DirectDrawCreate(GUID* pGUID, void** ppDD, void* pUnkOuter)
{
	if (systemDDrawDllPath.empty() && alternateDDrawDllPath.empty())
	{
		return 0;
	}

	if (!ddrawModule)
	{
		ddrawModule = LoadLibrary(alternateDDrawDllPath.empty() ? systemDDrawDllPath.c_str() : alternateDDrawDllPath.c_str());
	}

	if (ddrawModule)
	{
		auto fProc = reinterpret_cast<DirectDrawCreate_t>(GetProcAddress(ddrawModule, "DirectDrawCreate"));

		SPDLOG_DEBUG("Proxying DirectDrawCreate...");

		return fProc(pGUID, ppDD, pUnkOuter);
	}

	return 0;
}

extern "C" __declspec(dllexport) HRESULT __stdcall DirectDrawEnumerateExA(void* pCallback, void* pContext, DWORD dwFlags)
{
	if (systemDDrawDllPath.empty() && alternateDDrawDllPath.empty())
	{
		return 0;
	}

	if (!ddrawModule)
	{
		ddrawModule = LoadLibrary(alternateDDrawDllPath.empty() ? systemDDrawDllPath.c_str() : alternateDDrawDllPath.c_str());
	}

	if (ddrawModule)
	{
		auto fProc = reinterpret_cast<DirectDrawEnumerateExA_t>(GetProcAddress(ddrawModule, "DirectDrawEnumerateExA"));

		SPDLOG_DEBUG("Proxying DirectDrawEnumerateExA...");

		return fProc(pCallback, pContext, dwFlags);
	}

	return 0;
}

extern "C" __declspec(dllexport) HRESULT __stdcall DirectDrawEnumerateA(void* pCallback, void* pContext)
{
	if (systemDDrawDllPath.empty() && alternateDDrawDllPath.empty())
	{
		return 0;
	}

	if (!ddrawModule)
	{
		ddrawModule = LoadLibrary(alternateDDrawDllPath.empty() ? systemDDrawDllPath.c_str() : alternateDDrawDllPath.c_str());
	}

	if (ddrawModule)
	{
		auto fProc = reinterpret_cast<DirectDrawEnumerateA_t>(GetProcAddress(ddrawModule, "DirectDrawEnumerateA"));

		SPDLOG_DEBUG("Proxying DirectDrawEnumerateA...");

		return fProc(pCallback, pContext);
	}

	return 0;
}

extern "C" __declspec(dllexport) int __stdcall AcquireDDThreadLock()
{
	if (systemDDrawDllPath.empty() && alternateDDrawDllPath.empty())
	{
		return 0;
	}

	if (!ddrawModule)
	{
		ddrawModule = LoadLibrary(alternateDDrawDllPath.empty() ? systemDDrawDllPath.c_str() : alternateDDrawDllPath.c_str());
	}

	if (ddrawModule)
	{
		auto fProc = reinterpret_cast<AcquireDDThreadLock_t>(GetProcAddress(ddrawModule, "AcquireDDThreadLock"));

		SPDLOG_DEBUG("Proxying AcquireDDThreadLock...");

		return fProc();
	}

	return 0;
}

extern "C" __declspec(dllexport) void __stdcall ReleaseDDThreadLock()
{
	if (systemDDrawDllPath.empty() && alternateDDrawDllPath.empty())
	{
		return;
	}

	if (!ddrawModule)
	{
		ddrawModule = LoadLibrary(alternateDDrawDllPath.empty() ? systemDDrawDllPath.c_str() : alternateDDrawDllPath.c_str());
	}

	if (ddrawModule)
	{
		auto fProc = reinterpret_cast<ReleaseDDThreadLock_t>(GetProcAddress(ddrawModule, "ReleaseDDThreadLock"));

		SPDLOG_DEBUG("Proxying ReleaseDDThreadLock...");

		fProc();
	}
}

extern "C" __declspec(dllexport) int __stdcall DDInternalUnlock(int unk1)
{
	if (systemDDrawDllPath.empty() && alternateDDrawDllPath.empty())
	{
		return 0;
	}

	if (!ddrawModule)
	{
		ddrawModule = LoadLibrary(alternateDDrawDllPath.empty() ? systemDDrawDllPath.c_str() : alternateDDrawDllPath.c_str());
	}

	if (ddrawModule)
	{
		auto fProc = reinterpret_cast<DDInternalUnlock_t>(GetProcAddress(ddrawModule, "DDInternalUnlock"));

		SPDLOG_DEBUG("Proxying DDInternalUnlock...");

		return fProc(unk1);
	}

	return 0;
}

extern "C" __declspec(dllexport) int __stdcall DDInternalLock(int unk1, int unk2)
{
	if (systemDDrawDllPath.empty() && alternateDDrawDllPath.empty())
	{
		return 0;
	}

	if (!ddrawModule)
	{
		ddrawModule = LoadLibrary(alternateDDrawDllPath.empty() ? systemDDrawDllPath.c_str() : alternateDDrawDllPath.c_str());
	}

	if (ddrawModule)
	{
		auto fProc = reinterpret_cast<DDInternalLock_t>(GetProcAddress(ddrawModule, "DDInternalLock"));

		SPDLOG_DEBUG("Proxying DDInternalLock...");

		return fProc(unk1, unk2);
	}

	return 0;
}

extern "C" __declspec(dllexport) HRESULT __stdcall D3DParseUnknownCommand(void* pvCommands, void** ppvReturnedCommand)
{
	if (systemDDrawDllPath.empty() && alternateDDrawDllPath.empty())
	{
		return 0;
	}

	if (!ddrawModule)
{
		ddrawModule = LoadLibrary(alternateDDrawDllPath.empty() ? systemDDrawDllPath.c_str() : alternateDDrawDllPath.c_str());
	}

	if (ddrawModule)
	{
		auto fProc = reinterpret_cast<D3DParseUnknownCommand_t>(GetProcAddress(ddrawModule, "D3DParseUnknownCommand"));

		SPDLOG_DEBUG("Proxying D3DParseUnknownCommand...");

		return fProc(pvCommands, ppvReturnedCommand);
	}

	return 0;
}