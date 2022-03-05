#pragma once

#include <v8-local-handle.h>
#include <v8-script.h>

namespace spatial::script
{

v8::Local<v8::String> createString(v8::Isolate* isolate, std::string_view value);

std::string getValue(v8::Isolate* isolate, v8::Local<v8::String> string);

v8::Local<v8::Module> compileModule(v8::Local<v8::Context> context, std::unique_ptr<std::istream>&& stream, std::string_view moduleName);

v8::Local<v8::Object> evaluateModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module);

v8::Local<v8::Value> getAttribute(v8::Local<v8::Object> object, std::string_view key);

v8::Local<v8::Value> getAttributeOrDefault(v8::Local<v8::Object> object, std::string_view key, v8::Local<v8::Value> defaultValue);

const char* getTypeName(v8::Local<v8::Value> value);

}