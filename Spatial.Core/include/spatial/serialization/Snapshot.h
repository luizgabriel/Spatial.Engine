#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <entt/entity/snapshot.hpp>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Resource.h>

namespace cereal
{

template <typename Component>
struct ComponentRegistry
{
	spatial::ecs::Registry& reg;
	Component* _dummy{};
};

template <typename Component>
struct ConstComponentRegistry
{
	const spatial::ecs::Registry& reg;
	Component* _dummy{};
};

template <typename Archive, typename Component>
void save(Archive& ar, const ConstComponentRegistry<Component>& registry)
{
	if constexpr (std::is_same_v<Component, spatial::ecs::Mesh>) {
		// ecs::Mesh is a huge component, there is no need to storage in the scene file if there is a FileSystemResource attached
		auto view = registry.reg.template getEntities<const Component>(spatial::ecs::Exclude<spatial::ecs::FileSystemResource>);
		ar(cereal::make_size_tag(view.size_hint()));

		view.each([&](spatial::ecs::Entity entity, const Component& component) {
			ar(cereal::make_map_item(entity, component));
		});
	} else {
		auto view = registry.reg.template getEntities<const Component>();
		ar(cereal::make_size_tag(view.size()));

		if constexpr (std::is_empty_v<Component>)
			view.each([&](spatial::ecs::Entity entity) { ar(entity); });
		else
			view.each([&](spatial::ecs::Entity entity, const Component& component) {
				ar(cereal::make_map_item(entity, component));
			});
	}


}

template <typename Archive, typename Component>
void load(Archive& ar, ComponentRegistry<Component>& registry)
{
	auto entitiesCount = cereal::size_type{};
    ar(cereal::make_size_tag(entitiesCount));

	for (size_t i = 0; i < entitiesCount; i++)
	{
		auto entity = spatial::ecs::Entity{};

		if constexpr (std::is_empty_v<Component>)
		{
			ar(entity);
			registry.reg.template addComponent<Component>(entity);
		}
		else
		{
			auto component = Component{};
			ar(cereal::make_map_item(entity, component));
			registry.reg.template addComponent<Component>(entity, std::move(component));
		}
	}
}

} // namespace cereal

namespace spatial::ecs
{

class Snapshot
{

  public:
	explicit Snapshot(const Registry& source) noexcept : reg{source}
	{
	}

	template <typename Archive>
	const Snapshot& entities(Archive& archive) const
	{
		const auto sz = reg.getEntitiesCount();

		auto entitiesVector = std::vector<ecs::Entity>{};
		entitiesVector.reserve(sz);

		std::copy(reg.getEntities(), reg.getEntities() + sz, std::back_inserter(entitiesVector));

		archive(cereal::make_nvp("entities", entitiesVector));

		return *this;
	}

	template <typename Component, typename Archive>
	const Snapshot& component(Archive& archive) const
	{
		auto registry = cereal::ConstComponentRegistry<Component>{reg};
		archive(cereal::make_nvp(Component::typeName, registry));
		return *this;
	}

  private:
	const Registry& reg;
};

class SnapshotLoader
{
  public:
	explicit SnapshotLoader(Registry& source) noexcept : reg{source}
	{
		assert(reg.empty());
	}

	template <typename Archive>
	const SnapshotLoader& entities(Archive& archive) const
	{
		std::vector<ecs::Entity> entities{};
		archive(cereal::make_nvp("entities", entities));

		reg.assign(entities.cbegin(), entities.cend(), entt::tombstone);

		return *this;
	}

	template <typename Component, typename Archive>
	void component(Archive& archive) const
	{
		auto registry = cereal::ComponentRegistry<Component>{reg};
		archive(cereal::make_nvp(Component::typeName, registry));
	}

  private:
	Registry& reg;
};

} // namespace spatial::ecs