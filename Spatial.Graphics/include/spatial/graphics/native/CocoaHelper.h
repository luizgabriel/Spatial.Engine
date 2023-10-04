#pragma once

extern "C" void* cocoaGetContentViewFromWindow(void* windowId);

extern "C" void cocoaPrepareWindowColorSpace(void* windowId);

extern "C" void* cocoaSetUpMetalLayer(void* viewId);

extern "C" void* cocoaResizeMetalLayer(void* viewId);