#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include "spatial/resources/ResourceLoader.h"

namespace spatial::render
{

class SkyBoxController
{
  public:
	using ResourceId = uint32_t;

	explicit SkyBoxController(filament::Engine& engine);

	void onUpdateFrame(ecs::Registry& registry);

	void setRootPath(const std::filesystem::path& rootPath);

  private:
	filament::Engine& mEngine;
	std::filesystem::path mRootPath;
	std::unordered_map<ResourceId, Texture> mTextures;

	tl::expected<Texture, ResourceError> createTexture(const std::vector<uint8_t>&& data);
};

} // namespace spatial::render