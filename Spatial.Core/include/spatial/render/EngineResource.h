#pragma once

#include <filament/Engine.h>
#include <memory>

namespace fl = filament;

namespace spatial
{

/**
 * A unique_ptr like holder
 * @tparam ResourceType
 */
template <typename ResourceType>
class EngineResource
{
private:
	filament::Engine& m_engine;
	ResourceType* m_resource;

public:
	explicit EngineResource(fl::Engine& engine, ResourceType* resource = nullptr) noexcept
		: m_engine{engine}, m_resource{resource}
	{
	}

	EngineResource(const EngineResource&) = delete;
	EngineResource& operator=(const EngineResource&) = delete;

	EngineResource(EngineResource&& other) noexcept
		: m_engine{other.m_engine}, m_resource{std::exchange(other.m_resource, nullptr)}
	{
	}

	EngineResource& operator=(EngineResource&& other) noexcept
	{
		reset();
		m_resource = other.release();

		return *this;
	}

	auto& getEngine()
	{
		return m_engine;
	}

	ResourceType* get()
	{
		return m_resource;
	}

	ResourceType& ref()
	{
		return *m_resource;
	}

	const ResourceType* get() const
	{
		return m_resource;
	}

	void reset()
	{
		if (m_resource)
			m_engine.destroy(m_resource);
		m_resource = nullptr;
	}

	ResourceType* release()
	{
		return std::exchange(m_resource, nullptr);
	}

	~EngineResource()
	{
		reset();
	}

	ResourceType* operator->()
	{
		return m_resource;
	}

	const ResourceType* operator->() const
	{
		return m_resource;
	}
};



template <typename ResourceType>
using SharedEngineResource = std::shared_ptr<ResourceType>;

template <typename ResourceType>
EngineResource<ResourceType> createResource(filament::Engine& engine, ResourceType* resource)
{
	return EngineResource<ResourceType>{engine, resource};
}

template <typename ResourceType>
SharedEngineResource<ResourceType> createSharedResource(filament::Engine& engine, ResourceType* resource) noexcept
{
	return {resource, [&engine](ResourceType* resource) { engine.destroy(resource); }};
}

template <typename ResourceType>
SharedEngineResource<ResourceType> toShared(EngineResource<ResourceType>&& resource) noexcept
{
	return createSharedResource(resource.getEngine(), resource.release());
}

} // namespace spatial
