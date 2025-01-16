#pragma comment(linker, "/export:DirectDrawCreate=_DirectDrawCreate@12")

typedef HRESULT (__stdcall *DirectDrawCreate_t)(GUID* lpGUID, void** lplpDD, void* pUnkOuter);

extern "C" __declspec(dllexport) HRESULT __stdcall DirectDrawCreate(GUID* lpGUID, void** lplpDD, void* pUnkOuter)
{
	auto hLib = LoadLibrary("ddraw.dll");

	if (hLib)
	{
		auto fProc = reinterpret_cast<DirectDrawCreate_t>(GetProcAddress(hLib, "DirectDrawCreate"));

		return fProc(lpGUID, lplpDD, pUnkOuter);
	}

	return 0;
}