#pragma once

#include <spatial/ecs/Material.h>
#include <spatial/serialization/BoundingBox.h>
#include <spatial/serialization/Camera.h>
#include <spatial/serialization/Light.h>
#include <spatial/serialization/Mesh.h>
#include <spatial/serialization/Name.h>
#include <spatial/serialization/Relation.h>
#include <spatial/serialization/Scene.h>
#include <spatial/serialization/Texture.h>
#include <spatial/serialization/Transform.h>

namespace spatial::ecs
{

template <typename Archive, typename SnapshotType>
void serializeCoreComponents(Archive& ar, SnapshotType& snapshot)
{
	snapshot.template component<ecs::Name>(ar);
	snapshot.template component<ecs::Transform>(ar);
	snapshot.template component<ecs::PerspectiveCamera>(ar);
	snapshot.template component<ecs::OrthographicCamera>(ar);
	snapshot.template component<ecs::CustomCamera>(ar);
	snapshot.template component<ecs::PointLight>(ar);
	snapshot.template component<ecs::DirectionalLight>(ar);
	snapshot.template component<ecs::SpotLight>(ar);
	snapshot.template component<ecs::SunLight>(ar);
	snapshot.template component<ecs::IndirectLight>(ar);
	snapshot.template component<ecs::MeshInstance>(ar);
	snapshot.template component<ecs::MeshMaterial>(ar);
	snapshot.template component<ecs::Parent>(ar);
	snapshot.template component<ecs::Child>(ar);
	snapshot.template component<ecs::Resource>(ar);
	snapshot.template component<ecs::Scene>(ar);
	snapshot.template component<ecs::RuntimeTexture>(ar);
	snapshot.template component<ecs::AttachmentTexture>(ar);
	snapshot.template component<ecs::RuntimeMesh>(ar);

	snapshot.template component<ecs::tags::IsMaterial>(ar);
	snapshot.template component<ecs::tags::IsMesh>(ar);
	snapshot.template component<ecs::tags::IsRenderable>(ar);
	snapshot.template component<ecs::tags::IsCamera>(ar);
	snapshot.template component<ecs::tags::IsLight>(ar);
	snapshot.template component<ecs::tags::IsMainView>(ar);
	snapshot.template component<ecs::tags::IsMaterialInstance>(ar);
	snapshot.template component<ecs::tags::IsImageTexture>(ar);
	snapshot.template component<ecs::tags::IsCubeMapTexture>(ar);
	snapshot.template component<ecs::tags::IsIrradianceValues>(ar);
	snapshot.template component<ecs::tags::IsDummyCubeMapTexture>(ar);
	snapshot.template component<ecs::tags::IsDepthBufferTexture>(ar);
	snapshot.template component<ecs::tags::IsColorBufferTexture>(ar);
	snapshot.template component<ecs::tags::IsRenderedToTarget>(ar);
}

} // namespace spatial::ecs