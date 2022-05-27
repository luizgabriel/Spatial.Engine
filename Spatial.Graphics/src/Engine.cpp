#include <spatial/graphics/Engine.h>

namespace fl = filament;

namespace spatial::graphics
{

Engine createEngine(fl::backend::Backend backend)
{
	return {fl::Engine::create(backend), FilamentEngineDeleter{}};
}

void FilamentEngineDeleter::operator()(fl::Engine* engine)
{
	fl::Engine::destroy(engine);
}

} // namespace spatial::graphics