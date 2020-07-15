#include <spatial/desktop/native/WindowHelper.h>

#include <Cocoa/Cocoa.h>

void* getNativeWindow(const SDL_SysWMinfo& wmi) {
    NSWindow* win = wmi.info.cocoa.window;
    NSView* view = [win contentView];
    return view;
}