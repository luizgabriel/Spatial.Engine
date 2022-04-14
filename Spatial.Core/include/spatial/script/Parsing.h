#pragma once

#include "spatial/ecs/Resource.h"
#include <functional>
#include <spatial/ecs/Script.h>
#include <spatial/resources/FileSystem.h>
#include <v8-local-handle.h>
#include <v8-script.h>

namespace spatial::script
{

using PropertyParser = std::function<ecs::ScriptComponent::Property::Type(v8::Local<v8::Object>)>;
using ParserMap = std::unordered_map<std::string, PropertyParser>;

ecs::ScriptComponent::Property::NumberRangeType parseNumberRangeProperty(v8::Local<v8::Object> object);

ecs::ScriptComponent::Property::StringType parseStringProperty(v8::Local<v8::Object> object);

const ParserMap gDefaultPropertyParserMap{
	{ecs::ScriptComponent::Property::StringType::typeName, parseStringProperty},
	{ecs::ScriptComponent::Property::NumberRangeType::typeName, parseNumberRangeProperty},
};

v8::Local<v8::Module> compileModule(v8::Local<v8::Context> context, FileSystem& fileSystem,
									std::string_view modulePath);

using ScriptParseResult = std::variant<ecs::ScriptComponent, ecs::ResourceError>;

ecs::ScriptComponent::Property parseProperty(v8::Local<v8::Object> object, const ParserMap& parserMap = gDefaultPropertyParserMap);

ScriptParseResult parseModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module, std::string_view moduleName, const ParserMap& parserMap = gDefaultPropertyParserMap);

} // namespace spatial::script