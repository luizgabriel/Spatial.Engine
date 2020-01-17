#pragma once

#include <filament/Engine.h>
#include <utils/EntityManager.h>

namespace spatial::render
{

class EntityResource
{
private:
    filament::Engine *m_engine;
    utils::Entity m_entity;

public:
    EntityResource(filament::Engine *engine)
        : m_engine{engine},
          m_entity{}
    {
    }

    EntityResource(filament::Engine *engine, utils::Entity entity) noexcept
        : m_engine{engine},
          m_entity{entity}
    {
    }

    EntityResource(EntityResource &&other) noexcept
        : m_engine{other.m_engine},
          m_entity{std::exchange(other.m_entity, utils::Entity{})}
    {
    }

    EntityResource &operator=(EntityResource &&other) noexcept
	{
		if (!m_entity.isNull())
        {
            utils::EntityManager::get().destroy(m_entity);
            m_engine->destroy(m_entity);
        }

		m_entity = other.get();

		return *this;
	}

    EntityResource(const EntityResource &other) = delete;
	EntityResource &operator=(const EntityResource &w) = delete;

    ~EntityResource()
    {
        if (!m_entity.isNull())
        {
            utils::EntityManager::get().destroy(m_entity);
            m_engine->destroy(m_entity);
        }
    }

    utils::Entity get()
    {
        return m_entity;
    }
};

} // namespace spatial::render