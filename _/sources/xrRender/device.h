#pragma once

// Note:
// ZNear - always 0.0f
// ZFar - always 1.0f

#include "../xrcore/xrCore.h"
#include "../xrcore/pure.h"
#include "../xrcore/ftimer.h"
#include "../xrEngine/stats.h"
#include "../xrcore/build_config_defines.h"

#define VIEWPORT_NEAR  Device.ViewportNear //0.2f
#define R_VIEWPORT_NEAR 0.005f
#define DEVICE_RESET_PRECACHE_FRAME_COUNT 10

#include "FactoryPtr.h"
#include "RenderDeviceRender.h"

#pragma pack(push,4)

__interface IRenderDevice
{
	CStatsPhysics* __stdcall StatPhysics();
	void __stdcall AddSeqFrame(pureFrame* f, bool mt);
	void __stdcall RemoveSeqFrame(pureFrame* f);
};

class CRenderDeviceData
{
public:
	u32 dwWidth;
	u32 dwHeight;

	u32 dwPrecacheFrame;
	BOOL b_is_Ready;
	BOOL b_is_Active;
	BOOL b_hide_cursor;
	u32 dwFrame;

	float fTimeDelta;
	float fTimeGlobal;

	u32 dwTimeDelta;
	u32 dwTimeGlobal;
	u32 dwTimeContinual;

	Fvector vCameraPosition;
	Fvector vCameraDirection;
	Fvector vCameraTop;
	Fvector vCameraRight;

	Fmatrix mView;
	Fmatrix mProject;
	Fmatrix mFullTransform;	
	Fvector vCameraPosition_saved; // Copies of corresponding members. Used for synchronization.

	Fmatrix mView_saved;
	Fmatrix mProject_saved;
	Fmatrix mFullTransform_saved;

	float fFOV;
	float fASPECT;
	float ViewportNear = 0.2f;

protected:
	u32 Timer_MM_Delta;
	CTimer_paused Timer;
	CTimer_paused TimerGlobal;
	CTimer frame_timer;

public:
	CRegistrator<pureRender> seqRender;
	CRegistrator<pureAppActivate> seqAppActivate;
	CRegistrator<pureAppDeactivate> seqAppDeactivate;
	CRegistrator<pureAppStart> seqAppStart;
	CRegistrator<pureAppEnd> seqAppEnd;
	CRegistrator<pureFrame> seqFrame;
	CRegistrator<pureScreenResolutionChanged> seqResolutionChanged;

	HWND m_hWnd;
};

#pragma pack(pop)

class CSecondViewportParams
{
	bool isActive;
	u8 frameDelay;

public:
	bool isCamReady;

	inline bool IsSVPActive()
	{
		return isActive;
	}

	void SetSVPActive(bool bState);
	bool IsSVPFrame();

	inline u8 GetSVPFrameDelay()
	{
		return frameDelay;
	}

	void SetSVPFrameDelay(u8 iDelay)
	{
		frameDelay = iDelay;
		clamp<u8>(frameDelay, 2, u8(-1));
	}
};

class CRenderDevice : public IRenderDevice, public CRenderDeviceData
{
private:
	// Main objects used for creating and rendering the 3D scene
	u32 m_dwWindowStyle;
	RECT m_rcWindowBounds;
	RECT m_rcWindowClient;
	CTimer TimerMM;

	void _Create(LPCSTR shName);
	void _Destroy(BOOL bKeepTextures);
	void _SetupStates();

public:
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	u32 dwPrecacheTotal;
	float fWidth_2, fHeight_2;
	void OnWM_Activate(WPARAM wParam, LPARAM lParam);
	IRenderDeviceRender* m_pRender;
	BOOL m_bNearer;

	void SetNearer(BOOL enabled)
	{
		if (enabled && !m_bNearer)
		{
			m_bNearer = TRUE;
			mProject._43 -= EPS_L;
		}
		else if (!enabled && m_bNearer)
		{
			m_bNearer = FALSE;
			mProject._43 += EPS_L;
		}
		m_pRender->SetCacheXform(mView, mProject);
	}

	void DumpResourcesMemoryUsage() { m_pRender->ResourcesDumpMemoryUsage(); }

	CRegistrator<pureFrame> seqFrameMT;
	CRegistrator<pureDeviceReset> seqDeviceReset;
	xr_vector<fastdelegate::FastDelegate0<>> seqParallel;
	CStats* Statistic;
	Fmatrix mInvFullTransform;
	CSecondViewportParams m_SecondViewport;

