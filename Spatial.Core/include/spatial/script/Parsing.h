#pragma once

#include <spatial/ecs/Script.h>
#include <v8-local-handle.h>
#include <v8-script.h>

namespace spatial::script
{

using PropertyParser = std::function<ecs::ScriptModule::Property::Type(v8::Local<v8::Object>)>;
using ParserMap = std::unordered_map<std::string, PropertyParser>;

ecs::ScriptModule::Property::NumberRangeType parseNumberRangeProperty(v8::Local<v8::Object> object);

ecs::ScriptModule::Property::StringType parseStringProperty(v8::Local<v8::Object> object);

const ParserMap gDefaultPropertyParserMap{
	{ecs::ScriptModule::Property::StringType::typeName, parseStringProperty},
	{ecs::ScriptModule::Property::NumberRangeType::typeName, parseNumberRangeProperty},
};

using ScriptParseResult = std::variant<ecs::ScriptModule, std::string>;

ecs::ScriptModule::Property parseProperty(v8::Local<v8::Object> object,
										  const ParserMap& parserMap = gDefaultPropertyParserMap);

ScriptParseResult parseModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module, std::string_view moduleName,
							  const ParserMap& parserMap = gDefaultPropertyParserMap);

} // namespace spatial::script