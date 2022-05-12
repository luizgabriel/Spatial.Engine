#include <chrono>
#include <future>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/ResourceController.h>

namespace spatial::ecs
{

static auto gLogger = createDefaultLogger();

void ResourceController::loadResources(FileSystem& fileSystem, ecs::Registry& registry)
{
	using namespace std::chrono_literals;
	using FutureData = std::future<std::vector<uint8_t>>;

	registry
		.getEntities<const ecs::Resource>(ecs::ExcludeComponents<FutureData, ResourceData, ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const auto& resource) {
			if (resource.relativePath.empty())
				return;

			auto futureData = fileSystem.readBinaryAsync(resource.relativePath);
			registry.addComponent(entity, std::move(futureData));
		});

	registry
		.getEntities<const ecs::Resource, FutureData>(ecs::ExcludeComponents<ResourceData>)
		.each([&](ecs::Entity entity, const auto& resource, auto& futureData) {
			if (futureData.wait_for(10us) != std::future_status::ready)
				return;

			auto data = futureData.get();
			registry.addComponent<ResourceData>(entity, std::move(data));
			registry.removeComponent<FutureData>(entity);
		});
}

} // namespace spatial::ecs