#include <spatial/render/Engine.h>

namespace spatial
{

RenderEngine::RenderEngine(const filament::backend::Backend backend) : m_engine{filament::Engine::create(backend)}
{
}

RenderEngine::~RenderEngine()
{
	filament::Engine::destroy(&m_engine);
}

} // namespace spatial