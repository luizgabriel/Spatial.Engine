#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Scene.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Scene& sceneView)
{
	ar(make_nvp("size", sceneView.size));
	ar(make_nvp("blendMode", sceneView.blendMode));
	ar(make_nvp("isShadowingEnabled", sceneView.isShadowingEnabled));
	ar(make_nvp("isPostProcessingEnabled", sceneView.isPostProcessingEnabled));
	ar(make_nvp("camera", sceneView.camera));
	ar(make_nvp("indirectLight", sceneView.indirectLight));
}

} // namespace cereal
