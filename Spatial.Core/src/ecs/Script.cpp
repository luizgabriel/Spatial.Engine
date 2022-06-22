#include <spatial/ecs/Script.h>

namespace spatial::ecs {

const char* ScriptModule::Property::getTypeName() const
{
	return std::visit([](const auto& t) { return std::decay_t<decltype(t)>::typeName; }, type);
}

bool ScriptModule::Property::operator<(const ScriptModule::Property& rhs) const
{
	return name < rhs.name;
}

}