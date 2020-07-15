#pragma once

#include <entt/entity/entity.hpp>
#include <unordered_map>
#include <utils/EntityManager.h>
#include <utils/Entity.h>
#include <filament/Scene.h>
#include <filament/Engine.h>
#include <entt/entity/registry.hpp>
#include <filament/RenderableManager.h>

namespace spatial::ecs
{
	class RenderableSystem
	{
	private:
		filament::RenderableManager& mManager;
		filament::Scene& mScene;
	public:
		RenderableSystem(filament::Engine& engine, filament::Scene& scene);

		void onConstruct(entt::registry& registry, entt::entity entity);

		void onDestroy(entt::registry& registry, entt::entity entity);
	};
}
