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

    filament::View* createView()
    {
        return m_engine->createView();
    }

    filament::Engine *get()
    {
        return m_engine;
    }

    filament::Engine &ref()
    {
        return *m_engine;
    }

    const filament::Engine *get() const
    {
        return m_engine;
    }

    const filament::Engine &ref() const
    {
        return *m_engine;
    }

};

} // namespace spatial::render