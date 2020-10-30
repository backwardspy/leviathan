#pragma once

#if defined(_WIN32)
#define LV_PLATFORM_WINDOWS
#else
#define LV_PLATFORM_UNKNOWN
#endif

#if defined(LV_GRAPHICS_OPENGL)
constexpr int LVGLVersionMajor = 4;
constexpr int LVGLVersionMinor = 6;
#endif
