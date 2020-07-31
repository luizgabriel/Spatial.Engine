#include <spatial/desktop/native/WindowHelper.h>

#include <windef.h>

void* getNativeWindow(const SDL_SysWMinfo& wmi)
{
	HWND win = (HWND)wmi.info.win.window;
	return (void*)win;
}
