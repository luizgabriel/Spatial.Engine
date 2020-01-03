#pragma once

#include <utils/EntityManager.h>

#ifdef SPATIAL_DEBUG
#include <iostream>
#endif

namespace spatial::render
{

class Entity
{
private:
	utils::Entity m_entity;

public:
	Entity()
		: m_entity{utils::EntityManager::get().create()}
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

	~Entity()
	{
		if (m_entity.isNull())
		{
#ifdef SPATIAL_DEBUG
			std::cout << "\n[SPATIAL] Destroyed entity.";
#endif
			utils::EntityManager::get().destroy(m_entity);
		}
	}
};

}