#include "spatial/ecs/Resource.h"
#include <boost/algorithm/string/predicate.hpp>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Script.h>
#include <spatial/script/Parsing.h>
#include <spatial/script/ScriptController.h>
#include <spatial/script/ScriptSourceStream.h>
#include <spatial/script/Utils.h>
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

	registry.getEntities<const ecs::Resource, ecs::tags::IsScript>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty() || !ends_with(resource.relativePath.c_str(), ".js"))
				return;

			const auto scriptDefaultName = registry.hasAllComponents<ecs::Name>(entity)
				                               ? registry.getComponent<ecs::Name>(entity).name
				                               : resource.stem();

			auto scope = v8::HandleScope{mIsolate.get()};
			const auto context = v8::Context::New(mIsolate.get());
			const auto module = compileModule(context, mFileSystem, resource.relativePath);
			auto result = parseModule(context, module, scriptDefaultName);

			std::visit(
				[&registry, entity]<typename T>(T&& c) {
					registry.addOrReplaceComponent<T>(entity, std::forward<T>(c));
				},
				std::move(result));

			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}


} // namespace spatial::script