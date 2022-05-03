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

	void reserve(size_t capacity);

	template <typename Func>
	void each(Func func) const
	{
		std::for_each(mRegistries.begin(), mRegistries.end(), [&](ecs::Registry* r) {
			func(*r);
		});
	}

  private:
	std::vector<ecs::Registry*> mRegistries;
};

} // namespace spatial::ecs