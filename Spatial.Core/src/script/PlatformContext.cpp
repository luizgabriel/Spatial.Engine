#include <libplatform/libplatform.h>
#include <spatial/script/PlatformContext.h>

namespace spatial::script
{

PlatformContext::PlatformContext()
	: mPlatform{v8::platform::NewDefaultPlatform()}, mAllocator{v8::ArrayBuffer::Allocator::NewDefaultAllocator()}
{
	v8::V8::InitializePlatform(mPlatform.get());
	v8::V8::Initialize();
}

Isolate PlatformContext::createIsolate()
{
	auto params = v8::Isolate::CreateParams{};
	params.array_buffer_allocator = mAllocator.get();
	return Isolate{params};
}

PlatformContext::~PlatformContext()
{
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();
}

} // namespace spatial::script