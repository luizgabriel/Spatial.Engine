#include <Cocoa/Cocoa.h>
#include <spatial/desktop/native/CocoaHelper.h>

void* cocoaGetContentViewFromWindow(void* id)
{
	NSWindow* win = (NSWindow*)id;
	NSView* view = [win contentView];
	return view;
}