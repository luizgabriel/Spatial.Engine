#pragma once

#include <filament/Engine.h>
#include <filament/View.h>
#include <filament/Renderer.h>
#include <filament/Material.h>
#include <utils/EntityManager.h>
#include <spatial/render/EngineResource.h>
#include <memory>

namespace spatial::render
{

class RenderEngine
{
private:
    filament::Engine *m_engine;

public:
    RenderEngine(filament::backend::Backend backend);
    ~RenderEngine();

    filament::Engine *get()
    {
        return m_engine;
    }

    filament::Engine& get_ref()
    {
        return *m_engine;
    }

    const filament::Engine *get() const
    {
        return m_engine;
    }

    const filament::Engine& get_ref() const
    {
        return *m_engine;
    }

    operator filament::Engine*()
    {
        return get();
    }

    operator filament::Engine&()
    {
        return get_ref();
    }
};

} // namespace spatial::render