#include "stdafx.h"
#include "r4.h"
#include "HW.h"

void CRenderTarget::phase_rain()
{
	if (!RImplementation.o.dx10_msaa)
		u_setrt(rt_Color,NULL,NULL, HW.pBaseZB);
	else
		u_setrt(rt_Color,NULL,NULL, rt_MSAADepth->pZRT);
	RImplementation.rmNormal();
}
