#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/resources/ResourceLoaderUtils.h>
#include <unordered_map>
#include <spatial/resources/FilesSystem.h>

namespace spatial::render
{

class IndirectLightController
{
  public:
	using ResourceId = uint32_t;
	explicit IndirectLightController(filament::Engine& engine, FileSystem& fileSystem);

	void loadTexture(ResourceId resourceId, const uint8_t* data, size_t size);
	void loadIrradianceValues(ResourceId resourceId, const bands_t& bands);

	void onUpdateFrame(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;
	FileSystem& mFileSystem;

	std::unordered_map<ResourceId, Texture> mTextures;
	std::unordered_map<ResourceId, bands_t> mBands;
};

} // namespace spatial::render