#pragma once

#include <memory>
#include <spatial/script/Isolate.h>
#include <v8.h>

namespace spatial::script
{

class PlatformContext
{
  public:
	PlatformContext();
	~PlatformContext();

	Isolate createIsolate();

  private:
	std::unique_ptr<v8::Platform> mPlatform;
	std::unique_ptr<v8::ArrayBuffer::Allocator> mAllocator;
};

} // namespace spatial::v8