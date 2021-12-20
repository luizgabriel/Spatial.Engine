#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/SkyBox.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::SkyBoxColor& skybox)
{
	ar(make_nvp("name", skybox.color));
	ar(make_nvp("intensity", skybox.intensity));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::SkyBoxTexture& skybox)
{
	ar(make_nvp("texturePath", skybox.texturePath));
	ar(make_nvp("showSun", skybox.showSun));
	ar(make_nvp("intensity", skybox.intensity));
}

} // namespace cereal