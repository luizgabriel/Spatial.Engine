#pragma once

#include <filament/Engine.h>
#include <memory>

namespace spatial::graphics
{

struct FilamentEngineDeleter
{
	void operator()(filament::Engine* engine);
};

using Engine = std::unique_ptr<filament::Engine, FilamentEngineDeleter>;

Engine createEngine(filament::backend::Backend backend = filament::backend::Backend::DEFAULT);

} // namespace spatial::graphics