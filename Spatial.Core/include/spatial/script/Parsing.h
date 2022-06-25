#pragma once

#include <spatial/ecs/Script.h>
#include <v8-local-handle.h>
#include <v8-script.h>

namespace spatial::script
{

template <typename T>
using ObjectParser = std::function<T(v8::Local<v8::Object>)>;
using PropertyParser = ObjectParser<ecs::ScriptComponent::Property>;
using ParserMap = std::unordered_map<std::string, PropertyParser>;

ecs::ScriptComponent::NumberProperty parseNumberRangeProperty(v8::Local<v8::Object> object);

ecs::ScriptComponent::StringProperty parseStringProperty(v8::Local<v8::Object> object);

ecs::ScriptComponent::Property parseProperty(v8::Local<v8::Object> object);

ecs::ScriptComponent parseComponent(v8::Local<v8::Object> object);

ecs::ScriptModule parseModule(v8::Local<v8::Object> object);

} // namespace spatial::script