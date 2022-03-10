#include <spatial/common/StringHelpers.h>
#include <spatial/script/ScriptSourceStream.h>
#include <spatial/script/Utils.h>
#include <v8-context.h>
#include <v8-promise.h>
#include <fmt/format.h>

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

v8::Local<v8::Module> compileModule(v8::Local<v8::Context> context, std::unique_ptr<std::istream>&& stream,
									std::string_view moduleName)
{
	auto handle = v8::EscapableHandleScope{context->GetIsolate()};

	auto moduleId = static_cast<int>(HashedString{moduleName.data()}.value());
	auto resourceName = createString(context->GetIsolate(), moduleName);
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

const char* getTypeName(v8::Local<v8::Value> value)
{
	if (value.IsEmpty())
		return "Empty";
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

v8::Local<v8::Value> getAttribute(v8::Local<v8::Object> object, std::string_view key)
{
	auto handle = v8::EscapableHandleScope{object->GetIsolate()};
	auto context = object->GetCreationContextChecked();
	auto attribute = createString(object->GetIsolate(), key);
	auto value = object->Get(context, attribute).ToLocalChecked();

	return handle.Escape(value);
}

v8::Local<v8::Value> getAttributeOrDefault(v8::Local<v8::Object> object, std::string_view key,
										   v8::Local<v8::Value> defaultValue)
{
	auto resultValue = getAttribute(object, key);
	if (resultValue->IsUndefined())
		return defaultValue;

	return resultValue;
}

template <>
bool instanceOf<v8::String>(v8::Local<v8::Value> value)
{
	return value->IsString();
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


} // namespace spatial::script
