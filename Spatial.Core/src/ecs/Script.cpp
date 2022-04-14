#include <spatial/ecs/Script.h>

namespace spatial::ecs {

const char* ScriptComponent::Property::getTypeName() const
{
	return std::visit([](const auto& t){
		return std::decay_t<decltype(t)>::typeName;
	}, type);
}

bool ScriptComponent::Property::operator<(const ScriptComponent::Property& rhs) const
{
	return name < rhs.name;
}

}