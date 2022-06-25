#include <spatial/ecs/Script.h>

namespace spatial::ecs {

const char* ScriptComponent::getPropertyType(const Property& property)
{
	return std::visit([](const auto& t) { return std::decay_t<decltype(t)>::typeName; }, property);
}

}