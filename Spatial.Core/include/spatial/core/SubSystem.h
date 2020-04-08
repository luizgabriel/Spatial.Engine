#pragma once

#include <memory>
#include <utility>
#include <spatial/core/ApplicationEvents.h>

namespace spatial
{
template <typename S>
class SubSystem
{
private:
	Application* m_app;
	S m_system;

public:
	SubSystem(Application& application)
		: m_app{&application},
		  m_system{}
	{
		connect(application, &m_system);
	}

	template <typename... Args>
	SubSystem(Application& application, Args&&... args)
		: m_app{&application},
		  m_system{std::forward<Args>(args)...}
	{
		connect(*m_app, &m_system);
	}

	~SubSystem()
	{
		disconnect(*m_app, &m_system);
	}

	SubSystem(const SubSystem& other) = delete;

	SubSystem(SubSystem&& other) noexcept
		: m_app(other.m_app),
		  m_system(std::move(other.m_system))
	{
	}

	SubSystem& operator=(const SubSystem& other) = delete;

	SubSystem& operator=(SubSystem&& other) noexcept
	{
		m_app = other.m_app;
		m_system = std::move(other.m_system);

		return *this;
	}

	auto& get() { return m_system;  }
	auto& getSystem() { return m_system; }
	auto getApplication() const { return m_app; }
};

template <typename S>
using SystemRef = std::shared_ptr<SubSystem<S>>;

template <typename S, typename... Args>
SystemRef<S> makeSystem(Application& app, Args&&... args)
{
	return std::make_shared<SubSystem<S>>(&app, std::forward<Args>(args)...);
}
}
