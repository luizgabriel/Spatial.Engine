#pragma once

#include <filament/Engine.h>
#include <utils/EntityManager.h>

namespace spatial::render
{

class Entity
{
private:
	filament::Engine* m_engine;
	utils::Entity m_entity;

public:
	Entity(filament::Engine* engine) : m_engine{engine}, m_entity{} {}

	Entity(filament::Engine* engine, utils::Entity entity) noexcept : m_engine{engine}, m_entity{entity} {}

	Entity(Entity&& other) noexcept : m_engine{other.m_engine}, m_entity{std::exchange(other.m_entity, utils::Entity{})} {}

	Entity& operator=(Entity&& other) noexcept
	{
		if (!m_entity.isNull())
		{
			utils::EntityManager::get().destroy(m_entity);
			m_engine->destroy(m_entity);
		}

		m_entity = other.get();

		return *this;
	}

	Entity(const Entity& other) = delete;
	Entity& operator=(const Entity& w) = delete;

	~Entity()
	{
		if (!m_entity.isNull())
		{
			utils::EntityManager::get().destroy(m_entity);
			m_engine->destroy(m_entity);
		}
	}

	utils::Entity get() { return m_entity; }
};

} // namespace spatial::render