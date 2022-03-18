#include <boost/algorithm/string/predicate.hpp>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Script.h>
#include <spatial/script/ScriptController.h>
#include <spatial/script/ScriptSourceStream.h>
#include <spatial/script/Utils.h>
#include <spatial/script/Parsing.h>
#include <variant>

using namespace boost::algorithm;

namespace spatial::script
{

ScriptController::ScriptController(FileSystem& fileSystem, Isolate&& isolate)
	: mFileSystem{fileSystem}, mIsolate{std::move(isolate)}, mScope{mIsolate.get()}
{
}

void ScriptController::onUpdateFrame(ecs::Registry& registry, float delta)
{
	assert(mScope.NumberOfHandles(mIsolate.get()) == 0);

	registry.getEntities<const ecs::Script>(ecs::ExcludeComponents<ecs::ScriptInfo, ecs::ScriptError>)
		.each([&](ecs::Entity entity, const ecs::Script& script) {
			if (script.resource.isEmpty() || !ends_with(script.resource.filename(), ".js"))
				return;

			auto scriptDefaultName = registry.hasAllComponents<ecs::Name>(entity)
										 ? registry.getComponent<ecs::Name>(entity).name
										 : script.resource.relativePath.stem().string();

			auto scope = v8::HandleScope{mIsolate.get()};
			auto context = v8::Context::New(mIsolate.get());
			auto module = compileModule(context, mFileSystem, script.resource.relativePath.c_str());
			auto result = parseModule(context, module, scriptDefaultName);

			std::visit(
				[&registry, entity](auto&& c) {
					registry.addComponent<std::decay_t<decltype(c)>>(entity, std::forward<decltype(c)>(c));
				},
				std::move(result));
		});
}


} // namespace spatial::script