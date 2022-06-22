#include "spatial/core/Logger.h"
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

ecs::ScriptModule::Property::NumberRangeType parseNumberRangeProperty(v8::Local<v8::Object> object)
{
	return {
		getAttribute<v8::Number>(object, "default")->Value(),
		getAttribute<v8::Number>(object, "min")->Value(),
		getAttribute<v8::Number>(object, "max")->Value(),
	};
}

ecs::ScriptModule::Property::StringType parseStringProperty(v8::Local<v8::Object> object)
{
	return {
		getValue(object->GetIsolate(), getAttribute<v8::String>(object, "default")),
	};
}

ecs::ScriptModule::Property parseProperty(v8::Local<v8::Object> object, const ParserMap& parserMap)
{
	auto isolate = object->GetIsolate();
	auto emptyName = createString(isolate, "");
	auto propertyName = getValue(isolate, getAttributeOrDefault(object, "name", emptyName));
	auto propertyType = getValue(isolate, getAttribute<v8::String>(object, "type"));

	return ecs::ScriptModule::Property{propertyName, std::apply(parserMap.at(propertyType), std::make_tuple(object))};
}

ScriptParseResult parseModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module, std::string_view moduleName, const ParserMap& parserMap)
{
	auto isolate = context->GetIsolate();
	auto handle = v8::HandleScope{isolate};
	static auto logger = createDefaultLogger();

	try
	{
		auto moduleNamespace = evaluateModule(context, module);
		auto defaultExport = getAttribute<v8::Object>(moduleNamespace, "default");

		logger.info("----- Loading Script ------- {}", moduleName);

		for (auto [key, value] : toEntries<v8::String, v8::Object>(defaultExport)) {
			const auto type = getAttribute<v8::String>(value, "Type");
			logger.info("{} -> {}", getValue(isolate, key), getValue(isolate, type));
		}

		return ecs::ScriptModule{};
	}
	catch (const std::invalid_argument& e)
	{
		return e.what();
	}
}

} // namespace spatial::script