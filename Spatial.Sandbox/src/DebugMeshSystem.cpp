#include <spatial/render/ResourceLoaders.h>
#include <filament/TextureSampler.h>
#include <spatial/render/RegistryUtils.h>
#include <spatial/ecs/Components.h>
#include "DebugMeshSystem.h"
#include "Components.h"

namespace fl = filament;

namespace spatial::ecs
{

DebugMeshSystem::DebugMeshSystem(fl::Engine& engine)
	: m_engine{engine},
	  m_debugCubeMaterial{createMaterial(m_engine, "materials/default")},
	  m_debugMesh{createMesh(m_engine, "models/debug_cube")},
	  m_instances{}
{
}

void DebugMeshSystem::onUpdate(entt::registry& registry)
{
	for (auto entity : registry.view<DebugMesh>())
	{
		auto& dmc = registry.get<DebugMesh>(entity);
		auto& instance = m_instances.at(entity);

		instance->setParameter("albedo", dmc.color);
		instance->setParameter("metallic", dmc.metallic);
		instance->setParameter("roughness", dmc.roughness);
		instance->setParameter("clearCoat", dmc.clearCoat);
		instance->setParameter("clearCoatRoughness", dmc.clearCoatRoughness);
	}
}

void DebugMeshSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	if (!m_instances.contains(entity))
	{
		m_instances.emplace(entity, createMaterialInstance(m_engine, m_debugCubeMaterial.ref()));

		//const auto sampler = fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR};
		//materialInstance->setParameter("albedo", m_debugCubeTexture.get(), sampler);

		auto renderable = utils::EntityManager::get().create();
		m_debugMesh.build(renderable, m_instances.at(entity).get());
		registry.emplace<Renderable>(entity, renderable);
	}
}

void DebugMeshSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	if (m_instances.contains(entity))
	{
		m_instances.erase(entity);
		registry.remove<Renderable>(entity);
	}
}


} // namespace spatial::ecs
