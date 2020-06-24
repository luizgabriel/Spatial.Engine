#pragma once

#include <spatial/render/Resources.h>
#include <spatial/render/Mesh.h>
#include <entt/entity/registry.hpp>

namespace spatial::ecs
{

class DebugMeshSystem
{
private:
	filament::Engine& m_engine;
	Material m_debugCubeMaterial;
	Mesh m_debugMesh;

	std::unordered_map<entt::entity, MaterialInstance> m_instances;

public:
	explicit DebugMeshSystem(filament::Engine& engine);

	void onConstruct(entt::registry& registry, entt::entity entity);

	void onUpdate(entt::registry& registry);

	void onDestroy(entt::registry& registry, entt::entity entity);
};

}