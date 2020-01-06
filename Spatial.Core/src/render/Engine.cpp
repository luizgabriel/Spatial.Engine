#include <spatial/render/Engine.h>

namespace spatial::render
{

RenderEngine::RenderEngine(filament::backend::Backend backend)
    : m_engine{filament::Engine::create(backend)}
{
}

RenderEngine::~RenderEngine()
{
    filament::Engine::destroy(&m_engine);
}

} // namespace spatial::render