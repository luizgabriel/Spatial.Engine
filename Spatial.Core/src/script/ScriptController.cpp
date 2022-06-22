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
	auto handle = v8::HandleScope{isolate.get()};
	const auto context = v8::Context::New(isolate.get());

	registry
		.getEntities<const ecs::Resource, const ecs::ResourceData, ecs::tags::IsScript>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource, const ecs::ResourceData& data) {
			const auto scriptDefaultName = registry.hasAllComponents<ecs::Name>(entity)
											   ? registry.getComponent<ecs::Name>(entity).name
											   : resource.stem();

			auto module = compileModule(isolate.get(), data.data, resource.relativePath);
			if (auto instantiateError = instantiateModule(context, module)) {
				auto errorMessage = fmt::format("Could not instantiate module: {}. \nException: {}",
												resource.relativePath, instantiateError.value());
				registry.addComponent<ecs::ResourceError>(entity, std::move(errorMessage));
			}

			auto result = parseModule(context, module, scriptDefaultName);
			if (std::holds_alternative<std::string>(result)) {
				auto errorMessage = fmt::format("Could not parse module: {}. \nException: {}", resource.relativePath,
												std::get<std::string>(result));
				registry.addComponent<ecs::ResourceError>(entity, std::move(errorMessage));
			}

			if (std::holds_alternative<ecs::ScriptModule>(result)) {
				auto moduleComponent = std::get<ecs::ScriptModule>(std::move(result));
				registry.addComponent<ecs::ScriptModule>(entity, std::move(moduleComponent));
			}

			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

} // namespace spatial::script