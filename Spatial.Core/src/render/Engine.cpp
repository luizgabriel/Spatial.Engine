#include <spatial/render/Engine.h>

#ifdef SPATIAL_DEBUG
#include <iostream>
#endif

namespace spatial::render
{

void destroyEngine(filament::Engine *engine)
{
    #ifdef SPATIAL_DEBUG
    std::cout << "\n[SPATIAL] Cleaning rendering engine. ";
    #endif

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