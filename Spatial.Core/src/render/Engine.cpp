#include <spatial/render/Engine.h>

namespace fl = filament;

namespace spatial
{

Engine createEngine(fl::backend::Backend backend)
{
	return {fl::Engine::create(backend), FilamentEngineDeleter{}};
}

void FilamentEngineDeleter::operator()(fl::Engine* engine)
{
	fl::Engine::destroy(engine);
}

} // namespace spatial