	CRenderDevice()
		:
		m_pRender(0)
#ifdef INGAME_EDITOR
        , m_editor_module(0),
        m_editor_initialize(0),
        m_editor_finalize(0),
        m_editor(0),
        m_engine(0)
#endif // #ifdef INGAME_EDITOR
#ifdef PROFILE_CRITICAL_SECTIONS
        ,mt_csEnter(MUTEX_PROFILE_ID(CRenderDevice::mt_csEnter))
        ,mt_csLeave(MUTEX_PROFILE_ID(CRenderDevice::mt_csLeave))
#endif // #ifdef PROFILE_CRITICAL_SECTIONS
	{
		m_hWnd = NULL;
		b_is_Active = FALSE;
		b_is_Ready = FALSE;
		b_hide_cursor = FALSE;
		Timer.Start();
		m_bNearer = FALSE;
		
		m_SecondViewport.SetSVPActive(false);
		m_SecondViewport.SetSVPFrameDelay(2);
		m_SecondViewport.isCamReady = false;			
	};

	void Pause(BOOL bOn, BOOL bTimer, BOOL bSound, LPCSTR reason);
	bool Paused();

	// Scene control
	void PreCache(u32 amount, bool b_draw_loadscreen, bool b_wait_user_input);
	BOOL Begin();
	void Clear();
	void End();
	void FrameMove();

	void overdrawBegin();
	void overdrawEnd();

	//Console Screenshot
	void Screenshot();

	// Mode control
	void DumpFlags();
	IC CTimer_paused* GetTimerGlobal() { return &TimerGlobal; }
	u32 TimerAsync() { return TimerGlobal.GetElapsed_ms(); }
	u32 TimerAsync_MMT() { return TimerMM.GetElapsed_ms() + Timer_MM_Delta; }

	// Creation & Destroying
	void ConnectToRender();
	void Create(void);
	void Run(void);
	void Destroy(void);
	void Reset(bool precache = true);

	void Initialize(void);
	void ShutDown(void);

public:
	void time_factor(const float& time_factor)
	{
		Timer.time_factor(time_factor);
		TimerGlobal.time_factor(time_factor);
	}

	IC const float& time_factor() const
	{
		VERIFY(Timer.time_factor() == TimerGlobal.time_factor());
		return (Timer.time_factor());
	}

	// Multi-threading
	xrCriticalSection mt_csEnter;
	xrCriticalSection mt_csLeave;
	volatile BOOL mt_bMustExit;

	ICF void remove_from_seq_parallel(const fastdelegate::FastDelegate0<>& delegate)
	{
		xr_vector<fastdelegate::FastDelegate0<>>::iterator I = std::find(
			seqParallel.begin(),
			seqParallel.end(),
			delegate
		);
		if (I != seqParallel.end())
			seqParallel.erase(I);
	}

	IC u32 frame_elapsed()
	{
		return frame_timer.GetElapsed_ms();
	}

public:
	void xr_stdcall on_idle();
	bool xr_stdcall on_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);

private:
	void message_loop();
	virtual void _BCL AddSeqFrame(pureFrame* f, bool mt);
	virtual void _BCL RemoveSeqFrame(pureFrame* f);
	virtual CStatsPhysics* _BCL StatPhysics() { return Statistic; }

#ifdef INGAME_EDITOR
public:
    IC editor::ide* editor() const { return m_editor; }

private:
    void initialize_editor();
    void message_loop_editor();

private:
    typedef editor::initialize_function_ptr initialize_function_ptr;
    typedef editor::finalize_function_ptr finalize_function_ptr;

private:
    HMODULE m_editor_module;
    initialize_function_ptr m_editor_initialize;
    finalize_function_ptr m_editor_finalize;
    editor::ide* m_editor;
    engine_impl* m_engine;
#endif // #ifdef INGAME_EDITOR
};

extern CRenderDevice Device;

#ifndef _EDITOR
#define RDEVICE Device
#else
#define RDEVICE EDevice
#endif

#ifdef ECO_RENDER
extern float refresh_rate;
#endif // ECO_RENDER
extern bool g_bBenchmark;
typedef fastdelegate::FastDelegate0<bool> LOADING_EVENT;
extern xr_list<LOADING_EVENT> g_loading_events;

class CLoadScreenRenderer : public pureRender
{
public:
	CLoadScreenRenderer();
	void start(bool b_user_input);
	void stop();
	virtual void OnRender();

	bool b_registered;
	bool b_need_user_input;
};

extern CLoadScreenRenderer load_screen_renderer;
