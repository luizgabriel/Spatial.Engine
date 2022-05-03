#pragma once

#include <filament/Engine.h>
#include <spatial/common/EventQueue.h>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FileSystem.h>
#include <unordered_map>

namespace spatial::render
{

struct LoadResourceEvent
{
	std::string texturePath;
};

class MaterialLoaderController
{
  public:
	explicit MaterialLoaderController(filament::Engine& engine, FileSystem& fileSystem);

	void onUpdateFrame(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;
	FileSystem& mFileSystem;
};

class MaterialController
{
  public:
	explicit MaterialController(filament::Engine& engine, FileSystem& fileSystem);

	void onEvent(const LoadResourceEvent& event);

	void load(uint32_t resourceId, Texture&& texture);

	void onStartFrame() const;

	const filament::Texture* findResource(std::string_view resource);

	void onUpdateFrame(ecs::Registry& registry) const;

	template <typename MaterialComponent>
	void applyMaterialWithFinder(ecs::Registry& registry)
	{
		registry.getEntities<const MaterialComponent, SharedMaterialInstance>().each(
			[this](const auto& data, auto& materialInstance) {
				data.apply(*materialInstance->get(), [&](const auto& res) { return findResource(res); });
			});
	}

	template <typename MaterialComponent>
	static void applyMaterial(ecs::Registry& registry)
	{
		registry.getEntities<MaterialComponent, SharedMaterialInstance>().each(
			[](const auto& data, auto& materialInstance) { data.apply(*materialInstance->get()); });
	}

  private:
	filament::Engine& mEngine;
	FileSystem& mFileSystem;
	EventQueue mJobQueue;
	std::unordered_map<uint32_t, render::Texture> mTextures;
};

} // namespace spatial::render