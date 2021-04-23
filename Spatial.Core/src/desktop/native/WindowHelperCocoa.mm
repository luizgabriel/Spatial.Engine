#include <spatial/desktop/native/CocoaHelper.h>
#include <Cocoa/Cocoa.h>

void* cocoaGetContentViewFromWindow(void* id)
{
	NSWindow* win = (NSWindow*)id;
	NSView* view = [win contentView];
	return view;
}