#include "stdafx.h"
#include "xrAPI.h"

extern dxRenderFactory RenderFactoryImpl;

XRAPI_API IRender_interface* Render = NULL;
XRAPI_API dxRenderFactory* RenderFactory = &RenderFactoryImpl;
XRAPI_API CDUInterface* DU = NULL;
XRAPI_API xr_token* vid_mode_token = NULL;
XRAPI_API IUIRender* UIRender = NULL;
XRAPI_API CGameMtlLibrary* PGMLib = NULL;
XRAPI_API IDebugRender*	DRender = NULL;
