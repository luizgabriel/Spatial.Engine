#pragma once

#include <filament/Engine.h>
#include <utils/EntityManager.h>

namespace spatial
{

class Entity
{
private:
	filament::Engine& m_engine;
	utils::Entity m_entity;

public:
	explicit Entity(filament::Engine& engine);

	Entity(filament::Engine& engine, utils::Entity entity);

	~Entity();

	Entity(Entity&& other) noexcept;
	Entity& operator=(Entity&& other) noexcept;

	Entity(const Entity& other) = delete;
	Entity& operator=(const Entity& w) = delete;

	[[nodiscard]] utils::Entity release();

	void reset();

	utils::Entity get()
	{
		return m_entity;
	}
};

} // namespace spatial