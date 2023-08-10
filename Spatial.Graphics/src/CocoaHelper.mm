#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>
#include <spatial/graphics/native/CocoaHelper.h>

void* cocoaGetContentViewFromWindow(void* id)
{
	NSWindow* win = (NSWindow*)id;
	NSView* view = [win contentView];
	return view;
}

void cocoaPrepareWindowColorSpace(void* id)
{
	NSWindow* win = (NSWindow*)id;
	[win setColorSpace:[NSColorSpace sRGBColorSpace]];
}

void* cocoaSetUpMetalLayer(void* id)
{
	NSWindow* win = (NSWindow*)id;
	NSView* view = [win contentView];

	[view setWantsLayer:YES];
	CAMetalLayer* metalLayer = [CAMetalLayer layer];
	metalLayer.bounds = view.bounds;

	// It's important to set the drawableSize to the actual backing pixels. When rendering
	// full-screen, we can skip the macOS compositor if the size matches the display size.
	metalLayer.drawableSize = [view convertSizeToBacking:view.bounds.size];

	// In its implementation of vkGetPhysicalDeviceSurfaceCapabilitiesKHR, MoltenVK takes into
	// consideration both the size (in points) of the bounds, and the contentsScale of the
	// CAMetalLayer from which the Vulkan surface was created.
	// See also https://github.com/KhronosGroup/MoltenVK/issues/428
	metalLayer.contentsScale = view.window.backingScaleFactor;

	// This is set to NO by default, but is also important to ensure we can bypass the compositor
	// in full-screen mode
	// See "Direct to Display" http://metalkit.org/2017/06/30/introducing-metal-2.html.
	metalLayer.opaque = YES;

	[view setLayer:metalLayer];

	return metalLayer;
}