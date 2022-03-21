#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Mesh.h>
#include <spatial/serialization/BoundingBox.h>
#include <spatial/serialization/Resource.h>
#include <spatial/ecs/Script.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Script& script)
{
	ar(make_nvp("resource", script.resource));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::ScriptInstance::DataValueType& value)
{
	std::visit([&](auto& val) {
		ar(val);
	}, value);
}

template <typename Archive>
void save(Archive& ar, const spatial::ecs::ScriptInstance::DataContainer& customData)
{
	ar(make_size_tag(customData.size() * 2));
	for (const auto& [k, v]: customData) {
		ar(k);
		ar(v);
	}
}

template <typename Archive>
void load(Archive& ar, spatial::ecs::ScriptInstance::DataContainer& customData)
{
	auto size = cereal::size_type{};
	ar(cereal::make_size_tag(size));
	size /= 2;

	for (cereal::size_type i = 0; i < size; i++) {
		auto k = std::string{};
		auto v = spatial::ecs::ScriptInstance::DataValueType{};
		ar(k);
		ar(v);
		customData.emplace(std::move(k), std::move(v));
	}
}


template <typename Archive>
void serialize(Archive& ar, spatial::ecs::ScriptInstance& script)
{
	ar(make_nvp("source", script.source));
	ar(make_nvp("customData", script.customData));
}

} // namespace cereal
