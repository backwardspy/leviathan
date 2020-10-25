#pragma once

#if defined(_WIN32)
#define LV_PLATFORM_WINDOWS
#else
#define LV_PLATFORM_UNKNOWN
#endif

#if defined(LV_PLATFORM_WINDOWS)
#if defined(LV_BUILD_DLL)
#define LV_API __declspec(dllexport)
#else
#define LV_API __declspec(dllimport)
#endif
#else
#error Unsupported platform
#endif

constexpr int LVGLVersionMajor = 4;
constexpr int LVGLVersionMinor = 6;
