#include "stdafx.h"
#include "dxRenderFactory.h"
#include "dxUIRender.h"
#include "dxDebugRender.h"
#include "xrRender_console.h"
#include "r4.h"
#include "D3DUtils.h"

BOOL DllMainXrRenderR4(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		::Render = &RImplementation;
		::RenderFactory = &RenderFactoryImpl;
		::DU = &DUImpl;
		UIRender = &UIRenderImpl;
		DRender	= &DebugRenderImpl;
		xrRender_initconsole();
		break ;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


extern "C" {

bool SupportsDX11Rendering();
};

bool SupportsDX11Rendering()
{
	return xrRender_test_hw() ? true : false;
}
