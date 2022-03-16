#pragma once

namespace spatial::ecs::tags {

struct IsRenderable
{
	constexpr static auto typeName = "is_renderable";
};

struct ShouldRecreateRenderable
{
};

struct AddedToScene
{
};

struct CanDestroy
{
};

}