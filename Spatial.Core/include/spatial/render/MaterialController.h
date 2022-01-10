#pragma once

#include <filament/Engine.h>
#include <spatial/common/EventQueue.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/Resource.h>
#include <spatial/resources/ResourceLoader.h>
#include <unordered_map>

namespace spatial::render
{

class MaterialController
{
  public:
	explicit MaterialController(filament::Engine& engine);

	void onEvent(const LoadResourceEvent<ImageTexture>& event);

	void onEvent(const LoadResourceEvent<CubeMapTexture>& event);

	void load(uint32_t resourceId, Texture&& texture);

	void onStartFrame();

	template <ResourceType type, typename = std::enable_if_t<type == CubeMapTexture || type == ImageTexture>>
	constexpr const filament::Texture* operator()(const Resource<type>& resource) const
	{
		if (resource.isEmpty())
			return nullptr;

		auto it = std::as_const(mTextures).find(resource.getId());
		const auto* texture = (it != mTextures.end()) ? it->second.get() : nullptr;
		if (texture == nullptr)
			mJobQueue.enqueue(LoadResourceEvent{resource});

		return texture;
	}

	template <typename MaterialComponent>
	void onUpdateFrameWithFinder(ecs::Registry& registry, const filament::Material& material)
	{
		registry.getEntities<const MaterialComponent>(ecs::ExcludeComponents<render::MaterialInstance>)
			.each([&](ecs::Entity entity, const auto&) {
				auto materialInstance = render::createMaterialInstance(mEngine, material);
				registry.addComponent<MaterialInstance>(entity, std::move(materialInstance));
			});

		registry.getEntities<MaterialComponent, render::MaterialInstance>().each(
			[&](const auto& data, auto& materialInstance) { data.apply(materialInstance.ref(), *this); });
	}

	template <typename MaterialComponent>
	void onUpdateFrame(ecs::Registry& registry, const filament::Material& material)
	{
		registry.getEntities<const MaterialComponent>(ecs::ExcludeComponents<render::MaterialInstance>)
			.each([&](ecs::Entity entity, const auto&) {
				auto materialInstance = render::createMaterialInstance(mEngine, material);
				registry.addComponent<MaterialInstance>(entity, std::move(materialInstance));
			});

		registry.getEntities<MaterialComponent, render::MaterialInstance>().each(
			[&](const auto& data, auto& materialInstance) { data.apply(materialInstance.ref()); });
	}

	void setRootPath(const std::filesystem::path& rootPath);

  private:
	filament::Engine& mEngine;
	std::filesystem::path mRootPath;
	mutable EventQueue mJobQueue;
	std::unordered_map<uint32_t, render::Texture> mTextures;
};

} // namespace spatial::render