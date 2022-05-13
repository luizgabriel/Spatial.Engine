#include <chrono>
#include <future>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Resource.h>
#include <spatial/resources/ResourceController.h>

namespace spatial
{

void ResourceController::loadResources(FileSystem& fileSystem, ecs::Registry& registry)
{
	using namespace std::chrono_literals;
	using FutureData = std::future<std::vector<uint8_t>>;

	registry
		.getEntities<const ecs::Resource>(ecs::ExcludeComponents<FutureData, ecs::ResourceData, ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const auto& resource) {
			if (resource.relativePath.empty())
				return;

			auto futureData = fileSystem.readBinaryAsync(resource.relativePath);
			registry.addComponent(entity, std::move(futureData));
		});

	registry.getEntities<const ecs::Resource, FutureData>(ecs::ExcludeComponents<ecs::ResourceData>)
		.each([&](ecs::Entity entity, const auto& resource, auto& futureData) {
			if (futureData.wait_for(10us) != std::future_status::ready)
				return;

			auto data = futureData.get();
			registry.addComponent<ecs::ResourceData>(entity, std::move(data));
			registry.removeComponent<FutureData>(entity);
		});
}

} // namespace spatial