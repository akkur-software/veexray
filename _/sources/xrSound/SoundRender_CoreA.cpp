#include "stdafx.h"
#pragma hdrstop

#include "soundrender_coreA.h"
#include "soundrender_targetA.h"

#include "../xrCore/pure.h"
//#include "../xrEngine/XR_IOConsole.h"

namespace soundSmoothingParams {
	float power = 1.8;
	int steps = 15;
	float alpha = getAlpha();
	IC float getAlpha() {
		return 2.0f / (steps + 1);
	}
	IC float getTimeDeltaSmoothing() {
		return alpha;
		//return min(1.0f, alpha * (Device.fTimeDelta / steps));
	}
	IC float getSmoothedValue(float target, float current, float smoothing = getTimeDeltaSmoothing()) {
		return current + smoothing * (target - current);
	}
};

//extern CConsole* Console;

CSoundRender_CoreA* SoundRenderA = 0;

CSoundRender_CoreA::CSoundRender_CoreA(): CSoundRender_Core()
{
	pDevice = 0;
	pDeviceList = 0;
	pContext = 0;
}

CSoundRender_CoreA::~CSoundRender_CoreA()
{
}

BOOL CSoundRender_CoreA::EAXQuerySupport(BOOL bDeferred, const GUID* guid, u32 prop, void* val, u32 sz)
{
	return TRUE;
}

BOOL CSoundRender_CoreA::EAXTestSupport(BOOL bDeferred)
{
	return TRUE;
}

void CSoundRender_CoreA::_restart()
{
	inherited::_restart();
}

void CSoundRender_CoreA::_initialize(int stage)
{
	if (stage == 0)
	{
		pDeviceList = xr_new<ALDeviceList>();

		if (0 == pDeviceList->GetNumDevices())
		{
			CHECK_OR_EXIT(0, "OpenAL: Can't create sound device.");
			xr_delete(pDeviceList);
		}
		return;
	}

	pDeviceList->SelectBestDevice();
	R_ASSERT(snd_device_id>=0 && snd_device_id<pDeviceList->GetNumDevices());
	const ALDeviceDesc& deviceDesc = pDeviceList->GetDeviceDesc(snd_device_id);
	// OpenAL device
	pDevice = alcOpenDevice(deviceDesc.name);
	if (pDevice == NULL)
	{
		CHECK_OR_EXIT(0, "SOUND: OpenAL: Failed to create device.");
		bPresent = FALSE;
		return;
	}

	// Get the device specifier.
	const ALCchar* deviceSpecifier;
	deviceSpecifier = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);

	// Create context
	pContext = alcCreateContext(pDevice,NULL);
	if (0 == pContext)
	{
		CHECK_OR_EXIT(0, "SOUND: OpenAL: Failed to create context.");
		bPresent = FALSE;
		alcCloseDevice(pDevice);
		pDevice = 0;
		return;
	}

	// clear errors
	alGetError();
	alcGetError(pDevice);

	// Set active context
	AC_CHK(alcMakeContextCurrent(pContext));

	// initialize listener
	A_CHK(alListener3f (AL_POSITION,0.f,0.f,0.f));
	A_CHK(alListener3f (AL_VELOCITY,0.f,0.f,0.f));
	Fvector orient[2] = {{0.f, 0.f, 1.f}, {0.f, 1.f, 0.f}};
	A_CHK(alListenerfv (AL_ORIENTATION,&orient[0].x));
	A_CHK(alListenerf (AL_GAIN,1.f));

	// Check for EAX extension
	bEAX = deviceDesc.props.eax && !deviceDesc.props.eax_unwanted;
	bEAX = false;

	if (bEAX)
	{
		bDeferredEAX = EAXTestSupport(TRUE);
		bEAX = EAXTestSupport(FALSE);
	}

	// Init listener struct
	Listener.position.set(0.0f, 0.0f, 0.0f);
	Listener.prevVelocity.set(0.0f, 0.0f, 0.0f);
	Listener.curVelocity.set(0.0f, 0.0f, 0.0f);
	Listener.accVelocity.set(0.0f, 0.0f, 0.0f);
	Listener.orientation[0].set(0.0f, 0.0f, 0.0f);
	Listener.orientation[1].set(0.0f, 0.0f, 0.0f);

	inherited::_initialize(stage);

	if (stage == 1) //first initialize
	{
		// Pre-create targets
		CSoundRender_Target* T = 0;
		for (u32 tit = 0; tit < u32(psSoundTargets); tit++)
		{
			T = xr_new<CSoundRender_TargetA>();
			if (T->_initialize())
			{
				s_targets.push_back(T);
			}
			else
			{
				char command[256] = {0};
				Log("! SOUND: OpenAL: Max targets - ", tit);
				sprintf(command, "snd_targets %d", tit);
				T->_destroy();
				xr_delete(T);
				break;
			}
		}
	}
}

void CSoundRender_CoreA::set_master_volume(float f)
{
	if (bPresent)
	{
		A_CHK(alListenerf (AL_GAIN,f));
	}
}

void CSoundRender_CoreA::_clear()
{
	inherited::_clear();
	// remove targets
	CSoundRender_Target* T = 0;
	for (u32 tit = 0; tit < s_targets.size(); tit++)
	{
		T = s_targets[tit];
		T->_destroy();
		xr_delete(T);
	}
	// Reset the current context to NULL.
	alcMakeContextCurrent(NULL);
	// Release the context and the device.
	alcDestroyContext(pContext);
	pContext = 0;
	alcCloseDevice(pDevice);
	pDevice = 0;
	xr_delete(pDeviceList);
}

void CSoundRender_CoreA::i_eax_set(const GUID* guid, u32 prop, void* val, u32 sz)
{
}

void CSoundRender_CoreA::i_eax_get(const GUID* guid, u32 prop, void* val, u32 sz)
{
}

void CSoundRender_CoreA::update_listener(const Fvector& P, const Fvector& D, const Fvector& N, float dt)
{
	inherited::update_listener(P, D, N, dt);

	Listener.curVelocity.sub(P, Listener.position);

	float a = soundSmoothingParams::getTimeDeltaSmoothing();
	int p = soundSmoothingParams::power;
	Listener.accVelocity.x = soundSmoothingParams::getSmoothedValue(Listener.curVelocity.x * p / dt, Listener.accVelocity.x, a);
	Listener.accVelocity.y = soundSmoothingParams::getSmoothedValue(Listener.curVelocity.y * p / dt, Listener.accVelocity.y, a);
	Listener.accVelocity.z = soundSmoothingParams::getSmoothedValue(Listener.curVelocity.z * p / dt, Listener.accVelocity.z, a);
	
	Listener.prevVelocity.set(Listener.accVelocity);

	if (!Listener.position.similar(P))
	{
		Listener.position.set(P);
		bListenerMoved = TRUE;
	}
	Listener.orientation[0].set(D.x, D.y, -D.z);
	Listener.orientation[1].set(N.x, N.y, -N.z);

	A_CHK(alListener3f (AL_POSITION,Listener.position.x,Listener.position.y,-Listener.position.z));
	A_CHK(alListener3f (AL_VELOCITY, Listener.prevVelocity.x, Listener.prevVelocity.y, -Listener.prevVelocity.z));
	A_CHK(alListenerfv (AL_ORIENTATION,&Listener.orientation[0].x));
}