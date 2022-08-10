#pragma once

#include <algorithm>
#include <filament/Engine.h>
#include <functional>
#include <spatial/common/Signal.h>
#include <spatial/resources/FileSystem.h>
#include <spatial/script/PlatformContext.h>

namespace spatial::graphics
{

class RegistryRenderingSystem
{
  public:
	explicit RegistryRenderingSystem(FileSystem& fileSystem);

	void onStartFrame(float);

	void onUpdateFrame(float);

	void onRender(filament::Renderer& renderer);

	auto& getOnPublishRegistrySignal()
	{
		return mOnPublishRegistry;
	}

  private:
	FileSystem& mFileSystem;
	Signal<std::function<void(ecs::Registry&)>> mOnPublishRegistry;
};

} // namespace spatial::graphics