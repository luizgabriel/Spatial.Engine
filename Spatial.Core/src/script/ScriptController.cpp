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

void ScriptController::loadScripts(ecs::Registry& registry, FileSystem& fileSystem, Isolate& isolate)
{
	registry
		.getEntities<const ecs::FileSystemResource, const ecs::FileSystemResourceData, ecs::tags::IsScript>(
			ecs::Exclude<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::FileSystemResource& resource, const ecs::FileSystemResourceData& data) {
			const auto scriptDefaultName = registry.hasAllComponents<ecs::Name>(entity)
											   ? registry.getComponent<ecs::Name>(entity).name
											   : resource.stem();

			try
			{
				auto handle = v8::HandleScope{isolate.get()};
				auto context = v8::Context::New(isolate.get());
				auto contextScope = v8::Context::Scope{context};

				auto module = compileModule(isolate.get(), data.data, resource.relativePath);
				instantiateModule(context, module);

				auto moduleNamespace = evaluateModule(context, module);
				auto defaultExport = getAttribute<v8::Object>(moduleNamespace, "default");

				auto parsedModule = parseModule(defaultExport);
				registry.addComponent(entity, std::move(module));
			}
			catch (const std::invalid_argument& e)
			{
				auto errorMessage =
					fmt::format("Could not load module: {}. \nException: {}", resource.relativePath, e.what());
				registry.addComponent<ecs::ResourceError>(entity, std::move(errorMessage));
			}

			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

} // namespace spatial::script