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
	auto view = registry.view<DebugMesh>();
	for (auto entity : view) {
		auto& component = view.get<DebugMesh>(entity);
		auto& instance = m_instances.at(entity);

		instance->setParameter("albedo", component.color);
		instance->setParameter("metallic", component.metallic);
		instance->setParameter("roughness", component.roughness);
		instance->setParameter("clearCoat", component.clearCoat);
		instance->setParameter("clearCoatRoughness", component.clearCoatRoughness);
	}
}

void DebugMeshSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	if (m_instances.find(entity) != m_instances.end())
		return;

	m_instances.emplace(entity, createMaterialInstance(m_engine, m_debugCubeMaterial.ref()));

	//const auto sampler = fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR};
	//materialInstance->setParameter("albedo", m_debugCubeTexture.get(), sampler);

	auto renderable = createEntity(m_engine);
	m_debugMesh.build(renderable.get(), m_instances.at(entity).get());
	registry.emplace<Renderable>(entity, std::move(renderable));
}

void DebugMeshSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	if (m_instances.find(entity) != m_instances.end())
		m_instances.erase(entity);

	registry.remove<Renderable>(entity);
}


} // namespace spatial::ecs
