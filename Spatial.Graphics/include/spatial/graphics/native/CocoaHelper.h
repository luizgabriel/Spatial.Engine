#pragma once

/**
 * @description Returns the NSView from the NSWindow provided (native stuff)
 */
extern "C" void* cocoaGetContentViewFromWindow(void* id);

/**
 * @description Sets the color space of the NSWindow provided (native stuff)
 */
extern "C" void cocoaPrepareWindowColorSpace(void* id);

/**
 * @description Sets up the Metal layer of the NSWindow provided (native stuff)
 */
extern "C" void* cocoaSetUpMetalLayer(void* id);