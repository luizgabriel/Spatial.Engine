#pragma once

#include <filament/Engine.h>

namespace spatial
{

template <typename T>
class EngineRef
{
private:
	filament::Engine* m_engine;
	T* m_resource;

public:
	EngineRef(filament::Engine* engine, T* resource = nullptr) : m_engine{engine}, m_resource{resource}
	{
		assert(m_engine != nullptr);
	}

	EngineRef(EngineRef&& other) noexcept : m_engine{other.m_engine}, m_resource{other.release()} {}

	EngineRef(const EngineRef& w) = delete;

	EngineRef& operator=(EngineRef&& other) noexcept
	{
		if (m_resource)
			m_engine->destroy(m_resource);

		m_resource = other.release();

		return *this;
	}

	EngineRef& operator=(const EngineRef& w) = delete;

	/**
	 * \brief Releases the pointed resources and leaves the holder in an invalid state
	 */
	T* release() { return std::exchange(m_resource, nullptr); }

	const T* get() const { return m_resource; }

	T* get() { return m_resource; }

	T* operator->() { return get(); }

	const T* operator->() const { return get(); }

	~EngineRef()
	{
		if (m_resource)
			m_engine->destroy(m_resource);
	}
};

} // namespace spatial::render