#pragma once

#include <filament/Engine.h>
#include <memory>

namespace spatial::render
{

using EnginePtr = std::shared_ptr<filament::Engine>;

void destroyEngine(filament::Engine* engine);

EnginePtr createEngine();

} // namespace spatial::render