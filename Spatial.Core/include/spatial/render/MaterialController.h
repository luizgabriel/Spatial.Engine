#pragma once

#include <filament/Engine.h>
#include <spatial/common/EventQueue.h>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilesSystem.h>
#include <spatial/resources/Resource.h>
#include <unordered_map>

namespace spatial::render
{

template <ResourceType type>
struct LoadResourceEvent
{
	Resource<type> texture;
	explicit LoadResourceEvent(Resource<type> resource) : texture{std::move(resource)}
	{
	}
};

class MaterialController
{
  public:
	explicit MaterialController(filament::Engine& engine, FileSystem& fileSystem);

	void onEvent(const LoadResourceEvent<ImageTexture>& event);

	void onEvent(const LoadResourceEvent<CubeMapTexture>& event);

	void load(uint32_t resourceId, Texture&& texture);

	void onStartFrame();

	template <ResourceType type, typename = std::enable_if_t<type == CubeMapTexture || type == ImageTexture>>
	constexpr const filament::Texture* findResource(const Resource<type>& resource)
	{
		if (resource.isEmpty())
			return nullptr;

		auto it = mTextures.find(resource.getId());
		const auto* texture = (it != mTextures.end()) ? it->second.get() : nullptr;
		if (texture == nullptr)
			mJobQueue.enqueue(LoadResourceEvent{resource});

		return texture;
	}

	void onUpdateFrame(ecs::Registry& registry);

	template <typename MaterialComponent>
	void applyMaterialWithFinder(ecs::Registry& registry)
	{
		registry.getEntities<const MaterialComponent, SharedMaterialInstance>().each(
			[this](const auto& data, auto& materialInstance) {
				data.apply(*materialInstance->get(), [&](const auto& res) { return findResource(res); });
			});
	}

	template <typename MaterialComponent>
	void applyMaterial(ecs::Registry& registry)
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