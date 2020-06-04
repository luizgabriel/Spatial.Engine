#pragma once

#include <spatial/render/Resources.h>
#include <spatial/render/Mesh.h>
#include <entt/entity/registry.hpp>

namespace spatial::ecs
{

class DebugCubeSystem
{
private:
	filament::Engine* m_engine;
	Texture m_debugCubeTexture;
	Material m_debugCubeMaterial;
	Mesh m_debugCubeMesh;

	std::unordered_map<entt::entity, filament::MaterialInstance*> m_instances;

public:
	DebugCubeSystem(filament::Engine* engine);

	~DebugCubeSystem();

	void onConstruct(entt::registry& registry, entt::entity entity);

	void onUpdate(entt::registry& registry);

	void onDestroy(entt::registry& registry, entt::entity entity);
};

}