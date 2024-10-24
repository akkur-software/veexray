#pragma once

#ifdef VEEXCORE_EXPORTS
#define VEEX_CORE_API __declspec(dllexport)
#else
#define VEEX_CORE_API __declspec(dllimport)
#endif