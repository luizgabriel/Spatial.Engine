#pragma once

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, ecs::tags::IsRenderable& renderable)
{
	auto tag = "is-renderable";
	ar(CEREAL_NVP(tag));
}

template <typename Archive>
void serialize(Archive& ar, ecs::tags::IsMeshMaterial& renderable)
{
	auto tag = "is-material";
	ar(CEREAL_NVP(tag));
}

}