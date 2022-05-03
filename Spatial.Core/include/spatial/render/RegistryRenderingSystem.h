#pragma once

#include <algorithm>
#include <filament/Engine.h>
#include <spatial/common/Signal.h>
#include <spatial/ecs/RegistryCollection.h>
#include <spatial/render/IndirectLightController.h>

#include <spatial/resources/FileSystem.h>
#include <spatial/script/PlatformContext.h>

namespace spatial::render
{

class RegistryRenderingSystem
{
  public:
	RegistryRenderingSystem(filament::Engine& engine, FileSystem& fileSystem);

	void onRender(filament::Renderer& renderer);

	auto& getOnPublishRegistrySignal()
	{
		return mOnPublishRegistry;
	}

  private:
	filament::Engine& mEngine;
	FileSystem& mFileSystem;

	Signal<ecs::RegistryCollection&> mOnPublishRegistry;

	render::IndirectLightController mIndirectLightController;

	[[nodiscard]] ecs::RegistryCollection getPublishedRegistries() const;
};

} // namespace spatial::render