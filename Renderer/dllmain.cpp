// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "D3D11Renderer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C"
{
	RENDERER_API IRenderer* Renderer_Create()
	{
		return new D3D11Renderer();
	}

	RENDERER_API void Renderer_Destroy(IRenderer* renderer)
	{
		delete renderer;
	}
}
