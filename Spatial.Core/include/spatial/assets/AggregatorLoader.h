#pragma once

#include <memory>
#include <spatial/assets/ResourcesLoader.h>
#include <sstream>
#include <string>

namespace spatial::assets
{

class AggregatorLoader : public ResourcesLoader
{
  private:
	std::vector<std::shared_ptr<ResourcesLoader>> mData;

  public:
	AggregatorLoader();

	explicit AggregatorLoader(std::size_t size);

	AggregatorLoader(const AggregatorLoader& other) = delete;
	AggregatorLoader(AggregatorLoader&& other) = default;

	AggregatorLoader&& add(std::shared_ptr<ResourcesLoader> resource);

	template<typename LoaderType>
	AggregatorLoader&& emplace(LoaderType&& loader)
	{
		auto sharedLoader = std::make_shared<LoaderType>(std::move(loader));
		return add(sharedLoader);
	}

	template<typename LoaderType, typename... Args>
	AggregatorLoader&& emplace(Args&&... args)
	{
		auto loader = std::make_shared<LoaderType>(std::forward<Args>(args)...);
		return add(loader);
	}

	virtual assets::ResourceData load(const std::string_view resourceName) const override;
};

} // namespace spatial::assets