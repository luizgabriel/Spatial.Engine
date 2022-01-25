#pragma once

#include <spatial/ecs/EntityBuilder.h>
#include <spatial/render/Resources.h>

namespace spatial::render
{

class FilameshEntityBuilder;

class EntityBuilder : public ecs::EntityBuilder
{
  public:
	EntityBuilder(ecs::Registry& registry, ecs::Entity entity);

	FilameshEntityBuilder asFilamesh();
};

class FilameshEntityBuilder: public EntityBuilder
{
  public:
	FilameshEntityBuilder(ecs::Registry& registry, ecs::Entity entity);

	FilameshEntityBuilder& withFile(filament::Engine& engine, const FilameshFile& filamesh);
};

EntityBuilder build(ecs::Registry& registry);

}