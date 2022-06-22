#include <chrono>
#include <entt/core/hashed_string.hpp>
#include <spatial/script/ScriptSourceStream.h>
#include <spatial/script/Utils.h>
#include <v8-container.h>
#include <v8-context.h>
#include <v8-promise.h>

namespace spatial::script
{

v8::Local<v8::String> createString(v8::Isolate* isolate, std::string_view value)
{
	return v8::String::NewFromUtf8(isolate, value.data()).ToLocalChecked();
}

std::string getValue(v8::Isolate* isolate, v8::Local<v8::String> string)
{
	if (string.IsEmpty())
		return "";

	auto value = v8::String::Utf8Value{isolate, string};
	return std::string{*value, static_cast<size_t>(value.length())};
}

v8::Local<v8::Module> compileModule(v8::Isolate* isolate, const std::vector<uint8_t>& data, std::string_view moduleName)
{
	auto handle = v8::EscapableHandleScope{isolate};

	auto moduleId = static_cast<int>(entt::hashed_string{moduleName.data()}.value());

	// For some reason, V8 Caches the script source by the module name. By generating a custom module name, we avoid it.
	// TODO: How to enable/disable this module caching when appropriate?
	auto uniqueModuleName =
		fmt::format("{}-{}", moduleName, std::chrono::steady_clock::now().time_since_epoch().count());

	auto resourceName = createString(isolate, uniqueModuleName);
	auto fullSource = createString(isolate, "");

	auto origin =
		v8::ScriptOrigin{isolate, resourceName,			0, 0, false, moduleId, v8::Local<v8::Value>(), false, false,
						 true,	  v8::Local<v8::Data>()};

	auto sourceString = createString(isolate, std::string{data.begin(), data.end()});
	auto source = v8::ScriptCompiler::Source{sourceString, origin};

	auto module = v8::Local<v8::Module>{};
	if (!v8::ScriptCompiler::CompileModule(isolate, &source).ToLocal(&module))
		throw std::invalid_argument{"Could not parse module"};

	return handle.Escape(module);
}

v8::Local<v8::Module> compileModule(v8::Local<v8::Context> context, std::unique_ptr<std::istream>&& stream,
									std::string_view moduleName)
{
	auto handle = v8::EscapableHandleScope{context->GetIsolate()};

	auto moduleId = static_cast<int>(entt::hashed_string{moduleName.data()}.value());

	// For some reason, V8 Caches the script source by the module name. By generating a custom module name, we avoid it.
	// TODO: How to enable/disable this module caching when appropriate?
	auto uniqueModuleName =
		fmt::format("{}-{}", moduleName, std::chrono::steady_clock::now().time_since_epoch().count());

	auto resourceName = createString(context->GetIsolate(), uniqueModuleName);
	auto fullSource = createString(context->GetIsolate(), "");

	auto origin = v8::ScriptOrigin{
		context->GetIsolate(), resourceName, 0, 0, false, moduleId, v8::Local<v8::Value>(), false, false, true,
		v8::Local<v8::Data>()};

	auto streamedSource = v8::ScriptCompiler::StreamedSource{std::make_unique<ScriptSourceStream>(std::move(stream)),
															 v8::ScriptCompiler::StreamedSource::Encoding::UTF8};

	auto task = v8::ScriptCompiler::StartStreaming(context->GetIsolate(), &streamedSource, v8::ScriptType::kModule);
	auto thread = std::thread{[task]() { task->Run(); }};
	thread.join();

	auto module = v8::Local<v8::Module>{};
	if (!v8::ScriptCompiler::CompileModule(context, &streamedSource, fullSource, origin).ToLocal(&module))
		throw std::invalid_argument{"Could not parse module"};

	return handle.Escape(module);
}

std::optional<std::string> instantiateModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module)
{
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

	if (!result || module.IsEmpty()) {
		auto exception = module->GetException().As<v8::String>();
		return getValue(context->GetIsolate(), exception);
	}

	return std::nullopt;
}

v8::Local<v8::Object> evaluateModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module)
{
	auto handle = v8::EscapableHandleScope{context->GetIsolate()};

	auto moduleScope = v8::Context::Scope{context};

	assert(module->GetStatus() == v8::Module::kInstantiated);
	auto ns = module->GetModuleNamespace();

	auto result = unwrap(module->Evaluate(context), "Could not evaluate module");
	if (result->IsUndefined() || result.As<v8::Promise>()->State() == v8::Promise::kRejected)
		throw std::invalid_argument{"Module evaluation has thrown an exception"};

	assert(module->GetStatus() == v8::Module::kEvaluated);

	return handle.Escape(ns->ToObject(context).ToLocalChecked());
}

v8::Local<v8::Value> getAttributeValue(v8::Local<v8::Object> object, size_t key)
{
	auto handle = v8::EscapableHandleScope{object->GetIsolate()};
	auto context = object->GetCreationContextChecked();
	auto value = object->Get(context, key).ToLocalChecked();

	return handle.Escape(value);
}

