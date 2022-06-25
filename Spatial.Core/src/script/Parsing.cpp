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

ecs::ScriptComponent::NumberProperty parseNumberRangeProperty(v8::Local<v8::Object> object)
{
	return {
		getAttribute<v8::Number>(object, "default")->Value(),
		getAttribute<v8::Number>(object, "min")->Value(),
		getAttribute<v8::Number>(object, "max")->Value(),
	};
}

ecs::ScriptComponent::StringProperty parseStringProperty(v8::Local<v8::Object> object)
{
	return {
		getValue(object->GetIsolate(), getAttribute<v8::String>(object, "default")),
	};
}

ecs::ScriptComponent::Property parseProperty(v8::Local<v8::Object> object)
{
	static const ParserMap sDefaultPropertyParserMap{
		{ecs::ScriptComponent::StringProperty::typeName, parseStringProperty},
		{ecs::ScriptComponent::NumberProperty::typeName, parseNumberRangeProperty},
	};

	auto propertyType = getValue(object->GetIsolate(), getAttribute<v8::String>(object, "type"));

	return std::apply(sDefaultPropertyParserMap.at(propertyType), std::make_tuple(object));
}

ecs::ScriptComponent parseComponent(v8::Local<v8::Object> object)
{
	auto component = ecs::ScriptComponent{};

	auto properties = getAttribute<v8::Object>(object, "Properties");
	for (auto [key, value] : toEntries<v8::String, v8::Object>(properties)) {
		auto name = getValue(object->GetIsolate(), key);
		component.properties.emplace(name, parseProperty(value));
	}

	return component;
}

ecs::ScriptModule parseModule(v8::Local<v8::Object> object)
{
	auto isolate = object->GetIsolate();
	auto handle = v8::HandleScope{isolate};

	auto parseModule = ecs::ScriptModule{};

	for (auto [key, value] : toEntries<v8::String, v8::Object>(object)) {
		const auto type = getValue(isolate, getAttribute<v8::String>(value, "Type"));
		const auto name = getValue(isolate, key);

		if (type == "Component") {
			parseModule.components.emplace(std::move(name), parseComponent(value));
		}
	}

	return parseModule;
}

} // namespace spatial::script