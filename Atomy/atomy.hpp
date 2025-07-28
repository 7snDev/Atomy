#pragma once
#ifdef _WIN32
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "atomyWindows.hpp"
#elif defined(__linux__)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "atomyLinux.hpp"
#else
Unsupported platform.
#endif