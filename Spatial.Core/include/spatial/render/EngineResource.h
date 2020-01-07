#pragma once

#include <filament/Engine.h>

namespace spatial::render
{

template <typename T>
class EngineResource
{
private:
	filament::Engine *m_engine;
	T *m_resource;

public:
	EngineResource(filament::Engine *engine = nullptr, T *resource = nullptr)
		: m_engine{engine},
		  m_resource{resource}
	{
	}

	EngineResource(EngineResource &&other) noexcept
		: m_engine{other.m_engine},
		  m_resource{std::exchange(other.m_resource, nullptr)}
	{
	}

	EngineResource(const EngineResource &w) = delete;

	EngineResource &operator=(EngineResource &&other) noexcept
	{
		m_engine->destroy(m_resource);
		m_resource = std::exchange(other.m_resource, nullptr);

		return *this;
	}

	EngineResource &operator=(const EngineResource &w) = delete;

	const T *get() const
	{
		return m_resource;
	}

	T *get()
	{
		return m_resource;
	}

	T *operator->()
	{
		return get();
	}

	const T *operator->() const
	{
		return get();
	}

	operator T *()
	{
		return get();
	}

	~EngineResource()
	{
		if (m_resource)
			m_engine->destroy(m_resource);
	}
};

} // namespace spatial::render