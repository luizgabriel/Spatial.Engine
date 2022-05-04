#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Light.h>
#include <spatial/serialization/Math.h>
#include <spatial/serialization/Resource.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::PointLight& light)
{
	ar(make_nvp("intensity", light.intensity));
	ar(make_nvp("color", light.color));
	ar(make_nvp("falloff", light.falloff));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::SpotLight& light)
{
	ar(make_nvp("intensity", light.intensity));
	ar(make_nvp("color", light.color));
	ar(make_nvp("castShadows", light.castShadows));
	ar(make_nvp("falloff", light.falloff));
	ar(make_nvp("innerCone", light.innerAngle));
	ar(make_nvp("outerCone", light.outerAngle));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::DirectionalLight& light)
{
	ar(make_nvp("intensity", light.intensity));
	ar(make_nvp("direction", light.direction));
	ar(make_nvp("color", light.color));
	ar(make_nvp("castShadows", light.castShadows));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::SunLight& light)
{
	ar(make_nvp("intensity", light.intensity));
	ar(make_nvp("color", light.color));
	ar(make_nvp("angularRadius", light.angularRadius));
	ar(make_nvp("haloSize", light.haloSize));
	ar(make_nvp("haloFalloff", light.haloFalloff));
	ar(make_nvp("castShadows", light.castShadows));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::IndirectLight& light)
{
	ar(make_nvp("intensity", light.intensity));
	ar(make_nvp("reflectionsTexture", light.reflectionsTexture));
	ar(make_nvp("irradianceValues", light.irradianceValues));
}

} // namespace cereal