#include <fmt/format.h>
#include <spatial/script/Parsing.h>
#include <spatial/script/Utils.h>
#include <v8-container.h>
#include <v8-context.h>
#include <v8-function.h>
#include <v8-object.h>
#include <v8-primitive.h>
#include <v8-script.h>

namespace spatial::script
{

v8::Local<v8::Module> compileModule(v8::Local<v8::Context> context, FileSystem& fileSystem, std::string_view modulePath)
{
	auto resourceStream = fileSystem.openReadStream(modulePath);
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
												getValue(context->GetIsolate(), exception))};
	}

	return module;
}

ecs::ScriptComponent::Property::NumberRangeType parseNumberRangeProperty(v8::Local<v8::Object> object)
{
	return {
		getAttribute<v8::Number>(object, "default")->Value(),
		getAttribute<v8::Number>(object, "min")->Value(),
		getAttribute<v8::Number>(object, "max")->Value(),
	};
}

ecs::ScriptComponent::Property::StringType parseStringProperty(v8::Local<v8::Object> object)
{
	return {
		getValue(object->GetIsolate(), getAttribute<v8::String>(object, "default")),
	};
}

ecs::ScriptComponent::Property parseProperty(v8::Local<v8::Object> object, const ParserMap& parserMap)
{
	auto isolate = object->GetIsolate();
	auto emptyName = createString(isolate, "");
	auto propertyName = getValue(isolate, getAttributeOrDefault(object, "name", emptyName));
	auto propertyType = getValue(isolate, getAttribute<v8::String>(object, "type"));

	return ecs::ScriptComponent::Property{propertyName, std::apply(parserMap.at(propertyType), std::make_tuple(object))};
}

ScriptParseResult parseModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module, std::string_view moduleName, const ParserMap& parserMap)
{
	auto handle = v8::HandleScope{context->GetIsolate()};

	try
	{
		auto moduleNamespace = evaluateModule(context, module);
		auto defaultExport = getAttribute<v8::Object>(moduleNamespace, "default");

		auto defaultName = createString(context->GetIsolate(), moduleName);
		auto scriptName = getAttributeOrDefault(defaultExport, "name", defaultName);

		getAttribute<v8::Function>(defaultExport, "onUpdateEntity");

		auto properties = std::unordered_map<std::string, ecs::ScriptComponent::Property>{};
		auto props = getAttribute<v8::Object>(defaultExport, "props");

		for (auto [key, property] : toEntries<v8::String, v8::Object>(props))
		{
			auto propertyKey = getValue(context->GetIsolate(), key);
			properties.emplace(std::move(propertyKey), parseProperty(property, parserMap));
		}

		return ecs::ScriptComponent{getValue(context->GetIsolate(), scriptName), std::move(properties)};
	}
	catch (const std::invalid_argument& e)
	{
		return ecs::ResourceError{e.what()};
	}
}

} // namespace spatial::script