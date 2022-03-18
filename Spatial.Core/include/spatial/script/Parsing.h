#pragma once

#include <spatial/ecs/Script.h>
#include <spatial/resources/FileSystem.h>
#include <v8-local-handle.h>
#include <v8-script.h>

namespace spatial::script
{

v8::Local<v8::Module> compileModule(v8::Local<v8::Context> context, FileSystem& fileSystem,
									std::string_view modulePath);

using ScriptParseResult = std::variant<ecs::ScriptInfo, ecs::ScriptError>;

template <typename T>
T parsePropertyType(v8::Local<v8::Object> object);

ScriptParseResult parseModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module, std::string_view moduleName);

} // namespace spatial::script