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
		.getEntities<const ecs::FileSystemResource>(
			ecs::Exclude<FutureData, ecs::FileSystemResourceData, ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const auto& resource) {
			if (resource.relativePath.empty())
				return;

			auto futureData = fileSystem.readBinaryAsync(resource.relativePath);
			registry.addComponent(entity, std::move(futureData));
		});

	registry.getEntities<const ecs::FileSystemResource, FutureData>(ecs::Exclude<ecs::FileSystemResourceData, ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const auto& resource, auto& futureData) {
			if (futureData.wait_for(10us) != std::future_status::ready)
				return;

			auto data = futureData.get();

			if (data.empty()) {
				registry.addComponent<ecs::ResourceError>(entity, "Empty resource");
				registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
			} else {
				registry.removeComponent<ecs::ResourceError>(entity);
				registry.addComponent<ecs::FileSystemResourceData>(entity, std::move(data));
			}

			registry.removeComponent<FutureData>(entity);
		});
}

} // namespace spatial