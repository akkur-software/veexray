#pragma once

#pragma warning(disable:4995)
#pragma warning(disable:4995)
#pragma warning(default:4995)
#pragma warning(disable:4714)
#pragma warning( 4 : 4018 )
#pragma warning( 4 : 4244 )
#pragma warning(disable:4237)

#include <dxsdk/d3dx9.h>
#include <D3D11.h>
#include <dxsdk/D3Dx11core.h>
#include <D3DCompiler.h>
#include "xrD3DDefs.h"
#include "D3DExtensions.h"
#include "dxPixEventWrapper.h"

#define		R_R1	1
#define		R_R2	2
#define		R_R3	3
#define		R_R4	4
#define		RENDER	R_R4

#include "HW.h"
#include "Blender.h"
#include "r2_types.h"

inline void jitter(CBlender_Compile& C)
{
	C.r_dx10Texture("jitter0", JITTER(0));
	C.r_dx10Texture("jitter1", JITTER(1));
	C.r_dx10Texture("jitter2", JITTER(2));
	C.r_dx10Texture("jitter3", JITTER(3));
	C.r_dx10Texture("jitter4", JITTER(4));
	C.r_dx10Texture("jitterMipped", r2_jitter_mipped);
	C.r_dx10Sampler("smp_jitter");
}
