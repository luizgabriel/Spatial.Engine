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
        : m_engine{engine}, m_entity{utils::EntityManager::get().create()}
    {
    }

    ~EntityResource()
    {
        m_engine->destroy(m_entity);
        utils::EntityManager::get().destroy(m_entity);
    }

    utils::Entity get()
    {
        return m_entity;
    }
};

} // namespace spatial::render