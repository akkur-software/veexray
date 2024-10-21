#pragma once
#include "../xrCore/xrCore.h"

struct SEnumVerticesCallback
{
	virtual void operator ()(const Fvector& p) = 0;
};
