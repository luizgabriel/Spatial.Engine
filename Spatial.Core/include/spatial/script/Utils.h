#pragma once

#include <fmt/format.h>
#include <v8-isolate.h>
#include <v8-local-handle.h>

namespace spatial::script
{

template <typename T>
bool instanceOf(v8::Local<v8::Value> value);

template <typename T, typename E = std::invalid_argument>
v8::Local<T> cast(v8::Local<v8::Value> value, std::string_view errorMessage)
{
	if (!instanceOf<T>(value))
		throw E{errorMessage.data()};
	return value.template As<T>();
}

v8::Local<v8::String> createString(v8::Isolate* isolate, std::string_view value);

template <typename T>
const char* getTypeName();

const char* getTypeName(v8::Local<v8::Value> value);

std::string getValue(v8::Isolate* isolate, v8::Local<v8::String> string);

v8::Local<v8::Module> compileModule(v8::Isolate* isolate, const std::vector<uint8_t>& data,
									std::string_view moduleName);

v8::Local<v8::Module> compileModule(v8::Local<v8::Context> context, std::unique_ptr<std::istream>&& stream,
									std::string_view moduleName);

void instantiateModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module);

v8::Local<v8::Object> evaluateModule(v8::Local<v8::Context> context, v8::Local<v8::Module> module);

v8::Local<v8::Value> getAttributeValue(v8::Local<v8::Object> object, size_t key);

v8::Local<v8::Value> getAttributeValue(v8::Local<v8::Object> object, std::string_view key);

v8::Local<v8::Value> getAttributeValueOrDefault(v8::Local<v8::Object> object, std::string_view key,
												v8::Local<v8::Value> defaultValue);

template <typename T, typename K>
v8::Local<T> getAttribute(v8::Local<v8::Object> object, const K& key)
{
	auto scope = v8::EscapableHandleScope{object->GetIsolate()};
	static auto typeName = getTypeName<T>();
	return scope.Escape(
		cast<T>(getAttributeValue(object, key), fmt::format("Could not cast property '{}' to '{}'", key, typeName)));
}

template <typename T, typename K>
v8::Local<T> getAttributeOrDefault(v8::Local<v8::Object> object, const K& key, v8::Local<T> defaultValue)
{
	auto scope = v8::EscapableHandleScope{object->GetIsolate()};
	auto typeName = getTypeName(defaultValue);
	return scope.Escape(cast<T>(getAttributeValueOrDefault(object, key, defaultValue),
								fmt::format("Could not cast property '{}' to '{}'", key, typeName)));
}

std::vector<v8::Local<v8::Value>> toVector(v8::Local<v8::Array> array);

std::vector<std::pair<v8::Local<v8::Value>, v8::Local<v8::Value>>> toValueEntries(v8::Local<v8::Object> object);

template <typename K, typename V>
std::vector<std::pair<v8::Local<K>, v8::Local<V>>> toEntries(v8::Local<v8::Object> object)
{
	static auto keyType = getTypeName<K>();
	static auto valueType = getTypeName<V>();

	auto valueEntries = toValueEntries(object);
	auto newEntries = std::vector<std::pair<v8::Local<K>, v8::Local<V>>>{};
	newEntries.reserve(valueEntries.size());

	for (auto [key, value] : valueEntries)
	{
		newEntries.emplace_back(cast<K>(key, fmt::format("Could not cast Object key to '{}'", keyType)),
								cast<V>(value, fmt::format("Could not cast Object value to '{}'", valueType)));
	}

	return newEntries;
}

template <typename T, typename E = std::invalid_argument>
v8::Local<T> unwrap(v8::MaybeLocal<T> value, std::string_view errorMessage)
{
	if (value.IsEmpty())
		throw E{errorMessage.data()};
	return value.ToLocalChecked();
}

} // namespace spatial::script