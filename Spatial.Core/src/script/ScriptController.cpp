#include <boost/algorithm/string/predicate.hpp>
#include <spatial/ecs/Script.h>
#include <spatial/script/ScriptController.h>

namespace spatial::script
{

ScriptController::ScriptController(Isolate&& isolate) : mIsolate{std::move(isolate)}
{
}

void ScriptController::onUpdateFrame(ecs::Registry& registry, float delta)
{
	using namespace boost::algorithm;

	registry.getEntities<const ecs::Script>(ecs::ExcludeComponents<ecs::tags::IsScriptLoaded>)
		.each([](ecs::Entity entity, const ecs::Script& script) {
			if (script.resource.isEmpty() || !ends_with(script.resource.filename(), ".js"))
				return;
		});
}

} // namespace spatial::script