#pragma once

#include <vector>
#include <algorithm>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

class RegistryCollection
{
  public:
	RegistryCollection();

	void append(ecs::Registry& registry);

	template <typename Func>
	void each(Func func) const
	{
		std::for_each(mRegistries.begin(), mRegistries.end(), [&](ecs::Registry* r) {
			auto& registry = *r;
			func(registry);
		});
	}

  private:
	std::vector<ecs::Registry*> mRegistries;
};

} // namespace spatial::ecs