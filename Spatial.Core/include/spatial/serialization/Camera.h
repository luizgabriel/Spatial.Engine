#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Transform.h>
#include <spatial/serialization/Math.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::PerspectiveCamera& camera)
{
	ar(make_nvp("fieldOfView", camera.fieldOfView));
	ar(make_nvp("aspectRatio", camera.aspectRatio));
	ar(make_nvp("near", camera.near));
	ar(make_nvp("far", camera.far));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::OrthographicCamera& camera)
{
	ar(make_nvp("left", camera.left));
	ar(make_nvp("right", camera.right));
	ar(make_nvp("bottom", camera.bottom));
	ar(make_nvp("top", camera.top));
	ar(make_nvp("near", camera.near));
	ar(make_nvp("far", camera.far));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::CustomCamera& camera)
{
	ar(make_nvp("projectionMatrix", camera.projectionMatrix));
	ar(make_nvp("near", camera.near));
	ar(make_nvp("far", camera.far));
}

} // namespace spatial