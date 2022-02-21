#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/script/Isolate.h>

namespace spatial::script
{

class ScriptController
{
  public:
	explicit ScriptController(Isolate&& isolate);

	void onUpdateFrame(ecs::Registry& registry, float delta);

  private:
	Isolate mIsolate;
	v8::Isolate::Scope mIsolateScope;
};

} // namespace spatial::script