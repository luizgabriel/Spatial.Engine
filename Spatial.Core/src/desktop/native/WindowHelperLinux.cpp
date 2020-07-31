#include <spatial/desktop/native/WindowHelper.h>

#include <X11/Xlib.h>

void* getNativeWindow(const SDL_SysWMinfo& wmi)
{
	Window win = (Window)wmi.info.x11.window;
	return (void*)win;
}
