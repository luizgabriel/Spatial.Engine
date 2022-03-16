#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Script.h>
#include <spatial/script/ScriptController.h>
#include <spatial/script/ScriptSourceStream.h>
#include <spatial/script/Utils.h>
#include <variant>

using namespace boost::algorithm;

namespace spatial::script
{

using ScriptResult = std::variant<ecs::ScriptInfo, ecs::ScriptError>;

static auto gLogger = createDefaultLogger();

ScriptResult parseModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module,
						 const std::string& scriptDefaultName)
{
	auto handle = v8::HandleScope{context->GetIsolate()};

	try
	{
		auto moduleNamespace = evaluateModule(context, module);
		auto defaultExport = getAttribute<v8::Object>(moduleNamespace, "default");

		auto defaultName = createString(context->GetIsolate(), scriptDefaultName);
		auto moduleName = getAttributeOrDefault(defaultExport, "name", defaultName);

		getAttribute<v8::Function>(defaultExport, "onUpdateEntity");

		auto properties = std::set<ecs::ScriptInfo::Property>{};

		auto props = getAttribute<v8::Object>(defaultExport, "props");

		for (auto [key, property] : toEntries<v8::String, v8::Object>(props))
		{
			auto propertyKey = getValue(context->GetIsolate(), key);
			auto propertyName = getValue(context->GetIsolate(), getAttributeOrDefault(property, "name", key));

			auto propertyType = getValue(context->GetIsolate(), getAttribute<v8::String>(property, "type"));
			if (propertyType == ecs::ScriptInfo::Property::FloatRangeType::typeName)
			{
				properties.emplace(ecs::ScriptInfo::Property{
					propertyKey, propertyName,
					ecs::ScriptInfo::Property::FloatRangeType{
						getValue(getAttribute<v8::Number>(property, "default")),
						getValue(getAttribute<v8::Number>(property, "min")),
						getValue(getAttribute<v8::Number>(property, "max")),
					}});
			}
		}

		return ecs::ScriptInfo{getValue(context->GetIsolate(), moduleName), std::move(properties)};
	}
	catch (const std::invalid_argument& e)
	{
		return ecs::ScriptError{e.what()};
	}
}

ScriptController::ScriptController(FileSystem& fileSystem, Isolate&& isolate)
	: mFileSystem{fileSystem}, mIsolate{std::move(isolate)}, mScope{mIsolate.get()}
{
}

void ScriptController::onUpdateFrame(ecs::Registry& registry, float delta)
{
	registry.getEntities<const ecs::Script>(ecs::ExcludeComponents<ecs::ScriptInfo, ecs::ScriptError>)
		.each([&](ecs::Entity entity, const ecs::Script& script) {
			if (script.resource.isEmpty() || !ends_with(script.resource.filename(), ".js"))
				return;

			auto scriptDefaultName = registry.hasAllComponents<ecs::Name>(entity)
										 ? registry.getComponent<ecs::Name>(entity).name
										 : script.resource.relativePath.stem().string();

			auto context = v8::Context::New(mIsolate.get());
			auto module = compileModule(context, script.resource.relativePath.c_str());
			auto result = parseModule(context, module, scriptDefaultName);

			registry.addOrReplaceComponent<v8::Local<v8::Context>>(entity, context);
			registry.addOrReplaceComponent<v8::Local<v8::Module>>(entity, module);

			std::visit(
				[&registry, entity](auto&& c) {
					registry.addComponent<std::decay_t<decltype(c)>>(entity, std::forward<decltype(c)>(c));
				},
				std::move(result));
		});
}

v8::Local<v8::Module> ScriptController::compileModule(v8::Local<v8::Context> context, std::string_view modulePath)
{
	auto resourceStream = mFileSystem.openReadStream(modulePath);
	if (resourceStream->fail())
		throw std::invalid_argument{fmt::format("Script not found: {}", modulePath)};

	auto module = script::compileModule(context, std::move(resourceStream), modulePath);
	auto result =
		module
			->InstantiateModule(context,
								[](v8::Local<v8::Context> context, v8::Local<v8::String> specifier,
								   v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer) {
									context->GetIsolate()->ThrowException(
										createString(context->GetIsolate(), "Module importing is not implemented yet"));
									return v8::MaybeLocal<v8::Module>();
								})
			.ToChecked();

	if (!result || module.IsEmpty())
	{
		auto exception = module->GetException().As<v8::String>();
		throw std::invalid_argument{fmt::format("Could not instantiate module: {}, Exception: {}", modulePath,
												getValue(mIsolate.get(), exception))};
	}

	return module;
}

} // namespace spatial::script