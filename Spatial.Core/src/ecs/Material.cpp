#include <spatial/ecs/Material.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

void MaterialInstance::changeMaterialSource(Registry& registry, entt::entity materialInstance, Entity newMaterialSource)
{
	ecs::Child::changeParent(registry, materialInstance, newMaterialSource);
}

} // namespace spatial::ecs