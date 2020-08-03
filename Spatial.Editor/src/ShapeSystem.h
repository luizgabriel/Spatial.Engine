#pragma once

#include <filament/Engine.h>
#include <filament/Scene.h>

#include <spatial/assets/ResourcesLoader.h>
#include <spatial/render/Mesh.h>
#include <spatial/ecs/Components.h>

#include <entt/entity/registry.hpp>
#include <unordered_map>

#include "Components.h"

namespace spatial::ecs
{

class ShapeSystem
{
  private:
	filament::Engine& mEngine;
	const assets::ResourcesLoader& mResources;

	std::unordered_map<std::string, Mesh> mMeshDict;

	Material mMaterial;
	MaterialInstance mMaterialInstance;

  public:
	ShapeSystem(filament::Engine& engine, const assets::ResourcesLoader& resources);

	void setMaterial(const std::string& material);

	void onConstruct(entt::registry& registry, entt::entity entity);
};

}