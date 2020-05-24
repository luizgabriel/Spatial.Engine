#pragma once

#include <filament/Engine.h>
#include <memory>

namespace spatial
{

template <typename ResourceType>
class FilamentEngineResourceDeleter
{
private:
	filament::Engine* m_engine;

public:
	explicit FilamentEngineResourceDeleter(filament::Engine* engine) noexcept : m_engine{engine}
	{
		assert(engine != nullptr);
	}

	void operator()(ResourceType* resource)
	{
		if (resource)
			m_engine->destroy(resource);
	}
};

template <typename ResourceType>
using EngineResource = std::unique_ptr<ResourceType, FilamentEngineResourceDeleter<ResourceType>>;

template <typename ResourceType>
EngineResource<ResourceType> createResource(filament::Engine* engine, ResourceType* resource = nullptr) noexcept
{
	return {resource, FilamentEngineResourceDeleter<ResourceType>{engine}};
}

template <typename ResourceType>
using SharedEngineResource = std::shared_ptr<ResourceType>;

template <typename ResourceType>
SharedEngineResource<ResourceType> createSharedResource(filament::Engine* engine, ResourceType* resource) noexcept
{
	return {resource, FilamentEngineResourceDeleter<ResourceType>{engine}};
}

} // namespace spatial
