#pragma once

#include <SDL_syswm.h>

struct SDL_Window;

extern "C" void* getNativeWindow(const SDL_SysWMinfo& wmi);