v8::Local<v8::Value> getAttributeValue(v8::Local<v8::Object> object, std::string_view key)
{
	auto handle = v8::EscapableHandleScope{object->GetIsolate()};
	auto context = object->GetCreationContextChecked();
	auto attribute = createString(object->GetIsolate(), key);
	auto value = object->Get(context, attribute).ToLocalChecked();

	return handle.Escape(value);
}

v8::Local<v8::Value> getAttributeValueOrDefault(v8::Local<v8::Object> object, std::string_view key,
												v8::Local<v8::Value> defaultValue)
{
	auto resultValue = getAttributeValue(object, key);
	if (resultValue->IsUndefined())
		return defaultValue;

	return resultValue;
}

template <>
bool instanceOf<v8::String>(v8::Local<v8::Value> value)
{
	return value->IsString();
}

std::vector<v8::Local<v8::Value>> toVector(v8::Local<v8::Array> array)
{
	auto values = std::vector<v8::Local<v8::Value>>{};
	values.reserve(array->Length());

	for (auto i = 0; i < array->Length(); i++)
	{
		values.emplace_back(getAttributeValue(array, i));
	}

	return values;
}

const char* getTypeName(v8::Local<v8::Value> value)
{
	assert(!value.IsEmpty());
	if (value->IsArgumentsObject())
		return "ArgumentsObject";
	if (value->IsArrayBuffer())
		return "ArrayBuffer";
	if (value->IsArrayBufferView())
		return "ArrayBufferView";
	if (value->IsArray())
		return "Array";
	if (value->IsBooleanObject())
		return "BooleanObject";
	if (value->IsBoolean())
		return "Boolean";
	if (value->IsDataView())
		return "DataView";
	if (value->IsDate())
		return "Date";
	if (value->IsExternal())
		return "External";
	if (value->IsFalse())
		return "False";
	if (value->IsFloat32Array())
		return "Float32Array";
	if (value->IsFloat64Array())
		return "Float64Array";
	if (value->IsFunction())
		return "Function";
	if (value->IsGeneratorFunction())
		return "GeneratorFunction";
	if (value->IsGeneratorObject())
		return "GeneratorObject";
	if (value->IsInt16Array())
		return "Int16Array";
	if (value->IsInt32Array())
		return "Int32Array";
	if (value->IsInt32())
		return "Int32";
	if (value->IsInt8Array())
		return "Int8Array";
	if (value->IsMapIterator())
		return "MapIterator";
	if (value->IsMap())
		return "Map";
	if (value->IsName())
		return "Name";
	if (value->IsNativeError())
		return "NativeError";
	if (value->IsNull())
		return "Null";
	if (value->IsNumberObject())
		return "NumberObject";
	if (value->IsNumber())
		return "Number";
	if (value->IsObject())
		return "Object";
	if (value->IsPromise())
		return "Promise";
	if (value->IsRegExp())
		return "RegExp";
	if (value->IsSetIterator())
		return "SetIterator";
	if (value->IsSet())
		return "Set";
	if (value->IsStringObject())
		return "StringObject";
	if (value->IsString())
		return "String";
	if (value->IsSymbolObject())
		return "SymbolObject";
	if (value->IsSymbol())
		return "Symbol";
	if (value->IsTrue())
		return "True";
	if (value->IsTypedArray())
		return "TypedArray";
	if (value->IsUint16Array())
		return "Uint16Array";
	if (value->IsUint32Array())
		return "Uint32Array";
	if (value->IsUint32())
		return "Uint32";
	if (value->IsUint8Array())
		return "Uint8Array";
	if (value->IsUint8ClampedArray())
		return "Uint8ClampedArray";
	if (value->IsUndefined())
		return "Undefined";
	if (value->IsWeakMap())
		return "WeakMap";
	if (value->IsWeakSet())
		return "WeakSet";

	return "Unknown";
}

template <>
bool instanceOf<v8::Object>(v8::Local<v8::Value> value)
{
	return value->IsObject();
}

template <>
bool instanceOf<v8::Function>(v8::Local<v8::Value> value)
{
	return value->IsFunction();
}

template <>
bool instanceOf<v8::Number>(v8::Local<v8::Value> value)
{
	return value->IsNumber();
}

template <>
const char* getTypeName<v8::Function>()
{
	return "Function";
}

std::vector<std::pair<v8::Local<v8::Value>, v8::Local<v8::Value>>> toValueEntries(v8::Local<v8::Object> object)
{
	auto context = object->GetCreationContextChecked();
	auto keys = toVector(object->GetOwnPropertyNames(context).ToLocalChecked());
	auto entries = std::vector<std::pair<v8::Local<v8::Value>, v8::Local<v8::Value>>>{};
	entries.reserve(keys.size());

	for (auto key : keys)
	{
		entries.emplace_back(key, object->Get(context, key).ToLocalChecked());
	}

	return entries;
}

template <>
const char* getTypeName<v8::String>()
{
	return "String";
}

template <>
const char* getTypeName<v8::Number>()
{
	return "Number";
}

template <>
const char* getTypeName<v8::Object>()
{
	return "Object";
}

} // namespace spatial::script
