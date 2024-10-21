#pragma once

// Forward references
struct vertRender;
struct vertBoned1W;
struct vertBoned2W;
struct vertBoned3W;
struct vertBoned4W;
class CBoneInstance;
class light;
class CRenderDevice;

typedef void __stdcall xrSkin1W(vertRender* D, vertBoned1W* S, u32 vCount, CBoneInstance* Bones);
typedef void __stdcall xrSkin2W(vertRender* D, vertBoned2W* S, u32 vCount, CBoneInstance* Bones);
typedef void __stdcall xrSkin3W(vertRender* D, vertBoned3W* S, u32 vCount, CBoneInstance* Bones);
typedef void __stdcall xrSkin4W(vertRender* D, vertBoned4W* S, u32 vCount, CBoneInstance* Bones);

typedef void __stdcall xrPLC_calc3(int& c0, int& c1, int& c2, CRenderDevice& Device, Fvector* P, Fvector& N, light* L, float energy, Fvector& O);

#pragma pack(push,8)
struct xrDispatchTable
{
	xrSkin1W* skin1W;
	xrSkin2W* skin2W;
	xrSkin3W* skin3W;
	xrSkin4W* skin4W;
	xrPLC_calc3* PLC_calc3;
};
#pragma pack(pop)

typedef void __cdecl xrBinder(xrDispatchTable* T, _processor_info* ID);
