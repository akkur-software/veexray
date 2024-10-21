#include "stdafx.h"
#include "defines.h"

#ifdef DEBUG
ECORE_API BOOL bDebug = FALSE;

#endif

u32 psCurrentVidMode[2] = {0, 0};
u32 psCurrentBPP = 32;
Flags32 psDeviceFlags = 
{
	rsFullscreen | rsDetails | mtPhysics | mtSound | mtNetwork | rsDrawStatic | rsDrawDynamic
};

int psTextureLOD = 1;
