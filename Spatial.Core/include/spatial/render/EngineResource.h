#pragma once

#include <filament/Engine.h>
#include <memory>

namespace spatial::render
{

/**
 * A unique_ptr like holder
 * @tparam ResourceType
 */
template <typename ResourceType>
class EngineResource
{
  private:
	filament::Engine& mEngine;
	ResourceType* mResource;

  public:
	explicit EngineResource(filament::Engine& engine, ResourceType* resource) noexcept
		: mEngine{engine}, mResource{resource}
	{
	}

	EngineResource(const EngineResource&) = delete;
	EngineResource& operator=(const EngineResource&) = delete;

	EngineResource(EngineResource&& other) noexcept : mEngine{other.mEngine}, mResource{other.release()}
	{
	}

	EngineResource& operator=(EngineResource&& other) noexcept
	{
		reset();
		mResource = other.release();

		return *this;
	}

	auto& getEngine() noexcept
	{
		return mEngine;
	}

	const auto& getEngine() const noexcept
	{
		return mEngine;
	}

	const ResourceType* get() const noexcept
	{
		return mResource;
	}

	ResourceType* get() noexcept
	{
		return mResource;
	}

	void reset()
	{
		if (isValid())
			mEngine.destroy(mResource);

		mResource = nullptr;
	}

	ResourceType* release()
	{
		return std::exchange(mResource, nullptr);
	}

	~EngineResource()
	{
		reset();
	}

	ResourceType* operator->() noexcept
	{
		return mResource;
	}

	const ResourceType* operator->() const noexcept
	{
		return mResource;
	}

	ResourceType& operator*()
	{
		assert(isValid());
		return *mResource;
	}

	const ResourceType& operator*() const
	{
		assert(isValid());
		return *mResource;
	}

	[[nodiscard]] bool isValid() const noexcept
	{
		return mResource != nullptr;
	}

	operator bool() const noexcept
	{
		return isValid();
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

} // namespace spatial::render
