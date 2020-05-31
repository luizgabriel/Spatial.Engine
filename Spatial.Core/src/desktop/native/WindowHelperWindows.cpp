#include <spatial/desktop/native/WindowHelper.h>

#include <Windows.h>

void* getNativeWindow(const SDL_SysWMinfo& wmi) {
    HWND win = (HWND) wmi.info.win.window;
    return (void*) win;
}
