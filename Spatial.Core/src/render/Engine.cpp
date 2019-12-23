#include <spatial/render/Engine.h>

namespace spatial::render
{

void destroyEngine(filament::Engine *engine)
{
    filament::Engine::destroy(engine);
}

EnginePtr createEngine()
{
    return std::shared_ptr<filament::Engine>(
        filament::Engine::create(filament::backend::Backend::OPENGL),
        destroyEngine
    );
}

} // namespace spatial::render