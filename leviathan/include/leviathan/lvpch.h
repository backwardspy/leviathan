#pragma once

// platform detection
#include "core.h"

// platform
#if defined(LV_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

// libc
#include <cstdint>

// stl functionality
#include <algorithm>
#include <any>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <random>
#include <sstream>
#include <typeinfo>
#include <type_traits>

// stl data structures
#include <array>
#include <bitset>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// vendor
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
