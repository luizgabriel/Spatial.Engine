#include <spatial/render/ResourceLoaders.h>
#include <filament/TextureSampler.h>
#include <spatial/render/RegistryUtils.h>
#include <spatial/ecs/Components.h>
#include "DebugCubeSystem.h"
#include "Components.h"

namespace fl = filament;

namespace spatial::ecs
{

DebugCubeSystem::DebugCubeSystem(fl::Engine* engine)
	: m_engine{engine},
	  m_debugCubeTexture{createTexture(m_engine, "textures/debug_cube.png")},
	  m_debugCubeMaterial{createMaterial(m_engine, "materials/default")},
	  m_debugCubeMesh{createMesh(m_engine, "models/debug_cube")},
	  m_instances{}
{
}

DebugCubeSystem::~DebugCubeSystem()
{
	for (auto [entity, instance] : m_instances) {
		m_engine->destroy(instance);
	}
}

void DebugCubeSystem::onUpdate(entt::registry& registry)
{
	for (auto entity : registry.view<DebugCube>())
	{
		auto& dmc = registry.get<DebugCube>(entity);
		auto instance = m_instances.at(entity);

		instance->setParameter("metallic", dmc.metallic);
		instance->setParameter("roughness", dmc.roughness);
		instance->setParameter("clearCoat", dmc.clearCoat);
		instance->setParameter("clearCoatRoughness", dmc.clearCoatRoughness);
	}
}

void DebugCubeSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	if (!m_instances.contains(entity))
	{
		auto materialInstance = m_debugCubeMaterial->createInstance();
		m_instances.emplace(entity, materialInstance);

		const auto sampler = fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR};
		materialInstance->setParameter("albedo", m_debugCubeTexture.get(), sampler);

		auto renderable = utils::EntityManager::get().create();
		m_debugCubeMesh.build(renderable, materialInstance);
		registry.emplace<Renderable>(entity, renderable);
	}
}

void DebugCubeSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	if (m_instances.contains(entity))
	{
		auto materialInstance = m_instances[entity];
		m_engine->destroy(materialInstance);
		m_instances.erase(entity);
		registry.remove<Renderable>(entity);
	}
}


} // namespace spatial::ecs
