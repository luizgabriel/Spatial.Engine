#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Material.h>

namespace spatial::ecs
{

void MaterialInstance::changeMaterialSource(Registry& registry, entt::entity materialInstance, Entity newMaterialSource)
{
	ecs::Child::changeParent(registry, materialInstance, newMaterialSource);
}

}