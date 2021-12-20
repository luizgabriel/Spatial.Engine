#pragma once

#include <entt/entity/snapshot.hpp>
#include <spatial/ecs/Tags.h>
#include <spatial/serialization/Camera.h>
#include <spatial/serialization/FileSystem.h>
#include <spatial/serialization/Light.h>
#include <spatial/serialization/Mesh.h>
#include <spatial/serialization/Name.h>
#include <spatial/serialization/Relation.h>
#include <spatial/serialization/Transform.h>

namespace spatial::ecs
{

template <typename... CustomComponent, typename Archive>
void serialize(Archive& ar, const Registry& registry)
{
	auto snapshot = entt::snapshot{registry};
	snapshot.entities(ar);
	snapshot.component<ecs::Name>(ar);
	snapshot.component<ecs::Transform>(ar);
	snapshot.component<ecs::PerspectiveCamera>(ar);
	snapshot.component<ecs::OrthographicCamera>(ar);
	snapshot.component<ecs::CustomCamera>(ar);
	snapshot.component<ecs::PointLight>(ar);
	snapshot.component<ecs::DirectionalLight>(ar);
	snapshot.component<ecs::SpotLight>(ar);
	snapshot.component<ecs::SunLight>(ar);
	snapshot.component<ecs::Mesh>(ar);
	snapshot.component<ecs::UUID>(ar);
	snapshot.component<ecs::Parent>(ar);
	snapshot.component<ecs::Child>(ar);

	snapshot.component<ecs::tags::IsMaterial>(ar);
	snapshot.component<ecs::tags::IsRenderable>(ar);

	(snapshot.component<CustomComponent>(ar), ...);
}

template <typename... CustomComponent, typename Archive>
void deserialize(Archive& ar, Registry& registry)
{
	auto snapshot = entt::snapshot_loader{registry};
	snapshot.entities(ar);
	snapshot.component<ecs::Name>(ar);
	snapshot.component<ecs::Transform>(ar);
	snapshot.component<ecs::PerspectiveCamera>(ar);
	snapshot.component<ecs::OrthographicCamera>(ar);
	snapshot.component<ecs::CustomCamera>(ar);
	snapshot.component<ecs::PointLight>(ar);
	snapshot.component<ecs::DirectionalLight>(ar);
	snapshot.component<ecs::SpotLight>(ar);
	snapshot.component<ecs::SunLight>(ar);
	snapshot.component<ecs::Mesh>(ar);
	snapshot.component<ecs::UUID>(ar);
	snapshot.component<ecs::Parent>(ar);
	snapshot.component<ecs::Child>(ar);

	snapshot.component<ecs::tags::IsMaterial>(ar);
	snapshot.component<ecs::tags::IsRenderable>(ar);

	(snapshot.component<CustomComponent>(ar), ...);

	snapshot.orphans();
}

} // namespace spatial::ecs