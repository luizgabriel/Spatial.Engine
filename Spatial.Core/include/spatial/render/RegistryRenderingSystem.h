#pragma once

#include <algorithm>
#include <filament/Engine.h>
#include <spatial/common/Signal.h>
#include <spatial/ecs/RegistryCollection.h>
#include <spatial/resources/FileSystem.h>
#include <spatial/script/PlatformContext.h>

namespace spatial::render
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

	Signal<ecs::RegistryCollection&> mOnPublishRegistry;

	[[nodiscard]] ecs::RegistryCollection getPublishedRegistries() const;
};

} // namespace spatial::render