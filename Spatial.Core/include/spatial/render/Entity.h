#pragma once

#include <utils/EntityManager.h>

namespace spatial::render
{

class Entity
{
private:
	utils::Entity m_entity;

public:
	Entity()
		: Entity{utils::EntityManager::get().create()}
	{
	}

	Entity(utils::Entity entity)
		: m_entity{entity}
	{
	}

	Entity(Entity &&other) noexcept = default;
	Entity(const Entity &w) noexcept = default;
	Entity &operator=(Entity &&other) noexcept = default;
	Entity &operator=(const Entity &w) noexcept = default;

	const utils::Entity get() const
	{
		return m_entity;
	}

	utils::Entity get()
	{
		return m_entity;
	}

	operator utils::Entity()
	{
		return get();
	}

	~Entity()
	{
		if (!m_entity.isNull())
			utils::EntityManager::get().destroy(m_entity);
	}
};

}