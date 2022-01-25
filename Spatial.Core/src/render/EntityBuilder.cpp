#include <spatial/render/EntityBuilder.h>

namespace spatial::render
{

EntityBuilder::EntityBuilder(ecs::Registry& registry, ecs::Entity entity) : ecs::EntityBuilder{registry, entity}
{
}

FilameshEntityBuilder EntityBuilder::asFilamesh()
{
	return FilameshEntityBuilder{mRegistry, mEntity};
}

FilameshEntityBuilder::FilameshEntityBuilder(ecs::Registry& registry, ecs::Entity entity)
	: EntityBuilder{registry, entity}
{
}

FilameshEntityBuilder& FilameshEntityBuilder::withFile(filament::Engine& engine, const FilameshFile& filamesh)
{
	with(render::createVertexBuffer(engine, filamesh))
		.with(render::createIndexBuffer(engine, filamesh))
		.with(filament::Box{filamesh.header.aabb})
		.with(render::createMeshGeometries(filamesh))
		.with<ecs::tags::IsMeshLoaded>();

	return *this;
}

EntityBuilder build(ecs::Registry& registry)
{
	return EntityBuilder{registry, registry.createEntity()};
}

} // namespace spatial::render