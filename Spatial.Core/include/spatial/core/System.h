#pragma once

#include <memory>
#include <utility>
#include <spatial/core/ApplicationEvents.h>

namespace spatial
{
template <typename S>
class System
{
private:
	Application* m_app;
	S m_system;

public:
	System(Application& application)
		: m_app{&application},
		  m_system{}
	{
		connect(application, &m_system);
	}

	template <typename... Args>
	System(Application& application, Args&&... args)
		: m_app{&application},
		  m_system{std::forward<Args>(args)...}
	{
		connect(*m_app, &m_system);
	}

	~System()
	{
		disconnect(*m_app, &m_system);
	}

	System(const System& other) = delete;

	System(System&& other) noexcept
		: m_app(other.m_app),
		  m_system(std::move(other.m_system))
	{
	}

	System& operator=(const System& other) = delete;

	System& operator=(System&& other) noexcept
	{
		m_app = other.m_app;
		m_system = std::move(other.m_system);

		return *this;
	}

	auto& get() { return m_system;  }
};

template <typename S>
using SystemRef = std::shared_ptr<System<S>>;

template <typename S, typename... Args>
SystemRef<S> createSharedSystem(Application& app, Args&&... args)
{
	return std::make_shared<System<S>>(&app, std::forward<Args>(args)...);
}
}
