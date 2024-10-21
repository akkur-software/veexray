#pragma once
#include <windows.h>
#include "../xrCore/cpuid.h"

#ifdef _GPA_ENABLED
	#include <tal.h>
#endif

typedef void (*PTTAPI_WORKER_FUNC)(LPVOID lpWorkerParameters);
typedef PTTAPI_WORKER_FUNC LPPTTAPI_WORKER_FUNC;

extern "C" {

// Initializes subsystem
// Returns zero for error, and number of workers on success
DWORD ttapi_Init(_processor_info* ID);

// Destroys subsystem
void ttapi_Done();

// Return number of workers
DWORD ttapi_GetWorkersCount();

// Adds new task
// No more than TTAPI_HARDCODED_THREADS should be added
void ttapi_AddWorker(LPPTTAPI_WORKER_FUNC lpWorkerFunc, LPVOID lpvWorkerFuncParams);

// Runs and wait for all workers to complete job
void ttapi_RunAllWorkers();
}
