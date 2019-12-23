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
	EngineResource(filament::Engine *engine, T *resource)
		: m_engine{engine}, m_resource{resource}
	{
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

	const T* get_const() const
	{
		return m_resource;
	}

	T* get() const
	{
		return m_resource;
	}

	T* operator->() const
	{
		return get();
	}

	~EngineResource()
	{
		if (m_resource)
			m_engine->destroy(m_resource);
	}
};

using SwapChainPtr = EngineResource<filament::SwapChain>;
using RendererPtr = EngineResource<filament::Renderer>;
using ScenePtr = EngineResource<filament::Scene>;
using ViewPtr = EngineResource<filament::View>;
using CameraPtr = EngineResource<filament::Camera>;

} // namespace spatial::render