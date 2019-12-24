#pragma once

#include <filament/Engine.h>

#ifdef SPATIAL_DEBUG
#include <iostream>
#include <fmt/format.h>
#endif

namespace spatial::render
{

template <typename T, const char **RESOURCE_NAME>
class EngineResource
{
private:
	filament::Engine *m_engine;
	T *m_resource;

public:
	EngineResource(filament::Engine *engine, T *resource)
		: m_engine{engine}, m_resource{resource}
	{
		assert(resource != nullptr);
	}

	EngineResource(EngineResource &&other) noexcept
		: m_engine{other.m_engine}, m_resource{std::exchange(other.m_resource, nullptr)}
	{
	}

	EngineResource(const EngineResource &w) = delete;

	EngineResource &operator=(EngineResource &&other) noexcept
	{
		m_engine = other.m_engine;
		m_resource = std::exchange(other.m_resource, nullptr);
	}

	EngineResource &operator=(const EngineResource &w) = delete;

	const T *get_const() const
	{
		return m_resource;
	}

	T *get() const
	{
		return m_resource;
	}

	T *operator->() const
	{
		return get();
	}

	~EngineResource()
	{
		if (m_resource)
		{
#ifdef SPATIAL_DEBUG
			std::cout << fmt::format("\n[SPATIAL] Cleaned {} filament engine resource. ", *RESOURCE_NAME);
#endif

			m_engine->destroy(m_resource);
		}
	}
};

} // namespace spatial::render