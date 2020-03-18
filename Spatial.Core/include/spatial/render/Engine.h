#pragma once

#include <filament/Engine.h>
#include <memory>

namespace spatial::render
{

class RenderEngine
{
private:
	filament::Engine* m_engine;

public:
	RenderEngine(filament::backend::Backend backend);
	~RenderEngine();

	filament::Engine* get() { return m_engine; }

	filament::Engine& ref() { return *m_engine; }

	const filament::Engine* get() const { return m_engine; }

	const filament::Engine& ref() const { return *m_engine; }
};

} // namespace spatial::render